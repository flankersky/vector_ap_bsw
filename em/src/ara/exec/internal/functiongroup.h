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
/**        \file  functiongroup.h
 *        \brief  Structure of function group and its states
 *
 *        \details Structure can be used to store function group states globally.
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_FUNCTIONGROUP_H_
#define SRC_ARA_EXEC_INTERNAL_FUNCTIONGROUP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vac/container/static_list.h>
#include <vac/memory/leaky_array_allocator.h>

#include "types.h"

namespace ara {

namespace exec {

namespace internal {

/**
*  \brief Function group structure
*/
class FunctionGroup {
 public:
  /**
   * \brief List of Function Group Elements
   */
  using FunctionGroupStates =
      vac::container::StaticList<const SafeString, vac::memory::LeakyArrayAllocator<SafeString>>;

  /**
   * \brief Constructor
   * \param fg_name Name of function group
   */
  explicit FunctionGroup(StringView fg_name)
      : name_(fg_name.data(), fg_name.size()), active_state_(&kFunctionGroupStateOff) {}

  /**
   * \brief Check if function group is active
   * \return True if function group is active
   */
  bool IsFunctionGroupActive() const { return (*active_state_ != kFunctionGroupStateOff); }

  /**
   * \brief Get reference to active function group
   * \return Returns pointer to active state of function group
   */
  const SafeString* GetActiveState() const { return active_state_; }

  /**
   * \brief Get reference to active function group
   * \return Returns pointer to pointer to active state of function group
   */
  const SafeString* const* GetActiveStateRef() const { return &active_state_; }

  /**
   * \brief Get reference to active function group
   * \param state Name of state requested to be active
   * \return ReturnType::kOK if function could be set active
   */
  ReturnType UpdateActiveState(StringView state) {
    ReturnType ret_val = ReturnType::kNotOk;
    auto iter = std::find_if(states_.begin(), states_.end(), [state](const SafeString& group_state) {
      return (group_state.compare(state.data()) == 0);
    });
    if (iter != states_.end()) {
      active_state_ = &(*iter);
      ret_val = ReturnType::kOk;
    }
    return ret_val;
  }

  /**
   * \brief Get Pointer to referenced state
   * \param state Name of requested state
   * \return Returns pointer to referenced state. Returns nullptr, when state was not found.
   */
  const SafeString* GetStateRef(StringView state) const {
    const SafeString* ret_str = nullptr;
    if (!states_.empty()) {
      for (const SafeString& group_state : states_) {
        if (group_state.compare(state.data()) == 0) {
          ret_str = &group_state;
          break;
        }
      }
    }
    return ret_str;
  }

  /**
   * \brief Function returns function group name
   * \return Return function group name
   */
  const SafeString& GetName() const { return name_; }

  /**
   * \brief Function returns list of Function group states
   * \return Return list of states
   */
  FunctionGroupStates& GetStates() { return states_; }

  /**
   * \brief Function returns list of Function group states
   * \return Return list of states
   */
  const FunctionGroupStates& GetStates() const { return states_; }

 private:
  /**
   * \brief Function group name
   * \trace SWS_EM_01107
   */
  const SafeString name_;

  /**
   * \brief Status if function group is enabled.
   */
  const SafeString* active_state_;

  /**
   * \brief States of function group
   * \trace SWS_EM_01108
   */
  FunctionGroupStates states_;
};

}  // namespace internal

}  // namespace exec

}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_FUNCTIONGROUP_H_
