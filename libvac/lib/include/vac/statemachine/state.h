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
/**        \file  state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_STATEMACHINE_STATE_H_
#define LIB_INCLUDE_VAC_STATEMACHINE_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace vac {
namespace statemachine {

/**
 * Interface for State classes.
 */
template <typename StateHandle, class Context>
class State {
 public:
  /**
   * \brief The Type of the StateHandle.
   *
   * The StateHandle is used for identifying a particular state by the value of a basic data type, such
   * as enum values.
   */
  using HandleType = StateHandle;

  /**
   * \brief The Type of the Context.
   */
  using ContextType = Context;

  /**
   * \brief Constructor.
   */
  explicit State(const StateHandle handle) : handle_(handle) {}

  /**
   * \brief Copy Constructor
   */
  State(const State&) = delete;

  /**
   * \brief Copy Assignment
   */
  State& operator=(const State&) = delete;

  /**
   * \brief Move Constructor
   */
  State(State&&) = delete;

  /**
   * \brief Move Assignment
   */
  State& operator=(State&&) = delete;

  /**
   * \brief Destructor.
   */
  virtual ~State() = default;

  /**
   * \brief Context sensitive method called when state is entered.
   */
  virtual void OnEnter(Context& context) = 0;

  /**
   * \brief Context sensitive method called when state is left.
   */
  virtual void OnLeave(Context& context) = 0;

  /**
   * \brief Checks if change from current state to state with given handle is valid.
   *
   *  \param handle The handle of the requested state.
   *  \return true if change is valid, otherwise returns false.
   */
  virtual bool IsValidChange(const StateHandle handle) const = 0;

  /**
   * \brief Returns the handle of this State.
   */
  const StateHandle GetHandle() const { return handle_; }

 private:
  /**
   * \brief The handle for this State.
   */
  const StateHandle handle_;
};

}  // namespace statemachine
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_STATEMACHINE_STATE_H_
