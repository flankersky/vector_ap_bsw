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
/**        \file  service_discovery_client_state_timer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_TIMER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

class ServiceDiscoveryClientStateMachine;

/**
 * \brief A timer for Service Discovery Client states.
 */
class ServiceDiscoveryClientStateTimer : public vac::timer::Timer {
 public:
  using vac::timer::Timer::Timer;
  /**
   * \brief Constructor of ServiceDiscoveryClientStateTimer.
   *
   * \param timer_manager A timer manager.
   * \param state_machine A Service Discovery Client state machine.
   */
  ServiceDiscoveryClientStateTimer(vac::timer::TimerManager* timer_manager,
                                   ServiceDiscoveryClientStateMachine* state_machine);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientStateTimer(const ServiceDiscoveryClientStateTimer&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientStateTimer& operator=(const ServiceDiscoveryClientStateTimer&) = delete;

 private:
  /**
   * \brief Called upon timer expiration and forwards the timeout to the state machine.
   *
   * \return true if the timer should be initiated anew and false otherwise.
   */
  bool HandleTimer() override;
  /**
   * \brief A Service Discovery Client state machine.
   */
  ServiceDiscoveryClientStateMachine* state_machine_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_TIMER_H_
