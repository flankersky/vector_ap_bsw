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
/**        \file  service_discovery_tcp_endpoint.cc
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

#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryTcpEndpoint::ServiceDiscoveryTcpEndpoint(osabstraction::io::ReactorInterface* reactor,
                                                         packet_router::PacketRouterInterface* packet_router,
                                                         const someip_posix_common::someip::IpAddress& address,
                                                         someip_posix_common::someip::Port port,
                                                         const someip_posix_common::someip::SocketOptions& options)
    : reactor_(reactor),
      packet_router_(packet_router),
      address_(address),
      port_(port),
      logger_(ara::log::CreateLogger("ServiceDiscoveryTcpEndpoint", "")),
      server_(nullptr),
      server_users_(0),
      socket_options_(options) {}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryTcpEndpoint::GetAddress() const { return address_; }

someip_posix_common::someip::Port ServiceDiscoveryTcpEndpoint::GetPort() const { return port_; }

bool ServiceDiscoveryTcpEndpoint::HasActiveConnection(const someip_posix_common::someip::IpAddress& address,
                                                      someip_posix_common::someip::Port port) {
  auto it = GetConnection(address, port);
  if (it == connections_.end()) {
    return false;
  }
  return (*it)->IsActive() && (*it)->IsConnected();
}

bool ServiceDiscoveryTcpEndpoint::HasPassiveConnection(const someip_posix_common::someip::IpAddress& address,
                                                       someip_posix_common::someip::Port port) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": " << address << "," << port;
  auto it = GetConnection(address, port);
  if (it == connections_.end()) {
    return false;
  }
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": active " << (*it)->IsActive() << " connected "
                     << (*it)->IsConnected();
  return !(*it)->IsActive() && (*it)->IsConnected();
}

ServiceDiscoveryTcpEndpoint::ServiceDiscoveryTcpSenderPtr ServiceDiscoveryTcpEndpoint::GetSender(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  auto it = GetConnection(address, port);
  if (it == connections_.end()) {
    connections_.emplace_back(
        vac::language::make_unique<ServiceDiscoveryTcpConnection>(this, address, port, socket_options_));
    it = std::prev(connections_.end());
  }
  return vac::language::make_unique<ServiceDiscoveryTcpSender>(shared_from_this(), it->get());
}

ServiceDiscoveryTcpEndpoint::ServiceDiscoveryTcpReceiverPtr ServiceDiscoveryTcpEndpoint::GetReceiver() {
  return vac::language::make_unique<ServiceDiscoveryTcpReceiver>(shared_from_this());
}

void ServiceDiscoveryTcpEndpoint::AcceptConnection(ServiceDiscoveryTcpConnection::Socket&& socket) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": " << socket.GetRemoteAddress().toString();
  connections_.emplace_back(
      vac::language::make_unique<ServiceDiscoveryTcpConnection>(this, std::move(socket), socket_options_));
}

void ServiceDiscoveryTcpEndpoint::ProcessReceivedMessage(ServiceDiscoveryTcpConnection* connection,
                                                         someip_posix_common::someip::InstanceId instance_id,
                                                         SomeIpMessage&& message) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": instance id " << std::hex << instance_id << std::dec;
  auto response_sender = std::make_shared<ServiceDiscoveryTcpResponseSender>(shared_from_this(), connection);
  packet_router::Packet packet{new SomeIpMessage(std::move(message))};
  packet_router_->Forward(instance_id, response_sender, packet);
}

void ServiceDiscoveryTcpEndpoint::RegisterReadEventHandler(ServiceDiscoveryTcpConnection* connection, int handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  read_event_handlers_.insert({handle, connection});
  reactor_->RegisterEventHandler(handle, this, osabstraction::io::kReadEvent, nullptr);
}

void ServiceDiscoveryTcpEndpoint::UnregisterReadEventHandler(ServiceDiscoveryTcpConnection* connection, int handle) {
  (void)connection;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  reactor_->UnregisterEventHandler(handle, osabstraction::io::kReadEvent);
  read_event_handlers_.erase(handle);
}

void ServiceDiscoveryTcpEndpoint::RegisterWriteEventHandler(ServiceDiscoveryTcpConnection* connection, int handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  write_event_handlers_.insert({handle, connection});
  reactor_->RegisterEventHandler(handle, this, osabstraction::io::kWriteEvent, nullptr);
}

void ServiceDiscoveryTcpEndpoint::UnregisterWriteEventHandler(ServiceDiscoveryTcpConnection* connection, int handle) {
  (void)connection;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  reactor_->UnregisterEventHandler(handle, osabstraction::io::kWriteEvent);
  write_event_handlers_.erase(handle);
}

void ServiceDiscoveryTcpEndpoint::AcquireConnection(ServiceDiscoveryTcpConnection* connection) {
  auto users = connection->Acquire();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": users " << users;
}

void ServiceDiscoveryTcpEndpoint::ReleaseConnection(ServiceDiscoveryTcpConnection* connection) {
  auto users = connection->Release();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": users " << users;
  if (connection->IsActive() && (users == 0)) {
    CloseConnection(connection);
  }
}

void ServiceDiscoveryTcpEndpoint::AcquireServer() {
  if (++server_users_ == 1) {
    server_ = vac::language::make_unique<ServiceDiscoveryTcpServer>(reactor_, this);
  }
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": users " << server_users_.load();
}

void ServiceDiscoveryTcpEndpoint::ReleaseServer() {
  assert(server_users_ > 0);
  if (--server_users_ == 0) {
    server_.reset(nullptr);
    CloseAcceptedConnections();
  }
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": users " << server_users_.load();
}

void ServiceDiscoveryTcpEndpoint::RegisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.begin(), provided_service_instances_.end(),
                      ProvidedServiceInstance{service_id, instance_id});
  assert(it == provided_service_instances_.end());
  provided_service_instances_.emplace_back(service_id, instance_id);
}

void ServiceDiscoveryTcpEndpoint::UnregisterProvidedServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.begin(), provided_service_instances_.end(),
                      ProvidedServiceInstance{service_id, instance_id});
  assert(it != provided_service_instances_.end());
  provided_service_instances_.erase(it);
}

std::pair<bool, someip_posix_common::someip::InstanceId> ServiceDiscoveryTcpEndpoint::GetProvidedServiceInstanceId(
    someip_posix_common::someip::ServiceId service_id) {
  auto it = std::find_if(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                         [service_id](const ProvidedServiceInstance& psi) { return psi.first == service_id; });
  if (it != provided_service_instances_.cend()) {
    return {true, it->second};
  } else {
    return {false, 0};
  }
}

bool ServiceDiscoveryTcpEndpoint::HandleRead(int handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  auto it = read_event_handlers_.find(handle);
  if (it != read_event_handlers_.end()) {
    auto connection = it->second;
    if (!connection->HandleRead(handle)) {
      read_event_handlers_.erase(handle);
      if (!connection->IsConnected() && (connection->GetUsers() == 0)) {
        CloseConnection(connection);
      }
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

bool ServiceDiscoveryTcpEndpoint::HandleWrite(int handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": handle " << handle;
  auto it = write_event_handlers_.find(handle);
  if (it != write_event_handlers_.end()) {
    auto connection = it->second;
    if (!connection->HandleWrite(handle)) {
      write_event_handlers_.erase(handle);
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

void ServiceDiscoveryTcpEndpoint::CloseConnection(ServiceDiscoveryTcpConnection* connection) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << address_ << "," << port_ << " to "
                     << connection->GetAddress() << "," << connection->GetPort();
  connection->Disconnect();
  CleanUpConnectionEventHandlers(connection);
  auto it = std::find_if(connections_.begin(), connections_.end(),
                         [connection](const ServiceDiscoveryTcpConnectionPtr& c) { return c.get() == connection; });
  assert(it != connections_.end());
  connections_.erase(it);
}

void ServiceDiscoveryTcpEndpoint::CleanUpConnectionEventHandlers(ServiceDiscoveryTcpConnection* connection) {
  for (auto it = read_event_handlers_.begin(); it != read_event_handlers_.end();) {
    if (it->second == connection) {
      reactor_->UnregisterEventHandler(it->first, osabstraction::io::kReadEvent);
      it = read_event_handlers_.erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = write_event_handlers_.begin(); it != write_event_handlers_.end();) {
    if (it->second == connection) {
      reactor_->UnregisterEventHandler(it->first, osabstraction::io::kWriteEvent);
      it = write_event_handlers_.erase(it);
    } else {
      ++it;
    }
  }
}

ServiceDiscoveryTcpEndpoint::ServiceDiscoveryTcpConnectionContainerIter ServiceDiscoveryTcpEndpoint::GetConnection(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  return std::find_if(connections_.begin(), connections_.end(),
                      [&address, port](const ServiceDiscoveryTcpConnectionPtr& c) {
                        return c->GetAddress() == address && c->GetPort() == port;
                      });
}

void ServiceDiscoveryTcpEndpoint::CloseAcceptedConnections() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": total connections " << connections_.size();
  for (auto it = connections_.begin(); it != connections_.end();) {
    if (!(*it)->IsActive()) {
      (*it)->Disconnect();
      CleanUpConnectionEventHandlers(it->get());
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
