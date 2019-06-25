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
/**        \file  state_machine.h
 *        \brief  E2E state machine implementation
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_STATE_MACHINE_H_
#define LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <deque>
#include <mutex>
#include <stdexcept>
#include "ara/e2e/e2e_types.h"

namespace ara {
namespace e2e {
namespace e2exf {

/**
 * \brief E2E state machine implementation
 * \tparam E2ExfConfiguration E2E transformer configuration
 */
template <typename E2ExfConfiguration>
class StateMachine {
 public:
  /**
   * \brief Shortcut for E2ECheckStatus
   */
  using CheckStatus = ara::e2e::state_machine::CheckStatus;

  /**
   * \brief Shortcut for E2EState
   */
  using State = ara::e2e::state_machine::E2EState;

  /**
   * \brief Check E2E state machine
   * \param check_status E2E check status of a single event sample
   * \return Current E2E state machine state
   */
  State Check(CheckStatus check_status) {
    return SmCheck(typename E2ExfConfiguration::E2EProfileProps{}, check_status);
  }

 private:
  /**
   * \brief Update the E2E state machine
   * \uptrace SWS_E2E_00345
   * \tparam E2EProfileProps E2E profile properties type
   * \param profile_props E2E profile configuration properties
   * \param check_status E2E check status of the last event sample
   * \return Current event specific E2E state machine state
   */
  template <typename E2EProfileProps>
  State SmCheck(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // State specific handling
    switch (state_) {
      case State::NoData: {
        HandleStateNoData(profile_props, check_status);
        break;
      }
      case State::Init: {
        HandleStateInit(profile_props, check_status);
        break;
      }
      case State::Valid: {
        HandleStateValid(profile_props, check_status);
        break;
      }
      case State::Invalid: {
        HandleStateInvalid(profile_props, check_status);
        break;
      }
      default: {
        assert(false && "Unknown / Unsupported E2E state in state machine handling.");
        break;
      }
    }

    return state_;
  }

  /**
   * \brief Handle state machine state 'NoData'
   * \uptrace SWS_E2E_00345
   * \tparam E2EProfileProps E2E profile properties type
   * \param check_status E2E check status of the last event sample
   */
  template <typename E2EProfileProps>
  void HandleStateNoData(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // No AddStatus() handling in this state

    // State event / transition handling
    if ((check_status != CheckStatus::Error) && (check_status != CheckStatus::NoNewData)) {
      state_ = State::Init;
    }

    (void)profile_props;  // Not used in state 'NoData'
  }

  /**
   * \brief Handle state machine state 'Init'
   * \uptrace SWS_E2E_00345
   * \tparam E2EProfileProps E2E profile properties type
   * \param check_status E2E check status of the last event sample
   */
  template <typename E2EProfileProps>
  void HandleStateInit(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // Update state counter
    AddStatus(profile_props, check_status);

    // State event / transition handling
    if ((error_count_ <= profile_props.kMaxErrorStateInit) && (ok_count_ >= profile_props.kMinOkStateInit)) {
      state_ = State::Valid;
    } else if (error_count_ > profile_props.kMaxErrorStateInit) {
      state_ = State::Invalid;
    } else {
      state_ = State::Init;
    }
  }

  /**
   * \brief Handle state machine state 'Valid'
   * \uptrace SWS_E2E_00345
   * \tparam E2EProfileProps E2E profile properties type
   * \param check_status E2E check status of the last event sample
   */
  template <typename E2EProfileProps>
  void HandleStateValid(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // Update state counter
    AddStatus(profile_props, check_status);

    // State event / transition handling
    if ((error_count_ <= profile_props.kMaxErrorStateValid) && (ok_count_ >= profile_props.kMinOkStateValid)) {
      state_ = State::Valid;
    } else {
      state_ = State::Invalid;
    }
  }

  /**
   * \brief Handle state machine state 'Invalid'
   * \uptrace SWS_E2E_00345
   * \tparam E2EProfileProps E2E profile properties type
   * \param check_status E2E check status of the last event sample
   */
  template <typename E2EProfileProps>
  void HandleStateInvalid(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // Update state counter
    AddStatus(profile_props, check_status);

    // State event / transition handling
    if ((error_count_ <= profile_props.kMaxErrorStateInvalid) && (ok_count_ >= profile_props.kMinOkStateInvalid)) {
      state_ = State::Valid;
    }
  }

  /**
   * \brief Updated E2E state machine counters OkCount and ErrorCount
   * \uptrace SWS_E2E_00466
   * \tparam E2EProfileProps E2E profile properties type
   * \param profile_props E2E profile configuration properties
   * \param check_status E2E check status of the last event sample
   */
  template <typename E2EProfileProps>
  void AddStatus(const E2EProfileProps& profile_props, CheckStatus check_status) {
    // Add new E2E status to status window
    if (check_status_window_.size() >= profile_props.kWindowSize) {
      check_status_window_.pop_front();
    }
    check_status_window_.push_back(check_status);

    // Update OK and ERROR counter
    ok_count_ = static_cast<std::uint8_t>(
        std::count(check_status_window_.cbegin(), check_status_window_.cend(), CheckStatus::Ok));
    error_count_ = static_cast<std::uint8_t>(
        std::count(check_status_window_.cbegin(), check_status_window_.cend(), CheckStatus::Error));
  }

  // Shortcut for status window type
  using StatusWindowType = std::deque<CheckStatus>;

  /**
   * \brief E2E state machine status window
   */
  StatusWindowType check_status_window_;

  /**
   * \brief E2E state
   */
  State state_{ara::e2e::state_machine::E2EState::NoData};

  /**
   * \brief E2E state machine OK counter
   * \uptrace SWS_E2E_00343
   */
  std::uint8_t ok_count_{0U};

  /**
   * \brief E2E state machine error counter
   * \uptrace SWS_E2E_00343
   */
  std::uint8_t error_count_{0U};
};

}  // namespace e2exf
}  // namespace e2e
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_STATE_MACHINE_H_
