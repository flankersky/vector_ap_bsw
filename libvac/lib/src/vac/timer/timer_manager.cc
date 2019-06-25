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
/**        \file  timer_manager.cc
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
#include "vac/timer/timer_manager.h"
#include <algorithm>
#include "vac/timer/timer.h"
#include "vac/timer/timer_reactor_interface.h"

namespace vac {
namespace timer {

bool TimerExpiryCompare(const Timer* a, const Timer* b) { return a->GetNextExpiry() > b->GetNextExpiry(); }

TimerManager::TimerManager(TimerReactorInterface* reactor) : reactor_(reactor), timers_() {}

TimerManager::~TimerManager() = default;

void TimerManager::AddTimer(Timer* const timer) {
  if (timers_.empty()) {
    timers_.push_back(timer);
    if (reactor_ != nullptr) {
      reactor_->Unblock();
    }
  } else {
    Timer::Clock::time_point existing_next_expiry = GetNextTimer()->GetNextExpiry();

    // Heapified insert
    timers_.push_back(timer);
    Update();

    if (timer->GetNextExpiry() < existing_next_expiry && reactor_ != nullptr) {
      // New timer will expire before any existing timer
      reactor_->Unblock();
    }
  }
}

void TimerManager::RemoveTimer(const Timer* timer) {
  auto it = std::find(timers_.begin(), timers_.end(), timer);
  if (it != timers_.end()) {
    timers_.erase(it);
  }
  Update();
}

void TimerManager::Update() { std::make_heap(timers_.begin(), timers_.end(), TimerExpiryCompare); }

void TimerManager::HandleTimerExpiry() {
  // Loop over timers to find which ones have expired
  // Do not use an iterator, as handling a timer may modify the timer queue.
  for (Timer* next_timer = GetNextTimer(); next_timer != nullptr && next_timer->IsExpired();
       next_timer = GetNextTimer()) {
    next_timer->DoHandleTimer();
    // No call to update - call has to be done by the timer
  }
}

const std::pair<bool, struct timeval> TimerManager::GetNextExpiry() const {
  Timer* next_timer = GetNextTimer();
  std::pair<bool, struct timeval> result;
  if (next_timer == nullptr) {
    result.first = false;
  } else {
    result.first = true;
    result.second = next_timer->ToTimeval();
  }
  return result;
}

Timer* TimerManager::GetNextTimer() const {
  if (timers_.empty()) {
    return nullptr;
  } else {
    return timers_.front();
  }
}

}  // namespace timer
}  // namespace vac
