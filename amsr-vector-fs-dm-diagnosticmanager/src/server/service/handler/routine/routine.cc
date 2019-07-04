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
/**        \file  routine.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "routine.h"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <utility>

#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {

std::string Routine::ToString(RoutineOperationType operation) {
  std::stringstream log_stream;

  // Change parameter of the stream to print values in hexadecimal
  log_stream << std::setfill('0') << std::hex;

  log_stream << static_cast<std::uint16_t>(operation);
  return log_stream.str();
}

vac::memory::optional<operationhandler::RoutineOperationHandler> Routine::CreateOperationHandlerIfAvailable(
    RoutineOperationType operation) {
  vac::memory::optional<operationhandler::RoutineOperationHandler> result;

  const vac::memory::optional<ridmanager::RoutineOperation>& routine_operation_conf =
      routine_conf_info_.GetRoutineOperation(operation);
  if (!routine_operation_conf.has_value()) {
    // Routine operation is not available.
    ara::log::LogWarn() << "RoutineOperationHandler::" << __func__ << ": operation "
                        << static_cast<std::uint16_t>(operation) << " not available.";
    return result;
  }

  // Instantiate the operation handler and return the optional.
  ara::log::LogDebug() << "Routine::" << __func__ << ": Operation handler is available.";
  ridmanager::RoutineInfo::HandlerType handler_type = routine_conf_info_.GetHandlerType();
  result.emplace(routine_operation_conf.value(), operation, handler_type);
  return result;
}

}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
