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
/**        \file  doip_udp_timer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_udp_timer.h"

#include "ara/log/logging.hpp"

#include "udp/doip_udp_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace udp {

DoIPUDPTimer::DoIPUDPTimer(DoIPUDPHandler* udp_handler, vac::timer::TimerManager* timer_manager)
    : vac::timer::Timer(timer_manager), udp_handler_(udp_handler) {}

bool DoIPUDPTimer::HandleTimer() {
  ara::log::LogDebug() << "Timer for sending Vehicle Announcement Message elapsed";

  if (count_down_ > 0) {
    count_down_--;
    // Send the Message.
    udp_handler_->SendVehicleIdentMessage(broadcast_, remote_address_);
    // return true to let the timer work again.
    return true;
  } else {
    // return false to stop the timer.
    return false;
  }
}

}  // namespace udp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
