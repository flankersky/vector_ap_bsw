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
/**        \file  service_discovery_client_eventgroup_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_manager.h"
#include <algorithm>
#include <cassert>
#include <ios>
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupManager::ServiceDiscoveryClientEventgroupManager(
    const EventgroupIdContainer& eventgroups, ServiceDiscoveryClientEventgroupManagerContext* context,
    vac::timer::TimerManager* timer_manager)
    : logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupManager", "")},
      context_{context},
      timer_manager_{timer_manager} {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": number of eventgroups " << eventgroups.size();
  eventgroup_state_machines_.reserve(eventgroups.size());
  for (const auto& eid : eventgroups) {
    eventgroup_state_machines_.emplace_back(
        vac::language::make_unique<ServiceDiscoveryClientEventgroupStateMachine>(eid, this, timer_manager_));
  }
}

ServiceDiscoveryEventSubscriptionState ServiceDiscoveryClientEventgroupManager::GetEventgroupSubscriptionState(
    std::uint16_t eventgroup_id) const {
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  return (*it)->GetSubscriptionState();
}

std::size_t ServiceDiscoveryClientEventgroupManager::GetEventgroupSubscribersCount(std::uint16_t eventgroup_id) const {
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  return (*it)->GetSubscribersCount();
}

std::pair<std::string, std::uint16_t> ServiceDiscoveryClientEventgroupManager::GetEventgroupMulticastEndpoint(
    std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  return {(*it)->GetMulticastAddress(), (*it)->GetMulticastPort()};
}

void ServiceDiscoveryClientEventgroupManager::OnOfferService(bool is_multicast) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": multicast " << std::boolalpha << is_multicast
                     << std::noboolalpha;
  std::for_each(
      eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
      [is_multicast](ServiceDiscoveryClientEventgroupStateMachinePtr& sm) { sm->OnOfferService(is_multicast); });
}

void ServiceDiscoveryClientEventgroupManager::OnStopOfferService() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                [](ServiceDiscoveryClientEventgroupStateMachinePtr& sm) { sm->OnStopOfferService(); });
}

void ServiceDiscoveryClientEventgroupManager::OnSubscribe(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  (*it)->OnSubscribe();
}

void ServiceDiscoveryClientEventgroupManager::OnUnsubscribe(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  (*it)->OnUnsubscribe();
}

void ServiceDiscoveryClientEventgroupManager::OnSubscribeEventgroupAck(std::uint16_t eventgroup_id,
                                                                       const std::string& multicast_address,
                                                                       std::uint16_t multicast_port, TimerDelay ttl) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  (*it)->OnSubscribeEventgroupAck(multicast_address, multicast_port, ttl);
}

void ServiceDiscoveryClientEventgroupManager::OnSubscribeEventgroupNack(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = GetEventgroup(eventgroup_id);
  assert(it != eventgroup_state_machines_.end());
  (*it)->OnSubscribeEventgroupNack();
}

void ServiceDiscoveryClientEventgroupManager::OnServiceReleased() {
  std::for_each(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                [](ServiceDiscoveryClientEventgroupStateMachinePtr& sm) { sm->OnShutdown(); });
}

void ServiceDiscoveryClientEventgroupManager::SendSubscribeEventgroup(std::uint16_t eventgroup_id,
                                                                      bool request_initial_events) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec
                     << " request initial events " << std::boolalpha << request_initial_events << std::noboolalpha;
  context_->SendSubscribeEventgroup(eventgroup_id, request_initial_events);
}

void ServiceDiscoveryClientEventgroupManager::SendStopSubscribeEventgroup(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  context_->SendStopSubscribeEventgroup(eventgroup_id);
}

void ServiceDiscoveryClientEventgroupManager::OnEventgroupSubscribed(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  context_->OnEventgroupSubscribed(eventgroup_id);
}

void ServiceDiscoveryClientEventgroupManager::OnEventgroupUnsubscribed(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  context_->OnEventgroupUnsubscribed(eventgroup_id);
}

void ServiceDiscoveryClientEventgroupManager::OnEventgroupSubscriptionPending(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  context_->OnEventgroupSubscriptionPending(eventgroup_id);
}

ServiceDiscoveryClientEventgroupManager::ServiceDiscoveryClientEventgroupStateMachineContainer::iterator
ServiceDiscoveryClientEventgroupManager::GetEventgroup(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": container size " << eventgroup_state_machines_.size()
                     << " eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = std::find_if(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                         [eventgroup_id](const ServiceDiscoveryClientEventgroupStateMachinePtr& sm) {
                           assert(sm);
                           return sm->GetEventgroupId() == eventgroup_id;
                         });
  return it;
}

ServiceDiscoveryClientEventgroupManager::ServiceDiscoveryClientEventgroupStateMachineContainer::const_iterator
ServiceDiscoveryClientEventgroupManager::GetEventgroup(std::uint16_t eventgroup_id) const {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": container size " << eventgroup_state_machines_.size()
                     << " eventgroup " << std::hex << eventgroup_id << std::dec;
  auto it = std::find_if(eventgroup_state_machines_.begin(), eventgroup_state_machines_.end(),
                         [eventgroup_id](const ServiceDiscoveryClientEventgroupStateMachinePtr& sm) {
                           assert(sm);
                           return sm->GetEventgroupId() == eventgroup_id;
                         });
  return it;
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
