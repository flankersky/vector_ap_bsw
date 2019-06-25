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
/**        \file  service_discovery_udp_receiver.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_receiver.h"
#include <algorithm>
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_endpoint.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryUdpReceiver::ServiceDiscoveryUdpReceiver(ServiceDiscoveryUdpEndpointPtr endpoint)
    : endpoint_(endpoint) {
  endpoint_->AcquireServer();
}

ServiceDiscoveryUdpReceiver::~ServiceDiscoveryUdpReceiver() {
  std::for_each(required_service_instances_.cbegin(), required_service_instances_.cend(),
                [this](const RequiredServiceInstance& rsi) {
                  endpoint_->UnregisterRequiredServiceInstance(rsi.first, rsi.second);
                });
  std::for_each(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                [this](const ProvidedServiceInstance& psi) {
                  endpoint_->UnregisterProvidedServiceInstance(psi.first, psi.second);
                });
  endpoint_->ReleaseServer();
}

void ServiceDiscoveryUdpReceiver::RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.cbegin(), required_service_instances_.cend(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it == required_service_instances_.cend()) {
    required_service_instances_.emplace_back(service_id, instance_id);
    endpoint_->RegisterRequiredServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryUdpReceiver::UnregisterRequiredServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(required_service_instances_.begin(), required_service_instances_.end(),
                      RequiredServiceInstance{service_id, instance_id});
  if (it != required_service_instances_.cend()) {
    required_service_instances_.erase(it);
    endpoint_->UnregisterRequiredServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryUdpReceiver::RegisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                                                  someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                      ProvidedServiceInstance{service_id, instance_id});
  if (it == provided_service_instances_.cend()) {
    provided_service_instances_.emplace_back(service_id, instance_id);
    endpoint_->RegisterProvidedServiceInstance(service_id, instance_id);
  }
}

void ServiceDiscoveryUdpReceiver::UnregisterProvidedServiceInstance(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = std::find(provided_service_instances_.begin(), provided_service_instances_.end(),
                      ProvidedServiceInstance{service_id, instance_id});
  if (it != provided_service_instances_.cend()) {
    provided_service_instances_.erase(it);
    endpoint_->UnregisterProvidedServiceInstance(service_id, instance_id);
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
