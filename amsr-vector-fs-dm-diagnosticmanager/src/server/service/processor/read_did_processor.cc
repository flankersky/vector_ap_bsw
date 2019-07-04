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
/**        \file  read_did_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "read_did_processor.h"

#include <assert.h>

#include "ara/log/logging.hpp"
#include "server/data/did_manager.h"
#include "server/service/processor/did_parser.h"
#include "server/service/processor/read_did_processor.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

void ReadDidProcessor::PositiveResponseHandler::Init(vac::container::array_view<std::uint8_t> response_payload) {
  ara::log::LogDebug() << "ReadDidProcessor::PositiveResponseHandler::" << __func__ << ": response payload size is '"
                       << response_payload.size() << "'";
  payload_ = response_payload;
  curr_pos_ = 0;
}

ProcessingStatus ReadDidProcessor::HandleMessage() {
  switch (handle_message_status_) {
    case HandleMessageStatus::kFirstRun: {
      ProcessingStatus status = this->CheckRequestAndPrepareReading();
      if (status == ProcessingStatus::kDone) {
        handle_message_status_ = HandleMessageStatus::kDone;
        return ProcessingStatus::kDone;
      }
      handle_message_status_ = HandleMessageStatus::kContinue;
      this->StartPollingTaskForNextDataIdentifier();
      break;
    }
    case HandleMessageStatus::kContinue: {
      assert(read_did_polling_task_);
      read_did_polling_task_->Continue();
      break;
    }
    case HandleMessageStatus::kDone:
      return ProcessingStatus::kDone;
    default:
      throw std::runtime_error(
          "Invalid HandleMessage status. This case is not properly handled by the implementation and must not be "
          "reached.");
  }
  // Evaluate task status
  assert(read_did_polling_task_);
  ara::log::LogDebug() << "ReadDidProcessor::" << __func__ << ": polling status "
                       << amsr::diag::common::ToString(read_did_polling_task_->GetStatus());
  return EvaluatePollingTaskStatus(read_did_polling_task_->GetStatus());
}

ProcessingStatus ReadDidProcessor::CheckRequestAndPrepareReading() {
  using ConstPayloadView = vac::container::const_array_view<std::uint8_t>;
  using PayloadView = vac::container::array_view<std::uint8_t>;
  using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
  ara::log::LogDebug() << "ReadDidProcessor::" << __func__;
  if (!IsPayloadLengthOk()) {
    ara::log::LogError() << "ReadDidProcessor::" << __func__ << ": Payload length check failed.";
    return FinishServiceProcessing(UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat);
  }

  PayloadView payload_view = static_cast<PayloadView>(uds_message_->GetPayload());
  PayloadView payload_without_sid = PayloadView(payload_view.begin() + 1, payload_view.size() - 1);

  this->AcquireAvailableDids(ConstPayloadView(payload_without_sid));
  if (available_data_identifiers_.empty()) {
    ara::log::LogError() << "ReadDidProcessor::" << __func__ << ": No available DIDs found.";
    return FinishServiceProcessing(UdsNegativeResponseCode::kRequestOutOfRange);
  }

  const UdsNegativeResponseCode nrc = this->CheckConditionsForAvailableDataIdentifiers();
  if (nrc != UdsNegativeResponseCode::kPositiveResponse) {
    ara::log::LogError() << "ReadDidProcessor::" << __func__ << ": Condition check on some DID failed.";
    return FinishServiceProcessing(nrc);
  }

  const std::size_t response_message_size = this->GetResponseSizeForAvailableDids();
  response_message_ = processing_context_.AcquireResponseBuffer(response_message_size);
  if (response_message_ == nullptr) {
    ara::log::LogError() << "ReadDidProcessor::" << __func__ << ": No response message buffer available.";
    return FinishServiceProcessing(UdsNegativeResponseCode::kResponseTooLong);
  }

  response_handler_.Init(response_message_->GetPayload());

  response_handler_.WriteOneByteToResponse(kReadDidPositiveResponseSid);

  ara::log::LogDebug() << "ReadDidProcessor::" << __func__ << ": Positive response initialized.";

  return ProcessingStatus::kNotDone;
}

bool ReadDidProcessor::IsPayloadLengthOk() const {
  // in the future this has to be adapted because only the payload array view will be passed to the service processor.
  const std::size_t payload_size = uds_message_->GetPayload().size() - 1;

  return (payload_size >= 2) && (payload_size <= max_payload_length_) && ((payload_size % 2) == 0);
}

void ReadDidProcessor::AcquireAvailableDids(vac::container::const_array_view<uint8_t> payload_view) {
  assert(available_data_identifiers_.empty());

  DidParser did_parser(payload_view);
  while (did_parser.HasNextDid()) {
    assert(!available_data_identifiers_.full());
    const data::DidInfo::Did current_did = did_parser.PopNextDid();
    data::DidManager& did_manager = processing_context_.GetDidManager();
    vac::memory::optional<data::operationhandler::ReadDidOperationHandler> did_operation_handler =
        std::move(did_manager.CreateOpHandlerIfDidReadable(current_did, processing_context_));
    if (did_operation_handler.has_value()) {
      available_data_identifiers_.push_back(std::move(did_operation_handler.value()));
    }
  }
}

std::size_t ReadDidProcessor::GetResponseSizeForAvailableDids() {
  std::size_t response_size = 1;  // 1 Byte for SID
  constexpr std::size_t did_size = sizeof(data::DidInfo::Did);
  for (ReadDidOperationHandlerList::reference did_operation_handler : available_data_identifiers_) {
    response_size += did_size + did_operation_handler.GetMaxSize();
  }
  return response_size;
}

ara::diag::udstransport::UdsNegativeResponseCode ReadDidProcessor::CheckConditionsForAvailableDataIdentifiers() {
  using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
  for (ReadDidOperationHandlerList::reference did_operation_handler : available_data_identifiers_) {
    const UdsNegativeResponseCode nrc = did_operation_handler.CheckConditions(processing_context_);
    if (nrc != UdsNegativeResponseCode::kPositiveResponse) {
      return nrc;
    }
  }
  return UdsNegativeResponseCode::kPositiveResponse;
}

void ReadDidProcessor::StartPollingTaskForNextDataIdentifier() {
  assert(!available_data_identifiers_.empty());
  data::operationhandler::ReadDidOperationHandler& current_operation_handler = available_data_identifiers_.front();

  response_handler_.WriteDidToResponse(current_operation_handler.GetDid());
  vac::container::array_view<std::uint8_t> write_buffer =
      response_handler_.GetSubArrayViewForWriting(current_operation_handler.GetMaxSize());
  assert(!write_buffer.empty());
  read_did_polling_task_ = &current_operation_handler.StartReading(write_buffer, processing_context_);
}

ProcessingStatus ReadDidProcessor::EvaluatePollingTaskStatus(const common::PollingStatus polling_status) {
  switch (polling_status) {
    case common::PollingStatus::kComplete:
      available_data_identifiers_.pop_front();

      if (available_data_identifiers_.empty()) {
        processing_context_.FinishProcessing(std::move(response_message_));
        handle_message_status_ = HandleMessageStatus::kDone;
        return ProcessingStatus::kDone;
      }
      StartPollingTaskForNextDataIdentifier();
      return ProcessingStatus::kNotDone;
    case common::PollingStatus::kPending:
      return ProcessingStatus::kNotDone;
    case common::PollingStatus::kCanceled:
      handle_message_status_ = HandleMessageStatus::kDone;
      return ProcessingStatus::kDone;
    case common::PollingStatus::kFailed:
      handle_message_status_ = HandleMessageStatus::kDone;
      return FinishServiceProcessing(read_did_polling_task_->GetNRC());
    case common::PollingStatus::kNotStarted:
      // This case is not expected to be called, but does not harm => error log.
      ara::log::LogError() << "WriteDidProcessor::" << __func__ << ": Polling Task could not be started.";
      return ProcessingStatus::kNotDone;
    default:
      throw std::out_of_range("Invalid polling status: " + std::to_string(read_did_polling_task_->GetStatus()) + "!");
  }
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
