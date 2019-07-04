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
/**        \file  state_pool.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_
#define LIB_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace vac {
namespace statemachine {

/**
 * \brief State Pool.
 *
 * The state pool is responsible for resources of states.
 */
template <class State>
class StatePool {
 public:
  /**
   * \brief Type of the State Handle.
   */
  using Handle = typename State::HandleType;

  /**
   * \brief Destructor.
   */
  virtual ~StatePool() = default;

  /**
   * \brief Returns State of given handle.
   *
   * \param handle The handle.
   * \return The State with given handle.
   *
   * If handle does not refer to a State, exception of type std::out_of_range is thrown.
   */
  virtual State* GetState(const Handle handle) = 0;

  /**
     * \brief Checks if handle refers to an existing state.
     *
     * \param handle The handle.
     * \return true if state exists with given handle, otherwise returns false.
     */
  virtual bool IsValid(const Handle handle) const = 0;
};

}  // namespace statemachine
}  // namespace vac
#endif  // LIB_INCLUDE_VAC_STATEMACHINE_STATE_POOL_H_
