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
/**        \file  tester_present_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>

#include "ara/log/logging.hpp"
#include "common/multi_byte_type.h"
#include "server/conversation/conversation_manager.h"
#include "udstransport/uds_message.h"

#include "server/service/processor/tester_present_processor.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

TesterPresentProcessor::TesterPresentProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message,
                                               ServiceProcessingContext& processing_context,
                                               vac::memory::SmartObjectPoolDeleterContext* deleter_context)
    : ServiceProcessorBase(std::move(uds_message), processing_context, deleter_context) {
  // IMPORTANT: uds_message is moved (!!!) to the member uds_message_ by ServiceProcessorBase
  if (uds_message_ == nullptr) {
    throw std::invalid_argument("TesterPresentProcessor::ctor : Provided UDS message not valid (nullptr)!");
  }

  // when called with an invalid request (at least the SID has to be available) throw an exception
  const ara::diag::udstransport::ByteVector& raw_msg = uds_message_->GetPayload();
  if (raw_msg.size() < 1) {
    throw std::invalid_argument("TesterPresentProcessor::ctor : Provided UDS message not valid (empty message)!");
  }

  // when called with an invalid SID we shouldn't be called
  if (raw_msg.at(0) != kTesterPresentRequestSid) {
    throw std::invalid_argument(
        "TesterPresentProcessor::ctor : Provided UDS message not valid (invalid SID: not 0x3E)!");
  }
}

ProcessingStatus TesterPresentProcessor::HandleMessage() {
  assert(uds_message_ != nullptr);
  // check minimum length
  if (CheckRequestMinimumLength(uds_message_) == false) {
    // message is too short - e. g. sub-function missing -> negative response with NRC 0x13
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
    processing_context_.FinishProcessing(UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
    return ProcessingStatus::kDone;
  }

  // handle request message (size and content is valid) - TP request has no data only the subfunction
  return HandleSubfunction();
}

ProcessingStatus TesterPresentProcessor::HandleSubfunction() {
  // this method is only called when the SID is correct and a sub-function is available
  assert(uds_message_ != nullptr);

  using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
  using ByteVector = ara::diag::udstransport::ByteVector;
  const ByteVector& raw_msg = uds_message_->GetPayload();
  assert(raw_msg.size() > 1);
  assert(raw_msg.at(0) == kTesterPresentRequestSid);

  uint8_t subfunction = raw_msg.at(1);

  // 1. check the SPRMIB bit and store the state to the conversation
  if ((subfunction & kSupressPositiveResponseBit) == kSupressPositiveResponseBit) {
    processing_context_.SetSuppressBit(true);
    subfunction = static_cast<uint8_t>(~kSupressPositiveResponseBit & subfunction);  //< remove
    assert((subfunction & kSupressPositiveResponseBit) == 0x00);
    // NOTICE: the positive response is prepared also for this case, because the implementation of the ProcessingContext
    // decides if the response is sent
  }

  // 2. check sub-function value
  if (IsValidSubfunction(subfunction) == false) {
    ara::log::LogError() << "TesterPresentProcessor::HandleSubfunction: Message has invalid sub-function!";
    processing_context_.FinishProcessing(UdsNegativeResponseCode::kSubFunctionNotSupported);
    return ProcessingStatus::kDone;
  }

  // 3. check if sub-function is OK in current session
  // TODO(PAASR-1870): a check if the sub-function is supported in the current session has to be added in the future

  // 4. check correct length
  if (raw_msg.size() != kTesterPresentRequestMsgSize) {
    ara::log::LogError() << "TesterPresentProcessor::HandleSubfunction: Request has invalid size (too long)!";
    processing_context_.FinishProcessing(UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
    return ProcessingStatus::kDone;
  }

  FinishWithPositiveResponse(subfunction);
  return ProcessingStatus::kDone;
}

void TesterPresentProcessor::FinishWithPositiveResponse(uint8_t subfunction) {
  // prepare response message
  using UdsMessage = ara::diag::udstransport::UdsMessage;
  UdsMessage::Ptr response_message = processing_context_.AcquireResponseBuffer(kTesterPresentResponseMsgSize);

  if (response_message == nullptr) {
    ara::log::LogError() << "TesterPresentProcessor::FinishWithPositiveResponse: Couldn't create response message "
                            "(AcquireResponseBuffer returned nullptr)!";
    // kResponseTooLong (0x14) shall be  also used when "(...) the response message exceeds the server buffer
    // size allocated for that purpose" [Table A.1 of ISO 14229-1:2013(E)]
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
    processing_context_.FinishProcessing(UdsNegativeResponseCode::kResponseTooLong);
    return;
  }

  using ByteVector = ara::diag::udstransport::ByteVector;
  ByteVector payload_response_message = response_message->GetPayload();
  assert(payload_response_message.size() == kTesterPresentResponseMsgSize);
  WritePositiveResponseToPayload(payload_response_message, subfunction);

  processing_context_.FinishProcessing(std::move(response_message));
}

bool TesterPresentProcessor::CheckRequestMinimumLength(const ara::diag::udstransport::UdsMessage::Ptr& uds_message) {
  // the ctor checks if the provided message is not NULL and has enough bytes to work with the message
  assert(uds_message != nullptr);
  assert(uds_message->GetPayload().size() > 0);

  const ara::diag::udstransport::ByteVector& raw_msg = uds_message->GetPayload();
  const size_t msg_size = raw_msg.size();
  if (msg_size < kTesterPresentRequestMsgSize) {
    ara::log::LogError() << "TesterPresentProcessor::CheckRequestMinimumLength: Message has invalid size '"
                         << static_cast<int>(msg_size) << "'!";
    return false;
  }

  return true;
}

bool TesterPresentProcessor::IsValidSubfunction(uint8_t subfunction) {
  if (subfunction != 0x00 && subfunction != 0x80) {
    // UDS only defines 0x80 and 0x00 for the sub-function
    return false;
  }

  return true;
}

void TesterPresentProcessor::WritePositiveResponseToPayload(ara::diag::udstransport::ByteVector& payload,
                                                            std::uint8_t sub_function) {
  assert(payload.size() == TesterPresentProcessor::kTesterPresentResponseMsgSize);
  payload[0] = TesterPresentProcessor::kTesterPresentPositveResponseSid;
  assert(sub_function == 0x00);
  payload[1] = sub_function;
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
