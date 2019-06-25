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
/**        \file  service_discovery_client_state_repetition_phase.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_context.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief Service Discovery Client state 'REPETITION_PHASE'.
 */
class ServiceDiscoveryClientStateRepetitionPhase : public ServiceDiscoveryClientState {
 public:
  using ServiceDiscoveryClientState::ServiceDiscoveryClientState;
  /**
   * \brief Constructor of ServiceDiscoveryClientStateRepetitionPhase.
   */
  ServiceDiscoveryClientStateRepetitionPhase();
  /**
   * \brief Indicates whether a state change is valid or not.
   *
   * \param handle A state handle.
   * \return true if the state change is valid and false otherwise.
   */
  bool IsValidChange(const ServiceDiscoveryClientStateHandle handle) const override;
  /**
   * \brief Called when the state is entered.
   *
   * \param context A state context.
   */
  void OnEnter(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when the state is left.
   *
   * \param context A state context.
   */
  void OnLeave(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when network becomes active.
   *
   * \param context A state context.
   */
  void OnNetworkUp(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when network becomes inactive.
   *
   * \param context A state context.
   */
  void OnNetworkDown(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when service has been requested.
   *
   * \param context A state context.
   */
  void OnServiceRequested(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when service has been released.
   *
   * \param context A state context.
   */
  void OnServiceReleased(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param context A state context.
   */
  void OnOfferService(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param context A state context.
   */
  void OnStopOfferService(ServiceDiscoveryClientStateContext& context) override;
  /**
   * \brief Called when a previously scheduled delay has expired.
   *
   * \param context A state context.
   */
  void OnTimeout(ServiceDiscoveryClientStateContext& context) override;

 private:
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Counts number of repetitions so far.
   */
  std::size_t repetition_counter_;
  /**
   * \brief A time value to delay the next transmission of a FindService entry.
   */
  ServiceDiscoveryClientStateContext::TimerDelay timer_delay_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_REPETITION_PHASE_H_
