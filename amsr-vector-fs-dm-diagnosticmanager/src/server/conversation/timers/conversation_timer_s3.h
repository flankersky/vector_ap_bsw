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
/**        \file  conversation_timer_s3.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_S3_H_
#define SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_S3_H_

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
class ConversationTimerS3EventHandler {
 public:
  /**
   * Timer event handler.
   * \return true if the timer shall be scheduled again, false otherwise.
   * Returning true has no effect when the timer is not periodic.
   */
  virtual bool OnS3TimerElapsed() = 0;

  /** Destructor */
  virtual ~ConversationTimerS3EventHandler() = default;
};

/**
 * \brief Timer subclass
 */
class ConversationTimerS3 : public vac::timer::Timer {
 public:
  /**
   * \brief constructor taking a timer_manager this Timer is associated with.
   * \param timer_manager The TimerManager
   * \param timer_listener The subscriber/listener for the timer event
   * \remarks The provided listener has to stay alive for the life-cycle of this instance
   */
  explicit ConversationTimerS3(vac::timer::TimerManager& timer_manager, ConversationTimerS3EventHandler& timer_listener)
      : vac::timer::Timer(&timer_manager), listener_(timer_listener) {}

 private:
  /**
    * \brief Handle the timer.
    */
  bool HandleTimer() final { return listener_.OnS3TimerElapsed(); }

 private:
  ConversationTimerS3EventHandler& listener_;
};

}  // namespace timers
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_TIMERS_CONVERSATION_TIMER_S3_H_
