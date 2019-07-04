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
/**        \file  service_discovery_udp_endpoint.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_ENDPOINT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/udp_socket.h"
#include "osabstraction/io/reactor.h"
#include "someip-posix-common/someip/someip_datagram_message_reader.h"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_receiver.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_response_sender.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_sender.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

/**
 * \brief ServiceDiscoveryUdpEndpoint.
 */
class ServiceDiscoveryUdpEndpoint : public std::enable_shared_from_this<ServiceDiscoveryUdpEndpoint>,
                                    public osabstraction::io::EventHandler {
 public:
  /**
   * \brief An UDP sender pointer type.
   */
  using ServiceDiscoveryUdpSenderPtr = std::unique_ptr<ServiceDiscoveryUdpSender>;
  /**
   * \brief An UDP receiver pointer type.
   */
  using ServiceDiscoveryUdpReceiverPtr = std::unique_ptr<ServiceDiscoveryUdpReceiver>;
  /**
   * \brief Transmission trigger buffer size.
   */
  using TransmissionTriggerBufferSize = someip_posix_common::someip::TransmissionTriggerBufferSize;
  /**
   * \brief Constructor of ServiceDiscoveryUdpEndpoint.
   *
   * \param reactor A reactor.
   * \param packet_router A packet router.
   * \param timer_manager A timer manager.
   * \param transmission_trigger_policy_manager A transmission trigger policy manager.
   * \param address A local IP address.
   * \param port A local port number.
   * \param mtu A maximum length of a single datagram.
   * \param options Socket options for this UDP socket (QoS).
   * \param transmission_trigger_buffer_size Maximum number of bytes which can be queued before transmission.
   */
  ServiceDiscoveryUdpEndpoint(
      osabstraction::io::ReactorInterface* reactor, packet_router::PacketRouterInterface* packet_router,
      vac::timer::TimerManager* timer_manager,
      const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
      const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port,
      someip_posix_common::someip::MTU mtu, const someip_posix_common::someip::SocketOptions& options,
      TransmissionTriggerBufferSize transmission_trigger_buffer_size);
  /**
   * \brief Constructor of ServiceDiscoveryUdpEndpoint.
   *
   * \param reactor A reactor.
   * \param packet_router A packet router.
   * \param timer_manager A timer manager.
   * \param transmission_trigger_policy_manager A transmission trigger policy manager.
   * \param local_address A local IP address.
   * \param group_address A multicast IP address.
   * \param port A local port number.
   * \param mtu A maximum length of a single datagram.
   * \param transmission_trigger_buffer_size Maximum number of bytes which can be queued before transmission.
   */
  ServiceDiscoveryUdpEndpoint(
      osabstraction::io::ReactorInterface* reactor, packet_router::PacketRouterInterface* packet_router,
      vac::timer::TimerManager* timer_manager,
      const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
      const someip_posix_common::someip::IpAddress& local_address,
      const someip_posix_common::someip::IpAddress& group_address, someip_posix_common::someip::Port port,
      someip_posix_common::someip::MTU mtu, TransmissionTriggerBufferSize transmission_trigger_buffer_size);
  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryUdpEndpoint(const ServiceDiscoveryUdpEndpoint&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryUdpEndpoint& operator=(const ServiceDiscoveryUdpEndpoint&) = delete;
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
   * \brief Returns a new UDP sender.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return An UDP sender.
   */
  ServiceDiscoveryUdpSenderPtr GetSender(const someip_posix_common::someip::IpAddress& address,
                                         someip_posix_common::someip::Port port);
  /**
   * \brief Returns a new UDP receiver.
   *
   * \return An UDP receiver.
   */
  ServiceDiscoveryUdpReceiverPtr GetReceiver();
  /**
   * \brief Acquires a new reference to the given UDP connection.
   *
   * \param connection An UDP connection.
   */
  void AcquireConnection(ServiceDiscoveryUdpConnection* connection);
  /**
   * \brief Releases a reference to the given UDP connection.
   *
   * \param connection An UDP connection.
   */
  void ReleaseConnection(ServiceDiscoveryUdpConnection* connection);
  /**
   * \brief Increments the number of server users.
   */
  void AcquireServer();
  /**
   * \brief Decreases the number of server users.
   */
  void ReleaseServer();
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
   * \brief Sends data to the given remote address.
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \param socket_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent.
   */
  std::size_t Send(const osabstraction::io::network::socket::Socket::IovecContainer& iovec,
                   const osabstraction::io::network::address::SocketAddress& socket_address);

 private:
  /**
   * \brief An UDP connection pointer type.
   */
  using ServiceDiscoveryUdpConnectionPtr = std::unique_ptr<ServiceDiscoveryUdpConnection>;
  /**
   * \brief An UDP connection container type.
   */
  using ServiceDiscoveryUdpConnectionContainer = std::vector<ServiceDiscoveryUdpConnectionPtr>;
  /**
   * \brief An UDP connection container iterator type.
   */
  using ServiceDiscoveryUdpConnectionContainerIter = ServiceDiscoveryUdpConnectionContainer::iterator;
  /**
   * \brief An UDP response sender type.
   */
  using ServiceDiscoveryUdpResponseSenderPtr = std::shared_ptr<ServiceDiscoveryUdpResponseSender>;
  /**
   * \brief An UDP socket type.
   */
  using Socket = osabstraction::io::network::socket::UDPSocket;
  /**
   * \brief An UDP socket pointer type.
   */
  using SocketPtr = std::unique_ptr<Socket>;
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
   * \brief Represents a provided SOME/IP service instance.
   */
  using ProvidedServiceInstance =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Represents a container of provided SOME/IP service instances.
   */
  using ProvidedServiceInstanceContainer = std::vector<ProvidedServiceInstance>;
  /**
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = someip_posix_common::someip::SomeIpMessage;
  /**
   * \brief Opens UDP socket.
   */
  void OpenSocket();
  /**
   * \brief Closes UDP socket.
   */
  void CloseSocket();
  /**
   * \brief Processes a single received SOME/IP message.
   *
   * \param instance_id A SOME/IP service instance identifier.
   * \param message A SOME/IP message.
   */
  void ProcessReceivedMessage(someip_posix_common::someip::InstanceId instance_id, SomeIpMessage&& message);
  /**
   * \brief Processes a single received SOME/IP message.
   *
   * \param packet A SOME/IP message.
   */
  void ProcessMessage(SomeIpMessage&& message);
  /**
   * \brief Called when an UDP socket becomes readable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleRead(int handle) override;
  /**
   * \brief Called when an UDP socket becomes writable.
   *
   * \param handle A socket file descriptor.
   * \return true if no notification is required anymore and false otherwise.
   */
  bool HandleWrite(int handle) override;
  /**
   * \brief Looks up an UDP connection by its remote IP address and port number.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return An iterator pointing into UDP connection container.
   */
  ServiceDiscoveryUdpConnectionContainerIter GetConnection(const someip_posix_common::someip::IpAddress& address,
                                                           someip_posix_common::someip::Port port);
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
   * \brief Looks up the SOME/IP service instance identifier belonging to a registered SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \return A pair of a bool and a SOME/IP service instance identifier. The bool value indicates whether the second
   * value is valid.
   */
  std::pair<bool, someip_posix_common::someip::InstanceId> GetProvidedServiceInstanceId(
      someip_posix_common::someip::ServiceId service_id);
  /**
   * \brief A recator.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* packet_router_;
  /**
   * \brief A local IP address.
   */
  const someip_posix_common::someip::IpAddress local_address_;
  /**
   * \brief A group IP address.
   */
  const someip_posix_common::someip::IpAddress group_address_;
  /**
   * \brief A local port number.
   */
  const someip_posix_common::someip::Port port_;
  /**
   * \brief Indicates whether the local IP address is a multicast one.
   */
  const bool is_multicast_;
  /**
   * \brief A local socket address.
   */
  const osabstraction::io::network::address::IPSocketAddress socket_address_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief An UDP socket.
   */
  SocketPtr socket_;
  /**
   * \brief Counts how many times UDP socket is in use.
   */
  std::atomic<std::size_t> users_;
  /**
   * \brief A SOME/IP message reader for datagram sockets.
   */
  someip_posix_common::someip::SomeIpDatagramMessageReader message_reader_;
  /**
   * \brief Quality of service setting
   */
  const someip_posix_common::someip::SocketOptions socket_options_;
  /**
   * \brief A container of UDP connections.
   */
  ServiceDiscoveryUdpConnectionContainer connections_;
  /**
   * \brief A container of registered required SOME/IP service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
  /**
   * \brief A container of registered provided SOME/IP service instances.
   */
  ProvidedServiceInstanceContainer provided_service_instances_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A transmission trigger policy manager.
   */
  const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager_;
  /**
   * \brief Maximum number of bytes which can be queued before transmission.
   */
  const TransmissionTriggerBufferSize transmission_trigger_buffer_size_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_ENDPOINT_H_
