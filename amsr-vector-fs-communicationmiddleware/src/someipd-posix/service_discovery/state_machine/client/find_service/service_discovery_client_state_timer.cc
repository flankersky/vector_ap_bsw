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
/**        \file  service_discovery_client_state_timer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_timer.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateTimer::ServiceDiscoveryClientStateTimer(vac::timer::TimerManager* timer_manager,
                                                                   ServiceDiscoveryClientStateMachine* state_machine)
    : Timer(timer_manager), state_machine_{state_machine} {}

bool ServiceDiscoveryClientStateTimer::HandleTimer() {
  state_machine_->OnTimeout();
  return true;
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
