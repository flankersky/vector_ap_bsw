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
/**        \file  doip_channel_timer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_TIMER_H_
#define LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/timer/timer.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPChannel;  // forward declaration
namespace tcp {

/**
 * \brief Timer subclass
 */
class DoIPChannelTimer : public vac::timer::Timer {
 public:
  /**
   * \brief constructor taking a DoIPChannel and timer_manager this Timer is associated with.
   */
  explicit DoIPChannelTimer(DoIPChannel& channel, vac::timer::TimerManager* timer_manager);

  /**
   * \brief Handle Timer
   */
  bool HandleTimer();

 private:
  /**
   * \brief the DoIPChannel.
   */
  DoIPChannel& channel_;
};

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_TIMER_H_
