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
/**        \file  service_discovery_tcp_endpoint.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_ENDPOINT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_receiver.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_response_sender.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_sender.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_server.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpEndpoint;

/**
 * \brief ServiceDiscoveryTcpEndpoint.
 */
class ServiceDiscoveryTcpEndpoint : public std::enable_shared_from_this<ServiceDiscoveryTcpEndpoint>,
                                    public osabstraction::io::EventHandler {
 public:
  /**
   * \brief A TCP sender pointer type.
   */
  using ServiceDiscoveryTcpSenderPtr = std::unique_ptr<ServiceDiscoveryTcpSender>;
  /**
   * \brief A TCP receiver pointer type.
   */
  using ServiceDiscoveryTcpReceiverPtr = std::unique_ptr<ServiceDiscoveryTcpReceiver>;
  /**
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = someip_posix_common::someip::SomeIpMessage;
  /**
   * \brief Constructor of ServiceDiscoveryTcpEndpoint.
   *
   * \param reactor A reactor.
   * \param packet_router A packet router.
   * \param address A local IP address.
   * \param port A local port number.
   * \param options Additional options for TCP sockets (QoS, KeepAlive).
   */
  ServiceDiscoveryTcpEndpoint(osabstraction::io::ReactorInterface* reactor,
                              packet_router::PacketRouterInterface* packet_router,
                              const someip_posix_common::someip::IpAddress& address,
                              someip_posix_common::someip::Port port,
                              const someip_posix_common::someip::SocketOptions& options);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryTcpEndpoint(const ServiceDiscoveryTcpEndpoint&) = delete;

  /**
   * \brief Delete copy assignment operator
   */
  ServiceDiscoveryTcpEndpoint& operator=(const ServiceDiscoveryTcpEndpoint&) = delete;

  /**
   * \brief Returns the local IP address.
   *
   * \return A local IP address.
   */
  const someip_posix_common::someip::IpAddress& GetAddress() const;
  /**
   * \brief Returns the local port number.
   *
   * \return A local port number.
   */
  someip_posix_common::someip::Port GetPort() const;
  /**
   * \brief Indicates whether an active TCP connection exists.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return true if the specified active TCP connection exists and false otherwise.
   */
  bool HasActiveConnection(const someip_posix_common::someip::IpAddress& address,
                           someip_posix_common::someip::Port port);
  /**
   * \brief Indicates whether a passive TCP connection exists.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return true if the specified passive TCP connection exists and false otherwise.
   */
  bool HasPassiveConnection(const someip_posix_common::someip::IpAddress& address,
                            someip_posix_common::someip::Port port);
  /**
   * \brief Returns a new TCP sender.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return A TCP sender.
   */
  ServiceDiscoveryTcpSenderPtr GetSender(const someip_posix_common::someip::IpAddress& address,
                                         someip_posix_common::someip::Port port);
  /**
   * \brief Returns a new TCP receiver.
   *
   * \return A TCP receiver.
   */
  ServiceDiscoveryTcpReceiverPtr GetReceiver();
  /**
   * \brief Creates a new TCP connection from the given connected TCP socket.
   *
   * \param socket A connected TCP socket.
   */
  void AcceptConnection(ServiceDiscoveryTcpConnection::Socket&& socket);
  /**
   * \brief Processes a single received SOME/IP message on behalf of an existing TCP connection.
   *
   * \param connection A TCP connection that received the given packet.
   * \param instance_id A SOME/IP service instance identifier.
   * \param message A SOME/IP message.
   */
  void ProcessReceivedMessage(ServiceDiscoveryTcpConnection* connection,
                              someip_posix_common::someip::InstanceId instance_id, SomeIpMessage&& message);
  /**
   * \brief Registers the given TCP connection for read event notification.
   *
   * \param connection A TCP connection.
   * \param handle A TCP socket file descriptor.
   */
  void RegisterReadEventHandler(ServiceDiscoveryTcpConnection* connection, int handle);
  /**
   * \brief Unregisters the given TCP connection from read event notification.
   *
   * \param connection A TCP connection.
   * \param handle A TCP socket file descriptor.
   */
  void UnregisterReadEventHandler(ServiceDiscoveryTcpConnection* connection, int handle);
  /**
   * \brief Registers the given TCP connection for write event notification.
   *
   * \param connection A TCP connection.
   * \param handle A TCP socket file descriptor.
   */
  void RegisterWriteEventHandler(ServiceDiscoveryTcpConnection* connection, int handle);
  /**
   * \brief Unregisters the given TCP connection from write event notification.
   *
   * \param connection A TCP connection.
   * \param handle A TCP socket file descriptor.
   */
  void UnregisterWriteEventHandler(ServiceDiscoveryTcpConnection* connection, int handle);
  /**
   * \brief Acquires a new reference to the given TCP connection.
   *
   * \param connection A TCP connection.
   */
  void AcquireConnection(ServiceDiscoveryTcpConnection* connection);
  /**
   * \brief Releases a reference to the given TCP connection.
   *
   * \param connection A TCP connection.
   */
  void ReleaseConnection(ServiceDiscoveryTcpConnection* connection);
  /**
   * \brief Increments the number of server users.
   */
  void AcquireServer();
  /**
   * \brief Decreases the number of server users.
   */
  void ReleaseServer();
  /**
   * \brief Registers a provided SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void RegisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                       someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Unregisters a provided SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void UnregisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                         someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Looks up the SOME/IP service instance identifier belonging to a registered SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \return A pair of a bool and a SOME/IP service instance identifier. The bool value indicates whether the second
   * value is valid.
   */
  std::pair<bool, someip_posix_common::someip::InstanceId> GetProvidedServiceInstanceId(
      someip_posix_common::someip::ServiceId service_id);

 private:
  /**
   * \brief A TCP connection pointer type.
   */
  using ServiceDiscoveryTcpConnectionPtr = std::unique_ptr<ServiceDiscoveryTcpConnection>;
  /**
   * \brief A TCP connection container type.
   */
  using ServiceDiscoveryTcpConnectionContainer = std::vector<ServiceDiscoveryTcpConnectionPtr>;
  /**
   * \brief A TCP connection container iterator type.
   */
  using ServiceDiscoveryTcpConnectionContainerIter = ServiceDiscoveryTcpConnectionContainer::iterator;
  /**
   * \brief A TCP server pointer type.
   */
  using ServiceDiscoveryTcpServerPtr = std::unique_ptr<ServiceDiscoveryTcpServer>;
  /**
   * \brief A TCP response sender pointer type.
   */
  using ServiceDiscoveryTcpResponseSenderPtr = std::shared_ptr<ServiceDiscoveryTcpResponseSender>;
  /**
   * \brief A map from a file descriptor to a TCP connection.
   */
  using EventHandlerMap = std::unordered_map<int, ServiceDiscoveryTcpConnection*>;
  /**
   * \brief Represents a provided SOME/IP service instance.
   */
  using ProvidedServiceInstance =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Represents a container of provided SOME/IP service instances.
   */
  using ProvidedServiceInstanceContainer = std::vector<ProvidedServiceInstance>;
  /**
   * \brief Creates a new TCP response sender.
   *
   * \param connection A TCP connection.
   * \return A TCP response sender.
   */
  ServiceDiscoveryTcpResponseSenderPtr GetTcpResponseSender(ServiceDiscoveryTcpConnection* connection);
  /**
   * \brief Called when a TCP socket becomes readable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleRead(int handle) override;
  /**
   * \brief Called when a TCP socket becomes writable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleWrite(int handle) override;
  /**
   * \brief Closes a TCP connection.
   *
   * \param connection A TCP connection.
   *
   */
  void CloseConnection(ServiceDiscoveryTcpConnection* connection);
  /**
   * \brief Unregisters all TCP socket file descriptors belonging to the given TCP connection from event notification.
   *
   * \param connection A TCP connection.
   */
  void CleanUpConnectionEventHandlers(ServiceDiscoveryTcpConnection* connection);
  /**
   * \brief Looks up a TCP connection by its remote IP address and port number.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return An iterator pointing into TCP connection container.
   */
  ServiceDiscoveryTcpConnectionContainerIter GetConnection(const someip_posix_common::someip::IpAddress& address,
                                                           someip_posix_common::someip::Port port);
  /**
   * \brief Closes all passively established TCP connections.
   */
  void CloseAcceptedConnections();
  /**
   * \brief A reactor.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* packet_router_;
  /**
   * \brief A local IP address.
   */
  const someip_posix_common::someip::IpAddress address_;
  /**
   * \brief A local port number.
   */
  const someip_posix_common::someip::Port port_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A map from a TCP socket file descriptor registered for read notification to a TCP connection.
   */
  EventHandlerMap read_event_handlers_;
  /**
   * \brief A map from a TCP socket file descriptor registered for write notification to a TCP connection.
   */
  EventHandlerMap write_event_handlers_;
  /**
   * \brief A container of registered provided SOME/IP service instances.
   */
  ProvidedServiceInstanceContainer provided_service_instances_;
  /**
   * \brief A container of TCP connections.
   */
  ServiceDiscoveryTcpConnectionContainer connections_;
  /**
   * \brief A TCP server.
   */
  ServiceDiscoveryTcpServerPtr server_;
  /**
   * \brief Counts how many times TCP server is in use.
   */
  std::atomic<std::size_t> server_users_;
  /**
   * \brief Quality of service setting for this TCP endpoint.
   */
  const someip_posix_common::someip::SocketOptions socket_options_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_ENDPOINT_H_
