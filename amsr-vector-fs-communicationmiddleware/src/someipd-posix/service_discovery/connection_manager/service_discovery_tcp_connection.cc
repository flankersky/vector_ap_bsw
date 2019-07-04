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
/**        \file  service_discovery_tcp_connection.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <limits>

#include "osabstraction/io/network/address/ip_socket_address.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryTcpConnection::ServiceDiscoveryTcpConnection(ServiceDiscoveryTcpEndpointPtr endpoint,
                                                             const someip_posix_common::someip::IpAddress& address,
                                                             someip_posix_common::someip::Port port,
                                                             const someip_posix_common::someip::SocketOptions& options)
    : endpoint_(endpoint),
      address_(address),
      port_(port),
      is_active_(true),
      is_connected_(false),
      users_(0),
      logger_(ara::log::CreateLogger("ServiceDiscoveryTcpConnection", "")),
      inside_notify_(false),
      socket_options_(options) {
  Connect();
}

ServiceDiscoveryTcpConnection::ServiceDiscoveryTcpConnection(ServiceDiscoveryTcpEndpointPtr endpoint, Socket&& socket,
                                                             const someip_posix_common::someip::SocketOptions& options)
    : endpoint_(endpoint),
      address_(socket.GetRemoteAddress().toAddressPortStrings().first.c_str()),
      port_(GetSocketRemotePort(socket)),
      is_active_(false),
      is_connected_(true),
      socket_(std::move(socket)),
      users_(0),
      logger_(ara::log::CreateLogger("ServiceDiscoveryTcpConnection", "")),
      inside_notify_(false),
      socket_options_(options) {
  ApplySocketOptions();
  endpoint_->RegisterReadEventHandler(this, socket_.GetHandle());
}

ServiceDiscoveryTcpConnection::~ServiceDiscoveryTcpConnection() { Disconnect(); }

someip_posix_common::someip::Port ServiceDiscoveryTcpConnection::GetSocketRemotePort(Socket& socket) noexcept {
  auto port = std::strtoul(socket.GetRemoteAddress().toAddressPortStrings().second.c_str(), nullptr, 0);
  assert(port <= std::numeric_limits<someip_posix_common::someip::Port>::max());
  return static_cast<someip_posix_common::someip::Port>(port);
}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryTcpConnection::GetAddress() const { return address_; }

someip_posix_common::someip::Port ServiceDiscoveryTcpConnection::GetPort() const { return port_; }

bool ServiceDiscoveryTcpConnection::IsActive() const { return is_active_; }

bool ServiceDiscoveryTcpConnection::IsConnected() const { return is_connected_; }

void ServiceDiscoveryTcpConnection::Connect() {
  if (is_connected_) {
    return;
  }
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << endpoint_->GetAddress() << ","
                     << endpoint_->GetPort() << " to " << address_ << "," << port_;
  is_active_ = true;
  connector_ = vac::language::make_unique<SocketConnector>(
      osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(address_, std::to_string(port_)),
      osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(endpoint_->GetAddress(),
                                                                            std::to_string(endpoint_->GetPort())),
      true, *this, false);
  endpoint_->RegisterWriteEventHandler(this, connector_->GetHandle());
}

void ServiceDiscoveryTcpConnection::Disconnect() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (!is_connected_) {
    return;
  }
  Disconnected();
}

void ServiceDiscoveryTcpConnection::Forward(someip_posix_common::someip::InstanceId instance_id,
                                            packet_router::Packet packet) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": instance id " << std::hex << instance_id << std::dec;
  if (is_connected_) {
    try {
      socket_.Send(packet->GetIovecContainer());
    } catch (...) {
      Disconnected();
    }
  }
}

std::size_t ServiceDiscoveryTcpConnection::Acquire() { return ++users_; }

std::size_t ServiceDiscoveryTcpConnection::Release() {
  assert(users_ > 0);
  return --users_;
}

std::size_t ServiceDiscoveryTcpConnection::GetUsers() const { return users_; }

void ServiceDiscoveryTcpConnection::RegisterSender(ServiceDiscoveryTcpSenderPtr sender) { senders_.push_back(sender); }

void ServiceDiscoveryTcpConnection::UnregisterSender(ServiceDiscoveryTcpSenderPtr sender) {
  if (inside_notify_) {
    RemoveSenderInsideNotify(sender);
  } else {
    RemoveSender(sender);
  }
}

void ServiceDiscoveryTcpConnection::RegisterRequiredServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << service_id << "," << instance_id << std::dec
                     << ")";
  auto it = std::find(required_service_instances_.cbegin(), required_service_instances_.cend(),
                      RequiredServiceInstance{service_id, instance_id});
  assert(it == required_service_instances_.cend());
  required_service_instances_.emplace_back(RequiredServiceInstance{service_id, instance_id});
}

void ServiceDiscoveryTcpConnection::UnregisterRequiredServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << service_id << "," << instance_id << std::dec
                     << ")";
  auto it = std::find(required_service_instances_.cbegin(), required_service_instances_.cend(),
                      RequiredServiceInstance{service_id, instance_id});
  assert(it != required_service_instances_.cend());
  required_service_instances_.erase(it);
}

bool ServiceDiscoveryTcpConnection::HandleRead(int handle) {
  if (is_connected_ && handle == socket_.GetHandle()) {
    try {
      message_reader_.Read(&socket_);
      if (message_reader_.IsMessageAvailable()) {
        ProcessMessage(message_reader_.GetNextMessage());
      }
      return true;
    } catch (...) {
      /*
       * Mustn't unregister event handler here.
       * The caller will do it automatically when we return false.
       */
      is_connected_ = false;
      Notify();
      return false;
    }
  } else {
    return false;
  }
}

bool ServiceDiscoveryTcpConnection::HandleWrite(int handle) {
  if (!is_connected_ && is_active_ && connector_ && handle == connector_->GetHandle()) {
    return connector_->HandleWrite(handle);
  } else {
    return false;
  }
}

void ServiceDiscoveryTcpConnection::OnConnect(Socket&& socket) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  socket_ = std::move(socket);
  ApplySocketOptions();
  Connected();
}

void ServiceDiscoveryTcpConnection::Connected() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  is_connected_ = true;
  endpoint_->RegisterReadEventHandler(this, socket_.GetHandle());
  Notify();
}

void ServiceDiscoveryTcpConnection::Disconnected() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  is_connected_ = false;
  endpoint_->UnregisterReadEventHandler(this, socket_.GetHandle());
  socket_.Close();
  Notify();
}

void ServiceDiscoveryTcpConnection::Notify() {
  inside_notify_ = true;
  removed_senders_.resize(0);
  for (const auto s : senders_) {
    auto it = std::find(removed_senders_.cbegin(), removed_senders_.cend(), s);
    if (it == removed_senders_.cend()) {
      s->Notify();
    }
  }
  inside_notify_ = false;
  senders_.erase(std::remove_if(senders_.begin(), senders_.end(),
                                [this](const ServiceDiscoveryTcpSenderPtr& s) {
                                  return std::find(removed_senders_.cbegin(), removed_senders_.cend(), s) !=
                                         removed_senders_.cend();
                                }),
                 senders_.end());
}

void ServiceDiscoveryTcpConnection::ProcessMessage(SomeIpMessage&& message) {
  namespace someip = someip_posix_common::someip;
  auto header = message.GetHeader();
  const auto service_id = header.service_id_;
  auto instance_id = someip::kInstanceIdAny;
  if (header.message_type_ == someip::SomeIpMessageType::kRequest ||
      header.message_type_ == someip::SomeIpMessageType::kRequestNoReturn) {
    auto iid = endpoint_->GetProvidedServiceInstanceId(service_id);
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
    endpoint_->ProcessReceivedMessage(this, instance_id, std::move(message));
  }
}

void ServiceDiscoveryTcpConnection::RemoveSender(ServiceDiscoveryTcpSenderPtr sender) {
  assert(!inside_notify_);
  auto it = std::find(senders_.begin(), senders_.end(), sender);
  assert(it != senders_.end());
  senders_.erase(it);
}

void ServiceDiscoveryTcpConnection::RemoveSenderInsideNotify(ServiceDiscoveryTcpSenderPtr sender) {
  assert(inside_notify_);
  auto it = std::find(senders_.cbegin(), senders_.cend(), sender);
  assert(it != senders_.cend());
  removed_senders_.push_back(sender);
}

std::pair<bool, someip_posix_common::someip::InstanceId> ServiceDiscoveryTcpConnection::GetRequiredServiceInstanceId(
    someip_posix_common::someip::ServiceId service_id) {
  auto it = std::find_if(required_service_instances_.cbegin(), required_service_instances_.cend(),
                         [service_id](const RequiredServiceInstance& rsi) { return rsi.first == service_id; });
  if (it != required_service_instances_.cend()) {
    return {true, it->second};
  } else {
    return {false, 0};
  }
}

void ServiceDiscoveryTcpConnection::ApplySocketOptions() {
  const bool qos_enabled = socket_options_.qos_.first;
  if (qos_enabled) {
    const int qos_priority{socket_options_.qos_.second};
    socket_.SetPriority(qos_priority);
  }

  const bool keepalive_enabled = socket_options_.keep_alive_.first;
  if (keepalive_enabled) {
    const someip_posix_common::someip::KeepAliveParameters& params = socket_options_.keep_alive_.second;
    socket_.ParametrizeKeepAlive(params.keep_alive_time_, params.keep_alive_interval_, params.keep_alive_retry_count_);
    socket_.SetKeepAlive(keepalive_enabled);
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
