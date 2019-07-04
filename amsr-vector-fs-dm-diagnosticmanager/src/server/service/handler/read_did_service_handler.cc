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
/**        \file  read_did_service_handler.cc
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

#include "server/service/handler/read_did_service_handler.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

ReadDidServiceHandler::ReadDidServiceHandler(const Configuration config) : config_(config) {
  processor_factory_.reserve(config_.base.max_number_of_service_processors);
}

processor::ServiceProcessor::Ptr ReadDidServiceHandler::CreateServiceProcessor(
    ara::diag::udstransport::UdsMessage::Ptr message, ServiceProcessingContext& processing_context) {
  using ReadDidProcessor = amsr::diag::server::service::processor::ReadDidProcessor;

  assert(message != nullptr);
  assert(message->GetPayload().size() >= 1);
  assert(message->GetPayload()[0] == ReadDidProcessor::kReadDidSid);
  // TODO(PAASR-2155): Make ServiceHandlers return nullptr if maximum number of ServiceProcessors is exceeded
  return processor_factory_.create(std::move(message), processing_context, config_.maximum_number_of_dids);
}

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
