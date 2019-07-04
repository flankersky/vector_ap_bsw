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
/**        \file  session_control_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <memory>
#include <utility>

#include "server/service/handler/session_control_handler.h"
#include "server/service/processor/context/session_configuration.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

SessionControlHandler::SessionControlHandler(const Configuration config) : config_(config) {
  processor_factory_.reserve(config_.base.max_number_of_service_processors);
}

processor::ServiceProcessor::Ptr SessionControlHandler::CreateServiceProcessor(
    ara::diag::udstransport::UdsMessage::Ptr message, ServiceProcessingContext& processing_context) {
  using SessionServiceProcessor = amsr::diag::server::service::processor::SessionControlProcessor;

  assert(message != nullptr);
  assert(message->GetPayload().size() >= 1);
  assert(message->GetPayload()[0] == SessionServiceProcessor::kDiagnosticSessionControlSid);

  const processor::SessionControlProcessor::Configuration processor_config{processing_context, config_.session_config,
                                                                           processing_context};
  // TODO(PAASR-2155): Make ServiceHandlers return nullptr if maximum number of ServiceProcessors is exceeded
  return processor_factory_.create(std::move(message), processor_config);
}

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
