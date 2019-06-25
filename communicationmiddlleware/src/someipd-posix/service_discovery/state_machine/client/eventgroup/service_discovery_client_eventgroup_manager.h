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
/**        \file  service_discovery_client_eventgroup_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_manager_context.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief A Service Discovery Client eventgroup manager.
 */
class ServiceDiscoveryClientEventgroupManager : public ServiceDiscoveryClientEventgroupStateMachineContext {
 public:
  /**
   * \brief A timer delay.
   */
  using TimerDelay = ServiceDiscoveryClientEventgroupStateContext::TimerDelay;
  // TODO(PAASR-1034)
  /**
   * \brief A container of SOME/IP eventgroup identifiers.
   */
  using EventgroupIdContainer = std::vector<std::uint16_t>;
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupManager.
   *
   * \param eventgroups A container of all SOME/IP eventgroups supported by the service.
   * \param context An eventgroup manager context.
   * \param timer_manager A timer manager.
   */
  ServiceDiscoveryClientEventgroupManager(const EventgroupIdContainer& eventgroups,
                                          ServiceDiscoveryClientEventgroupManagerContext* context,
                                          vac::timer::TimerManager* timer_manager);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientEventgroupManager(const ServiceDiscoveryClientEventgroupManager&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientEventgroupManager& operator=(const ServiceDiscoveryClientEventgroupManager&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryClientEventgroupManager.
   */
  virtual ~ServiceDiscoveryClientEventgroupManager() = default;

  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current eventgroup subscription state.
   */
  ServiceDiscoveryEventSubscriptionState GetEventgroupSubscriptionState(std::uint16_t eventgroup_id) const;
  /**
   * \brief Gets the current subscribers count of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current eventgroup subscribers count.
   */
  std::size_t GetEventgroupSubscribersCount(std::uint16_t eventgroup_id) const;
  /**
   * \brief Returns the multicast IP address and UDP port assigned to the given SOME/IP eventgroup by server.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return A pair of a multicast IP address and an UDP port.
   */
  std::pair<std::string, std::uint16_t> GetEventgroupMulticastEndpoint(std::uint16_t eventgroup_id);
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
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnSubscribe(std::uint16_t eventgroup_id);
  /**
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnUnsubscribe(std::uint16_t eventgroup_id);
  /**
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param multicast_address A multicast IP address assigned to the SOME/IP eventgroup by server.
   * \param multicast_port A multicast UDP port assigned to the SOME/IP eventgroup by server.
   * \param ttl A TTL value sent by server.
   */
  void OnSubscribeEventgroupAck(std::uint16_t eventgroup_id, const std::string& multicast_address,
                                std::uint16_t multicast_port, TimerDelay ttl);
  /**
   * \brief Called when a matching StopSubscribeEventgroupAck entry has been received.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnSubscribeEventgroupNack(std::uint16_t eventgroup_id);
  /**
   * \brief Called when the service has been released.
   */
  void OnServiceReleased();

 protected:
  /**
   * \brief Initiates the transmission of a SubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   */
  void SendSubscribeEventgroup(std::uint16_t eventgroup_id, bool request_initial_events) override;
  /**
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void SendStopSubscribeEventgroup(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupSubscribed(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupUnsubscribed(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup subscription is pending.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupSubscriptionPending(std::uint16_t eventgroup_id) override;

 private:
  /**
   * \brief An eventgroup state machine pointer.
   */
  using ServiceDiscoveryClientEventgroupStateMachinePtr = std::unique_ptr<ServiceDiscoveryClientEventgroupStateMachine>;
  /**
   * \brief A container of eventgroup state machine pointers.
   */
  using ServiceDiscoveryClientEventgroupStateMachineContainer =
      std::vector<ServiceDiscoveryClientEventgroupStateMachinePtr>;
  /**
   * \brief Returns an eventgroup state machine for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return An iterator pointing to an eventgroup state machine.
   */
  ServiceDiscoveryClientEventgroupStateMachineContainer::iterator GetEventgroup(std::uint16_t eventgroup_id);
  /**
   * \brief Returns an eventgroup state machine for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return A const iterator pointing to an eventgroup state machine.
   */
  ServiceDiscoveryClientEventgroupStateMachineContainer::const_iterator GetEventgroup(
      std::uint16_t eventgroup_id) const;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief An eventgroup manager context.
   */
  ServiceDiscoveryClientEventgroupManagerContext* context_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A container of all eventgroup state machines.
   */
  ServiceDiscoveryClientEventgroupStateMachineContainer eventgroup_state_machines_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_H_
