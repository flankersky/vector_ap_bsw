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
/**        \file  application.h
 *        \brief  Application
 *
 *      \details  This class represents the connection to a single application.
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket.h"
#include "osabstraction/io/network/socket/unix_domain_stream_socket.h"
#include "someip-posix-common/someipd_posix/routing/message_reader.h"
#include "someipd-posix/application/event_subscription_state_application_observer.h"
#include "someipd-posix/application/findservice_application_observer.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"

namespace someipd_posix {
namespace application {

/**
 * \brief Application.
 */
class Application : public std::enable_shared_from_this<Application>,
                    public packet_router::PacketSink,
                    public FindServiceApplicationObserver,
                    public EventSubscriptionStateApplicationObserver {
 public:
  /**
   * \brief Shutdown handler type.
   */
  using ShutdownHandler = std::function<void(Application*)>;
  /**
   * \brief Offered service instance entry.
   */
  struct OfferedServiceInstanceEntry {
    /**
     * \brief SOME/IP service identifier.
     */
    someip_posix_common::someip::ServiceId service_id_;
    /**
     * \brief SOME/IP instance identifier.
     */
    someip_posix_common::someip::InstanceId instance_id_;

    /**
     * \brief Compares this entry with another one.
     *
     * \param other Another entry to compare with.
     * \return true if both entries are equal and false otherwise.
     */
    bool operator<(const OfferedServiceInstanceEntry& other) const {
      return std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>(service_id_,
                                                                                                         instance_id_) <
             std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>(
                 other.service_id_, other.instance_id_);
    }
  };
  /**
   * \brief Represents a subscribed event.
   */
  struct SubscribedEvent {
    someip_posix_common::someip::ServiceId service_id_;    ///< SOME/IP service identifier
    someip_posix_common::someip::InstanceId instance_id_;  ///< SOME/IP service instance identifier
    someip_posix_common::someip::EventId event_id_;        ///< SOME/IP event identifier

    /**
     * \brief Compares this event with another one.
     *
     * \param other Another event to compare with.
     * \return true if both events are equal and false otherwise.
     */
    bool operator<(const SubscribedEvent& other) const {
      return std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId,
                        someip_posix_common::someip::EventId>(service_id_, instance_id_, event_id_) <
             std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId,
                        someip_posix_common::someip::EventId>(other.service_id_, other.instance_id_, other.event_id_);
    }
  };
  /**
   * \brief Requested service instance entry.
   */
  using RequestedServiceInstanceEntry =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Type-alias, because it's a bit long to type out the namespaces.
   */
  using MessageServiceDiscoveryOfferedServiceEntry =
      someip_posix_common::someipd_posix::routing::MessageServiceDiscoveryOfferedServiceEntry;
  /**
   * \brief Type-alias, because it's a bit long to type out the namespaces.
   */
  using MessageServiceDiscoveryEventSubscriptionStateEntry =
      someip_posix_common::someipd_posix::routing::MessageServiceDiscoveryEventSubscriptionStateEntry;
  /**
   * \brief Constructor of Application.
   *
   * \param packet_router A packet router.
   * \param socket A socket connected to an application.
   */
  Application(packet_router::PacketRouterInterface* packet_router,
              osabstraction::io::network::socket::UnixDomainStreamSocket&& socket);
  /**
   * \brief Destructor of Application.
   */
  virtual ~Application();
  /**
   * \brief Application is not copy-constructable.
   */
  Application(const Application& other) = delete;
  /**
   * \brief Application is not copy-assignable.
   */
  Application& operator=(const Application& other) = delete;
  /**
   * \brief Forwards a SOME/IP message to the connected application.
   *
   * \param instance_id Service instance id.
   * \param packet A pointer to a SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet) override;
  /**
   * \brief Notify about a service update (offer),
   * in case an asynchronous StartFindService() is requested by the client.
   * \remark This method is called by the ServiceDiscovery per application and the updated service instance once.
   *
   * \param service_id The Service identifier which has been offered.
   * \param instance_id The instance identifier from the StartFindService call.
   */
  void NotifyOfferService(const someip_posix_common::someip::ServiceId service_id,
                          const someip_posix_common::someip::InstanceId instance_id) override;
  /**
   * \brief Notify about a service update (stopped offer),
   * in case an asynchronous StartFindService() is requested by the client.
   *
   * \param service_id The service identifier of the stopped service.
   * \param instance_id The instance identifier of the stopped service.
   */
  void NotifyStopOfferService(const someip_posix_common::someip::ServiceId service_id,
                              const someip_posix_common::someip::InstanceId instance_id) override;
  /**
   * \brief Notify about event subscription state change, in case an event subscription is done by the client.
   *  Realizes the pure virtual method of the observer interface.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param event_id The event identifier of the event with the subscription state change.
   * \param new_state The new subscription state for this event.
   */
  void NotifyEventSubscriptionStateChange(
      const someip_posix_common::someip::ServiceId service_id,
      const someip_posix_common::someip::InstanceId instance_id, const someip_posix_common::someip::EventId event_id,
      const service_discovery::ServiceDiscoveryEventSubscriptionState new_state) override;
  /**
   * \brief Returns the routing channel identifier associated with the application.
   *
   * \return A routing channel identifier.
   */
  someip_posix_common::someip::ChannelId GetChannelId() const { return socket_.GetHandle(); }
  /**
   * \brief Registers a shutdown handler.
   *
   * \param handler A shutdown handler.
   */
  void SetShutdownHandler(ShutdownHandler handler) { shutdown_handler_ = handler; }
  /**
   * \brief Registers a new client identifier with the application.
   *
   * \param client_id A client identifier.
   */
  void AddClientId(someip_posix_common::someip::ClientId client_id) { client_ids_.insert(client_id); }
  /**
   * \brief Unregisters a client identifier with the application.
   *
   * \param client_id A client identifier.
   */
  void DeleteClientId(someip_posix_common::someip::ClientId client_id) { client_ids_.erase(client_id); }
  /**
   * \brief Registers an offered service instance with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   */
  void AddOfferedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                 someip_posix_common::someip::InstanceId instance_id) {
    offered_service_instances_.insert({service_id, instance_id});
  }
  /**
   * \brief Unregisters an offered service instance with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   */
  void DeleteOfferedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                    someip_posix_common::someip::InstanceId instance_id) {
    offered_service_instances_.erase({service_id, instance_id});
  }
  /**
   * \brief Registers a requested service instance with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   */
  void AddRequestedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id) {
    requested_service_instances_.insert({service_id, instance_id});
  }
  /**
   * \brief Unregisters a requested service instance with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   */
  void DeleteRequestedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                      someip_posix_common::someip::InstanceId instance_id) {
    auto it = requested_service_instances_.find({service_id, instance_id});
    assert(it != requested_service_instances_.end());
    requested_service_instances_.erase(it);
  }
  /**
   * \brief Registers an event with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   * \param event_id An event identifier.
   */
  void AddSubscribedEvent(someip_posix_common::someip::ServiceId service_id,
                          someip_posix_common::someip::InstanceId instance_id,
                          someip_posix_common::someip::EventId event_id) {
    subscribed_events_.insert({service_id, instance_id, event_id});
  }
  /**
   * \brief Unregisters an event with the application.
   *
   * \param service_id A service identifier.
   * \param instance_id An instance identifier.
   * \param event_id An event identifier.
   */
  void DeleteSubscribedEvent(someip_posix_common::someip::ServiceId service_id,
                             someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::EventId event_id) {
    subscribed_events_.erase({service_id, instance_id, event_id});
  }
  /**
   * \brief Returns a container of the client identifiers still registered with the application.
   *
   * \return A container of client identifiers.
   */
  const std::set<someip_posix_common::someip::ClientId>& GetClientIds() const { return client_ids_; }
  /**
   * \brief Returns a container of the offered service instances still registered with the application.
   *
   * \return A container of service instances.
   */
  const std::set<OfferedServiceInstanceEntry>& GetOfferedServiceInstances() const { return offered_service_instances_; }
  /**
   * \brief Returns a container of the requested service instances still registered with the application.
   *
   * \return A container of service instances.
   */
  const std::multiset<RequestedServiceInstanceEntry>& GetRequestedServiceInstances() const {
    return requested_service_instances_;
  }
  /**
   * \brief Returns a container of the subscribed events still registered with the application.
   *
   * \return A container of subscribed events.
   */
  const std::set<SubscribedEvent>& GetSubscribedEvents() const { return subscribed_events_; }
  /**
   * \brief Handler for asynchronous event notification on the socket connected to the application.
   *
   * \param handle A file descriptor.
   * \return false if the socket operation failed, true otherwise.
   */
  bool HandleRead(int handle);

  /**
   * \brief Set a pointer to the SD on init of the application object.
   *
   * \param service_discovery A pointer to the service discovery instance.
   */
  void SetServiceDiscoveryPtr(const someipd_posix::service_discovery::ServiceDiscovery* service_discovery);

 private:
  /**
   * \brief I/O vector container.
   */
  using IovecContainer = osabstraction::io::network::socket::Socket::IovecContainer;
  /**
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = someip_posix_common::someip::SomeIpMessage;
  /**
   * \brief A message reader type.
   */
  using MessageReader = someip_posix_common::someipd_posix::routing::MessageReader;
  /**
   * \brief Notify the client about a service discovery update, after a StartFindService()
   * has been called by the client.
   *
   * \param response A reference to the assembled response, that contains all service instances.
   * libARA then decides how to proceed and what to forward to which callback.
   */
  void ForwardServiceDiscoveryUpdate(std::vector<MessageServiceDiscoveryOfferedServiceEntry>& response);
  /**
   * \brief Notify the client about an event subscription state update, after the event is subscribed by the client.
   *
   * \param response A reference to the assembled response, that contains the exact event identifier and the
   * subscription state.
   * libARA then decides how to proceed and what to forward to which callback.
   */
  void ForwardServiceDiscoveryEventSubscriptionStateUpdate(
      MessageServiceDiscoveryEventSubscriptionStateEntry& response);
  /**
   * \brief Sends the routing channel id to the connected application.
   *
   * Each routing connection has a unique routing channel id. This routing channel id must be sent in some
   * of control messages sent by application to the SomeIP daemon in order to find the corresponding
   * routing socket.
   */
  void WriteRoutingChannelId();
  /**
   * \brief Handles a complete routing message received from an application.
   *
   * \param message A complete routing message received from the connected application.
   */
  void ProcessRoutingMessage(MessageReader::Message&& message);
  /**
   * \brief Sends I/O vector containers on the routing connection.
   *
   * \param iovc An I/O vector container.
   */
  void SendRoutingMessage(IovecContainer iovc);
  /**
   * \brief Sends I/O vector containers on the routing connection.
   *
   * \param header_iovc An I/O vector container for message header.
   * \param body_iovc An I/O vector container for message body.
   */
  void SendRoutingMessage(IovecContainer header_iovc, IovecContainer body_iovc);
  /**
   * \brief A packet router for SOME/IP message routing.
   */
  packet_router::PacketRouterInterface* const packet_router_;
  /** \brief subLogger for globally we use subLogger to create context Id for the logging messages
   *
   */
  ara::log::Logger& subLogger;
  /**
   * \brief A socket representing an established connection to an application.
   */
  osabstraction::io::network::socket::UnixDomainStreamSocket socket_;
  /**
   * \brief Reader for routing messages.
   */
  MessageReader message_reader_;
  /**
   * \brief A shutdown handler.
   */
  ShutdownHandler shutdown_handler_;
  /**
   * \brief Client IDs allocated by the application.
   */
  std::set<someip_posix_common::someip::ClientId> client_ids_;
  /**
   * \brief Service instances offered by the application.
   */
  std::set<OfferedServiceInstanceEntry> offered_service_instances_;
  /**
   * \brief Service instances requested by the application.
   */
  std::multiset<RequestedServiceInstanceEntry> requested_service_instances_;
  /**
   * \brief Events subscribed by the application.
   */
  std::set<SubscribedEvent> subscribed_events_;
  /**
   * \brief A pointer to the service discovery to get offered service instances.
   */
  const someipd_posix::service_discovery::ServiceDiscovery* service_discovery_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_H_
