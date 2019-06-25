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
/**        \file  service_discovery_client_state_machine.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine_context.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_owner.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_pool.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_timer.h"
#include "vac/statemachine/state_owner.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief A Service Discovery Client state machine.
 */
class ServiceDiscoveryClientStateMachine : public ServiceDiscoveryClientStateContext {
 public:
  /**
   * \brief A timer delay.
   */
  using TimerDelay = ServiceDiscoveryClientStateContext::TimerDelay;
  /**
   * \brief Constructor of ServiceDiscoveryClientStateMachine.
   *
   * \param context A state machine context.
   * \param timer_manager A timer manager.
   */
  explicit ServiceDiscoveryClientStateMachine(ServiceDiscoveryClientStateMachineContext* context,
                                              vac::timer::TimerManager* timer_manager);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientStateMachine(const ServiceDiscoveryClientStateMachine&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientStateMachine& operator=(const ServiceDiscoveryClientStateMachine&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryClientStateMachine.
   */
  virtual ~ServiceDiscoveryClientStateMachine() = default;
  /**
   * \brief Called when network has been activated.
   */
  void OnNetworkUp();
  /**
   * \brief Called when network has been deactivated.
   */
  void OnNetworkDown();
  /**
   * \brief Called when the service has been requested.
   */
  void OnServiceRequested();
  /**
   * \brief Called when the service has been released.
   */
  void OnServiceReleased();
  /**
   * \brief Called when a matching OfferService entry has been received.
   */
  void OnOfferService();
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   */
  void OnStopOfferService();
  /**
   * \brief Called when a previously scheduled delay has expired.
   */
  void OnTimeout();

 protected:
  /**
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   */
  bool IsServiceAvailable() const override;
  /**
   * \brief Updates the availability of the service.
   *
   * \param is_available Indicates the availability of the service.
   */
  void SetServiceAvailable(bool is_available) override;
  /**
   * \brief Indicates whether network is active or not.
   *
   * \return true if network is active and false otherwise.
   */
  bool IsNetworkUp() const override;
  /**
   * \brief Indicates whether the service is requested or not.
   *
   * \return true if the service is requested and false otherwise.
   */
  bool IsServiceRequested() const override;
  /**
   * \brief Returns the TTL value from the last matching OfferService entry.
   *
   * \return TTL value.
   */
  TimerDelay GetOfferServiceTtl() const override;
  /**
   * \brief Returns the minimum time value to delay the transmission of a FindService entry.
   *
   * \return Minimum delay.
   */
  TimerDelay GetFindServiceInitialDelayMin() const override;
  /**
   * \brief Returns the maximum time value to delay the transmission of a FindService entry.
   *
   * \return Maximum delay.
   */
  TimerDelay GetFindServiceInitialDelayMax() const override;
  /**
   * \brief Returns the maximum number of transmissions of FindService entry.
   *
   * \return Maximum number of repetitions.
   */
  std::size_t GetFindServiceRepetitionsMax() const override;
  /**
   * \brief Returns the base delay for the transmission of FindService entry.
   *
   * \return Base delay.
   */
  TimerDelay GetFindServiceRepetitionsBaseDelay() const override;
  /**
   * \brief Transmits a FindService entry.
   */
  void SendFindService() override;
  /**
   * \brief Initiates a state change.
   *
   * \param handle The handle of a new state.
   */
  void ChangeState(const ServiceDiscoveryClientStateHandle handle) override;
  /**
   * \brief Starts the timer with a given delay.
   *
   * \param delay A timer delay.
   */
  void StartTimer(TimerDelay delay) override;
  /**
   * \brief Stops the timer.
   */
  void StopTimer() override;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Indicates whether the service is available.
   */
  bool is_service_available_;
  /**
   * \brief Indicates whether network is active or not.
   */
  bool is_network_up_;
  /**
   * \brief Indicates whether the service is requested or not.
   */
  bool is_service_requested_;
  /**
   * \brief A state machine context.
   */
  ServiceDiscoveryClientStateMachineContext* context_;
  /**
   * \brief A state pool.
   */
  ServiceDiscoveryClientStatePool state_pool_;
  /**
   * \brief A state owner.
   */
  ServiceDiscoveryClientStateOwner state_owner_;
  /**
   * \brief A timer for states.
   */
  ServiceDiscoveryClientStateTimer state_timer_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_H_
