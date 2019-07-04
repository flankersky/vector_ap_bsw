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
/**        \file  write_did_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/service/processor/write_did_processor.h"
#include "ara/log/logging.hpp"
#include "common/multi_byte_type.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

ProcessingStatus WriteDidProcessor::HandleMessage() {
  ara::log::LogDebug() << __func__;
  switch (handle_message_status_) {
    case HandleMessageStatus::kFirstRun: {
      // First call to HandleMessage.
      ara::log::LogDebug() << __func__ << ": first run.";
      ProcessingStatus status = this->CheckRequestAndPrepareWriting();
      if (status == ProcessingStatus::kDone) {
        handle_message_status_ = HandleMessageStatus::kDone;
        return ProcessingStatus::kDone;
      }
      handle_message_status_ = HandleMessageStatus::kContinue;
      this->StartWritePollingTask();
      break;
    }
    case HandleMessageStatus::kContinue: {
      // Other Calls to HandleMessage.
      ara::log::LogDebug() << __func__ << ": continue.";
      assert(write_did_polling_task_ != nullptr);
      write_did_polling_task_->Continue();
      break;
    }
    case HandleMessageStatus::kDone:
      return ProcessingStatus::kDone;
    default:
      throw std::runtime_error(
          "WriteDidProcessor::HandleMessage : Invalid HandleMessage status. This case is not properly handled by the "
          "implementation and must not be reached.");
  }
  assert(write_did_polling_task_);
  ara::log::LogDebug() << "WriteDidProcessor::" << __func__ << ": polling status "
                       << amsr::diag::common::ToString(write_did_polling_task_->GetStatus());
  return EvaluatePollingTaskStatus(write_did_polling_task_->GetStatus());
}

ProcessingStatus WriteDidProcessor::EvaluatePollingTaskStatus(const common::PollingStatus polling_status) {
  // Evaluate task status.
  switch (polling_status) {
    case common::PollingStatus::kComplete:
      processing_context_.FinishProcessing(std::move(uds_message_pos_response_));
      handle_message_status_ = HandleMessageStatus::kDone;
      return ProcessingStatus::kDone;
    case common::PollingStatus::kPending:
      return ProcessingStatus::kNotDone;
    case common::PollingStatus::kCanceled:
      handle_message_status_ = HandleMessageStatus::kDone;
      return ProcessingStatus::kDone;
    case common::PollingStatus::kFailed:
      handle_message_status_ = HandleMessageStatus::kDone;
      return FinishServiceProcessing(write_did_polling_task_->GetNRC());
    case common::PollingStatus::kNotStarted:
      // This case is not expected to be called, but does not harm => error log.
      ara::log::LogError() << "WriteDidProcessor::" << __func__ << ": Polling Task could not be started.";
      return ProcessingStatus::kNotDone;
    default:
      throw std::runtime_error("Invalid polling status: " + std::to_string(write_did_polling_task_->GetStatus()) + "!");
  }
}

ProcessingStatus WriteDidProcessor::CheckRequestAndPrepareWriting() {
  ara::log::LogDebug() << "WriteDidProcessor::" << __func__;
  // First Call to HandleMessage.
  const ara::diag::udstransport::ByteVector payload_view = uds_message_->GetPayload();

  // Check min. length.
  if (payload_view.size() < kMinSizePayload) {
    return FinishServiceProcessing(
        ara::diag::udstransport::UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
  }

  // Get the DID
  data::DidInfo::Did did = this->AcquireAvailableDid();

  // Get the Did Manager and call CreateOpHandlerIfDidWriteable.
  data::DidManager& did_manager = processing_context_.GetDidManager();

  // TODO(visbft): Implement the move operator of DataIdentifier in order to avoid use of data_identifier_temp.
  vac::memory::optional<data::operationhandler::WriteDidOperationHandler> did_operation_handler_temp =
      did_manager.CreateOpHandlerIfDidWriteable(did, processing_context_);

  // Finish processing if no DataIdentifier is available.
  if (!did_operation_handler_temp.has_value()) {
    ara::log::LogInfo() << "WriteDidProcessor::" << __func__ << ": DID '" << did << " ' not supported!";
    return FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kRequestOutOfRange);
  }

  ara::log::LogDebug() << "WriteDidProcessor::" << __func__ << ": ";
  did_operation_handler_.emplace(std::move(did_operation_handler_temp.value()));

  // Check if total length of payload is correct.
  if (payload_view.size() != GetExpectedRequestPayloadSize()) {
    return FinishServiceProcessing(
        ara::diag::udstransport::UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
  }

  // Check the conditions and finish processing if they are not full filled.
  ara::log::LogDebug() << "WriteDidProcessor::" << __func__ << ": calling CheckConditions of DataIdentifier.";
  ara::diag::udstransport::UdsNegativeResponseCode nrc = did_operation_handler_->CheckConditions(processing_context_);
  if (nrc != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
    ara::log::LogError() << "WriteDidProcessor::" << __func__ << ": Condition check on some DID failed.";
    return FinishServiceProcessing(nrc);
  }

  // Since Did is available, a positive response has to be prepared.
  return PreparePositiveResponseUDSMessage();
}

void WriteDidProcessor::StartWritePollingTask() {
  // Writing can now be started.
  ara::log::LogDebug() << "WriteDidProcessor::" << __func__;
  assert(did_operation_handler_.has_value());
  ara::diag::udstransport::ByteVector request_payload_view = uds_message_->GetPayload();
  vac::container::const_array_view<std::uint8_t> data_buffer{request_payload_view.begin() + kSidDidSizeInBytes,
                                                             did_operation_handler_->GetMaxSize()};
  write_did_polling_task_ = &did_operation_handler_->StartWriting(data_buffer, processing_context_);
}

data::DidInfo::Did WriteDidProcessor::AcquireAvailableDid() {
  const ara::diag::udstransport::ByteVector payload_view = uds_message_->GetPayload();
  const std::uint8_t high_byte = payload_view.at(1);
  const std::uint8_t low_byte = payload_view.at(2);
  data::DidInfo::Did result = 0;
  amsr::diag::common::SetByte(result, low_byte, 0);
  amsr::diag::common::SetByte(result, high_byte, 1);

  return result;
}

std::size_t WriteDidProcessor::GetResponseSizeForAvailableDid() {
  assert(did_operation_handler_.has_value());
  std::size_t response_size = 1;  // 1 Byte for SID
  constexpr std::size_t did_size = kDidSizeInBytes;
  response_size += did_size;
  return response_size;
}

std::size_t WriteDidProcessor::GetExpectedRequestPayloadSize() {
  assert(did_operation_handler_.has_value());
  std::size_t response_size = 1;  // 1 Byte for SID
  constexpr std::size_t did_size = kDidSizeInBytes;
  response_size += did_size + did_operation_handler_->GetMaxSize();
  return response_size;
}

ProcessingStatus WriteDidProcessor::PreparePositiveResponseUDSMessage() {
  ara::log::LogDebug() << __func__;
  const ara::diag::udstransport::ByteVector payload_view_request = uds_message_->GetPayload();
  // Get an available UDS Message to send the positive response.
  std::size_t size_buffer = GetResponseSizeForAvailableDid();
  uds_message_pos_response_ = processing_context_.AcquireResponseBuffer(size_buffer);
  if (uds_message_pos_response_ == nullptr) {
    // No message is available, call FinishProcessing.
    return FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kResponseTooLong);
  }
  // Fill the UDS message response.
  ara::diag::udstransport::ByteVector payload_view_response = uds_message_pos_response_->GetPayload();
  payload_view_response.at(0) = kWriteDidPositiveResponseSid;
  payload_view_response.at(1) = payload_view_request.at(1);
  payload_view_response.at(2) = payload_view_request.at(2);
  return ProcessingStatus::kNotDone;
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
