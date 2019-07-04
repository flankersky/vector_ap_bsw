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
/**        \file  routine_control_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "routine_control_handler.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {

RoutineControlHandler::RoutineControlHandler(const Configuration config) : rid_manager_(config.rids_table) {
  processor_factory_.reserve(config.base.max_number_of_service_processors);
}

processor::ServiceProcessor::Ptr RoutineControlHandler::CreateServiceProcessor(
    ara::diag::udstransport::UdsMessage::Ptr message, ServiceProcessingContext& processing_context) {
  assert(message != nullptr);
  assert(message->GetPayload().size() >= 1);
  assert(message->GetPayload()[0] == processor::RoutineControlProcessor::kRoutineControlSid);
  // TODO(PAASR-2155): Make ServiceHandlers return nullptr if maximum number of ServiceProcessors is exceeded
  return processor_factory_.create(rid_manager_, std::move(message), processing_context);
}
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
