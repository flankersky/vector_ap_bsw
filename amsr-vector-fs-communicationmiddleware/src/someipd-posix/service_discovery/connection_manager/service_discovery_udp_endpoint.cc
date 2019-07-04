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
/**        \file  service_discovery_udp_endpoint.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_endpoint.h"
#include <algorithm>
#include <cassert>
#include <limits>
#include "osabstraction/io/network/address/ip_socket_address.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryUdpEndpoint::ServiceDiscoveryUdpEndpoint(
    osabstraction::io::ReactorInterface* reactor, packet_router::PacketRouterInterface* packet_router,
    vac::timer::TimerManager* timer_manager,
    const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port,
    someip_posix_common::someip::MTU mtu, const someip_posix_common::someip::SocketOptions& options,
    TransmissionTriggerBufferSize transmission_trigger_buffer_size)
    : reactor_(reactor),
      packet_router_(packet_router),
      local_address_(address),
      port_(port),
      is_multicast_(false),
      socket_address_(
          osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(address, std::to_string(port))),
      logger_(ara::log::CreateLogger("ServiceDiscoveryUdpEndpoint", "")),
      users_(0),
      message_reader_(mtu),
      socket_options_(options),
      timer_manager_{timer_manager},
      transmission_trigger_policy_manager_(transmission_trigger_policy_manager),
      transmission_trigger_buffer_size_{transmission_trigger_buffer_size} {}

ServiceDiscoveryUdpEndpoint::ServiceDiscoveryUdpEndpoint(
    osabstraction::io::ReactorInterface* reactor, packet_router::PacketRouterInterface* packet_router,
    vac::timer::TimerManager* timer_manager,
    const transmission_trigger::TransmissionTriggerPolicyManager& transmission_trigger_policy_manager,
    const someip_posix_common::someip::IpAddress& local_address,
    const someip_posix_common::someip::IpAddress& group_address, someip_posix_common::someip::Port port,
    someip_posix_common::someip::MTU mtu, TransmissionTriggerBufferSize transmission_trigger_buffer_size)
    : reactor_(reactor),
      packet_router_(packet_router),
      local_address_(local_address),
      group_address_(group_address),
      port_(port),
      is_multicast_(true),
      socket_address_(osabstraction::io::network::address::IPSocketAddress::FromMulticastGroupAndPort(
          local_address, group_address, std::to_string(port))),
      logger_(ara::log::CreateLogger("ServiceDiscoveryUdpEndpoint", "")),
      users_(0),
      message_reader_(mtu),
      timer_manager_{timer_manager},
      transmission_trigger_policy_manager_(transmission_trigger_policy_manager),
      transmission_trigger_buffer_size_{transmission_trigger_buffer_size} {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": local address " << local_address << " group address "
                     << group_address << "," << port;
}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryUdpEndpoint::GetAddress() const {
  if (is_multicast_) {
    return group_address_;
  } else {
    return local_address_;
  }
}

someip_posix_common::someip::Port ServiceDiscoveryUdpEndpoint::GetPort() const { return port_; }

ServiceDiscoveryUdpEndpoint::ServiceDiscoveryUdpSenderPtr ServiceDiscoveryUdpEndpoint::GetSender(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  auto it = GetConnection(address, port);
  if (it == connections_.end()) {
    connections_.emplace_back(vac::language::make_unique<ServiceDiscoveryUdpConnection>(
        this, timer_manager_, transmission_trigger_policy_manager_, address, port, transmission_trigger_buffer_size_));
    it = std::prev(connections_.end());
  }
  return vac::language::make_unique<ServiceDiscoveryUdpSender>(shared_from_this(), it->get());
}

ServiceDiscoveryUdpEndpoint::ServiceDiscoveryUdpReceiverPtr ServiceDiscoveryUdpEndpoint::GetReceiver() {
  return vac::language::make_unique<ServiceDiscoveryUdpReceiver>(shared_from_this());
}

void ServiceDiscoveryUdpEndpoint::AcquireConnection(ServiceDiscoveryUdpConnection* connection) {
  if (connection->Acquire() == 1) {
    if (++users_ == 1) {
      OpenSocket();
    }
  }
}

void ServiceDiscoveryUdpEndpoint::ReleaseConnection(ServiceDiscoveryUdpConnection* connection) {
  if (connection->Release() == 0) {
    assert(users_ > 0);
    if (--users_ == 0) {
      CloseSocket();
    }
  }
}

void ServiceDiscoveryUdpEndpoint::AcquireServer() {
  if (++users_ == 1) {
    OpenSocket();
  }
}

void ServiceDiscoveryUdpEndpoint::ReleaseServer() {
  assert(users_ > 0);
  if (--users_ == 0) {
    CloseSocket();
  }
}

void ServiceDiscoveryUdpEndpoint::RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  required_service_instances_.emplace_back(service_id, instance_id);
}

void ServiceDiscoveryUdpEndpoint::UnregisterRequiredServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.begin(), required_service_instances_.end(),
                      RequiredServiceInstance{service_id, instance_id});
  assert(it != required_service_instances_.end());
  required_service_instances_.erase(it);
}

void ServiceDiscoveryUdpEndpoint::RegisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.begin(), provided_service_instances_.end(),
                      ProvidedServiceInstance{service_id, instance_id});
  assert(it == provided_service_instances_.end());
  provided_service_instances_.emplace_back(service_id, instance_id);
}

void ServiceDiscoveryUdpEndpoint::UnregisterProvidedServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.begin(), provided_service_instances_.end(),
                      ProvidedServiceInstance{service_id, instance_id});
  assert(it != provided_service_instances_.end());
  provided_service_instances_.erase(it);
}

std::size_t ServiceDiscoveryUdpEndpoint::Send(
    const osabstraction::io::network::socket::Socket::IovecContainer& iovec,
    const osabstraction::io::network::address::SocketAddress& socket_address) {
  if (socket_) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": " << socket_address.toString();
    return socket_->Send(iovec, socket_address);
  } else {
    return 0;
  }
}

void ServiceDiscoveryUdpEndpoint::OpenSocket() {
  assert(!socket_);
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": address " << socket_address_.toString() << " multicast "
                     << is_multicast_;
  socket_ = vac::language::make_unique<Socket>(Socket::CreateSocket(socket_address_, true));
  const bool qos_enabled = socket_options_.qos_.first;

  if (qos_enabled) {
    const someip_posix_common::someip::QoSPriority qos_priority{socket_options_.qos_.second};
    socket_->SetPriority(qos_priority);
  }

  socket_->SetMulticastSendInterface(local_address_);
  if (is_multicast_) {
    socket_->JoinMulticastGroup(local_address_, group_address_);
  }
  reactor_->RegisterEventHandler(socket_->GetHandle(), this, osabstraction::io::kReadEvent, nullptr);
}

void ServiceDiscoveryUdpEndpoint::CloseSocket() {
  assert(socket_);
  reactor_->UnregisterEventHandler(socket_->GetHandle(), osabstraction::io::kReadEvent);
  if (is_multicast_) {
    socket_->LeaveMulticastGroup(local_address_, group_address_);
  }
  socket_.reset(nullptr);
}

void ServiceDiscoveryUdpEndpoint::ProcessReceivedMessage(someip_posix_common::someip::InstanceId instance_id,
                                                         SomeIpMessage&& message) {
  const auto& from_address = message.GetFromAddress().first;
  auto ap = from_address.toAddressPortStrings();
  auto address = ap.first;
  auto port = std::strtoul(ap.second.c_str(), nullptr, 0);
  assert(port <= std::numeric_limits<someip_posix_common::someip::Port>::max());
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": instance id " << std::hex << instance_id << std::dec
                     << " from " << from_address.toString();
  auto it = GetConnection(address, static_cast<someip_posix_common::someip::Port>(port));
  if (it == connections_.end()) {
    connections_.emplace_back(vac::language::make_unique<ServiceDiscoveryUdpConnection>(
        this, timer_manager_, transmission_trigger_policy_manager_, address,
        static_cast<someip_posix_common::someip::Port>(port), transmission_trigger_buffer_size_));
    it = std::prev(connections_.end());
  }
  auto response_sender = std::make_shared<ServiceDiscoveryUdpResponseSender>(shared_from_this(), it->get());
  packet_router::Packet packet(new SomeIpMessage(std::move(message)));
  packet_router_->Forward(instance_id, response_sender, std::move(packet));
}

void ServiceDiscoveryUdpEndpoint::ProcessMessage(SomeIpMessage&& message) {
  namespace someip = someip_posix_common::someip;
  const auto& from_address = message.GetFromAddress().first;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": " << from_address.toString();
  auto header = message.GetHeader();
  const auto service_id = header.service_id_;
  auto instance_id = someip::kInstanceIdAny;
  if (header.message_type_ == someip::SomeIpMessageType::kRequest ||
      header.message_type_ == someip::SomeIpMessageType::kRequestNoReturn) {
    auto iid = GetProvidedServiceInstanceId(service_id);
    if (iid.first) {
      instance_id = iid.second;
    }
  } else if (header.message_type_ == someip::SomeIpMessageType::kResponse ||
             header.message_type_ == someip::SomeIpMessageType::kError ||
             header.message_type_ == someip::SomeIpMessageType::kNotification) {
    auto iid = GetRequiredServiceInstanceId(service_id);
    if (iid.first) {
      instance_id = iid.second;
    }
  } else {
    assert(0);
  }
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << service_id << "," << instance_id << std::dec
                     << ")";
  if (instance_id != someip::kInstanceIdAny) {
    ProcessReceivedMessage(instance_id, std::move(message));
  }
}

bool ServiceDiscoveryUdpEndpoint::HandleRead(int handle) {
  (void)handle;
  message_reader_.Read(socket_.get());
  while (message_reader_.IsMessageAvailable()) {
    auto message = message_reader_.GetNextMessage();
    ProcessMessage(std::move(message));
  }
  return true;
}

bool ServiceDiscoveryUdpEndpoint::HandleWrite(int handle) {
  (void)handle;
  return false;
}

ServiceDiscoveryUdpEndpoint::ServiceDiscoveryUdpConnectionContainerIter ServiceDiscoveryUdpEndpoint::GetConnection(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  return std::find_if(connections_.begin(), connections_.end(),
                      [&address, port](const ServiceDiscoveryUdpConnectionPtr& c) {
                        return c->GetAddress() == address && c->GetPort() == port;
                      });
}

std::pair<bool, someip_posix_common::someip::InstanceId> ServiceDiscoveryUdpEndpoint::GetRequiredServiceInstanceId(
    someip_posix_common::someip::ServiceId service_id) {
  auto it = std::find_if(required_service_instances_.cbegin(), required_service_instances_.cend(),
                         [service_id](const RequiredServiceInstance& rsi) { return rsi.first == service_id; });
  if (it != required_service_instances_.cend()) {
    return {true, it->second};
  } else {
    return {false, 0};
  }
}

std::pair<bool, someip_posix_common::someip::InstanceId> ServiceDiscoveryUdpEndpoint::GetProvidedServiceInstanceId(
    someip_posix_common::someip::ServiceId service_id) {
  auto it = std::find_if(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                         [service_id](const ProvidedServiceInstance& psi) { return psi.first == service_id; });
  if (it != provided_service_instances_.cend()) {
    return {true, it->second};
  } else {
    return {false, 0};
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
