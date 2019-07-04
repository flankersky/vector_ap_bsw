/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 *      \file  conversation.cc
 *      \brief  Contains the implementation of the Conversation class
 *
 *      \details  The Conversation implements the "diagnostic protocol" of the DM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>

#include "ara/log/logging.hpp"

#include "server/conversation/conversation.h"
#include "server/conversation/conversation_manager.h"
#include "server/diagnostic_server.h"
#include "udstransport/protocol_manager_with_conversation_manager_handling.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_message_helper.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

/**
 * \brief Length of the payload for a negative response.
 */
constexpr std::size_t kLengthNegativeResponsePayload = 3;

/**
 * Negative response service identifier (see ISO 14229-1 for details)
 */
constexpr std::uint8_t kNegativeResponseSID = 0x7f;

/**
 * TesterPresent Request UDS ServiceID
 */
constexpr std::size_t kTesterPresentRequestMsgSize = 2;

/**
 * TesterPresent ServiceID
 */
constexpr std::uint8_t kTesterPresentRequestSid = 0x3E;

/**
 * TesterPresent sub-function with active suppress bit
 */
constexpr std::uint8_t kTesterPresentSubfunctionWithActiveSupressBit = 0x80;

Conversation::Conversation(ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier channel_id,
                           ara::diag::udstransport::UdsMessage::Address source_address,
                           vac::timer::TimerManager& timer_manager, ConversationManager& conversation_manager,
                           std::uint8_t response_pending_limit)
    : p2_(timer_manager, *this),
      s3_(timer_manager, *this),
      session_(access::AccessStateChangeInfo{0, SessionId::kDefault, access::AccessCategory::kSession, *this},
               conversation_manager.GetAccessNotificationManager()),
      security_level_(access::AccessStateChangeInfo{0, 0x00, access::AccessCategory::kSecurityLevel, *this},
                      conversation_manager.GetAccessNotificationManager()),
      channel_id_(channel_id),
      source_address_(source_address),
      conversation_manager_(conversation_manager),
      max_number_of_response_pending_(response_pending_limit),
      message_handler_(*this) {}

ConversationManager& Conversation::GetConversationManager() { return conversation_manager_; }

void Conversation::Shutdown() {
  StopS3Timer();
  message_handler_.Shutdown();
  // PAASR-1926 Conversations should support cancellation
  // TODO(PAASR-1926): the state change below might be only intended for the Cancel method (not existing yet)
  ChangeState(ConversationState::kFree);
}

MessageHandler& Conversation::GetMessageHandler() { return message_handler_; }

ara::diag::udstransport::UdsMessage::Ptr Conversation::AcquireResponseBuffer(std::size_t payload_size) {
  conversation::UdsMessageProvider& message_provider = this->GetConversationManager().GetUdsMessageProvider();
  const ara::diag::udstransport::UdsMessage::Address resp_sa = this->GetConversationManager().GetTA();
  const ara::diag::udstransport::UdsMessage::Address resp_ta = this->GetSourceAddress();

  const struct UdsMessageParameter uds_parameter = {
      resp_sa, resp_ta, ara::diag::udstransport::UdsMessage::TargetAddressType::PHYSICAL, GetConnectionId()};

  return message_provider.Acquire(payload_size, uds_parameter);
}

void Conversation::SetSessionId(std::uint8_t session) {
  session_.Set(session);
  if ((GetSessionId() == SessionId::kDefault) && (current_state_ == ConversationState::kIdle)) {
    // when the session state drops to default session the state of the conversation shall be changed to kFree
    // (see also Cancel and post processor finished)
    ChangeState(ConversationState::kFree);
  }
}

void Conversation::SetRxSid(std::uint8_t rx_sid) {
  rx_sid_.reset();
  rx_sid_.emplace(rx_sid);
}

std::uint8_t Conversation::GetRxSid() const {
  // throw runtime_error when no value was provided
  if (!rx_sid_.has_value()) {
    throw std::runtime_error("Conversation::GetRxSid : RX SID not available.");
  }

  return rx_sid_.value();
}

void Conversation::SetSuppressBit(bool suppress_positive_response) {
  suppress_positive_response_ = suppress_positive_response;
}

ara::diag::udstransport::UdsTransportProtocolMgr::IndicationPair Conversation::IndicateMessage(
    const std::size_t size, const ara::diag::udstransport::UdsMessage::TargetAddressType target_address_type) {
  // Firstly, stop S3 timer.
  StopS3Timer();

  // a valid UDS message has at least one byte - ignore shorter messages
  if (size < 1) {
    ara::log::LogError() << "Conversation::" << __func__ << " : the provided size is zero, returning nullptr!";
    return std::make_pair(IndicationResult::kIndicationOverflow, nullptr);  //< no further processing possible
  }

  // check the state
  if (current_state_ != ConversationState::kIdle && current_state_ != ConversationState::kFree) {
    ara::log::LogError()
        << "Conversation::::IndicateMessage: Current state not kIdle and not kFree, returning nullptr!";
    return std::make_pair(IndicationResult::kIndicationBusy,
                          nullptr);  //< when indicate message fails the resulting state is not changed
  }

  // when called in state kIdle or kFree create a new UDS message
  ConversationManager& conversation_mgr = GetConversationManager();
  UdsMessageProvider& uds_message_provider = conversation_mgr.GetUdsMessageProvider();
  const struct UdsMessageParameter uds_parameter = {GetSourceAddress(), conversation_mgr.GetTA(), target_address_type,
                                                    GetConnectionId()};
  ara::diag::udstransport::UdsMessage::Ptr uds_msg = uds_message_provider.Acquire(size, uds_parameter);
  if (uds_msg != nullptr) {
    // only change the state to kReceive when a UDS message was successfully created
    this->ChangeState(ConversationState::kReceive);
  }

  // return pair of kIndicationOk & valid UdsMessage unique pointer.
  return std::make_pair(IndicationResult::kIndicationOk, std::move(uds_msg));
}

void Conversation::HandleMessage(ara::diag::udstransport::UdsMessage::Ptr message) {
  ara::log::LogDebug() << "Conversation::" << __func__ << " called";

  // HandleMessage should be only called when the state is kReceive
  // and IndicateMessage was executed successfully before
  if (current_state_ != ConversationState::kReceive) {
    throw std::logic_error("Conversation::HandleMessage: Current state not kReceive!");
  }

  // Print UDS Message.
  std::string uds_msg_log = amsr::diag::udstransport::ToString(message, log_max_number_payload_bytes_);
  ara::log::LogDebug() << uds_msg_log;

  // Argument checks
  if (message == nullptr) {
    // when IndicateMessage was successfully executed this should never happen,
    // however it is possible so we throw an exception
    throw std::logic_error("Conversation::HandleMessage: Provided message is nullptr!");
  }

  ara::diag::udstransport::ByteVector& payload = message->GetPayload();
  if (payload.size() < 1) {
    // we need the SID for further processing (e. g. in case of a negative response)
    // also IndicateMessage will fail when the message size is not greater than zero
    throw std::logic_error("Conversation::HandleMessage: Payload size is zero!");
  }

  // special handling for functional tester present requests that matches the pattern 0x3E80
  if (IsFunctionalTesterPresentWithActiveSuppressBit(*message)) {
    ara::log::LogDebug() << "Conversation::" << __func__
                         << " : Functional tester present with suppress bit -> no further processing";
    ChangeState(ConversationState::kIdle);
    return;
  }

  // Store the SID (e. g. needed for SendNegativeResponse)
  SetRxSid(payload[0]);

  InitializeP2Timers();

  ChangeState(ConversationState::kProcess);
  GetMessageHandler().HandleMessage(std::move(message));

  ara::log::LogDebug() << "Conversation::" << __func__ << " : Message was forwarded to MessageHandler";
}

bool Conversation::IsFunctionalTesterPresentWithActiveSuppressBit(const ara::diag::udstransport::UdsMessage& message) {
  // when not a functional stop further processing
  if (message.GetTaType() != ara::diag::udstransport::UdsMessage::TargetAddressType::FUNCTIONAL) {
    return false;
  }

  const ara::diag::udstransport::ByteVector& payload = message.GetPayload();
  if (payload.size() != kTesterPresentRequestMsgSize) {
    return false;
  }

  if (payload[0] != kTesterPresentRequestSid) {
    return false;
  }

  if (payload[1] != kTesterPresentSubfunctionWithActiveSupressBit) {
    return false;
  }

  return true;
}

void Conversation::InitializeP2Timers() {
  // when this message is called a service ID has to be available!
  assert(rx_sid_.has_value());

  // Prepare response handling
  response_pending_counter_ = 0;
  response_pending_active_ = false;

  // Initializes response pending timer dependent to current session
  const ConversationManager& conversation_mgr = GetConversationManager();
  const DiagnosticServerConfiguration& diag_server_cfg = conversation_mgr.GetDiagnosticServerConfiguration();
  std::uint8_t session_id = GetActiveSession();
  if (diag_server_cfg.ContainsSessionInfo(session_id)) {
    SessionInfo sessionInfo = diag_server_cfg.GetSessionInfo(session_id);
    ara::log::LogDebug() << "Conversation::" << __func__ << " >> SessionInfo available: P2='"
                         << sessionInfo.p2_time.count() << "', P2star='" << sessionInfo.p2_star_time.count()
                         << "' [milliseconds]";
    p2_.SetOneShot(sessionInfo.p2_time);
    p2star_time_ = sessionInfo.p2_star_time;
  } else {
    ara::log::LogError() << "Conversation::" << __func__ << " >> No SessionInfo available for SessionId '"
                         << static_cast<std::uint32_t>(session_id) << "'. Using defaults: P2='"
                         << std::to_string(SessionInfo::kP2DefaultTime) << "', P2star='"
                         << std::to_string(SessionInfo::kP2StartDefaultTime) << "' [milliseconds]";
    std::chrono::milliseconds p2_default_timeout{SessionInfo::kP2DefaultTime};
    p2_.SetOneShot(p2_default_timeout);
    std::chrono::milliseconds p2star_default_timeout{SessionInfo::kP2StartDefaultTime};
    p2star_time_ = p2star_default_timeout;
  }

  p2_.Start();
}

bool Conversation::OnP2TimerElapsed(const vac::timer::Timer& owning_timer) {
  try {
    ara::log::LogDebug() << "Conversation::" << __func__ << " >> P2/P2star timer elapsed (response pending counter: '"
                         << response_pending_counter_ << "')";

    // critical section to synchronize response pending with positive response
    std::unique_lock<std::mutex> guard(finish_processing_mutex_);

    if (current_state_ == ConversationState::kProcess) {
      // check if limit of response pending is reached
      constexpr std::uint8_t kNoResponsePendingLimit = 0xFF;
      if (response_pending_counter_ < max_number_of_response_pending_ ||
          max_number_of_response_pending_ == kNoResponsePendingLimit) {
        // send response pending and increment counter
        SendResponsePendingMessage(guard);

        response_pending_counter_ += 1;

        p2_.SetOneShot(p2star_time_);
        p2_.Start();
      } else {
        ara::log::LogError() << "Conversation::" << __func__
                             << " : Sending of response pending interrupted: configured limit reached!";

        // TODO(PAASR-2388): stop message handler and call FinishProcessing (maybe done by MessageHandler::Cancel)
        // currently we take no actions (and use the P2 timeout of the tester)
      }
    } else {
      ara::log::LogError() << "Conversation::" << __func__ << " : Called in unexpected state '"
                           << ToString(current_state_) << "' -> no action!";
    }
  } catch (const std::exception& ex) {
    ara::log::LogError() << "Conversation::" << __func__ << " >> Exception: " << ex.what();
    response_pending_active_ = false;
  }

  return true;
}

void Conversation::StopP2Timer() {
  ara::log::LogDebug() << "Conversation::" << __func__ << " : Stopping P2 + P2* timers.";
  p2_.Stop();
  response_pending_active_ = false;
}

void Conversation::StartS3Timer() {
  std::chrono::milliseconds s3_timeout{SessionInfo::kS3Time};
  s3_.SetOneShot(s3_timeout);
  s3_.Start();
}

bool Conversation::OnS3TimerElapsed() {
  std::uint8_t session_id = GetActiveSession();
  if (session_id != SessionId::kDefault) {
    SetSessionId(SessionId::kDefault);
  }
  return true;
}

void Conversation::StopS3Timer() {
  ara::log::LogDebug() << "Conversation::" << __func__ << " : Stopping S3 timer.";
  s3_.Stop();
}

void Conversation::FinishProcessing(ara::diag::udstransport::UdsNegativeResponseCode nrc) {
  // check the state
  if (current_state_ != ConversationState::kProcess) {
    // when HandleMessage was successfully executed this should never happen,
    // however it is possible so we throw an exception
    throw std::logic_error("Conversation::FinishProcessing: Current state not kProcess!");
  }

  // critical section to synchronize response pending with negative response
  {
    std::unique_lock<std::mutex> guard(finish_processing_mutex_);

    StopP2Timer();

    ara::log::LogDebug() << "Conversation::FinishProcessing : sending negative response";
    // when this message is called a service ID has to be available!
    assert(rx_sid_.has_value());
    uint8_t request_sid = GetRxSid();
    SendNegativeResponse(request_sid, nrc, guard);
  }
}

void Conversation::FinishProcessing(ara::diag::udstransport::UdsMessage::Ptr message) {
  // Print UDS Message.
  std::string uds_msg_log = amsr::diag::udstransport::ToString(message, log_max_number_payload_bytes_);
  ara::log::LogDebug() << "Conversation::" << __func__ << " : " << uds_msg_log;

  // check the state
  if (current_state_ != ConversationState::kProcess) {
    throw std::logic_error("Conversation::FinishProcessing: Current state not kProcess!");
  }

  // critical section to synchronize response pending with positive response
  {
    std::unique_lock<std::mutex> lock(finish_processing_mutex_);
    StopP2Timer();

    if (suppress_positive_response_) {
      ara::log::LogDebug() << "Conversation::" << __func__ << " : positive response suppressed";
      ChangeState(ConversationState::kIdle);
    } else {
      ara::log::LogDebug() << "Conversation::" << __func__ << " : sending positive response";
      // Transmit positive response to tester, that will finally also trigger a call of SwitchToIdleState() by
      // TransmitConfirmation. Because Transmit (the DoIP/Transport layer implementation) might try to call
      // us in a reentrant way we have to free the critical section before we left the context of this class
      Transmit(std::move(message), lock);
    }
  }
}

void Conversation::SendResponsePendingMessage(std::unique_lock<std::mutex>& tx_lock) {
  // activate sending responses again when a response pending message is send to the tester
  if (suppress_positive_response_) {
    suppress_positive_response_ = false;
    ara::log::LogInfo() << "Conversation::" << __func__
                        << " : deactivated suppression of positive response, because of response pending is send";
  }

  // when this message is called a service ID has to be available!
  assert(rx_sid_.has_value());
  uint8_t request_sid = GetRxSid();

  ara::log::LogInfo() << "Conversation::" << __func__ << " : Sending response pending to tester";

  // flag is set to false by TransmitConfirmation and in the exception handler of OnP2TimerElapsed
  response_pending_active_ = true;
  SendNegativeResponse(request_sid,
                       ara::diag::udstransport::UdsNegativeResponseCode::kRequestCorrectlyReceived_responsePending,
                       tx_lock);
}

void Conversation::SendNegativeResponse(uint8_t request_sid, ara::diag::udstransport::UdsNegativeResponseCode nrc,
                                        std::unique_lock<std::mutex>& tx_lock) {
  ara::log::LogDebug() << "Conversation::" << __func__ << " : SID='" << static_cast<std::uint32_t>(request_sid)
                       << "', NRC='" << static_cast<std::uint32_t>(nrc) << "'";

  // Get the BufferProvider and invert source/target address
  ConversationManager& conversation_mgr = GetConversationManager();
  UdsMessageProvider& uds_msg_provider = conversation_mgr.GetUdsMessageProvider();
  const struct UdsMessageParameter uds_parameter = {conversation_mgr.GetTA(), source_address_,
                                                    ara::diag::udstransport::UdsMessage::TargetAddressType::PHYSICAL,
                                                    channel_id_};
  ara::diag::udstransport::UdsMessage::Ptr message =
      uds_msg_provider.Acquire(kLengthNegativeResponsePayload, uds_parameter);

  if (message == nullptr) {
    // SendNegativeResponse could be called when an exception (e. g. raised by ARA COM) is handled by
    // the MessageHandler, so do NOT throw exceptions here. Instead log the error and
    // let a P2 timeout happen - which also will be an error that is monitored by the tester (but less dangerous)
    ara::log::LogError() << "Conversation::" << __func__ << " : No UdsMessage available for negative response.";
    ChangeState(ConversationState::kIdle);
  } else {
    // Fill the payload of negative response and transmit
    ara::diag::udstransport::ByteVector& payload = message->GetPayload();
    payload[0] = kNegativeResponseSID;
    payload[1] = request_sid;
    payload[2] = static_cast<std::underlying_type<ara::diag::udstransport::UdsNegativeResponseCode>::type>(nrc);
    Transmit(std::move(message), tx_lock);
  }
}

void Conversation::Transmit(ara::diag::udstransport::UdsMessage::Ptr message, std::unique_lock<std::mutex>& tx_lock) {
  // Prepare and send the response (positive or negative) to the tester

  ara::log::LogDebug() << "Conversation::" << __func__ << " called (response_pending_active='"
                       << std::to_string(response_pending_active_) << "')";

  // Get the conversation Manager.
  ConversationManager& conversation_manager = GetConversationManager();

  // Get the transport protocol Manager.
  amsr::diag::udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_manager =
      conversation_manager.GetUdsTransportProtocolManager();

  // Get the transport protocol Handler.
  ara::diag::udstransport::UdsMessage::GlobalChannelIdentifier global_channel_id =
      message->GetGlobalChannelIdentifier();
  ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id = std::get<0>(global_channel_id);
  ara::diag::udstransport::UdsTransportProtocolHandler* uds_transport_protocol_handler_ptr =
      uds_transport_protocol_manager.GetUdsTransportProtocolHandler(handler_id);

  // If transport protocol handler does not exist, then throw.
  if (uds_transport_protocol_handler_ptr == nullptr) {
    throw std::runtime_error("Transport protocol handler cannot be found.");
  }

  if (response_pending_active_ == false) {
    ChangeState(ConversationState::kTransmit);
    ara::log::LogDebug() << "Conversation::" << __func__ << " : Sending message (Conversation state now kTransmit)";
  }
  tx_lock.unlock();
  uds_transport_protocol_handler_ptr->Transmit(std::move(message), std::get<1>(global_channel_id));
}

void Conversation::TransmitConfirmation(ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult result) {
  // check state
  if ((!response_pending_active_ && current_state_ != ConversationState::kTransmit) ||
      (response_pending_active_ && current_state_ != ConversationState::kProcess)) {
    throw std::logic_error("Conversation::TransmitConfirmation: Current state not kTransmit!");
  }

  ara::log::LogDebug() << "Conversation::" << __func__ << " called (response_pending_active='"
                       << std::to_string(response_pending_active_) << "')";

  // critical section to synchronize response pending with positive response
  {
    std::lock_guard<std::mutex> guard(finish_processing_mutex_);

    // change the conversation state only when response pending processing is not active
    if (response_pending_active_) {
      response_pending_active_ = false;  //< reset the flag
    } else {
      ChangeState(ConversationState::kIdle);
    }
  }

  ara::log::LogDebug() << "Conversation::" << __func__ << " finished (response_pending_active='"
                       << std::to_string(response_pending_active_) << "')";
}

void Conversation::ChangeState(ConversationState state) {
  ara::log::LogDebug() << "Conversation::" << __func__ << " called with " << ToString(state);

  current_state_ = state;

  if (state == ConversationState::kIdle) {
    StartS3Timer();

    // reset suppress bit, so the next message is processed with a clean setup
    suppress_positive_response_ = false;

    // TODO(PAASR-1864): Here, currently post-processing is triggered directly - that might be changed in the future

    if (GetSessionId() == SessionId::kDefault) {
      // when the conversation is currently in the default session state the conversation
      // can be recycled by the ConversationManager so the current state is changed to kFree
      ara::log::LogDebug() << "Conversation::" << __func__ << " : current session state is default "
                                                              "so Conversation state is changed to kFree)";
      current_state_ = ConversationState::kFree;
    }
  }
}

std::string Conversation::ToString(ConversationState state) {
  switch (state) {
    case ConversationState::kFree:
      return "kFree";
    case ConversationState::kIdle:
      return "kIdle";
    case ConversationState::kProcess:
      return "kProcess";
    case ConversationState::kReceive:
      return "kReceive";
    case ConversationState::kTransmit:
      return "kTransmit";
    default:
      return "<unknown ConversationState>";
  }
}

data::DidManager& Conversation::GetDidManager() const {
  return conversation_manager_.GetDiagnosticServer().GetDidManager();
}

proxys::ProxysPool& Conversation::GetProxysPool() const {
  return conversation_manager_.GetDiagnosticServer().GetProxysPool();
}

std::size_t Conversation::log_max_number_payload_bytes_ = amsr::diag::configuration::kDefaultLogMaxNumberPayloadBytes;

access::ObservableAccessState& Conversation::GetAccessState(access::AccessCategory category) {
  using access::AccessCategory;
  switch (category) {
    case AccessCategory::kSession:
      return session_;
    case AccessCategory::kSecurityLevel:
      return security_level_;
    default:
      // other categories are not implemented
      assert(false);
  }
}

const access::ObservableAccessState& Conversation::GetAccessState(access::AccessCategory category) const {
  using access::AccessCategory;
  switch (category) {
    case AccessCategory::kSession:
      return session_;
    case AccessCategory::kSecurityLevel:
      return security_level_;
    default:
      // other categories are not implemented
      assert(false);
  }
}

std::uint8_t Conversation::GetActiveSession() const { return GetAccessState(access::AccessCategory::kSession).Get(); }

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr
