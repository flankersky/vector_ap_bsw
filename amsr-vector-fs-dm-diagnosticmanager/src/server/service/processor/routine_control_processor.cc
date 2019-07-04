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
/**        \file  routine_control_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "ara/log/logging.hpp"
#include "common/multi_byte_type.h"
#include "routine_control_processor.h"
#include "vac/container/array_view.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
ProcessingStatus RoutineControlProcessor::HandleMessage() {
  switch (handle_message_status_) {
    case HandleMessageStatus::kFirstRun: {
      ara::log::LogInfo() << "RoutineControlProcessor::" << __func__ << ": kFirstRun";
      ProcessingStatus status = HandleMessageFirstRun();
      if (status == ProcessingStatus::kDone) {
        handle_message_status_ = HandleMessageStatus::kDone;
        return status;
      }
      status = StartPollingTask();
      if (status == ProcessingStatus::kDone) {
        handle_message_status_ = HandleMessageStatus::kDone;
        return status;
      }
      handle_message_status_ = HandleMessageStatus::kContinue;
      break;
    }
    case HandleMessageStatus::kContinue: {
      ara::log::LogInfo() << "RoutineControlProcessor::" << __func__ << ": kContinue";
      assert(routine_polling_task_);
      routine_polling_task_->Continue();
      break;
    }
    case HandleMessageStatus::kDone: {
      break;
    }
    default: {
      throw std::runtime_error(
          "RoutineControlProcessor::HandleMessage : Invalid HandleMessage status. This case is not properly handled by "
          "the implementation and must not be reached.");
    }
  }
  assert(routine_polling_task_);
  ara::log::LogDebug() << "RoutineControlProcessor::" << __func__ << ": polling status "
                       << amsr::diag::common::ToString(routine_polling_task_->GetStatus());
  return EvaluatePollingTaskStatus(routine_polling_task_->GetStatus());
}

ProcessingStatus RoutineControlProcessor::HandleMessageFirstRun() {
  ara::log::LogDebug() << "RoutineControlProcessor::" << __func__;
  assert(uds_message_->GetPayload().size() >= 1);
  assert(uds_message_->GetPayload()[0] == 0x31);

  // Do the minimum length check.
  const ara::diag::udstransport::ByteVector& request_payload = uds_message_->GetPayload();
  if (request_payload.size() < kMinLengthRoutineRequest) {
    ara::log::LogError() << "RoutineControlProcessor::" << __func__ << ": payload length is too small.";
    return FinishServiceProcessing(
        ara::diag::udstransport::UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
  }

  // TODO(PAASR-2313): Terminate implementation of routine control CheckConditions.

  // Acquire Rid and get Routine
  handler::routine::Rid rid = AcquireAvailableRid();
  vac::memory::optional<handler::routine::Routine> routine_temp = rid_manager_.CreateRoutineIfAvailable(rid);
  if (!routine_temp.has_value()) {
    ara::log::LogInfo() << "RoutineControlProcessor::" << __func__ << ": RID '" << rid << "' not supported!";
    return FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kRequestOutOfRange);
  }

  // Check conditions on routine level.
  ara::diag::udstransport::UdsNegativeResponseCode nrc = routine_temp->CheckConditions(processing_context_);
  if (nrc != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
    ara::log::LogInfo() << "RoutineControlProcessor::" << __func__
                        << ": Conditions for routine control are not correct. Received NRC = " << std::hex
                        << static_cast<std::uint8_t>(nrc);
    return FinishServiceProcessing(nrc);
  }

  // Get the routine operation handler.
  handler::routine::RoutineOperationType operation = GetOperation();
  // TODO(visbft): Implement the move operator of RoutineOperationHandler in order to avoid use of
  // routine_operation_handler_temp .
  OptionalRoutineOperationHandler routine_operation_handler_temp =
      routine_temp->CreateOperationHandlerIfAvailable(operation);
  if (!routine_operation_handler_temp.has_value()) {
    ara::log::LogInfo() << "RoutineControlProcessor::" << __func__ << ": operation '"
                        << static_cast<std::uint16_t>(operation) << "' not supported!";
    return FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kSubFunctionNotSupported);
  }
  routine_operation_handler_.emplace(std::move(routine_operation_handler_temp.value()));

  // Check the conditions and finish processing if they are not full filled.
  ara::log::LogInfo() << "RoutineControlProcessor::" << __func__ << ": operation '"
                      << static_cast<std::uint16_t>(operation) << "' checking conditions!";
  nrc = routine_operation_handler_->CheckConditions(processing_context_, request_payload);
  if (nrc != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
    ara::log::LogInfo() << "RoutineControlProcessor::" << __func__
                        << ": Conditions for routine control are not correct. Received NRC = " << std::hex
                        << static_cast<std::uint8_t>(nrc);
    return FinishServiceProcessing(nrc);
  }

  return ProcessingStatus::kNotDone;
}

handler::routine::Rid RoutineControlProcessor::AcquireAvailableRid() const {
  const ara::diag::udstransport::ByteVector& request_payload = uds_message_->GetPayload();
  const std::uint8_t high_byte = request_payload.at(2);
  const std::uint8_t low_byte = request_payload.at(3);
  handler::routine::Rid result = 0;
  amsr::diag::common::SetByte(result, low_byte, 0);
  amsr::diag::common::SetByte(result, high_byte, 1);

  return result;
}

ProcessingStatus RoutineControlProcessor::StartPollingTask() {
  assert(routine_operation_handler_.has_value());
  ara::log::LogDebug() << "RoutineControlProcessor::" << __func__;

  // prepare the positive response.
  ProcessingStatus status = PreparePositiveResponseUDSMessage();
  if (status == ProcessingStatus::kDone) {
    return status;
  }

  // TODO(PAASR-2314): Handle RIB.
  // Get array_view of request parameters and response data for routine control operation.
  ara::diag::udstransport::ByteVector& request_payload = uds_message_->GetPayload();
  ara::diag::udstransport::ByteVector& response_payload = uds_message_pos_response_->GetPayload();
  assert(request_payload.size() >= kMinLengthRoutineRequest);
  assert(response_payload.size() >= kMinLengthRoutineRequest);

  vac::container::const_array_view<std::uint8_t> in_buffer(
      request_payload.begin() + kMinLengthRoutineRequest,
      routine_operation_handler_->GetRequestMaxSize(GetOperation()));
  vac::container::array_view<std::uint8_t> out_buffer(response_payload.begin() + kMinLengthRoutineRequest,
                                                      routine_operation_handler_->GetResponseMaxSize(GetOperation()));

  routine_polling_task_ = &routine_operation_handler_->StartOperation(in_buffer, out_buffer, processing_context_);
  if (routine_polling_task_ == nullptr) {
    // Should never happen.
    // No message is available, call FinishProcessing.
    ara::log::LogError() << "RoutineControlProcessor::" << __func__ << ": No polling task has been returned.";
    return FinishServiceProcessing(
        ara::diag::udstransport::UdsNegativeResponseCode::kFailurePreventsExecutionOfRequestedAction);
  }

  return ProcessingStatus::kNotDone;
}

ProcessingStatus RoutineControlProcessor::PreparePositiveResponseUDSMessage() {
  assert(routine_operation_handler_.has_value());
  ara::log::LogDebug() << "RoutineControlProcessor::" << __func__;

  // Get UDS message for positive response.
  // TODO(PAASR-2314): Handle RIB.
  std::size_t response_payload_length =
      kMinLengthRoutineRequest + routine_operation_handler_->GetResponseMaxSize(GetOperation());
  uds_message_pos_response_ = processing_context_.AcquireResponseBuffer(response_payload_length);
  if (uds_message_pos_response_ == nullptr) {
    // No message is available, call FinishProcessing.
    ara::log::LogWarn() << "RoutineControlProcessor::" << __func__ << ": cannot get uds message for positive response.";
    return FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kResponseTooLong);
  }

  // Fill the first bytes of the response payload.
  const ara::diag::udstransport::ByteVector& request_payload = uds_message_->GetPayload();
  ara::diag::udstransport::ByteVector& response_payload = uds_message_pos_response_->GetPayload();
  response_payload.at(0) = kRoutineControlPositiveResponseSid;
  response_payload.at(1) = request_payload.at(1);
  response_payload.at(2) = request_payload.at(2);
  response_payload.at(3) = request_payload.at(3);
  return ProcessingStatus::kNotDone;
}

handler::routine::RoutineOperationType RoutineControlProcessor::GetOperation() {
  ara::log::LogDebug() << "RoutineControlProcessor::" << __func__;

  if (operation_.has_value()) {
    return operation_.value();
  } else {
    const ara::diag::udstransport::ByteVector& request_payload = uds_message_->GetPayload();
    handler::routine::RoutineOperationType routine_operation =
        static_cast<handler::routine::RoutineOperationType>(request_payload[1] & kNotSuppressPosRspMsgIndicationBit);

    operation_.emplace(routine_operation);
    return routine_operation;
  }
}

ProcessingStatus RoutineControlProcessor::EvaluatePollingTaskStatus(const common::PollingStatus polling_status) {
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
      processing_context_.FinishProcessing(routine_polling_task_->GetNRC());
      handle_message_status_ = HandleMessageStatus::kDone;
      return ProcessingStatus::kDone;
    case common::PollingStatus::kNotStarted:
      // This case is not expected to be called, but does not harm => error log.
      ara::log::LogError() << "RoutineControlProcessor::" << __func__ << ": Polling Task could not be started.";
      return ProcessingStatus::kNotDone;
    default:
      throw std::runtime_error("Invalid polling status: " + std::to_string(routine_polling_task_->GetStatus()) + "!");
  }
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
