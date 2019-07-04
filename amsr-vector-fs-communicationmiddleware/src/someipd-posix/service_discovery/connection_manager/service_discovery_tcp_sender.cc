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
/**        \file  service_discovery_tcp_sender.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_sender.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryTcpSender::ServiceDiscoveryTcpSender(ServiceDiscoveryTcpEndpointPtr endpoint,
                                                     ServiceDiscoveryTcpConnectionPtr connection)
    : endpoint_(endpoint),
      connection_(connection),
      handler_(nullptr),
      logger_(ara::log::CreateLogger("ServiceDiscoveryTcpSender", "")) {
  endpoint_->AcquireConnection(connection_);
  connection_->RegisterSender(this);
}

ServiceDiscoveryTcpSender::~ServiceDiscoveryTcpSender() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  std::for_each(required_service_instances_.cbegin(), required_service_instances_.cend(),
                [this](const RequiredServiceInstance& rsi) {
                  connection_->UnregisterRequiredServiceInstance(rsi.first, rsi.second);
                });
  connection_->UnregisterSender(this);
  endpoint_->ReleaseConnection(connection_);
}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryTcpSender::GetRemoteAddress() const {
  return connection_->GetAddress();
}

someip_posix_common::someip::Port ServiceDiscoveryTcpSender::GetRemotePort() const { return connection_->GetPort(); }

void ServiceDiscoveryTcpSender::SetConnectionStateChangeHandler(
    ServiceDiscoveryTcpConnectionStateChangeHandlerPtr handler) {
  handler_ = handler;
}

void ServiceDiscoveryTcpSender::RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.cbegin(), required_service_instances_.cend(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it == required_service_instances_.cend()) {
    required_service_instances_.emplace_back(service_id, instance_id);
    connection_->RegisterRequiredServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryTcpSender::UnregisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.begin(), required_service_instances_.end(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it != required_service_instances_.cend()) {
    required_service_instances_.erase(it);
    connection_->UnregisterRequiredServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryTcpSender::Forward(someip_posix_common::someip::InstanceId instance_id,
                                        packet_router::Packet packet) {
  connection_->Forward(instance_id, std::move(packet));
}

void ServiceDiscoveryTcpSender::Notify() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (handler_) {
    if (connection_->IsConnected()) {
      handler_->OnConnect(this);
    } else {
      handler_->OnDisconnect(this);
    }
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
