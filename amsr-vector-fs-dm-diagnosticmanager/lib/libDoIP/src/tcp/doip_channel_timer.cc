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
/**        \file  doip_channel_timer.cc
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
 *  Walid Farouk Hussien               viswhn       Vector Informatik GmbH
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "ara/log/logging.hpp"

#include "doip_channel.h"
#include "doip_channel_timer.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace tcp {

DoIPChannelTimer::DoIPChannelTimer(DoIPChannel& channel, vac::timer::TimerManager* timer_manager)
    : vac::timer::Timer(timer_manager), channel_(channel) {}

bool DoIPChannelTimer::HandleTimer() {
  ara::log::LogDebug() << "Timer for DoIPChannel Elapse";
  bool success_to_transit_to_state = channel_.TryChangeState(StateHandle::kFinalize);
  assert(success_to_transit_to_state);
  return false;
}

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
