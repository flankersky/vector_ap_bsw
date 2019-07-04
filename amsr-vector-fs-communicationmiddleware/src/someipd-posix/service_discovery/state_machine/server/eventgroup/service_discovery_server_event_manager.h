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
/**        \file  service_discovery_server_event_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_SERVICE_DISCOVERY_SERVER_EVENT_MANAGER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_SERVICE_DISCOVERY_SERVER_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_message_sender.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/server/eventgroup/eventgroup_subscription.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {
namespace eventgroup {

/**
 * \brief Internal class within the server statemachine to manage un/subscription for eventgroups.
 */
class ServiceDiscoveryServerEventManager final : public EventgroupSubscriptionExpiryHandler {
 public:
  /**
   * \brief Constructor.
   *
   * \param timer_manager A timer manager.
   * \param address The IP address of this server service instance.
   * \param service_instance A server service instance.
   * \param config A configuration.
   * \param instance_config Provided Service configuration.
   * \param message_builder An SD Server message builder.
   * \param message_sender A message sender.
   */
  ServiceDiscoveryServerEventManager(vac::timer::TimerManager* timer_manager, const std::string& address,
                                     std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance,
                                     const configuration::Configuration* config,
                                     const configuration::Configuration::ProvidedServiceInstance* instance_config,
                                     ServiceDiscoveryServerMessageBuilder& message_builder,
                                     ServiceDiscoveryMessageSender& message_sender);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryServerEventManager(const ServiceDiscoveryServerEventManager&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServerEventManager& operator=(const ServiceDiscoveryServerEventManager&) = delete;

  /**
   * \brief Destructor.
   */
  ~ServiceDiscoveryServerEventManager();

  /**
   * \brief Adds a new subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void SubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Removes a subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void UnsubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                  const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Removes all eventgroup subscribers.
   */
  void UnsubscribeAllEventgroupEntries();

  /**
   * \brief Removes all event group subscription sent from from_address.
   * \param from_address An IP address identifying sender's IP address
   * \param from_port A port identifying sender's port.
   */
  void UnsubscribeAllEventgroupsOnReboot(const someip_posix_common::someip::IpAddress& from_address,
                                         someip_posix_common::someip::Port from_port);

  /**
   * \brief Called on eventgroup subscription expiration.
   *
   * \param egs An eventgroup subscription which expired.
   *
   * \return Always false (for OneShot timers, return value is ignored).
   */
  void OnSubscribeEventgroupExpired(EventgroupSubscription* egs) override;

 private:
  /**
   * \brief Handles a new eventgroup subscription.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void NewEventgroupSubscription(const std::string& from_address, std::uint16_t from_port,
                                 const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Handles a renewal of an eventgroup subscription.
   *
   * \param egs An eventgroup subscription context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   */
  void RenewEventgroupSubscription(EventgroupSubscription* egs, const std::string& from_address,
                                   std::uint16_t from_port, const ServiceDiscoveryEntry& entry);

  /**
   * \brief Cancels an eventgroup subscription.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void CancelEventgroupSubscription(const std::string& from_address, std::uint16_t from_port,
                                    const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Searches for an established eventgroup subscription.
   *
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   * \return An eventgroup subscription iterator.
   */
  EventgroupSubscriptionContainer::iterator FindEventgroupSubscription(const ServiceDiscoveryEntry& entry,
                                                                       const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief A container of active eventgroup subscriptions.
   */
  EventgroupSubscriptionContainer eventgroup_subscriptions_;
  /**
   * \brief A server service instance.
   */
  std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A configuration.
   */
  const configuration::Configuration* config_;
  /**
   * \brief The corresponding configuration of this server service instance.
   */
  const configuration::Configuration::ProvidedServiceInstance* instance_config_;
  /**
   * \brief The corresponding port mapping of this server service instance.
   */
  const configuration::Configuration::NetworkEndpointPortMapping* port_mapping_config_;
  /**
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder& message_builder_;
  /**
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSender& message_sender_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace eventgroup
}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_SERVICE_DISCOVERY_SERVER_EVENT_MANAGER_H_
