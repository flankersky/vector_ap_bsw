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
/**        \file  service_dispatcher.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#include <assert.h>

#include "server/service/processor/generic_service_processor.h"
#include "server/service/processor/read_did_processor.h"
#include "server/service/processor/session_control_processor.h"
#include "server/service/service_dispatcher.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {

ServiceDispatcher::ServiceDispatcher(const DiagnosticServerConfiguration& server_config,
                                     const amsr::diag::configuration::DextConfiguration& dext_config)
    : service_table_(server_config, dext_config) {
  service_preconditions_map_.reserve(dext_config.services.size());
  for (amsr::diag::configuration::DextConfiguration::ServicesArray::const_reference service_configuration :
       dext_config.services) {
    if (service_configuration.preconditions.has_value()) {
      service_preconditions_map_.emplace(std::piecewise_construct, std::forward_as_tuple(service_configuration.id),
                                         std::forward_as_tuple(service_configuration.preconditions.value()));
    }
  }
}

void ServiceDispatcher::Shutdown() {
  service_table_.Shutdown();
  service_preconditions_map_.clear();
}

ServiceDispatcher::PairProcessorNRC ServiceDispatcher::FindServiceProcessor(
    ara::diag::udstransport::UdsMessage::Ptr message, ServiceProcessingContext& processing_context) {
  using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;
  ara::log::LogDebug() << "ServiceDispatcher::" << __func__;
  // usual checks.
  if (message == nullptr) {
    throw std::runtime_error("ServiceDispatcher::GetServiceProcessor UDS message not available.");
  }
  ara::diag::udstransport::ByteVector& payload = message->GetPayload();
  std::size_t payload_size = payload.size();
  if (payload_size < 1) {
    throw std::runtime_error("ServiceDispatcher::GetServiceProcessor SID not available.");
  }

  // Get the service handler for the current SID and return the processor.
  std::uint8_t sid = payload[0];
  handler::ServiceHandler* service_handler = service_table_.GetHandlerIfAvailable(sid);

  if (!service_handler) {
    return PairProcessorNRC(nullptr, UdsNegativeResponseCode::kServiceNotSupported);
  }

  // Get the current session and verify that SID is supported in active session.
  if (!IsSidSupportedActiveSession(processing_context, sid)) {
    return PairProcessorNRC(nullptr, UdsNegativeResponseCode::kServiceNotSupportedInActiveSession);
  }

  // Return the service processor.
  return PairProcessorNRC(service_handler->CreateServiceProcessor(std::move(message), processing_context),
                          UdsNegativeResponseCode::kPositiveResponse);
}

bool ServiceDispatcher::IsSidSupportedActiveSession(ServiceProcessingContext& processing_context, std::uint8_t sid) {
  ServicePreconditionsMap::iterator preconditions_iterator = service_preconditions_map_.find(sid);
  if (preconditions_iterator != service_preconditions_map_.end()) {
    // Preconditions have to be checked.
    ara::log::LogDebug() << "ServiceDispatcher::" << __func__ << " : Checking preconditions on active session.";
    if (!preconditions_iterator->second.IsAllowedIn(processing_context,
                                                    server::conversation::access::AccessCategory::kSession)) {
      ara::log::LogWarn() << "ServiceDispatcher::" << __func__ << " : Service " << static_cast<std::uint16_t>(sid)
                          << " not available in active sessions.";
      return false;
    }
  }
  return true;
}

}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
