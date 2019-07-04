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
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_pool.h"
#include <stdexcept>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientState* ServiceDiscoveryClientStatePool::GetState(const ServiceDiscoveryClientStateHandle handle) {
  switch (handle) {
    case ServiceDiscoveryClientStateHandle::kDownPhase:
      return &state_down_phase_;
      break;
    case ServiceDiscoveryClientStateHandle::kInitialWaitPhase:
      return &state_initial_wait_phase_;
      break;
    case ServiceDiscoveryClientStateHandle::kRepetitionPhase:
      return &state_repetition_phase_;
      break;
    case ServiceDiscoveryClientStateHandle::kMainPhase:
      return &state_main_phase_;
      break;
    default:
      throw std::out_of_range("ServiceDiscoveryClientStatePool: state handle is out of range");
      break;
  }
}

bool ServiceDiscoveryClientStatePool::IsValid(const ServiceDiscoveryClientStateHandle handle) const {
  return (handle == ServiceDiscoveryClientStateHandle::kDownPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kInitialWaitPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kRepetitionPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kMainPhase);
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
