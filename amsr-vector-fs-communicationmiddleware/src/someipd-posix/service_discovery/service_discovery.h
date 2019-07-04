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
/**        \file  service_discovery.h
 *        \brief  Service discovery
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "osabstraction/io/reactor.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_connection_manager.h"
#include "someipd-posix/service_discovery/service_discovery_client_service_instance.h"
#include "someipd-posix/service_discovery/service_discovery_endpoint.h"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_context.h"
#include "someipd-posix/service_discovery/state_machine/service_discovery_service_instance_state_machine.h"
#include "vac/language/cpp14_backport.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {

// Forward declarations to avoid unnecessary includes.
class ServiceDiscoveryEventSubscriptionStateObserver;
class ServiceDiscoveryOfferServiceObserver;

namespace state_machine {
namespace client {
class ServiceDiscoveryClientServiceInstanceStateMachine;
}
namespace server {
class ServiceDiscoveryServerServiceInstanceStateMachine;
}
}

/**
 * \brief ServiceDiscovery.
 */
class ServiceDiscovery : public state_machine::client::ServiceDiscoveryClientContext,
                         public ServiceDiscoveryEndpointObserver {
 public:
  /**
   * \brief A SOME/IP service instance.
   */
  struct OfferedService {
    /**
     * \brief A SOME/IP service identifier.
     */
    std::uint16_t service_id_;
    /**
     * \brief A SOME/IP service instance identifier.
     */
    std::uint16_t instance_id_;
  };
  /**
   * \brief Container of offered SOME/IP services instances.
   */
  using OfferedServiceContainer = std::vector<OfferedService>;
  /**
   * \brief Container of service offers update observers.
   */
  using ServiceDiscoveryOfferServiceObserverContainer = std::vector<ServiceDiscoveryOfferServiceObserver*>;
  /**
   * \brief Container of event subscription state observers.
   */
  using ServiceDiscoveryEventSubscriptionStateObserverContainer =
      std::vector<ServiceDiscoveryEventSubscriptionStateObserver*>;
  /**
   * \brief Container of Service Instance StateMachines
   * Key: Unicast address of the remote SD.
   * value: Pointer to Service Instance StateMachine.
   */
  using ServiceDiscoveryServiceInstanceStateMachineContainer =
      std::multimap<someip_posix_common::someip::IpAddress,
                    state_machine::ServiceDiscoveryServiceInstanceStateMachine*>;
  /**
   * \brief Container of Server StateMachines
   */
  using ServiceDiscoveryServiceInstanceServerStateMachineContainer =
      std::vector<std::unique_ptr<state_machine::server::ServiceDiscoveryServerServiceInstanceStateMachine>>;
  /**
   * \brief Container of Client StateMachines
   */
  using ServiceDiscoveryServiceInstanceClientStateMachineContainer =
      std::vector<std::unique_ptr<state_machine::client::ServiceDiscoveryClientServiceInstanceStateMachine>>;
  /**
   * \brief Constructor of ServiceDiscovery.
   *
   * \param config A configuration.
   * \param reactor A reactor for asynchronous event notification on file descriptors.
   * \param packet_router A packet router.
   * \param timer_manager A timer manager.
   */
  ServiceDiscovery(const configuration::Configuration* config, osabstraction::io::ReactorInterface* reactor,
                   vac::timer::TimerManager* timer_manager, packet_router::PacketRouterInterface* packet_router);
  /**
   * \brief Destructor of ServiceDiscovery.
   */
  virtual ~ServiceDiscovery();
  /**
   * \brief ServiceDiscovery is not copy-constructable.
   */
  ServiceDiscovery(const ServiceDiscovery& other) = delete;
  /**
   * \brief ServiceDiscovery is not copy-assignable.
   */
  ServiceDiscovery& operator=(const ServiceDiscovery& other) = delete;
  /**
   * \brief Offers a new SOME/IP service instance.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param to A destination packet sink.
   */
  void OfferService(std::uint16_t service_id, std::uint16_t instance_id, std::shared_ptr<packet_router::PacketSink> to);
  /**
   * \brief Stops offering a SOME/IP service instance.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  void StopOfferService(std::uint16_t service_id, std::uint16_t instance_id);
  /**
   * \brief Returns a container of offered services.
   *
   * Return a copy in order to avoid concurrency issues.
   *
   * \return A container of offered services.
   */
  OfferedServiceContainer GetOfferedServices() const;
  /**
   * \brief Subscribes a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  void SubscribeEvent(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id,
                      std::shared_ptr<packet_router::PacketSink> to);
  /**
   * \brief Unsubscribes a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  void UnsubscribeEvent(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id,
                        std::shared_ptr<packet_router::PacketSink> to);
  /**
   * \brief Request a given SOME/IP service instance.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  void RequestService(std::uint16_t service_id, std::uint16_t instance_id);
  /**
   * \brief Release a given SOME/IP service instance.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  void ReleaseService(std::uint16_t service_id, std::uint16_t instance_id);
  /**
   * \brief Register an observer for service offers updates.
   */
  void RegisterServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver* observer);
  /**
   * \brief De-Register an observer for service offers updates.
   */
  void UnRegisterServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver* observer);
  /**
   * \brief Register an observer for event subscription state updates.
   *
   * \param observer The observer to be registered.
   */
  void RegisterServiceDiscoveryEventSubscriptionStateObserver(ServiceDiscoveryEventSubscriptionStateObserver* observer);
  /**
   * \brief De-Register an observer for event subscription state updates.
   *
   * \param observer The observer to be unregistered.
   */
  void UnRegisterServiceDiscoveryEventSubscriptionStateObserver(
      ServiceDiscoveryEventSubscriptionStateObserver* observer);

 private:
  /**
   * \brief Called when an eventgroup is subscribed.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  void OnEventgroupSubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                              const std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when an eventgroup is unsubscribed.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  void OnEventgroupUnsubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                                const std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when an eventgroup subscription is pending.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  void OnEventgroupSubscriptionPending(const std::uint16_t service_id, const std::uint16_t instance_id,
                                       const std::uint16_t eventgroup_id) override;
  /**
   * \brief Creates SD endpoints.
   */
  void CreateSdEndpoints();
  /**
   * \brief Creates SD server service instance state machines.
   */
  void CreateServerServiceInstanceStateMachines();
  /**
   * \brief Creates SD client service instance state machines.
   */
  void CreateClientServiceInstanceStateMachines();
  /**
   * \brief Creates SD static client service instances.
   */
  void CreateStaticClientServiceInstances();
  /**
   * \brief Starts SD client and server service instance state machines.
   */
  void StartServiceInstanceStateMachines();
  /**
   * \brief Stops SD client and server service instance state machines.
   */
  void StopServiceInstanceStateMachines();
  /**
   * \brief Subscribes a remote eventgroup.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param event_id A SOME/IP event identifier.
   */
  void SubscribeRemoteEventgroup(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id);
  /**
   * \brief Unsubscribes a remote eventgroup.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param event_id A SOME/IP event identifier.
   */
  void UnsubscribeRemoteEventgroup(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id);
  /**
   * \brief Called on reception of a single unicast SD message.
   * The function forwards then all entries and options to the all statemachines, which must then filter out there own
   * entries according to the entry service id, instance id and major version. If, for an entry, no matching state
   * machine is available, the entry is either ignored or, in case of subscribe eventgroup entries, is replied with a
   * NACK.
   * \uptrace [PRS_SOMEIPSD_00566]
   *
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address The unicast IP address of the sender of the SD message.
   * \param from_port The UDP port of the sender of the SD message.
   * \param entries SD entries contained in the message.
   * \param options SD options contained in the message.
   */
  void OnUnicastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                          const someip_posix_common::someip::IpAddress& from_address,
                          someip_posix_common::someip::Port from_port, const ServiceDiscoveryEntryContainer& entries,
                          const ServiceDiscoveryOptionContainer& options) override;
  /**
   * \brief Called on reception of a single multicast SD message.
   * The function forwards then all entries and options to the all statemachines, which must then filter out there own
   * entries according to the entry service id, instance id and major version. If, for an entry, no matching state
   * machine is available, the entry is ignored.
   * \uptrace [PRS_SOMEIPSD_00566]
   *
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address The unicast IP address of the sender of the SD message.
   * \param from_port The UDP port of the sender of the SD message.
   * \param entries SD entries contained in the message.
   * \param options SD options contained in the message.
   */
  void OnMulticastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                            const someip_posix_common::someip::IpAddress& from_address,
                            someip_posix_common::someip::Port from_port, const ServiceDiscoveryEntryContainer& entries,
                            const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Function to handle reboot detection for unicast and  multicast SOME/IP SD message.
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   */
  void OnRebootDetected(const someip_posix_common::someip::IpAddress& my_address,
                        const someip_posix_common::someip::IpAddress& from_address,
                        someip_posix_common::someip::Port from_port) override;

  /**
   * \brief Search offered_services_ for an instance with the given parameters.
   *
   * \param service_id The Service id.
   * \param instance_id The instance id.
   *
   * \return Iterator locating the found instance, or offered_services_.end() if no such instance found
   */
  OfferedServiceContainer::iterator FindOfferedService(std::uint16_t service_id, std::uint16_t instance_id);

  /**
   * \brief Search sd_service_instance_state_machine_map_ for an instance with the given parameters.
   *
   * \param address The unicast ip address of the remote SD.
   * \param service_id The Service id of the service instance.
   * \param instance_id The instance id of the service instance.
   * \param major_version Major version of the service instance.
   *
   * \return Iterator locating the found instance, or sd_service_instance_state_machine_map_.end() if no such
   * instance found
   */
  ServiceDiscoveryServiceInstanceStateMachineContainer::iterator FindStateMachine(
      const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::ServiceId service_id,
      someip_posix_common::someip::InstanceId instance_id, someip_posix_common::someip::MajorVersion major_version);

  /**
   * \brief Search sd_client_service_instance_state_machines_ for an instance with the given parameters.
   *
   * \param service_id The Service id.
   * \param instance_id The instance id.
   *
   * \return Iterator locating the found instance, or sd_client_service_instance_state_machines_.end() if no such
   * instance found
   */
  ServiceDiscoveryServiceInstanceClientStateMachineContainer::iterator FindClientStateMachine(
      std::uint16_t service_id, std::uint16_t instance_id);

  /**
   * \brief Search sd_server_service_instance_state_machines_ for an instance with the given parameters.
   *
   * \param service_id The Service id.
   * \param instance_id The instance id.
   *
   * \return Iterator locating the found instance, or sd_server_service_instance_state_machines_.end() if no such
   * instance found
   */
  ServiceDiscoveryServiceInstanceServerStateMachineContainer::iterator FindServerStateMachine(
      std::uint16_t service_id, std::uint16_t instance_id);

  /**
   * \brief Called when an event subscription state is changed.
   *  This implementation updates then all event_subscription_observers_ with that subscription state change.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param event_id The event identifier of the event with the subscription change.
   * \param new_state The new subscription state for this event.
   */
  void UpdateEventSubscriptionState(const std::uint16_t service_id, const std::uint16_t instance_id,
                                    const std::uint16_t event_id,
                                    const ServiceDiscoveryEventSubscriptionState new_state);

  /**
   * \brief Called when an eventgroup subscription state is changed.
   * This implementation calls UpdateEventSubscriptionState for each event within the given eventgroup.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier of the eventgroup with the subscription change.
   * \param new_state The new subscription state for this eventgroup.
   */
  void UpdateEventgroupSubscriptionState(const std::uint16_t service_id, const std::uint16_t instance_id,
                                         const std::uint16_t eventgroup_id,
                                         const ServiceDiscoveryEventSubscriptionState new_state);

  /**
   * \brief Update subscription state for all events within specific service instance.
   * This implementation calls UpdateEventSubscriptionState for each event within the given eventgroup.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param new_state The new subscription state for this eventgroup.
   */
  void UpdateServiceInstanceEventsSubscriptionState(const std::uint16_t service_id, const std::uint16_t instance_id,
                                                    const ServiceDiscoveryEventSubscriptionState new_state);
  /**
   * \brief Find the corresponding eventgroup id containing the given event.
   * If more than one eventgroup are found, the first one is returned.
   * //TODO(PAASR-3085) use a better selection strategy.
   *
   * \param service_id The service identifier of the service.
   * \param event_id The event identifier of the event.
   *
   * \return A pair; first entry is true if an eventgroup is found, false otherwise. Second entry is the
   * eventgroup id of the eventgroup containing the given event, or 0 if no eventgroup is found.
   */
  std::pair<bool, someip_posix_common::someip::EventgroupId> FindEventgroup(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::EventId event_id);
  /**
   * \brief A configuration
   */
  const configuration::Configuration* config_;
  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* packet_router_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Container of offered services.
   */
  OfferedServiceContainer offered_services_;
  /**
   * \brief A container of registered service offers observers.
   */
  ServiceDiscoveryOfferServiceObserverContainer offer_service_observers_;
  /**
   * \brief A container of registered event subscription state observers.
   */
  ServiceDiscoveryEventSubscriptionStateObserverContainer event_subscription_observers_;

  /**
   * \brief SD endpoints.
   */
  std::vector<std::unique_ptr<ServiceDiscoveryEndpoint>> sd_endpoints_;
  /**
   * \brief A container of service endpoints.
   */
  connection_manager::ServiceDiscoveryConnectionManager conman_;
  /**
   * \brief SD client service instance state machines.
   */
  ServiceDiscoveryServiceInstanceClientStateMachineContainer sd_client_service_instance_state_machines_;
  /**
   * \brief SD server service instance state machines.
   */
  ServiceDiscoveryServiceInstanceServerStateMachineContainer sd_server_service_instance_state_machines_;
  /**
   * \brief Maps a unicast IP address to the corresponding SD service instance state machines.
   */
  ServiceDiscoveryServiceInstanceStateMachineContainer sd_service_instance_state_machine_map_;
  /**
   * \brief Static client service instances.
   */
  std::vector<std::shared_ptr<ServiceDiscoveryClientServiceInstance>> static_client_service_instances_;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_H_
