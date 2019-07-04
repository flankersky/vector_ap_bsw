/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  conversation_timer_p2.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_P2_H_
#define SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_P2_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/timer/timer.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {
namespace timers {

/**
 * Interface for timer event subscriber
 */
class ConversationTimerP2EventHandler {
 public:
  /**
   * Timer event handler.
   * \param owning_timer The owning timer (to distinguish/multiplex between different timers in one handler)
   * \return true if the timer shall be scheduled again, false otherwise.
   * Returning true has no effect when the timer is not periodic.
   */
  virtual bool OnP2TimerElapsed(const vac::timer::Timer& owning_timer) = 0;

  /** Destructor */
  virtual ~ConversationTimerP2EventHandler() = default;
};

/**
 * \brief Timer implementation for P2 and P2* (P2 star)
 * This timer implementation is used by the Conversation for P2 and P2* handling.
 */
class ConversationTimerP2 : public vac::timer::Timer {
 public:
  /**
   * \brief constructor taking a timer_manager this Timer is associated with.
   * \param timer_manager The TimerManager
   * \param timer_listener The subscriber/listener for the timer event
   * \remarks The provided listener has to stay alive for the life-cycle of this instance
   */
  explicit ConversationTimerP2(vac::timer::TimerManager& timer_manager, ConversationTimerP2EventHandler& timer_listener)
      : vac::timer::Timer(&timer_manager), listener_(timer_listener) {}

 private:
  /**
   * \brief Handles the timer event
   * \return true if the timer shall be scheduled again, false otherwise.
   * Returning true has no effect when the timer is not periodic.
   */
  bool HandleTimer() final { return listener_.OnP2TimerElapsed(*this); }

 private:
  ConversationTimerP2EventHandler& listener_;
};

}  // namespace timers
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_P2_H_
