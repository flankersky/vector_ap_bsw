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
/**        \file  service_discovery_server_state_owner.cc
 *        \brief  Service Discovery Server State Owner
 *
 *      \details  This class is the owner of the State Pool. It receives events and forward them to the current state.
 *It holds as well more variables that are not represented as separate states.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_owner.h"
#include <cassert>
#include "someipd-posix/configuration/configuration.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerStateOwner::ServiceDiscoveryServerStateOwner(ServiceDiscoveryServerStatePool& state_pool,
                                                                   vac::timer::TimerManager* timer_manager)
    : vac::statemachine::StateOwner<ServiceDiscoveryServerState>(state_pool,
                                                                 ServiceDiscoveryServerStateHandle::kDownPhase),
      sm_timer_(timer_manager, this),
      logger_(ara::log::CreateLogger("ServiceDiscoveryServerStateOwner", "")) {}

void ServiceDiscoveryServerStateOwner::SetContext(ServiceDiscoveryServerContextInterface* context) {
  context_ = context;
}

ServiceDiscoveryServerContextInterface& ServiceDiscoveryServerStateOwner::GetContext() { return *context_; }

/// Current State Variables

bool ServiceDiscoveryServerStateOwner::IsNetworkUp() { return network_up_; }

bool ServiceDiscoveryServerStateOwner::IsServiceUp() { return service_up_; }

/// State Control

void ServiceDiscoveryServerStateOwner::UpdateState() {
  ServiceDiscoveryServerStateHandle requested_next_state = GetContext().GetRequestedNextState();
  if (requested_next_state != GetState()->GetHandle()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__
                       << "(Next State: " << static_cast<std::uint32_t>(requested_next_state) << ")";
    bool transit_success = TryChangeState(requested_next_state);
    assert(transit_success && "Cannot change current State");
    assert((requested_next_state == GetState()->GetHandle()) && "State is changed again within OnEnter !!");
  }
}

/// Timer

void ServiceDiscoveryServerStateOwner::StartTimer(vac::timer::Timer::Clock::duration period, bool periodic) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << (periodic ? " Periodic" : " OneShot") << "("
                     << std::chrono::nanoseconds(period).count() << " ns)";

  if (periodic) {
    sm_timer_.SetPeriod(period);
  } else {
    sm_timer_.SetOneShot(period);
  }
  sm_timer_.Start();
}

void ServiceDiscoveryServerStateOwner::StopTimer() { sm_timer_.Stop(); }

bool ServiceDiscoveryServerStateOwner::OnTimeOut() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  GetState()->OnTimeOut(GetContext());
  UpdateState();
  return true;
}

/// Events

void ServiceDiscoveryServerStateOwner::OnNetworkUp() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  network_up_ = true;
  GetState()->OnNetworkUp(GetContext());
  UpdateState();
}

void ServiceDiscoveryServerStateOwner::OnNetworkDown() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  network_up_ = false;
  GetState()->OnNetworkDown(GetContext());
  UpdateState();
}

void ServiceDiscoveryServerStateOwner::OnServiceUp() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  service_up_ = true;
  GetState()->OnServiceUp(GetContext());
  UpdateState();
}

void ServiceDiscoveryServerStateOwner::OnServiceDown() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  service_up_ = false;

  GetState()->OnServiceDown(GetContext());

  UpdateState();
}

void ServiceDiscoveryServerStateOwner::OnUnicastFindServiceEntry(const std::string& from_address,
                                                                 std::uint16_t from_port,
                                                                 const ServiceDiscoveryEntry& entry,
                                                                 const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  GetState()->OnUnicastFindServiceEntry(GetContext(), from_address, from_port, entry, options);
}

void ServiceDiscoveryServerStateOwner::OnMulticastFindServiceEntry(const std::string& from_address,
                                                                   std::uint16_t from_port,
                                                                   const ServiceDiscoveryEntry& entry,
                                                                   const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")";

  GetState()->OnMulticastFindServiceEntry(GetContext(), from_address, from_port, entry, options);
}

void ServiceDiscoveryServerStateOwner::OnSubscribeEventgroupEntry(const std::string& from_address,
                                                                  std::uint16_t from_port,
                                                                  const ServiceDiscoveryEntry& entry,
                                                                  const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")"
                     << ": SubscribeEventgroup from " << from_address << "," << from_port << std::hex << " ("
                     << entry.eventgroup_id_ << "," << static_cast<int>(entry.counter_) << ")" << std::dec;

  GetState()->OnSubscribeEventgroupEntry(GetContext(), from_address, from_port, entry, options);
}

void ServiceDiscoveryServerStateOwner::OnStopSubscribeEventgroupEntry(const std::string& from_address,
                                                                      std::uint16_t from_port,
                                                                      const ServiceDiscoveryEntry& entry,
                                                                      const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (State " << static_cast<std::uint32_t>(GetState()->GetHandle())
                     << ")"
                     << ": StopSubscribeEventgroup from " << from_address << "," << from_port << std::hex << " ("
                     << entry.eventgroup_id_ << "," << static_cast<int>(entry.counter_) << ")" << std::dec;

  GetState()->OnStopSubscribeEventgroupEntry(GetContext(), from_address, from_port, entry, options);
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
