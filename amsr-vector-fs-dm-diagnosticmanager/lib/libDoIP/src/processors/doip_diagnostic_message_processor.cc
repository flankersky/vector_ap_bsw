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
/**        \file  doip_diagnostic_message_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <stdexcept>
#include <tuple>
#include <utility>

#include "doip_diagnostic_message_processor.h"

#include "doip_transport_protocol_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

bool DoIPDiagnosticMessageProcessor::IsValidPayloadLength(uint32_t length) const {
  return length >= doip::kDiagnosticMessageMinimumLength;
}

void DoIPDiagnosticMessageProcessor::HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const {
  ara::log::LogDebug() << "DoIPDiagnosticMessageProcessor::" << __func__;
  ara::diag::udstransport::UdsMessage::Ptr uds_message = channel.ReleaseUdsMessage();
  if (uds_message == nullptr) {
    ara::log::LogError() << __func__ << ": UDS Message is not available.";
    throw std::runtime_error("HandleMessage call expects a valid UdsMessage stored in Channel.");
  }
  std::unique_ptr<DoIPMessage> response_msg = channel.CreateDiagnosticMessageResponse(
      uds_message->GetSa(), uds_message->GetTa(), DiagnosticMessageResponseCode::kConfirmationAck);
  channel.SendResponse(*response_msg);
  ara::diag::udstransport::UdsTransportProtocolMgr& transport_protcol_mgr =
      channel.GetTcpHandler().GetConnectionProvider().GetUdsTransportProtocolMgr();
  transport_protcol_mgr.HandleMessage(std::move(uds_message));
}

ActionUponDiagnosticMessageHandler DoIPDiagnosticMessageProcessor::DiagnosticMessagePreprocessor(const DoIPMessage& msg,
                                                                                                 DoIPChannel& channel) {
  ara::log::LogDebug() << "Enter DiagnosticMessagePreprocessor in DiagnosticMessageProcessor";
  // Extract the SA firstly.
  uint16_t source_address = msg.payload[1];
  amsr::diag::common::SetByte(source_address, msg.payload[0], 1);
  // Extract the TA firstly.
  uint16_t target_address = msg.payload[3];
  amsr::diag::common::SetByte(target_address, msg.payload[2], 1);
  DiagnosticMessageResponseCode response_code = DiagnosticMessageResponseCode::kReserved;

  ActionUponDiagnosticMessageHandler action = ActionUponDiagnosticMessageHandler::kDiscardMessage;

  // Check for the state of the channel if it is registered state or not.
  // If not, so discard the message.
  if (channel.GetState()->GetHandle() != tcp::StateHandle::kRegisteredRoutingActive) {
    return action;
  }

  // Implementation of the diagnostic message handler.
  if (source_address != channel.GetSourceAddress()) {
    // [DoIP-070] send negative response & close the channel.
    response_code = DiagnosticMessageResponseCode::kInvalidSA;
    action = ActionUponDiagnosticMessageHandler::kCloseSocket;
  } else if (false /* target_address is unknown*/) {
    // TODO(PAASR-1661) : Implement checking the TA is known.
    // [DoIP-071]
    response_code = DiagnosticMessageResponseCode::kUnknownTA;
  } else if (!DoIPDiagnosticMessageProcessor::AskForUdsMessageStorage(
                 msg, channel, target_address) /* Check if the message_size is OK for the target network*/) {
    // [DoIP-072]
    response_code = DiagnosticMessageResponseCode::kMessageTooLarge;
  } else if (false /* destination buffer is smaller than message size*/) {
    // [DoIP-073]
    response_code = DiagnosticMessageResponseCode::kOutOfMemory;
  } else if (false /* Routing to TA is unreachable*/) {  // This is optional as this is a Gateway functionality.
    // [DoIP-103]
    response_code = DiagnosticMessageResponseCode::kTargetUnreachable;
  } else if (false /* While passing the message the target network/transport layer is unknown*/) {
    // This is optional as this is a Gateway functionality.
    // [DoIP-107]
    response_code = DiagnosticMessageResponseCode::kUnknownnetwork;
  } else {
    // [DoIP-67]
    response_code = DiagnosticMessageResponseCode::kConfirmationAck;
    action = ActionUponDiagnosticMessageHandler::kProceedWithMessage;
  }
  if (response_code != DiagnosticMessageResponseCode::kConfirmationAck) {
    std::unique_ptr<DoIPMessage> response_msg =
        channel.CreateDiagnosticMessageResponse(source_address, target_address, response_code);
    channel.SendResponse(*response_msg);
  }
  return action;
}

bool DoIPDiagnosticMessageProcessor::AskForUdsMessageStorage(const DoIPMessage& msg, DoIPChannel& channel,
                                                             uint16_t target_address) {
  ara::log::LogDebug() << "DoIPDiagnosticMessageProcessor::AskForUdsMessageStorage";
  ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier global_channel_id(
      channel.GetTcpHandler().GetConnectionProvider().GetHandlerID(), channel.GetChannelId());

  ara::diag::udstransport::UdsTransportProtocolMgr& transport_protcol_mgr =
      channel.GetTcpHandler().GetConnectionProvider().GetUdsTransportProtocolMgr();
  // TODO(PAASR-2053): Replace the following nullptr by meta-info provided by request UdsMessage
  IndicationPair transport_protocol_mgr_indication = transport_protcol_mgr.IndicateMessage(
      channel.GetSourceAddress(), target_address, DoIPDiagnosticMessageProcessor::GetTaType(target_address),
      global_channel_id, msg.payload_length - doip::kSaTaSize, nullptr);
  ara::diag::udstransport::UdsMessage::Ptr uds_message = std::move(transport_protocol_mgr_indication.second);
  if (uds_message != nullptr) {
    channel.SetUdsMessage(std::move(uds_message));
    return true;
  } else {
    return false;
  }
}

ara::diag::udstransport::UdsMessage::TargetAddressType DoIPDiagnosticMessageProcessor::GetTaType(
    uint16_t target_address) {
  if ((target_address >= kBeginFunctionalAddress) && (target_address <= kEndFunctionalAddress)) {
    return ara::diag::udstransport::UdsMessage::TargetAddressType::FUNCTIONAL;
  } else {
    return ara::diag::udstransport::UdsMessage::TargetAddressType::PHYSICAL;
  }
}

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
