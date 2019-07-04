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
/**        \file  timer.h
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

#ifndef LIB_INCLUDE_VAC_TIMER_TIMER_H_
#define LIB_INCLUDE_VAC_TIMER_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>
#include <chrono>

#include "vac/timer/timer_manager.h"

namespace vac {
namespace timer {

/**
 * \brief Base Class representing a timer.
 *
 * Timers can be periodic or one-shot. Create a subclass of Timer to implement a time-based callback.
 */
class Timer {
 public:
  /**
   * \brief Typedef representing the clock type this timer operates on.
   *
   *Since QNX cannot convert between different clock resolutions we have to use the clock with the smallest tick.
   */
  using Clock = std::chrono::high_resolution_clock;

  /**
   * \brief constructor taking a timer_manager this Timer is associated with.
   */
  explicit Timer(TimerManager* timer_manager);

  Timer(const Timer&) = delete;
  Timer& operator=(const Timer&) = delete;
  Timer(const Timer&&) = delete;
  Timer& operator=(const Timer&&) = delete;

  virtual ~Timer();

  /**
   * \brief Returns the time_point at which this timer expires next.
   *
   * Note that the value is only valid if the Timer is started.
   *
   * \return The next_expiry_ point.
   */
  const Clock::time_point& GetNextExpiry() const { return next_expiry_; }

  /**
   * \brief Sets the timer to periodic mode with the given period.
   *
   * The running state of the timer is not modified.
   *
   * \param period The period at which this timer should fire.
   */
  void SetPeriod(Clock::duration period);

  /**
   * \brief Sets the timer to one-shot mode with the given expiry point.
   *
   * The running state of the timer is not modified.
   *
   * \param time_point the Time Point at which this timer should fire.
   */
  void SetOneShot(Clock::time_point time_point);

  /**
   * \brief Sets the timer to one-shot mode with an expiry point relative to the current time.
   *
   * The running state of the timer is not modified.
   *
   * \param timeout The Time until this timer should fire.
   */
  void SetOneShot(Clock::duration timeout);

  /**
   * Starts the timer
   */
  void Start();

  /**
   * \brief Stops the timer
   */
  void Stop();

  /**
   * \brief Callback that is activated when the timer expires.
   *
   * This callback can indicate whether the timer should be scheduled again.
   *
   * \return true if the timer shall be scheduled again, false otherwise. Returning true has no effect when the timer is
   * not periodic.
   */
  virtual bool HandleTimer() = 0;

  /**
   * \brief Handler method called by the TimerManager.
   */
  void DoHandleTimer();

  /**
   * \brief Returns whether the expiry point of this timer is greater than the current time.
   *
   * \return true if the next_expiry_ point has passed, false otherwise.
   */
  bool IsExpired() const;

  /**
   * \brief Convert the next_expiry_ of this timer to a timeval struct.
   *
   * \return The current next_expiry_ converted to a timeval relative to the current time.
   */
  const struct timeval ToTimeval() const;

 private:
  /*
   * \brief Reschedules a periodic timer.
   */
  void RestartPeriodic();

  TimerManager* timer_manager_;
  bool one_shot_;
  Clock::duration period_;
  Clock::time_point next_expiry_;
};

}  // namespace timer
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_TIMER_TIMER_H_
