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
/**        \file  service_discovery_client_eventgroup_state_machine.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_MACHINE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <string>
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_machine_context.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_owner.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_pool.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief A Service Discovery Client eventgroup state machine.
 */
class ServiceDiscoveryClientEventgroupStateMachine : public ServiceDiscoveryClientEventgroupStateContext {
 public:
  /**
   * \brief A timer delay.
   */
  using TimerDelay = ServiceDiscoveryClientEventgroupStateContext::TimerDelay;
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupStateMachine.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param context A state machine context.
   * \param timer_manager A timer manager.
   */
  ServiceDiscoveryClientEventgroupStateMachine(std::uint16_t eventgroup_id,
                                               ServiceDiscoveryClientEventgroupStateMachineContext* context,
                                               vac::timer::TimerManager* timer_manager);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientEventgroupStateMachine(const ServiceDiscoveryClientEventgroupStateMachine&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientEventgroupStateMachine& operator=(const ServiceDiscoveryClientEventgroupStateMachine&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryClientEventgroupStateMachine.
   */
  virtual ~ServiceDiscoveryClientEventgroupStateMachine() = default;
  /**
   * \brief Returns the SOME/IP eventgroup identifier.
   *
   * \return A SOME/IP eventgroup identifier.
   */
  std::uint16_t GetEventgroupId() const;
  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \return Current eventgroup subscription state.
   */
  ServiceDiscoveryEventSubscriptionState GetSubscriptionState() const;
  /**
   * \brief Gets the current count of the eventgroup subscribers.
   *
   * \return Current eventgroup subscribers count.
   */
  std::size_t GetSubscribersCount() const;
  /**
   * \brief Returns the multicast IP address assigned to the SOME/IP eventgroup.
   *
   * \return A multicast IP address.
   */
  const std::string& GetMulticastAddress() const;
  /**
   * \brief Returns the multicast UDP port number assigned to the SOME/IP eventgroup.
   *
   * \return A multicast UDP port.
   */
  std::uint16_t GetMulticastPort() const;
  /**
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   */
  void OnOfferService(bool is_multicast);
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   */
  void OnStopOfferService();
  /**
   * \brief Called when the SOME/IP eventgroup has been requested.
   */
  void OnSubscribe();
  /**
   * \brief Called when the SOME/IP eventgroup has been released.
   */
  void OnUnsubscribe();
  /**
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param multicast_address A multicast IP address assigned to the SOME/IP eventgroup by the server.
   * \param multicast_port A multicast UDP port assigned to the SOME/IP eventgroup by the server.
   * \param ttl A time period after which the subscription becomes invalid.
   */
  void OnSubscribeEventgroupAck(const std::string& multicast_address, std::uint16_t multicast_port, TimerDelay ttl);
  /**
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   */
  void OnSubscribeEventgroupNack();
  /**
   * \brief Called when a previously scheduled delay has expired.
   */
  void OnTimeout();
  /**
   * \brief Called when a previously scheduled TTL delay has expired.
   */
  void OnTtlTimeout();
  /**
   * \brief Called when the eventgroup shall be shut down.
   */
  void OnShutdown();

 protected:
  /**
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   */
  bool IsServiceAvailable() const override;
  /**
   * \brief Indicates whether the SOME/IP eventgroup is requested or not.
   *
   * \return true if the SOME/IP eventgroup is requested and false otherwise.
   */
  bool IsEventgroupRequested() const override;
  /**
   * \brief Returns the maximum wait time for a SubscribeEventgroupAck entry.
   *
   * \return A time value.
   */
  TimerDelay GetSubscribeEventgroupAckTimeout() const override;
  /**
   * \brief Returns the time value after which the subscription of the SOME/IP eventgroup expires.
   *
   * \return A time value.
   */
  TimerDelay GetSubscriptionTtl() const override;
  /**
   * \brief Initiates the transmission of a SubscribeEventgroup entry.
   *
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   */
  void SendSubscribeEventgroup(bool request_initial_events) override;
  /**
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry.
   */
  void SendStopSubscribeEventgroup() override;
  /**
   * \brief Initiates a state change.
   *
   * \param handle The handle of a new state.
   */
  void ChangeState(const ServiceDiscoveryClientEventgroupStateHandle handle) override;
  /**
   * \brief Starts the generic timer with a given delay.
   *
   * \param delay A timer delay.
   */
  void StartTimer(TimerDelay delay) override;
  /**
   * \brief Stops the generic timer.
   */
  void StopTimer() override;
  /**
   * \brief Starts the TTL timer with a given delay.
   *
   * \param delay A timer delay.
   */
  void StartTtlTimer(TimerDelay delay) override;
  /**
   * \brief Stops the TTL timer.
   */
  void StopTtlTimer() override;
  /**
   * \brief Called by a state when the SOME/IP eventgroup has been subscribed.
   */
  void OnSubscribed() override;
  /**
   * \brief Called by a state when the SOME/IP eventgroup has been unsubscribed.
   */
  void OnUnsubscribed() override;
  /**
   * \brief Called by a state when the SOME/IP eventgroup subscription is pending.
   */
  void OnSubscriptionPending() override;
  /**
   * \brief The maximum wait time for a SubscribeEventgroupAck entry after the transmission of a SubscribeEvntgroup
   * entry.
   */
  static const std::chrono::milliseconds kSubscribeEventgroupAckTimeout;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A SOME/IP eventgroup identifier.
   */
  const std::uint16_t eventgroup_id_;
  /**
   * \brief Indicates whether the service is available or not.
   */
  bool is_service_available_;
  /**
   * \brief The number of times the SOME/IP eventgroup has been subscribed.
   */
  std::atomic<std::size_t> subscription_counter_;
  /**
   * \brief A multicast IP address assigned to the SOME/IP eventgroup by server.
   */
  std::string subscription_multicast_address_;
  /**
   * \brief A multicast UDP port assigned to the SOME/IP eventgroup by server.
   */
  std::uint16_t subscription_multicast_port_;
  /**
   * \brief A time value after which the subscription of the SOME/IP eventgroup expires.
   */
  TimerDelay subscription_ttl_;
  /**
   * \brief A state machine context.
   */
  ServiceDiscoveryClientEventgroupStateMachineContext* context_;
  /**
   * \brief A state pool.
   */
  ServiceDiscoveryClientEventgroupStatePool state_pool_;
  /**
   * \brief A state owner.
   */
  ServiceDiscoveryClientEventgroupStateOwner state_owner_;
  /**
   * \brief A generic timer for states.
   */
  ServiceDiscoveryClientEventgroupStateTimer state_timer_;
  /**
   * \brief A TTL timer for states.
   */
  ServiceDiscoveryClientEventgroupStateTtlTimer state_ttl_timer_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_MACHINE_H_
