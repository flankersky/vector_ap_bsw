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
/**        \file  service_discovery_server_state_pool.cc
 *        \brief  Service Discovery Server State Pool
 *
 *      \details  This class initializes and contains all states of the SD Server State Machine
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_pool.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerState* ServiceDiscoveryServerStatePool::GetState(const Handle handle) {
  switch (handle) {
    case Handle::kDownPhase:
      return &down_state_;
    case Handle::kWaitPhase:
      return &initial_wait_state_;
    case Handle::kRepetitionPhase:
      return &repetition_state_;
    case Handle::kMainPhase:
      return &main_state_;
    default:
      return nullptr;
  }
}

bool ServiceDiscoveryServerStatePool::IsValid(const Handle handle) const {
  switch (handle) {
    case Handle::kDownPhase:
    case Handle::kWaitPhase:
    case Handle::kRepetitionPhase:
    case Handle::kMainPhase:
      return true;
    default:
      return false;
  }
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
