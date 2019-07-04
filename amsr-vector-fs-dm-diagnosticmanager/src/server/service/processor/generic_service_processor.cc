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
/**        \file  generic_service_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#include "server/service/processor/generic_service_processor.h"

#include <algorithm>
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoType.h"
#include "ara/log/logging.hpp"
#include "server/conversation/conversation_manager.h"
#include "server/conversation/uds_message_provider.h"
#include "udstransport/meta_info_map_conversion.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

ProcessingStatus GenericServiceProcessor::HandleMessage() {
  bool service_output_future_valid = future_service_output_.valid();
  ara::log::LogDebug() << "GenericServiceProcessor::" << __func__ << " (service_output_future_valid='"
                       << std::to_string(service_output_future_valid) << "')";
  // If Service has not been called yet, HandleMessage is beeing called for the first time.
  if (!service_output_future_valid) {
    HandleMessageFirstCall();
  }

  // Check if future is_ready.
  // TODO(PAASSR-1627) : ara::com does not currently provide method is_ready(). Wait_for will be used until the method
  // is available.
  constexpr std::chrono::nanoseconds kWaitTimeFuture(10);
  if (ara::com::FutureStatus::timeout == future_service_output_.wait_for(kWaitTimeFuture)) {
    return ProcessingStatus::kNotDone;
  } else {
    // Future is ready.
    HandleFutureReady();
    return ProcessingStatus::kDone;
  }
}

void GenericServiceProcessor::HandleMessageFirstCall() {
  using MetaInfo = ara::diag::service_interfaces::generic_uds_service::MetaInfoType;
  ara::log::LogDebug() << "GenericServiceProcessor::HandleMessage : ara::com::future not valid.";
  if (uds_message_ == nullptr) {
    throw std::runtime_error("GenericServiceProcessor::HandleMessage : UDS message invalid (nullptr)!");
  }

  ara::diag::udstransport::ByteVector uds_payload = uds_message_->GetPayload();
  std::size_t uds_payload_size = uds_payload.size();
  if (uds_payload_size < 1) {
    throw std::runtime_error("GenericServiceProcessor::HandleMessage : SID not available!");
  }

  // Copy the UDS message payload data located after the SID to the data array.
  ara::diag::service_interfaces::generic_uds_service::DataArrayType request_data(std::next(uds_payload.begin()),
                                                                                 uds_payload.end());

  // Check if generic_uds_service_proxy already contains proxy.
  if (!generic_uds_service_proxy_.has_value()) {
    // Find the UDS service proxy. TODO(visbft):  throw if not found ?
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        ara::diag::service_interfaces::generic_uds_service::GenericUDSServiceProxy::FindService(
            ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      std::runtime_error("GenericServiceProcessor::HandleMessage : GenericUDSServiceProxy not found!");
    }
    generic_uds_service_proxy_.emplace(handles[0]);
  }
  MetaInfo meta_info =
      ara::diag::udstransport::ConvertToAraComMetaInfo<MetaInfo, decltype(uds_message_->GetMetaInfo())>(
          uds_message_->GetMetaInfo());
  future_service_output_ =
      std::move(generic_uds_service_proxy_.value().Service(uds_payload[0], request_data, meta_info));
  // destroy UdsMessage
  uds_message_.reset();
}

void GenericServiceProcessor::HandleFutureReady() {
  // Get the response data, create a new UDS Message and call finishProcessing()
  // Get the response data.
  ara::diag::service_interfaces::generic_uds_service::proxy::methods::Service::Output service_output;
  assert(future_service_output_.is_ready());
  try {
    // This code is executed if future is not returning an exception.
    service_output = future_service_output_.get();
    ara::diag::service_interfaces::generic_uds_service::DataArrayType& response_data = service_output.ResponseData;

    ara::diag::udstransport::UdsMessage::Ptr uds_message_response =
        processing_context_.AcquireResponseBuffer(response_data.size());
    if (uds_message_response == nullptr) {
      ara::log::LogError() << "GenericServiceProcessor::" << __func__ << " : Couldn't create response message of size '"
                           << response_data.size() << "'!";

      // TODO(visbft): No exception shall be thrown in the case no UDS Message is available.
      throw std::runtime_error(
          "GenericServiceProcessor::HandleMessage : UDSMessageProvider cannot provide UDS message for response!");
    }

    // Copy the response_data to the payload
    ara::diag::udstransport::ByteVector& uds_payload_response = uds_message_response->GetPayload();
    std::copy(response_data.begin(), response_data.end(), uds_payload_response.begin());

    ara::log::LogDebug() << "GenericServiceProcessor::" << __func__ << " : Returning response of size '"
                         << uds_payload_response.size() << "'";

    // Call FinishProcessing().
    processing_context_.FinishProcessing(std::move(uds_message_response));
  } catch (const ara::diag::service_interfaces::generic_uds_service::proxy::application_errors::UDSServiceFailed&
               uds_service_failed) {
    // Future is returning an exception : Get error code and call finish processing.
    ara::log::LogError() << uds_service_failed.what();
    ara::diag::udstransport::UdsNegativeResponseCode nrc =
        static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed.geterrorContext());
    processing_context_.FinishProcessing(nrc);
  }
}

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
