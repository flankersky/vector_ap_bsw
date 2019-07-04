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
/**        \file  routine_operation.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "routine_operation.h"

#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

RoutineOperation::RoutineOperation(const configuration::RidOperationConfiguration& operation_conf) {
  // Get response and request data.
  if (operation_conf.res_data.has_value()) {
    response_data_.emplace(operation_conf.res_data.value());
  }
  if (operation_conf.req_data.has_value()) {
    request_data_.emplace(operation_conf.req_data.value());
  }

  // Get Preconditions.
  if (operation_conf.preconditions.has_value()) {
    preconditions_.emplace(operation_conf.preconditions.value());
  }
}

ara::diag::udstransport::UdsNegativeResponseCode RoutineOperation::CheckConditions(
    const ServiceProcessingContext& context) const {
  if (!preconditions_.has_value()) {
    ara::log::LogDebug() << "RoutineOperation::" << __func__
                         << " no precondition found: returning NRC positive response.";
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

const RoutineOperation::RidData& RoutineOperation::GetResponseData() const {
  // This method should only be called if data is available.
  assert(response_data_.has_value());
  return response_data_.value();
}

const RoutineOperation::RidData& RoutineOperation::GetRequestData() const {
  // This method should only be called if data is available.
  assert(request_data_.has_value());
  return request_data_.value();
}

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
