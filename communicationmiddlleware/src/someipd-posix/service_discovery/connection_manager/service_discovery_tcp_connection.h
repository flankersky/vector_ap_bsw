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
/**        \file  service_discovery_tcp_connection.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket_connector.h"
#include "osabstraction/io/network/socket/tcp_socket.h"
#include "someip-posix-common/someip/someip_stream_message_reader.h"
#include "someipd-posix/packet_router/packet_router_interface.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpEndpoint;
class ServiceDiscoveryTcpSender;

/**
 * \brief ServiceDiscoveryTcpConnection.
 */
class ServiceDiscoveryTcpConnection {
 public:
  /**
   * \brief A TCP endpoint pointer type.
   */
  using ServiceDiscoveryTcpEndpointPtr = ServiceDiscoveryTcpEndpoint*;
  /**
   * \brief A TCP sender pointer type.
   */
  using ServiceDiscoveryTcpSenderPtr = ServiceDiscoveryTcpSender*;
  /**
   * \brief A TCP socket type.
   */
  using Socket = osabstraction::io::network::socket::TCPSocket;
  /**
   * \brief Constructor of ServiceDiscoveryTcpConnection.
   *
   * \param endpoint A TCP endpoint this connection belongs to.
   * \param address A remote IP address.
   * \param port A remote port number.
   * \param options Specific socket options to apply for this TCP socket.
   */
  ServiceDiscoveryTcpConnection(ServiceDiscoveryTcpEndpointPtr endpoint,
                                const someip_posix_common::someip::IpAddress& address,
                                someip_posix_common::someip::Port port,
                                const someip_posix_common::someip::SocketOptions& options);
  /**
   * \brief Constructor of ServiceDiscoveryTcpConnection.
   *
   * \param endpoint A TCP endpoint this connection belongs to.
   * \param socket A connected TCP socket that was accepted by TCP server.
   * \param options Specific socket options to apply for this TCP socket
   * of this TCP connection.
   */
  ServiceDiscoveryTcpConnection(ServiceDiscoveryTcpEndpointPtr endpoint, Socket&& socket,
                                const someip_posix_common::someip::SocketOptions& options);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryTcpConnection(const ServiceDiscoveryTcpConnection&) = delete;
  /**
   * \brief Delete copy assignment operator
   */
  ServiceDiscoveryTcpConnection& operator=(const ServiceDiscoveryTcpConnection&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryTcpConnection.
   */
  virtual ~ServiceDiscoveryTcpConnection();
  /**
   * \brief Returns the remote IP address.
   *
   * \return A remote IP address.
   */
  const someip_posix_common::someip::IpAddress& GetAddress() const;
  /**
   * \brief Returns the remote port number.
   *
   * \return A remote port number.
   */
  someip_posix_common::someip::Port GetPort() const;
  /**
   * \brief Indicates whether this connection was established actively or passively.
   *
   * \return true if TCP connection is active and false otherwise.
   */
  bool IsActive() const;
  /**
   * \brief Indicates whether this connection is still alive.
   *
   * \return true if TCP connection is alive and false otherwise.
   */
  bool IsConnected() const;
  /**
   * \brief Connects actively to the remote peer if TCP connection is dead.
   */
  void Connect();
  /**
   * \brief Shuts down TCP connection if it is still alive.
   */
  void Disconnect();
  /**
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet);
  /**
   * \brief Increments the number of users of this TCP connection.
   *
   * \return The current number of users.
   */
  std::size_t Acquire();
  /**
   * \brief Decrements the number of users of this TCP connection.
   *
   * \return The current number of users.
   */
  std::size_t Release();
  /**
   * \brief Returns the number of users of this TCP connection.
   *
   * \return The current number of users.
   */
  std::size_t GetUsers() const;
  /**
   * \brief Registers a TCP sender.
   *
   * \param sender A TCP sender.
   */
  void RegisterSender(ServiceDiscoveryTcpSenderPtr sender);
  /**
   * \brief Unregisters a TCP sender.
   *
   * \param sender A TCP sender.
   */
  void UnregisterSender(ServiceDiscoveryTcpSenderPtr sender);
  /**
   * \brief Registers a required SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                       someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Unregisters a required SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void UnregisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                         someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Called when a TCP socket becomes readable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleRead(int handle);
  /**
   * \brief Called when a TCP socket becomes writable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleWrite(int handle);

 private:
  /**
   * \brief A TCP sender pointer type.
   */
  using ServiceDiscoveryTcpSenderContainer = std::vector<ServiceDiscoveryTcpSenderPtr>;
  /**
   * \brief Convert a string-based port to a numeric value. This method is used
   * for setting the constant port_ attribute in the constructor.
   */
  someip_posix_common::someip::Port GetSocketRemotePort(Socket& socket) noexcept;
  /**
   * \brief Called when an active TCP connection becomes alive.
   *
   * \param socket A TCP socket representing the established connection.
   */
  void OnConnect(Socket&& socket);
  /**
   * \brief A TCP socket connector type.
   */
  using SocketConnector =
      osabstraction::io::network::socket::SocketConnector<Socket, ServiceDiscoveryTcpConnection,
                                                          &ServiceDiscoveryTcpConnection::OnConnect>;
  /**
   * \brief A TCP socket connector pointer type.
   */
  using SocketConnectorPtr = std::unique_ptr<SocketConnector>;
  /**
   * \brief Represents a required SOME/IP service instance.
   */
  using RequiredServiceInstance =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Represents a container of required SOME/IP service instances.
   */
  using RequiredServiceInstanceContainer = std::vector<RequiredServiceInstance>;
  /**
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = someip_posix_common::someip::SomeIpMessage;
  /**
   * \brief Called when a TCP connection is established.
   */
  void Connected();
  /**
   * \brief Called when a TCP connection dies or is closed.
   */
  void Disconnected();
  /**
   * \brief Notifies registered TCP senders about a change in the TCP connection state.
   */
  void Notify();
  /**
   * \brief Processes a single received SOME/IP message.
   *
   * \param message A received SOME/IP message.
   */
  void ProcessMessage(SomeIpMessage&& message);
  /**
   * \brief Removes a TCP sender.
   *
   * \param sender A TCP sender.
   */
  void RemoveSender(ServiceDiscoveryTcpSenderPtr sender);
  /**
   * \brief Removes a TCP sender while a notification is in progress.
   *
   * \param sender A TCP sender.
   */
  void RemoveSenderInsideNotify(ServiceDiscoveryTcpSenderPtr sender);
  /**
   * \brief Looks up the SOME/IP service instance identifier belonging to a registered SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \return A pair of a bool and a SOME/IP service instance identifier. The bool value indicates whether the second
   * value is valid.
   */
  std::pair<bool, someip_posix_common::someip::InstanceId> GetRequiredServiceInstanceId(
      someip_posix_common::someip::ServiceId service_id);
  /**
   * \brief Applies the socket options from the configuration on an established connection.
   */
  void ApplySocketOptions();
  /**
   * \brief A TCP endpoint this connection belongs to.
   */
  ServiceDiscoveryTcpEndpointPtr endpoint_;
  /**
   * \brief A remote IP address.
   */
  const someip_posix_common::someip::IpAddress address_;
  /**
   * \brief A remote port number.
   */
  const someip_posix_common::someip::Port port_;
  /**
   * \brief Indicates whether this connection was established actively or passively..
   */
  bool is_active_;
  /**
   * \brief Indicates whether this connection is still alive..
   */
  bool is_connected_;
  /**
   * \brief A TCP socket.
   */
  Socket socket_;
  /**
   * \brief Counts how many times TCP connection is in use.
   */
  std::atomic<std::size_t> users_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A TCP connector.
   */
  SocketConnectorPtr connector_;
  /**
   * \brief A SOME/IP message reader for stream sockets.
   */
  someip_posix_common::someip::SomeIpStreamMessageReader message_reader_;
  /**
   * \brief A container of registered TCP senders.
   */
  ServiceDiscoveryTcpSenderContainer senders_;
  /**
   * \brief A container of removed TCP senders.
   */
  ServiceDiscoveryTcpSenderContainer removed_senders_;
  /**
   * \brief Indicates whether a notification of regsitered TCP senders is in progress or not.
   */
  bool inside_notify_;
  /**
   * \brief TCP socket options for this connection handed over from the TCP endpoint.
   */
  const someip_posix_common::someip::SocketOptions socket_options_;
  /**
   * \brief A container of registered required SOME/IP service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_H_
