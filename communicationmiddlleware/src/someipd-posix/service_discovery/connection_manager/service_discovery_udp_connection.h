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
/**        \file  service_discovery_udp_connection.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_CONNECTION_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_CONNECTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/address/ip_socket_address.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_message_queue.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryUdpEndpoint;

/**
 * \brief ServiceDiscoveryUdpConnection.
 */
class ServiceDiscoveryUdpConnection : public transmission_trigger::TransmissionTriggerMessageQueueContext {
 public:
  /**
   * \brief An UDP endpoint pointer type.
   */
  using ServiceDiscoveryUdpEndpointPtr = ServiceDiscoveryUdpEndpoint*;
  /**
   * \brief Constructor of ServiceDiscoveryUdpConnection.
   *
   * \param endpoint An UDP endpoint this connections belongs to.
   * \param timer_manager A timer manager.
   * \param transmission_trigger_policy_manager A transmission trigger policy manager.
   * \param address A remote IP address.
   * \param port A remote port number.
   * \param transmission_trigger_buffer_size Maximum number of bytes which can be queued before transmission.
   */
  ServiceDiscoveryUdpConnection(
      ServiceDiscoveryUdpEndpointPtr endpoint, vac::timer::TimerManager* timer_manager,
      const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
      const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port,
      someip_posix_common::someip::TransmissionTriggerBufferSize transmission_trigger_buffer_size);
  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryUdpConnection(const ServiceDiscoveryUdpConnection&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryUdpConnection& operator=(const ServiceDiscoveryUdpConnection&) = delete;
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
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet);
  /**
   * \brief Increments the number of users of this UDP connection.
   *
   * \return The current number of users.
   */
  std::size_t Acquire();
  /**
   * \brief Decrements the number of users of this UDP connection.
   *
   * \return The current number of users.
   */
  std::size_t Release();
  /**
   * \see TransmissionTriggerMessageQueueContext.
   */
  void OnSendDatagram() override;

 private:
  /**
   * \brief An I/O vector container.
   */
  using IovecContainer = std::vector<struct iovec>;
  /**
   * \brief A container of SOME/IP messages.
   */
  using MessageContainer = transmission_trigger::TransmissionTriggerMessageQueue::MessageContainer;

  /**
   * \brief Builds a I/O vector container from a container of SOME/IP messages.
   *
   * \param messages A container of SOME/IP messages.
   * \return An I/O vector container.
   */
  IovecContainer GetDatagramIovecContainer(const MessageContainer& messages);

  /**
   * \brief An UDP endpoint this connection belongs to.
   */
  ServiceDiscoveryUdpEndpointPtr endpoint_;
  /**
   * \brief A remote IP address.
   */
  const someip_posix_common::someip::IpAddress address_;
  /**
   * \brief A remote port number.
   */
  const someip_posix_common::someip::Port port_;
  /**
   * \brief A remote socket address.
   */
  const osabstraction::io::network::address::IPSocketAddress socket_address_;
  /**
   * \brief Counts how many times UDP connection is in use.
   */
  std::atomic<std::size_t> users_;
  /**
   * \brief A transmission trigger message queue.
   */
  transmission_trigger::TransmissionTriggerMessageQueue transmission_trigger_message_queue_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_CONNECTION_H_
