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
/**        \file  diag_state_precondition.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_DIAGPRECONDITIONS_DIAG_STATE_PRECONDITION_H_
#define SRC_DIAGPRECONDITIONS_DIAG_STATE_PRECONDITION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "server/conversation/access/access_state.h"
#include "vac/container/static_list.h"
#include "vac/memory/three_phase_allocator.h"

namespace amsr {
namespace diag {
namespace diagpreconditions {
/**
 * \brief Implementation of DiagStatePrecondition Class.
 */
class DiagStatePrecondition {
 public:
  /**
   * \brief type definition for list of AccessStateValues.
   */
  using AccessStateValuesList = vac::container::StaticList<
      server::conversation::access::ObservableAccessState::ValueType,
      vac::memory::ThreePhaseAllocator<server::conversation::access::ObservableAccessState::ValueType>>;

  /**
   * \brief constructor.
   */
  DiagStatePrecondition(server::conversation::access::AccessCategory access_category,
                        const configuration::Preconditions::ListIDs& list_ids);

  /**
   * \brief Return the list of access state values.
   */
  const AccessStateValuesList& GetAccessStateValuesList() const { return values_list_; }

  // Remove copy/move ctor/assignment.
  DiagStatePrecondition(const DiagStatePrecondition&) = delete;
  DiagStatePrecondition(const DiagStatePrecondition&&) = delete;
  DiagStatePrecondition& operator=(const DiagStatePrecondition&) = delete;
  DiagStatePrecondition& operator=(const DiagStatePrecondition&&) = delete;

 private:
  /**
   * \brief Access category.
   */
  server::conversation::access::AccessCategory access_category_;

  /**
   * \brief List of access state values.
   */
  AccessStateValuesList values_list_;

  FRIEND_TEST(DiagStatePreconditionTestFixture, DiagStatePreconditionConstruct);
};

}  // namespace diagpreconditions
}  // namespace diag
}  // namespace amsr
#endif  // SRC_DIAGPRECONDITIONS_DIAG_STATE_PRECONDITION_H_
