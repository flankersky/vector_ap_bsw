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
/**        \file  timer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp               visdap       Vector Informatik GmbH
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/timer/timer.h"
#include <sys/time.h>
#include <cassert>
#include <chrono>
#include <limits>
#include "vac/timer/timer_manager.h"

namespace vac {
namespace timer {

Timer::Timer(TimerManager* timer_manager)
    : timer_manager_(timer_manager), one_shot_(true), period_(Clock::duration::zero()), next_expiry_() {}

Timer::~Timer() { Stop(); }

void Timer::SetPeriod(Clock::duration period) {
  one_shot_ = false;
  period_ = period;
}

void Timer::SetOneShot(Clock::time_point time_point) {
  one_shot_ = true;
  period_ = Clock::duration::zero();
  next_expiry_ = time_point;
}

void Timer::SetOneShot(Clock::duration timeout) { SetOneShot(Clock::now() + timeout); }

void Timer::Start() {
  if (!one_shot_) {
    // Update expiry time for periodic timer
    next_expiry_ = Clock::now() + period_;
  }
  timer_manager_->AddTimer(this);
}

void Timer::Stop() { timer_manager_->RemoveTimer(this); }

const struct timeval Timer::ToTimeval() const {
  Clock::time_point now = Clock::now();
  Clock::duration time_to_expiry = next_expiry_ - now;

  std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(time_to_expiry);
  std::chrono::microseconds microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(time_to_expiry - seconds);

  struct timeval expiry;

  expiry.tv_sec = static_cast<time_t>(
      seconds.count()); /* WARNING: This only works until year 2038, in case of non-matching datatypes! */
  expiry.tv_usec = static_cast<suseconds_t>(
      microseconds.count()); /* WARNING: This only works until year 2038, in case of non-matching datatypes! */

  return expiry;
}

bool Timer::IsExpired() const {
  Clock::time_point now = Clock::now();
  return now >= next_expiry_;
}

void Timer::DoHandleTimer() {
  /*
   * A timer might destroy itself in 'HandleTimer'.
   * Therefore, we have to remove it before that and reschedule it again when requested.
   */
  timer_manager_->RemoveTimer(this);
  // Cache member variables which are accessed after 'HandleTimer' has been called
  const bool one_shot{one_shot_};
  const bool rerun = HandleTimer();
  // Take extra care accessing member variables after 'HandleTimer' call
  if (!one_shot && rerun) {
    // We are on a safe ground in here, full access to all member variables is allowed
    RestartPeriodic();
  }
}

void Timer::RestartPeriodic() {
  assert(!one_shot_);

  Clock::time_point now = Clock::now();

  // Advance next_expiry_ to a point in the future, in case the period was overrun.
  while (next_expiry_ <= now) {
    next_expiry_ += period_;
  }

  timer_manager_->AddTimer(this);
}

}  // namespace timer
}  // namespace vac
