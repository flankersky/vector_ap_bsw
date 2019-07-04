/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  routine_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "routine_operation_handler.h"

#include <iomanip>
#include <sstream>

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace operationhandler {

ara::diag::udstransport::UdsNegativeResponseCode RoutineOperationHandler::CheckConditions(
    const ServiceProcessingContext& context, const ara::diag::udstransport::ByteVector& payload) const {
  ara::log::LogDebug() << "RoutineOperationHandler::" << __func__ << ": operation '" << ToString(operation_type_);
  assert(payload.size() >= kMinLengthRoutineRequest);
  assert(static_cast<RoutineOperationType>(payload[1] & kNotSuppressPosRspMsgIndicationBit) == operation_type_);

  // Check the total length.
  if (payload.size() != (kMinLengthRoutineRequest + GetRequestMaxSize(operation_type_))) {
    return ara::diag::udstransport::UdsNegativeResponseCode::kIncorrectMessageLengthOrInvalidFormat;
  }

  // TODO(PAASR-2313): Terminate implementation of routine control CheckConditions.
  return ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse;
}

std::string RoutineOperationHandler::ToString(RoutineOperationType operation) {
  std::stringstream log_stream;

  // Change parameter of the stream to print values in hexadecimal
  log_stream << std::setfill('0') << std::hex;

  log_stream << static_cast<std::uint16_t>(operation);
  return log_stream.str();
}

}  // namespace operationhandler
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
