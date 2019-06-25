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
/**        \file  service_discovery_server_state_main.cc
 *        \brief  Main Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the main phase
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_main.h"
#include <string>
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerStateMain::ServiceDiscoveryServerStateMain()
    : ServiceDiscoveryServerState(ServiceDiscoveryServerStateHandle::kMainPhase) {}

void ServiceDiscoveryServerStateMain::OnEnter(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " [State " << static_cast<std::uint32_t>(GetHandle())
                     << ": Main Phase]";

  if (context.GetCyclicOfferServiceDelay() > vac::timer::Timer::Clock::duration::zero()) {
    context.StartTimer(context.GetCyclicOfferServiceDelay(), true);
  }
}

void ServiceDiscoveryServerStateMain::OnLeave(ServiceDiscoveryServerContextInterface& context) {
  // Cancel CyclicOfferTimer
  context.StopTimer();

  context.UnsubscribeAllEventgroupEntries();
  context.StopServiceInstance();
}

bool ServiceDiscoveryServerStateMain::IsValidChange(const ServiceDiscoveryServerStateHandle handle) const {
  switch (handle) {
    case ServiceDiscoveryServerStateHandle::kDownPhase:
      return true;
      break;
    case ServiceDiscoveryServerStateHandle::kWaitPhase:
    case ServiceDiscoveryServerStateHandle::kRepetitionPhase:
    case ServiceDiscoveryServerStateHandle::kMainPhase:
    default:
      return false;
      break;
  }
}

void ServiceDiscoveryServerStateMain::OnServiceDown(ServiceDiscoveryServerContextInterface& context) {
  // send StopOfferService
  context.SendMulticastStopOfferServiceMessage();

  // enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateMain::OnNetworkDown(ServiceDiscoveryServerContextInterface& context) {
  // enter the Down Phase
  context.RequestStateChange(ServiceDiscoveryServerStateHandle::kDownPhase);
}

void ServiceDiscoveryServerStateMain::OnTimeOut(ServiceDiscoveryServerContextInterface& context) {
  context.SendMulticastOfferServiceMessage();

  if (message_optimization_used_) {  // [PRS_SOMEIPSD_00423]
    latest_offer_timestamp_ = vac::timer::Timer::Clock::now();
  }
}

void ServiceDiscoveryServerStateMain::OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                const std::string& from_address,
                                                                std::uint16_t from_port,
                                                                const ServiceDiscoveryEntry& entry,
                                                                const ServiceDiscoveryOptionContainer& options) {
  (void)entry;
  (void)options;

  // No delay shall be set here
  if (!message_optimization_used_) {
    context.SendUnicastOfferServiceMessage(from_address, from_port);
  } else {  // [PRS_SOMEIPSD_00423]
    if ((vac::timer::Timer::Clock::now() - latest_offer_timestamp_) > context.GetCyclicOfferServiceDelay() / 2) {
      context.SendUnicastOfferServiceMessage(from_address, from_port);
    } else {
      context.SendMulticastOfferServiceMessage();
    }
    latest_offer_timestamp_ = vac::timer::Timer::Clock::now();
  }
}

void ServiceDiscoveryServerStateMain::OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                                  const std::string& from_address,
                                                                  std::uint16_t from_port,
                                                                  const ServiceDiscoveryEntry& entry,
                                                                  const ServiceDiscoveryOptionContainer& options) {
  (void)entry;
  (void)options;

  if (context.GetRequestResponseDelayMax() == vac::timer::Timer::Clock::duration::zero()) {
    context.SendUnicastOfferServiceMessage(from_address, from_port);
  } else {
    if (!message_optimization_used_) {
      context.SendUnicastOfferServiceMessage(from_address, from_port, context.GetRequestResponseDelayRandom());
    } else {  // [PRS_SOMEIPSD_00423]
      context.SendMulticastOfferServiceMessage(context.GetRequestResponseDelayRandom());
      latest_offer_timestamp_ = vac::timer::Timer::Clock::now();
    }
  }
}

void ServiceDiscoveryServerStateMain::OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                 const std::string& from_address,
                                                                 std::uint16_t from_port,
                                                                 const ServiceDiscoveryEntry& entry,
                                                                 const ServiceDiscoveryOptionContainer& options) {
  context.SubscribeEventgroupEntry(from_address, from_port, entry, options);
}

void ServiceDiscoveryServerStateMain::OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                     const std::string& from_address,
                                                                     std::uint16_t from_port,
                                                                     const ServiceDiscoveryEntry& entry,
                                                                     const ServiceDiscoveryOptionContainer& options) {
  context.UnsubscribeEventgroupEntry(from_address, from_port, entry, options);
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
