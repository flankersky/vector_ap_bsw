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
/**        \file  diag_state_precondition.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "diag_state_precondition.h"

namespace amsr {
namespace diag {
namespace diagpreconditions {
DiagStatePrecondition::DiagStatePrecondition(server::conversation::access::AccessCategory access_category,
                                             const configuration::Preconditions::ListIDs& list_ids)
    : access_category_(access_category) {
  // Initialize the list of access state values.
  values_list_.reserve(list_ids.size());
  for (const auto& iterator_id : list_ids) {
    values_list_.emplace_back(iterator_id);
  }
}

}  // namespace diagpreconditions
}  // namespace diag
}  // namespace amsr
