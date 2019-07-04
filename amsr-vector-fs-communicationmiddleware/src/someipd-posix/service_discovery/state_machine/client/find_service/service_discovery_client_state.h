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
/**        \file  service_discovery_client_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"
#include "vac/statemachine/state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief Service Discovery Client states.
 */
enum class ServiceDiscoveryClientStateHandle {
  kDownPhase = 0,         ///< DOWN_PHASE state
  kInitialWaitPhase = 1,  ///< INITIAL_WAIT_PHASE state
  kRepetitionPhase = 2,   ///< REPETITION_PHASE state
  kMainPhase = 3          ///< MAIN_PHASE state
};

/**
 * \brief A Service Discovery Client state.
 */
class ServiceDiscoveryClientState
    : public vac::statemachine::State<ServiceDiscoveryClientStateHandle, ServiceDiscoveryClientStateContext> {
 public:
  using vac::statemachine::State<ServiceDiscoveryClientStateHandle, ServiceDiscoveryClientStateContext>::State;
  /**
   * \brief Destructor of ServiceDiscoveryClientState.
   */
  virtual ~ServiceDiscoveryClientState() = default;
  /**
   * \brief Called when network becomes active.
   *
   * \param context A state context.
   */
  virtual void OnNetworkUp(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when network becomes inactive.
   *
   * \param context A state context.
   */
  virtual void OnNetworkDown(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when service has been requested.
   *
   * \param context A state context.
   */
  virtual void OnServiceRequested(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when service has been released.
   *
   * \param context A state context.
   */
  virtual void OnServiceReleased(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param context A state context.
   */
  virtual void OnOfferService(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param context A state context.
   */
  virtual void OnStopOfferService(ServiceDiscoveryClientStateContext& context) = 0;
  /**
   * \brief Called when a previously scheduled delay has elapsed.
   *
   * \param context A state context.
   */
  virtual void OnTimeout(ServiceDiscoveryClientStateContext& context) = 0;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_H_
