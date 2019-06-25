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
/**        \file  service_discovery_server_state_owner.h
 *        \brief  Service Discovery Server State Owner
 *
 *      \details  This class is the owner of the State Pool. It receives events and forward them to the current state.
 *It holds as well more variables that are not represented as separate states.
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_pool.h"
#include "vac/statemachine/state_owner.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief ServiceDiscoveryServerStateOwner Server State Owner
 */
class ServiceDiscoveryServerStateOwner : public vac::statemachine::StateOwner<ServiceDiscoveryServerState> {
 private:
  /**
   * \brief State machine timer.
   */
  class ServiceDiscoveryServerStateMachineTimer : public vac::timer::Timer {
   public:
    /**
     * \brief Constructor of Timer.
     *
     * \param timer_manager A timer manager.
     * \param owner State Machine owner.
     */
    ServiceDiscoveryServerStateMachineTimer(vac::timer::TimerManager* timer_manager,
                                            ServiceDiscoveryServerStateOwner* owner)
        : vac::timer::Timer(timer_manager), owner_(owner) {}

    /**
     * \brief Delete copy constructor
     */
    ServiceDiscoveryServerStateMachineTimer(const ServiceDiscoveryServerStateMachineTimer&) = delete;

    /**
     * \brief Delete copy assignment operator.
     */
    ServiceDiscoveryServerStateMachineTimer& operator=(const ServiceDiscoveryServerStateMachineTimer&) = delete;
    /**
     * \brief Called on timer expiration.
     *
     * \return true if the timer shall be scheduled again, false otherwise. Returning true has no effect when the timer
     * is
     * not periodic.
     */
    bool HandleTimer() override { return owner_->OnTimeOut(); }

   private:
    /**
     * \brief A timer callback.
     */
    ServiceDiscoveryServerStateOwner* owner_;
  };

 public:
  /**
   * \brief Constructor
   *
   * \param state_pool State pool.
   * \param timer_manager A timer manager.
   */
  ServiceDiscoveryServerStateOwner(ServiceDiscoveryServerStatePool& state_pool,
                                   vac::timer::TimerManager* timer_manager);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryServerStateOwner(const ServiceDiscoveryServerStateOwner&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServerStateOwner& operator=(const ServiceDiscoveryServerStateOwner&) = delete;

  /**
   * \brief Sets the Context.
   *
   * \param context The server state machine context.
   */
  void SetContext(ServiceDiscoveryServerContextInterface* context);

  /**
   * \brief Returns the context used by context depending State methods.
   */
  ServiceDiscoveryServerContextInterface& GetContext();

  /// Current State Variables

  /**
   * \brief Checks if service discovery is initialized
   *
   * \return True if service discovery is initialized, false otherwise
   */
  bool IsServiceStarted();

  /**
   * \brief Checks the current network state
   *
   * \return True if network is up, false otherwise
   */
  bool IsNetworkUp();

  /**
   * \brief Checks the current service state
   *
   * \return true of service is requested, false if released
   */
  bool IsServiceUp();

  /// State Control

  /**
   * \brief Changes current state to requested_next_state.
   * if requested_next_state is different from current state, a trial to change the current state to
   * requested_next_state is done. Otherwise, does nothing.
   */
  void UpdateState();

  /// Timer

  /**
   * \brief Start a timer with the given period.
   *
   * \param period timer period.
   * \param periodic true if the timer runs periodically, false if the timer should start as one shot.
   */
  void StartTimer(vac::timer::Timer::Clock::duration period, bool periodic);

  /**
   * \brief Stop the currently running timer.
   */
  void StopTimer();

  /**
   * \brief Called when timer is timed out
   *
   * \return true if the timer should rerun for another period, false otherwise
   */
  bool OnTimeOut();

  /// Events

  /**
   * \brief Called when network is brought up.
   */
  void OnNetworkUp();

  /**
   * \brief Called when network is brought down.
   */
  void OnNetworkDown();

  /**
   * \brief Called when a provided service instance is brought up.
   */
  void OnServiceUp();

  /**
   * \brief Called when a provided service instance is brought down.
   */
  void OnServiceDown();

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via unicast.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnUnicastFindServiceEntry(const std::string& from_address, std::uint16_t from_port,
                                 const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via multicast.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnMulticastFindServiceEntry(const std::string& from_address, std::uint16_t from_port,
                                   const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called on reception of a SOME/IP SD SubscribeEventgroup entry.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnSubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                  const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called on reception of a SOME/IP SD StopSubscribeEventgroup entry.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD StopSubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnStopSubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                      const ServiceDiscoveryEntry& entry,
                                      const ServiceDiscoveryOptionContainer& options);

 private:
  /**
   * \brief state machine context
   */
  ServiceDiscoveryServerContextInterface* context_{nullptr};
  /**
   * \brief A timer for cyclic offer service SD messages.
   */
  ServiceDiscoveryServerStateMachineTimer sm_timer_;
  /**
   * \brief network status; true = connected, false = disconnected.
   */
  bool network_up_{false};
  /**
   * \brief server service status; true = available, false = not available.
   */
  bool service_up_{false};
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_OWNER_H_
