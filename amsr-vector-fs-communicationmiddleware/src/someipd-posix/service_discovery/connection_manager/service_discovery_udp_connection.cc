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
/**        \file  service_discovery_udp_connection.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <utility>
#include <vector>

#include "osabstraction/io/network/address/ip_socket_address.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_endpoint.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryUdpConnection::ServiceDiscoveryUdpConnection(
    ServiceDiscoveryUdpEndpointPtr endpoint, vac::timer::TimerManager* timer_manager,
    const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port,
    someip_posix_common::someip::TransmissionTriggerBufferSize transmission_trigger_buffer_size)
    : endpoint_(endpoint),
      address_(address),
      port_(port),
      socket_address_(
          osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(address, std::to_string(port))),
      users_(0U),
      transmission_trigger_message_queue_{timer_manager, transmission_trigger_policy_manager, *this,
                                          transmission_trigger_buffer_size} {}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryUdpConnection::GetAddress() const { return address_; }

someip_posix_common::someip::Port ServiceDiscoveryUdpConnection::GetPort() const { return port_; }

void ServiceDiscoveryUdpConnection::Forward(someip_posix_common::someip::InstanceId instance_id,
                                            packet_router::Packet packet) {
  transmission_trigger_message_queue_.EnqueueMessage(instance_id, packet);
}

std::size_t ServiceDiscoveryUdpConnection::Acquire() { return ++users_; }

std::size_t ServiceDiscoveryUdpConnection::Release() {
  assert(users_ > 0);
  return --users_;
}

void ServiceDiscoveryUdpConnection::OnSendDatagram() {
  // TODO(PAASR-3062)
  // Get the next UDP datagram which represents all SOME/IP messages which shall be transmitted in a single UDP packet
  auto next_datagram = transmission_trigger_message_queue_.DequeueNextDatagram();
  // Repeat until all datagrams are sent
  while (next_datagram.second) {
    // Get the IO vector container which described the whole datagram
    auto io_vec_container{GetDatagramIovecContainer(next_datagram.first)};
    // Transmitt the next UDP datagram
    endpoint_->Send(osabstraction::io::network::socket::Socket::IovecContainer{io_vec_container}, socket_address_);
    // Fetch the next UDP datagram
    next_datagram = transmission_trigger_message_queue_.DequeueNextDatagram();
  }
}

ServiceDiscoveryUdpConnection::IovecContainer ServiceDiscoveryUdpConnection::GetDatagramIovecContainer(
    const MessageContainer& messages) {
  IovecContainer io_vec_container{};
  for (const auto& message : messages) {
    for (const auto& iov : message->GetIovecContainer()) {
      io_vec_container.push_back(iov);
    }
  }
  return io_vec_container;
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
