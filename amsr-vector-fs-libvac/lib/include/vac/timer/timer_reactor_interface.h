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
/**        \file
 *        \brief  Defines the interface between the timer manager and the reactor.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_TIMER_TIMER_REACTOR_INTERFACE_H_
#define LIB_INCLUDE_VAC_TIMER_TIMER_REACTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace vac {
namespace timer {

/**
 * \brief Interface for a class which wants to be informed about timer expiry events.
 */
class TimerReactorInterface {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~TimerReactorInterface() = default;

  /**
   * \brief Function which shall be called, if a timer event is added which is the new earliest event.
   */
  virtual void Unblock() = 0;
};

}  // namespace timer
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_TIMER_TIMER_REACTOR_INTERFACE_H_
