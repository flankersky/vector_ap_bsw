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
/**        \file  service_discovery_client_state_pool.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_pool.h"
#include <stdexcept>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupState* ServiceDiscoveryClientEventgroupStatePool::GetState(
    const ServiceDiscoveryClientEventgroupStateHandle handle) {
  switch (handle) {
    case ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed:
      return &state_not_subscribed_;
      break;
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending:
      return &state_subscription_pending_;
      break;
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscribed:
      return &state_subscribed_;
      break;
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal:
      return &state_subscription_renewal_;
      break;
    default:
      throw std::out_of_range("ServiceDiscoveryClientEventgroupStatePool: state handle is out of range");
      break;
  }
}

bool ServiceDiscoveryClientEventgroupStatePool::IsValid(
    const ServiceDiscoveryClientEventgroupStateHandle handle) const {
  return handle == ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed ||
         handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending ||
         handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscribed ||
         handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal;
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
