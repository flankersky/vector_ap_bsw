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
/**        \file  state_owner.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_
#define LIB_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/statemachine/state.h"
#include "vac/statemachine/state_pool.h"

namespace vac {
namespace statemachine {

/**
 * Interface for classes owning a State.
 */
template <class State>
class StateOwner {
  /**
   * \brief Type of the State Handle.
   */
  using Handle = typename State::HandleType;

  /**
   * \brief Type of the State Context.
   */
  using Context = typename State::ContextType;

 public:
  /**
   * \brief Constructor.
   */
  StateOwner(StatePool<State>& state_pool, Handle handle)
      : state_pool_(state_pool), state_(state_pool_.GetState(handle)) {}

  /**
   * \brief Copy Constructor
   */
  StateOwner(const StateOwner&) = delete;

  /**
   * \brief Copy Assignment
   */
  StateOwner& operator=(const StateOwner&) = delete;

  /**
   * \brief Move Constructor
   */
  StateOwner(StateOwner&&) = delete;

  /**
   * \brief Move Assignment
   */
  StateOwner& operator=(StateOwner&&) = delete;

  /**
   * \brief Destructor.
   */
  virtual ~StateOwner() = default;

  /**
   * \brief Returns the current state.
   */
  State* GetState() const { return state_; }

  /**
   * \brief Returns the Context used by context depending State methods.
   */
  virtual Context& GetContext() = 0;

  /**
   * \brief Tries to change current state to state associated to given handle.
   *
   * \param handle The handle of the state to which change is requested.
   * \return true if state change was successful, otherwise retuns false.
   */
  bool TryChangeState(const Handle handle) {
    if (state_->IsValidChange(handle)) {
      state_->OnLeave(GetContext());
      state_ = state_pool_.GetState(handle);
      state_->OnEnter(GetContext());
      return true;
    } else {
      return false;
    }
  }

 private:
  /**
   * \brief Reference to associated State Pool.
   */
  StatePool<State>& state_pool_;

  /**
   * \brief Pointer to current state.
   */
  State* state_;
};

}  // namespace statemachine
}  // namespace vac
#endif  // LIB_INCLUDE_VAC_STATEMACHINE_STATE_OWNER_H_
