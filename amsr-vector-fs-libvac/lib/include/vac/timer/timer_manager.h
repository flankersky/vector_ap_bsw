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
/**        \file  timer_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_TIMER_TIMER_MANAGER_H_
#define LIB_INCLUDE_VAC_TIMER_TIMER_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>
#include <cstddef>
#include <queue>
#include <utility>
#include <vector>
#include "vac/testing/test_adapter.h"
#include "vac/timer/timer_reactor_interface.h"

namespace vac {
namespace timer {

class Timer;

/**
 * \brief Comparison operator that compares Timer pointers based on their expiry timestamps
 *
 * \param a left timer
 * \param b right timer
 *
 * \return true, in case a expires before b. False otherwise.
 */
bool TimerExpiryCompare(const Timer* a, const Timer* b);

/**
 * \brief An event queue for Timer objects
 */
class TimerManager {
 public:
  /**
   * \brief Constructor for a TimerManager linked to a given reactor.
   */
  explicit TimerManager(TimerReactorInterface* reactor);
  TimerManager(const TimerManager&) = delete;
  TimerManager& operator=(const TimerManager&) = delete;
  TimerManager(const TimerManager&&) = delete;
  TimerManager&& operator=(const TimerManager&&) = delete;
  virtual ~TimerManager();

  /**
   * \brief Add a timer to be considered when computing the next expiry.
   *
   * \param timer Pointer to a Timer object.
   */
  virtual void AddTimer(Timer* const timer);

  /**
   * \brief No longer consider a timer when computing the next expiry.
   *
   * \param timer Pointer to a Timer object.
   */
  virtual void RemoveTimer(const Timer* timer);

  /**
   * \brief Return a pair of bool and timeval struct for the next expiring timer.
   *
   * \return a pair of bool (if valid NextExpiry) and timeval (relative to the current time representing the next
   * expiring timer if valid).
   */
  const std::pair<bool, struct timeval> GetNextExpiry() const;

  /**
   * \brief Callback to trigger firing timers.
   */
  void HandleTimerExpiry();

  /**
   * \brief Signals the TimerManager that any Timer has made changes to its expiry point.
   */
  virtual void Update();

  /**
   * \brief Determine whether there are any times currently running on this TimerManager.
   * \return true if there are active timers, false otherwise.
   */
  bool empty() const { return timers_.empty(); }

  /**
   * \brief Determine whether there are any times currently running on this TimerManager.
   * \return true if there are active timers, false otherwise.
   */
  size_t size() const { return timers_.size(); }

 private:
  Timer* GetNextTimer() const;

  /**
   * \brief Container for storing active timers.
   */
  using TimerContainer = std::vector<Timer*>;

  /**
   * \brief The reactor which is linked to the timer manager.
   */
  TimerReactorInterface* reactor_;

  /**
   * \brief The set of timers to consider for firing
   */
  TimerContainer timers_;

  FRIEND_TEST(TimerManager, AddTimer_MultipleDeadlines_1);
  FRIEND_TEST(TimerManager, AddTimer_MultipleDeadlines_2);
};

}  // namespace timer
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_TIMER_TIMER_MANAGER_H_
