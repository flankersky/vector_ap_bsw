/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights redown with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_server_state_down.cc
 *        \brief  Down Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the down phase
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_down.h"
#include <string>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerStateDown::ServiceDiscoveryServerStateDown()
    : ServiceDiscoveryServerState(ServiceDiscoveryServerStateHandle::kDownPhase) {}

void ServiceDiscoveryServerStateDown::OnEnter(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " [State " << static_cast<std::uint32_t>(GetHandle())
                     << ": Down Phase]";

  assert(!(context.IsNetworkUp()) || !(context.IsServiceUp()));
}

void ServiceDiscoveryServerStateDown::OnLeave(ServiceDiscoveryServerContextInterface& context) { (void)context; }

bool ServiceDiscoveryServerStateDown::IsValidChange(const ServiceDiscoveryServerStateHandle handle) const {
  switch (handle) {
    case ServiceDiscoveryServerStateHandle::kWaitPhase:
      return true;
      break;
    case ServiceDiscoveryServerStateHandle::kDownPhase:
    case ServiceDiscoveryServerStateHandle::kRepetitionPhase:
    case ServiceDiscoveryServerStateHandle::kMainPhase:
    default:
      return false;
      break;
  }
}

void ServiceDiscoveryServerStateDown::OnServiceUp(ServiceDiscoveryServerContextInterface& context) {
  if (context.IsNetworkUp()) {
    context.RequestStateChange(ServiceDiscoveryServerStateHandle::kWaitPhase);
  }
}

void ServiceDiscoveryServerStateDown::OnNetworkUp(ServiceDiscoveryServerContextInterface& context) {
  if (context.IsServiceUp()) {
    context.RequestStateChange(ServiceDiscoveryServerStateHandle::kWaitPhase);
  }
}

void ServiceDiscoveryServerStateDown::OnServiceDown(ServiceDiscoveryServerContextInterface& context) {
  (void)context;
  // Do nothing
}

void ServiceDiscoveryServerStateDown::OnNetworkDown(ServiceDiscoveryServerContextInterface& context) {
  (void)context;
  // Do nothing
}

void ServiceDiscoveryServerStateDown::OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                const std::string& from_address,
                                                                std::uint16_t from_port,
                                                                const ServiceDiscoveryEntry& entry,
                                                                const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // Do nothing
}

void ServiceDiscoveryServerStateDown::OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                  const std::string& from_address,
                                                                  std::uint16_t from_port,
                                                                  const ServiceDiscoveryEntry& entry,
                                                                  const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // Do nothing
}

void ServiceDiscoveryServerStateDown::OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                 const std::string& from_address,
                                                                 std::uint16_t from_port,
                                                                 const ServiceDiscoveryEntry& entry,
                                                                 const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // Do nothing
}

void ServiceDiscoveryServerStateDown::OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                     const std::string& from_address,
                                                                     std::uint16_t from_port,
                                                                     const ServiceDiscoveryEntry& entry,
                                                                     const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // Do nothing
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
