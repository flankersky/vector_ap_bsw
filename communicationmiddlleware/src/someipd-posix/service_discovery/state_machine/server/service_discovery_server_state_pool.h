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
/**        \file  service_discovery_server_state_pool.h
 *        \brief  Service Discovery Server State Pool
 *
 *      \details  This class initializes and contains all states of the SD Server State Machine
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_down.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_main.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_repetition.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_wait.h"
#include "vac/statemachine/state_pool.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief ServiceDiscoveryServerStatePool.
 */
class ServiceDiscoveryServerStatePool : public vac::statemachine::StatePool<ServiceDiscoveryServerState> {
 public:
  /**
   * \brief ServiceDiscoveryServerStatePool
   */
  ServiceDiscoveryServerStatePool() = default;

  /**
   * \brief Returns State of given handle.
   *
   * \param handle The state handle.
   *
   * \return The State with given handle.
   * If handle does not refer to a State, exception of type std::out_of_range is thrown.
   */
  virtual ServiceDiscoveryServerState* GetState(const Handle handle);

  /**
   * \brief Checks if handle refers to an existing state.
   *
   * \param handle The state handle.
   *
   * \return true if state exists with given handle, otherwise returns false.
   */
  virtual bool IsValid(const Handle handle) const;

 private:
  /**
   * \brief Down phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateDown down_state_;

  /**
   * \brief Initial Wait phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateWait initial_wait_state_;

  /**
   * \brief Repetition phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateRepetition repetition_state_;

  /**
   * \brief Main phase of the SD server state machine.
   */
  ServiceDiscoveryServerStateMain main_state_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_POOL_H_
