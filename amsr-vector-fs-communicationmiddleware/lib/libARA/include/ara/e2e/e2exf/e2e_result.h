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
/**        \file  e2e_result.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_E2E_RESULT_H_
#define LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_E2E_RESULT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/e2e/e2e_types.h"

namespace ara {
namespace e2e {
namespace e2exf {

/**
 * \brief Class providing the E2ECheckStatus and E2EState result.
 * \uptrace SWS_CM_90423
 */
class Result {
 public:
  /**
   * \brief Constructor of result sets the state of the stm and the status from the check.
   */
  Result(ara::e2e::state_machine::State state, ara::e2e::state_machine::CheckStatus check_status) noexcept
      : sm_state_{state},
        check_status_{check_status} {}

  /**
   * \brief Receive the latest check status..
   */
  ara::e2e::state_machine::CheckStatus GetCheckStatus() const noexcept { return check_status_; }

  /**
   * \brief Get the latest state from the STM based on the latest check.
   */
  ara::e2e::state_machine::State GetState() const noexcept { return sm_state_; }

  /**
   * \brief Compare two results.
   *
   * \param rhs The other result object to compare to.
   * \return true if state and check status; false if one of them differ.
   */
  inline bool operator==(const Result& rhs) const noexcept {
    return ((sm_state_ == rhs.sm_state_) && (check_status_ == rhs.check_status_));
  }

 private:
  /**
   * \brief The E2EState of the result
   */
  ara::e2e::state_machine::State sm_state_;
  /**
   * \brief The E2ECheckStatus of the result
   */
  ara::e2e::state_machine::CheckStatus check_status_;
};

/**
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 */
using E2EResult = Result;

}  // namespace e2exf
}  // namespace e2e
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_E2E_RESULT_H_
