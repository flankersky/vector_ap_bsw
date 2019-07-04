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
/**        \file  service_discovery_udp_sender.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_sender.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryUdpSender::ServiceDiscoveryUdpSender(ServiceDiscoveryUdpEndpointPtr endpoint,
                                                     ServiceDiscoveryUdpConnectionPtr connection)
    : endpoint_(endpoint), connection_(connection) {
  endpoint_->AcquireConnection(connection_);
}

ServiceDiscoveryUdpSender::~ServiceDiscoveryUdpSender() {
  std::for_each(required_service_instances_.cbegin(), required_service_instances_.cend(),
                [this](const RequiredServiceInstance& rsi) {
                  endpoint_->UnregisterRequiredServiceInstance(rsi.first, rsi.second);
                });
  endpoint_->ReleaseConnection(connection_);
}

const someip_posix_common::someip::IpAddress& ServiceDiscoveryUdpSender::GetRemoteAddress() const {
  return connection_->GetAddress();
}

someip_posix_common::someip::Port ServiceDiscoveryUdpSender::GetRemotePort() const { return connection_->GetPort(); }

void ServiceDiscoveryUdpSender::Forward(someip_posix_common::someip::InstanceId instance_id,
                                        packet_router::Packet packet) {
  connection_->Forward(instance_id, std::move(packet));
}

void ServiceDiscoveryUdpSender::RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.cbegin(), required_service_instances_.cend(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it == required_service_instances_.cend()) {
    required_service_instances_.emplace_back(service_id, instance_id);
    endpoint_->RegisterRequiredServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryUdpSender::UnregisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.begin(), required_service_instances_.end(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it != required_service_instances_.cend()) {
    required_service_instances_.erase(it);
    endpoint_->UnregisterRequiredServiceInstance(service_id, instance_id);
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
