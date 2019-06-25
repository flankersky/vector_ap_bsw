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
/**        \file  service_discovery_server_state_wait.cc
 *        \brief  Initial Wait Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the initial wait phase
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_wait.h"
#include <string>
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerStateWait::ServiceDiscoveryServerStateWait()
    : ServiceDiscoveryServerState(ServiceDiscoveryServerStateHandle::kWaitPhase) {}

void ServiceDiscoveryServerStateWait::OnEnter(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " [State " << static_cast<std::uint32_t>(GetHandle())
                     << ": Initial Wait Phase]";

  // Open all Socket Connections associated with this Server Service Instance.
  context.StartServiceInstance();

  // Start InitialDelayTimer
  vac::timer::Timer::Clock::duration delay = context.GetInitialDelayRandom();
  context.StartTimer(delay, false);
}

void ServiceDiscoveryServerStateWait::OnLeave(ServiceDiscoveryServerContextInterface& context) {
  if (context.GetRequestedNextState() == ServiceDiscoveryServerStateHandle::kDownPhase) {
    // Cancel InitialDelayTimer
    context.StopTimer();

    context.StopServiceInstance();
  }
}

bool ServiceDiscoveryServerStateWait::IsValidChange(const ServiceDiscoveryServerStateHandle handle) const {
  switch (handle) {
    case ServiceDiscoveryServerStateHandle::kDownPhase:
    case ServiceDiscoveryServerStateHandle::kRepetitionPhase:
    case ServiceDiscoveryServerStateHandle::kMainPhase:
      return true;
    case ServiceDiscoveryServerStateHandle::kWaitPhase:
    default:
      return false;
  }
}

void ServiceDiscoveryServerStateWait::OnServiceDown(ServiceDiscoveryServerContextInterface& context) {
  // Enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateWait::OnNetworkDown(ServiceDiscoveryServerContextInterface& context) {
  // Enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateWait::OnTimeOut(ServiceDiscoveryServerContextInterface& context) {
  context.SendMulticastOfferServiceMessage();

  if (context.GetInitialRepetitionMaxCount() > 0) {
    context.RequestStateChange(ServiceDiscoveryServerStateHandle::kRepetitionPhase);
  } else {
    context.RequestStateChange(ServiceDiscoveryServerStateHandle::kMainPhase);
  }
}

void ServiceDiscoveryServerStateWait::OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                const std::string& from_address,
                                                                std::uint16_t from_port,
                                                                const ServiceDiscoveryEntry& entry,
                                                                const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // ignore
}

void ServiceDiscoveryServerStateWait::OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                  const std::string& from_address,
                                                                  std::uint16_t from_port,
                                                                  const ServiceDiscoveryEntry& entry,
                                                                  const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;
  // ignore
}

void ServiceDiscoveryServerStateWait::OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                 const std::string& from_address,
                                                                 std::uint16_t from_port,
                                                                 const ServiceDiscoveryEntry& entry,
                                                                 const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  // ignore
}

void ServiceDiscoveryServerStateWait::OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                     const std::string& from_address,
                                                                     std::uint16_t from_port,
                                                                     const ServiceDiscoveryEntry& entry,
                                                                     const ServiceDiscoveryOptionContainer& options) {
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  // ignore
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
