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
/**        \file  diag_preconditions.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "diag_preconditions.h"

#include "server/conversation/access/access_state.h"

namespace amsr {
namespace diag {
namespace diagpreconditions {
const DiagStatePrecondition& DiagPreconditions::GetPreconditionsOf(
    server::conversation::access::AccessCategory access_category) const {
  switch (access_category) {
    case server::conversation::access::AccessCategory::kSession:
      return sessions_precondition_;
    case server::conversation::access::AccessCategory::kSecurityLevel:
      return security_access_levels_precondition_;
    default:
      throw std::runtime_error("DiagPreconditions::GetPreconditionsOf : access category not known.");
  }
}

bool DiagPreconditions::IsAllowedIn(const server::service::ServiceProcessingContext& context,
                                    server::conversation::access::AccessCategory access_category) const {
  // Get the current category value.
  const server::conversation::access::ObservableAccessState& access_state_observable =
      context.GetAccessState(access_category);
  server::conversation::access::ObservableAccessState::ValueType access_state_observable_value =
      access_state_observable.Get();

  // Get the preconditon for the access category
  const DiagStatePrecondition& diag_state_precondition = GetPreconditionsOf(access_category);
  const DiagStatePrecondition::AccessStateValuesList& values_list_preconditions =
      diag_state_precondition.GetAccessStateValuesList();

  // Empty precondition list means -> no restrictions -> pass through!
  if (!values_list_preconditions.empty()) {
    // Check if the current state is in the preconditions list.
    auto preconditions_values_iterator =
        std::find(values_list_preconditions.begin(), values_list_preconditions.end(), access_state_observable_value);
    if (preconditions_values_iterator == values_list_preconditions.end()) {
      return false;
    }
  }

  return true;
}

}  // namespace diagpreconditions
}  // namespace diag
}  // namespace amsr
