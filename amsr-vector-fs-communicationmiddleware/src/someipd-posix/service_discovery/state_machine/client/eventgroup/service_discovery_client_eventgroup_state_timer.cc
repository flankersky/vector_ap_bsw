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
/**        \file  service_discovery_client_eventgroup_state_timer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_timer.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupStateTimer::ServiceDiscoveryClientEventgroupStateTimer(
    vac::timer::TimerManager* timer_manager, ServiceDiscoveryClientEventgroupStateMachine* state_machine)
    : Timer(timer_manager), state_machine_{state_machine} {}

bool ServiceDiscoveryClientEventgroupStateTimer::HandleTimer() {
  state_machine_->OnTimeout();
  return true;
}

ServiceDiscoveryClientEventgroupStateTtlTimer::ServiceDiscoveryClientEventgroupStateTtlTimer(
    vac::timer::TimerManager* timer_manager, ServiceDiscoveryClientEventgroupStateMachine* state_machine)
    : Timer(timer_manager), state_machine_{state_machine} {}

bool ServiceDiscoveryClientEventgroupStateTtlTimer::HandleTimer() {
  state_machine_->OnTtlTimeout();
  return true;
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
