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
/**        \file  service_discovery_server_state_repetition.cc
 *        \brief  Repetition Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the repetition phase
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_repetition.h"
#include <cassert>
#include <string>
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerStateRepetition::ServiceDiscoveryServerStateRepetition()
    : ServiceDiscoveryServerState(ServiceDiscoveryServerStateHandle::kRepetitionPhase),
      current_repetition_(0U),
      current_repetition_delay_(vac::timer::Timer::Clock::duration::zero()) {}

void ServiceDiscoveryServerStateRepetition::OnEnter(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " [State " << static_cast<std::uint32_t>(GetHandle())
                     << ": Repetition Phase]";

  assert(context.GetInitialRepetitionMaxCount() != 0);

  current_repetition_ = 0;  // delay = RepetitionBaseDelay * 2^current_repetition = RepetitionBaseDelay
  current_repetition_delay_ = context.GetInitialRepetitionBaseDelay();

  context.StartTimer(std::chrono::duration_cast<vac::timer::Timer::Clock::duration>(
                         std::chrono::nanoseconds(current_repetition_delay_)),
                     false);
}

void ServiceDiscoveryServerStateRepetition::OnLeave(ServiceDiscoveryServerContextInterface& context) {
  if (context.GetRequestedNextState() == ServiceDiscoveryServerStateHandle::kDownPhase) {
    // Cancel RepetitionDelayTimer
    context.StopTimer();

    context.UnsubscribeAllEventgroupEntries();

    context.StopServiceInstance();
  }
}

bool ServiceDiscoveryServerStateRepetition::IsValidChange(const ServiceDiscoveryServerStateHandle handle) const {
  switch (handle) {
    case ServiceDiscoveryServerStateHandle::kDownPhase:
    case ServiceDiscoveryServerStateHandle::kMainPhase:
      return true;
    case ServiceDiscoveryServerStateHandle::kWaitPhase:
    case ServiceDiscoveryServerStateHandle::kRepetitionPhase:
    default:
      return false;
  }
}

void ServiceDiscoveryServerStateRepetition::OnServiceDown(ServiceDiscoveryServerContextInterface& context) {
  // send StopOfferService
  context.SendMulticastStopOfferServiceMessage();

  // enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateRepetition::OnNetworkDown(ServiceDiscoveryServerContextInterface& context) {
  // enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateRepetition::OnTimeOut(ServiceDiscoveryServerContextInterface& context) {
  current_repetition_++;

  context.SendMulticastOfferServiceMessage();

  if (context.GetInitialRepetitionMaxCount() == current_repetition_) {
    context.RequestStateChange(ServiceDiscoveryServerStateHandle::kMainPhase);
  } else {
    current_repetition_delay_ *= 2;  // delay = RepetitionBaseDelay * 2^current_repetition
    context.StartTimer(current_repetition_delay_, false);
  }
}

void ServiceDiscoveryServerStateRepetition::OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                      const std::string& from_address,
                                                                      std::uint16_t from_port,
                                                                      const ServiceDiscoveryEntry& entry,
                                                                      const ServiceDiscoveryOptionContainer& options) {
  (void)entry;
  (void)options;

  // No delay shall be set here
  context.SendUnicastOfferServiceMessage(from_address, from_port);
}

void ServiceDiscoveryServerStateRepetition::OnMulticastFindServiceEntry(
    ServiceDiscoveryServerContextInterface& context, const std::string& from_address, std::uint16_t from_port,
    const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options) {
  (void)entry;
  (void)options;

  if (context.GetRequestResponseDelayMax() == vac::timer::Timer::Clock::duration::zero()) {
    context.SendUnicastOfferServiceMessage(from_address, from_port);
  } else {
    context.SendUnicastOfferServiceMessage(from_address, from_port, context.GetRequestResponseDelayRandom());
  }
}

void ServiceDiscoveryServerStateRepetition::OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                       const std::string& from_address,
                                                                       std::uint16_t from_port,
                                                                       const ServiceDiscoveryEntry& entry,
                                                                       const ServiceDiscoveryOptionContainer& options) {
  context.SubscribeEventgroupEntry(from_address, from_port, entry, options);
}

void ServiceDiscoveryServerStateRepetition::OnStopSubscribeEventgroupEntry(
    ServiceDiscoveryServerContextInterface& context, const std::string& from_address, std::uint16_t from_port,
    const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options) {
  context.UnsubscribeEventgroupEntry(from_address, from_port, entry, options);
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
