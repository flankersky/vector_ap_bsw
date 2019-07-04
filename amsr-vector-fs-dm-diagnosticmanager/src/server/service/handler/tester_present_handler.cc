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
/**
 *      \file  tester_present_handler.cc
 *      \brief Contains the implementation of the TesterPresentHandler class
 *
 *     \details ServiceHandler implementation for Tester Present requests (0x3E)
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <utility>

#include "server/service/handler/tester_present_handler.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

TesterPresentHandler::TesterPresentHandler(const Configuration config) {
  processor_factory_.reserve(config.base.max_number_of_service_processors);
}

processor::ServiceProcessor::Ptr TesterPresentHandler::CreateServiceProcessor(
    ara::diag::udstransport::UdsMessage::Ptr message, ServiceProcessingContext& processing_context) {
  using TesterPresentProcessor = amsr::diag::server::service::processor::TesterPresentProcessor;

  assert(message != nullptr);
  assert(message->GetPayload().size() >= 1);
  assert(message->GetPayload()[0] == TesterPresentProcessor::kTesterPresentRequestSid);
  // TODO(PAASR-2155): Make ServiceHandlers return nullptr if maximum number of ServiceProcessors is exceeded
  return processor_factory_.create(std::move(message), processing_context);
}

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
