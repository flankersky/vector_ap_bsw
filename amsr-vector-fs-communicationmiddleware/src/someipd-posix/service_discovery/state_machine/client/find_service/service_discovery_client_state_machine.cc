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
/**        \file  service_discovery_client_state_machine.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine.h"
#include <cassert>
#include <stdexcept>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateMachine::ServiceDiscoveryClientStateMachine(
    ServiceDiscoveryClientStateMachineContext* context, vac::timer::TimerManager* timer_manager)
    : logger_{ara::log::CreateLogger("ServiceDiscoveryClientStateMachine", "")},
      is_service_available_{false},
      is_network_up_{false},
      is_service_requested_{false},
      context_{context},
      state_owner_{state_pool_, *this},
      state_timer_{timer_manager, this} {}

void ServiceDiscoveryClientStateMachine::OnNetworkUp() {
  is_network_up_ = true;
  state_owner_.GetState()->OnNetworkUp(*this);
}

void ServiceDiscoveryClientStateMachine::OnNetworkDown() {
  is_network_up_ = false;
  state_owner_.GetState()->OnNetworkDown(*this);
}

void ServiceDiscoveryClientStateMachine::OnServiceRequested() {
  assert(!is_service_requested_);
  is_service_requested_ = true;
  state_owner_.GetState()->OnServiceRequested(*this);
}

void ServiceDiscoveryClientStateMachine::OnServiceReleased() {
  assert(is_service_requested_);
  is_service_requested_ = false;
  state_owner_.GetState()->OnServiceReleased(*this);
}

void ServiceDiscoveryClientStateMachine::OnOfferService() { state_owner_.GetState()->OnOfferService(*this); }

void ServiceDiscoveryClientStateMachine::OnStopOfferService() { state_owner_.GetState()->OnStopOfferService(*this); }

void ServiceDiscoveryClientStateMachine::OnTimeout() { state_owner_.GetState()->OnTimeout(*this); }

bool ServiceDiscoveryClientStateMachine::IsServiceAvailable() const { return is_service_available_; }

void ServiceDiscoveryClientStateMachine::SetServiceAvailable(bool is_available) {
  is_service_available_ = is_available;
  context_->SetServiceAvailable(is_available);
}

bool ServiceDiscoveryClientStateMachine::IsNetworkUp() const { return is_network_up_; }

bool ServiceDiscoveryClientStateMachine::IsServiceRequested() const { return is_service_requested_; }

ServiceDiscoveryClientStateMachine::TimerDelay ServiceDiscoveryClientStateMachine::GetOfferServiceTtl() const {
  return context_->GetOfferServiceTtl();
}

ServiceDiscoveryClientStateMachine::TimerDelay ServiceDiscoveryClientStateMachine::GetFindServiceInitialDelayMin()
    const {
  return context_->GetFindServiceInitialDelayMin();
}

ServiceDiscoveryClientStateMachine::TimerDelay ServiceDiscoveryClientStateMachine::GetFindServiceInitialDelayMax()
    const {
  return context_->GetFindServiceInitialDelayMax();
}

std::size_t ServiceDiscoveryClientStateMachine::GetFindServiceRepetitionsMax() const {
  return context_->GetFindServiceRepetitionsMax();
}

ServiceDiscoveryClientStateMachine::TimerDelay ServiceDiscoveryClientStateMachine::GetFindServiceRepetitionsBaseDelay()
    const {
  return context_->GetFindServiceRepetitionsBaseDelay();
}

void ServiceDiscoveryClientStateMachine::SendFindService() { context_->SendFindService(); }

void ServiceDiscoveryClientStateMachine::ChangeState(const ServiceDiscoveryClientStateHandle handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const auto ret = state_owner_.TryChangeState(handle);
  if (!ret) {
    throw std::logic_error{"ServiceDiscoveryClientStateMachine: illegal state change"};
  }
}

void ServiceDiscoveryClientStateMachine::StartTimer(TimerDelay delay) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": delay " << delay.count();
  if (delay.count() == 0) {
    throw std::logic_error{"ServiceDiscoveryClientStateMachine: illegal timer delay"};
  }
  state_timer_.SetOneShot(delay);
  state_timer_.Start();
}

void ServiceDiscoveryClientStateMachine::StopTimer() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_timer_.Stop();
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
