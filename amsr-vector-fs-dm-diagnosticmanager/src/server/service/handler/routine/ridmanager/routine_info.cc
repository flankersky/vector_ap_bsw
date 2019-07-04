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
/**        \file  routine_info.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "routine_info.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

// Definition of the static member as well as the declaration have to be provided. The declaration and the initializer
// go inside the class definition, but the member definition has to be separate.
constexpr char RoutineInfo::kInternalStr[];

constexpr char RoutineInfo::kExternalStr[];

RoutineInfo::RoutineInfo(const configuration::RidConfiguration& rid_conf) : rid_(rid_conf.id) {
  handler_type_ = GetHandlerTypeFromStr(rid_conf.handler_info);

  // Get the different operations.
  if (rid_conf.start_operation.has_value()) {
    start_operation_.emplace(rid_conf.start_operation.value());
  }
  if (rid_conf.stop_operation.has_value()) {
    stop_operation_.emplace(rid_conf.stop_operation.value());
  }
  if (rid_conf.request_results_operation.has_value()) {
    request_results_operation_.emplace(rid_conf.request_results_operation.value());
  }
  // This following assert should already have been checked during parsing configuration.
  assert(request_results_operation_.has_value() || stop_operation_.has_value() || start_operation_.has_value());

  // Get the preconditions.
  if (rid_conf.preconditions.has_value()) {
    preconditions_.emplace(rid_conf.preconditions.value());
  }
}

RoutineInfo::UdsNegativeResponseCode RoutineInfo::CheckConditions(const ServiceProcessingContext& context) const {
  if (!preconditions_.has_value()) {
    return ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse;
  }

  // Check conditions for active session.
  if (!preconditions_.value().IsAllowedIn(context, conversation::access::AccessCategory::kSession)) {
    return ara::diag::udstransport::UdsNegativeResponseCode::kSubfunctionNotSupportedInActiveSession;
  }

  // Check conditions for current security level.
  if (!preconditions_.value().IsAllowedIn(context, conversation::access::AccessCategory::kSecurityLevel)) {
    return ara::diag::udstransport::UdsNegativeResponseCode::kSecurityAccessDenied;
  }

  return ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse;
}

RoutineInfo::UdsNegativeResponseCode RoutineInfo::CheckConditions(const ServiceProcessingContext& context,
                                                                  RoutineOperationType operation) const {
  // For the selected operation, check first if the operation is supported. Then check if conditions are correct.
  const vac::memory::optional<RoutineOperation>* routine_operation;

  switch (operation) {
    case RoutineOperationType::kStart:
      routine_operation = &start_operation_;
      break;

    case RoutineOperationType::kStop:
      routine_operation = &stop_operation_;
      break;

    case RoutineOperationType::kRequestResult:
      routine_operation = &request_results_operation_;
      break;

    default:
      ara::log::LogError() << "RoutineInfo::" << __func__ << ": Operation" << static_cast<std::uint16_t>(operation)
                           << "is unknown.";
      return UdsNegativeResponseCode::kRequestOutOfRange;
  }

  if (!routine_operation->has_value()) {
    ara::log::LogError() << "RoutineInfo::" << __func__ << ": " << static_cast<std::uint16_t>(operation)
                         << " operation is not supported.";
    return UdsNegativeResponseCode::kSubFunctionNotSupported;
  }
  return routine_operation->value().CheckConditions(context);
}

RoutineInfo::HandlerType RoutineInfo::GetHandlerTypeFromStr(
    const amsr::diag::configuration::HandlerInfo& handler_info) const {
  const std::string& handler_type = handler_info.handler_type;
  if (handler_type.compare(kInternalStr) == 0) {
    return HandlerType::kInternal;
  }

  if (handler_type.compare(kExternalStr) == 0) {
    return HandlerType::kExternal;
  }

  // Handler Type is unknown. Internal is used as default.
  ara::log::LogError() << "RoutineInfo::" << __func__ << " : Handler type is unknown. Internal is used as default";
  return HandlerType::kInternal;
}

const vac::memory::optional<RoutineOperation>& RoutineInfo::GetRoutineOperation(RoutineOperationType operation) const {
  // This method should normally be called after the conditions have been checked. It should never throw.
  const vac::memory::optional<RoutineOperation>* routine_operation;

  switch (operation) {
    case RoutineOperationType::kStart:
      routine_operation = &start_operation_;
      break;

    case RoutineOperationType::kStop:
      routine_operation = &stop_operation_;
      break;

    case RoutineOperationType::kRequestResult:
      routine_operation = &request_results_operation_;
      break;

    default:
      throw std::runtime_error("RoutineInfo::GetRoutineOperation: operation " +
                               std::to_string(static_cast<std::uint16_t>(operation)) + " is unknown.");
  }

  return *routine_operation;
}

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
