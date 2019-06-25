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
/**        \file  service_discovery_client_state_pool.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_down_phase.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_initial_wait_phase.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_main_phase.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_repetition_phase.h"
#include "vac/statemachine/state_pool.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief A Service Discovery Client state pool.
 */
class ServiceDiscoveryClientStatePool : public vac::statemachine::StatePool<ServiceDiscoveryClientState> {
 public:
  /**
   * \brief Returns the instance of a state identified by the given state handle.
   *
   * \param handle A state handle.
   * \return An instance of a state.
   */
  ServiceDiscoveryClientState* GetState(const ServiceDiscoveryClientStateHandle handle) override;
  /**
   * \brief Indicates whether there is a state identified by the given handle.
   *
   * \param handle A state handle.
   * \return true if the state is valid and false otherwise.
   */
  bool IsValid(const ServiceDiscoveryClientStateHandle handle) const override;

 private:
  /**
   * \brief An instance of state 'DOWN_PHASE'.
   */
  ServiceDiscoveryClientStateDownPhase state_down_phase_;
  /**
   * \brief An instance of state 'INITIAL_WAIT_PHASE'.
   */
  ServiceDiscoveryClientStateInitialWaitPhase state_initial_wait_phase_;
  /**
   * \brief An instance of state 'REPETITION_PHASE'.
   */
  ServiceDiscoveryClientStateRepetitionPhase state_repetition_phase_;
  /**
   * \brief An instance of state 'MAIN_PHASE'.
   */
  ServiceDiscoveryClientStateMainPhase state_main_phase_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_POOL_H_
