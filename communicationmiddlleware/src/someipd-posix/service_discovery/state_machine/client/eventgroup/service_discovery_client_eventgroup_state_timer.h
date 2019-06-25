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
/**        \file  service_discovery_client_eventgroup_state_timer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_TIMER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

class ServiceDiscoveryClientEventgroupStateMachine;

/**
 * \brief A generic timer for Service Discovery Client Eventgroup states.
 */
class ServiceDiscoveryClientEventgroupStateTimer : public vac::timer::Timer {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupStateTimer.
   *
   * \param timer_manager A timer manager.
   * \param state_machine A Service Discovery Client state machine.
   */
  ServiceDiscoveryClientEventgroupStateTimer(vac::timer::TimerManager* timer_manager,
                                             ServiceDiscoveryClientEventgroupStateMachine* state_machine);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientEventgroupStateTimer(const ServiceDiscoveryClientEventgroupStateTimer&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientEventgroupStateTimer& operator=(const ServiceDiscoveryClientEventgroupStateTimer&) = delete;

 private:
  /**
   * \brief Called upon timer expiration and forwards the timeout to the state machine.
   *
   * \return true if the timer should be initiated anew and false otherwise.
   */
  bool HandleTimer() override;
  /**
   * \brief A Service Discovery Client Eventgroup state machine.
   */
  ServiceDiscoveryClientEventgroupStateMachine* state_machine_;
};

/**
 * \brief A TTL timer for Service Discovery Client Eventgroup states.
 */
class ServiceDiscoveryClientEventgroupStateTtlTimer : public vac::timer::Timer {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupStateTtlTimer.
   *
   * \param timer_manager A timer manager.
   * \param state_machine A Service Discovery Client state machine.
   */
  ServiceDiscoveryClientEventgroupStateTtlTimer(vac::timer::TimerManager* timer_manager,
                                                ServiceDiscoveryClientEventgroupStateMachine* state_machine);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientEventgroupStateTtlTimer(const ServiceDiscoveryClientEventgroupStateTtlTimer&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientEventgroupStateTtlTimer& operator=(const ServiceDiscoveryClientEventgroupStateTtlTimer&) =
      delete;

 private:
  /**
   * \brief Called upon timer expiration and forwards the timeout to the state machine.
   *
   * \return true if the timer should be initiated anew and false otherwise.
   */
  bool HandleTimer() override;
  /**
   * \brief A Service Discovery Client Eventgroup state machine.
   */
  ServiceDiscoveryClientEventgroupStateMachine* state_machine_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_TIMER_H_
