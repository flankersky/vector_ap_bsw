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
/**        \file  session_control_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <utility>

#include "ara/log/logging.hpp"
#include "common/multi_byte_type.h"
#include "server/conversation/conversation_manager.h"
#include "server/service/processor/session_control_processor.h"
#include "udstransport/uds_message.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

SessionControlProcessor::SessionControlProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message,
                                                 Configuration processor_config,
                                                 vac::memory::SmartObjectPoolDeleterContext* deleter_context)
    : ServiceProcessorBase(std::move(uds_message), config_.uds_response_context, deleter_context),
      config_(processor_config) {
  assert(uds_message_->GetPayload()[0] == SessionControlProcessor::kDiagnosticSessionControlSid);
}

ProcessingStatus SessionControlProcessor::HandleMessage() {
  using UdsMessage = ara::diag::udstransport::UdsMessage;
  ara::log::LogDebug() << "SessionServiceProcessor::" << __func__;
  assert(uds_message_ != nullptr);
  if (this->CheckUdsMessageLength()) {
    const std::uint8_t subfunction = uds_message_->GetPayload()[1];
    if (config_.session_config.ContainsSessionInfo(subfunction)) {
      UdsMessage::Ptr response_message = CreatePositiveResponseMessage(subfunction);
      SetNewSessionInConversation(subfunction);
      config_.uds_response_context.FinishProcessing(std::move(response_message));
    } else {
      config_.uds_response_context.FinishProcessing(
          ara::diag::udstransport::UdsNegativeResponseCode::kSubFunctionNotSupported);
    }
  } else {
    config_.uds_response_context.FinishProcessing(
        ara::diag::udstransport::UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
  }
  return ProcessingStatus::kDone;
}

bool SessionControlProcessor::CheckUdsMessageLength() const { return uds_message_->GetPayload().size() == 2; }

ara::diag::udstransport::UdsMessage::Ptr SessionControlProcessor::CreatePositiveResponseMessage(
    std::uint8_t subfunction) const {
  using UdsMessage = ara::diag::udstransport::UdsMessage;
  constexpr std::size_t kPositiveResponsePayloadSize = 6;
  UdsMessage::Ptr response_message = config_.uds_response_context.AcquireResponseBuffer(kPositiveResponsePayloadSize);
  if (response_message == nullptr) {
    ara::log::LogError() << "SessionServiceProcessor::" << __func__ << "UDS response buffer could not be acquired";
    throw std::runtime_error("UDS response buffer could not be acquired!");
  }
  ara::diag::udstransport::ByteVector payload_response_message = response_message->GetPayload();

  const SessionInfo session_info = config_.session_config.GetSessionInfo(subfunction);
  // p2 resolution is 1ms
  const std::uint16_t p2 = ConvertToMilliseconds(session_info.p2_time);
  // p2_star resolution is 10ms
  const std::uint16_t p2_star = ConvertToTensOfMilliseconds(session_info.p2_star_time);
  SessionControlProcessor::WritePositiveResponseToPayload(payload_response_message, subfunction, p2, p2_star);
  return std::move(response_message);
}

void SessionControlProcessor::SetNewSessionInConversation(std::uint8_t subfunction) {
  conversation::access::ObservableAccessState& session =
      config_.conversation_access_state.GetAccessState(conversation::access::AccessCategory::kSession);
  session.Set(subfunction);
}

void SessionControlProcessor::WritePositiveResponseToPayload(ara::diag::udstransport::ByteVector& payload,
                                                             std::uint8_t sub_function, std::uint16_t p2,
                                                             std::uint16_t p2_star) {
  payload[0] = SessionControlProcessor::kPositveResponseSid;
  payload[1] = sub_function;
  payload[2] = amsr::diag::common::GetByte(p2, 1);
  payload[3] = amsr::diag::common::GetByte(p2, 0);
  payload[4] = amsr::diag::common::GetByte(p2_star, 1);
  payload[5] = amsr::diag::common::GetByte(p2_star, 0);
}

std::uint16_t SessionControlProcessor::ConvertToTensOfMilliseconds(std::chrono::milliseconds time) {
  assert(time.count() % 10 == 0);         // must be integer multiple of tens of milliseconds
  assert(time.count() <= (0xFFFF * 10));  // tens of milliseconds value must fit into 2 Bytes
  return static_cast<std::uint16_t>(time.count() / 10);
}

std::uint16_t SessionControlProcessor::ConvertToMilliseconds(std::chrono::milliseconds time) {
  assert(time.count() <= 0xFFFF);  // value must fit into 2 Bytes
  return static_cast<std::uint16_t>(time.count());
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
