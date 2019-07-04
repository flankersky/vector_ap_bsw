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
/**        \file  service_discovery_client_eventgroup_state_machine.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_machine.h"
#include <cassert>
#include <ios>
#include <stdexcept>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

const std::chrono::milliseconds ServiceDiscoveryClientEventgroupStateMachine::kSubscribeEventgroupAckTimeout{2000};

ServiceDiscoveryClientEventgroupStateMachine::ServiceDiscoveryClientEventgroupStateMachine(
    std::uint16_t eventgroup_id, ServiceDiscoveryClientEventgroupStateMachineContext* context,
    vac::timer::TimerManager* timer_manager)
    : logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupStateMachine", "")},
      eventgroup_id_{eventgroup_id},
      is_service_available_{false},
      subscription_counter_{0},
      subscription_multicast_address_{},
      subscription_multicast_port_{},
      subscription_ttl_{std::chrono::milliseconds{0}},
      context_{context},
      state_owner_{state_pool_, *this},
      state_timer_{timer_manager, this},
      state_ttl_timer_{timer_manager, this} {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
}

std::uint16_t ServiceDiscoveryClientEventgroupStateMachine::GetEventgroupId() const { return eventgroup_id_; }

ServiceDiscoveryEventSubscriptionState ServiceDiscoveryClientEventgroupStateMachine::GetSubscriptionState() const {
  switch (state_owner_.GetState()->GetHandle()) {
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscribed:
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal: {
      return ServiceDiscoveryEventSubscriptionState::kSubscribed;
      break;
    }
    case ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed: {
      return ServiceDiscoveryEventSubscriptionState::kNotSubscribed;
      break;
    }
    case ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending: {
      return ServiceDiscoveryEventSubscriptionState::kSubscriptionPending;
      break;
    }
    default: {
      assert(false && "Unknown state");
      return ServiceDiscoveryEventSubscriptionState::kNotSubscribed;
      break;
    }
  }
}

std::size_t ServiceDiscoveryClientEventgroupStateMachine::GetSubscribersCount() const { return subscription_counter_; }

const std::string& ServiceDiscoveryClientEventgroupStateMachine::GetMulticastAddress() const {
  return subscription_multicast_address_;
}

std::uint16_t ServiceDiscoveryClientEventgroupStateMachine::GetMulticastPort() const {
  return subscription_multicast_port_;
}

void ServiceDiscoveryClientEventgroupStateMachine::OnOfferService(bool is_multicast) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": multicast " << std::boolalpha << is_multicast
                     << std::noboolalpha;
  is_service_available_ = true;
  state_owner_.GetState()->OnOfferService(*this, is_multicast);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnStopOfferService() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  is_service_available_ = false;
  state_owner_.GetState()->OnStopOfferService(*this);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnSubscribe() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  ++subscription_counter_;
  if (subscription_counter_ == 1) {
    state_owner_.GetState()->OnRequested(*this);
  }
}

void ServiceDiscoveryClientEventgroupStateMachine::OnUnsubscribe() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (subscription_counter_ > 0) {
    --subscription_counter_;
    if (subscription_counter_ == 0) {
      state_owner_.GetState()->OnReleased(*this);
    }
  } else {
    throw std::logic_error{"ServiceDiscoveryClientEventgroupStateMachine: eventgroup is already released"};
  }
}

void ServiceDiscoveryClientEventgroupStateMachine::OnSubscribeEventgroupAck(const std::string& multicast_address,
                                                                            std::uint16_t multicast_port,
                                                                            TimerDelay ttl) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  subscription_multicast_address_ = multicast_address;
  subscription_multicast_port_ = multicast_port;
  subscription_ttl_ = ttl;
  state_owner_.GetState()->OnSubscribeEventgroupAck(*this);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnSubscribeEventgroupNack() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_owner_.GetState()->OnSubscribeEventgroupNack(*this);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnTimeout() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_owner_.GetState()->OnTimeout(*this);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnTtlTimeout() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_owner_.GetState()->OnTtlTimeout(*this);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnShutdown() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_owner_.GetState()->OnShutdown(*this);
  is_service_available_ = false;
  subscription_counter_ = 0;
}

bool ServiceDiscoveryClientEventgroupStateMachine::IsServiceAvailable() const { return is_service_available_; }

bool ServiceDiscoveryClientEventgroupStateMachine::IsEventgroupRequested() const { return subscription_counter_ > 0; }

ServiceDiscoveryClientEventgroupStateMachine::TimerDelay
ServiceDiscoveryClientEventgroupStateMachine::GetSubscribeEventgroupAckTimeout() const {
  return kSubscribeEventgroupAckTimeout;
}

ServiceDiscoveryClientEventgroupStateMachine::TimerDelay
ServiceDiscoveryClientEventgroupStateMachine::GetSubscriptionTtl() const {
  return subscription_ttl_;
}

void ServiceDiscoveryClientEventgroupStateMachine::SendSubscribeEventgroup(bool request_initial_events) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context_->SendSubscribeEventgroup(eventgroup_id_, request_initial_events);
}

void ServiceDiscoveryClientEventgroupStateMachine::SendStopSubscribeEventgroup() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context_->SendStopSubscribeEventgroup(eventgroup_id_);
}

void ServiceDiscoveryClientEventgroupStateMachine::ChangeState(
    const ServiceDiscoveryClientEventgroupStateHandle handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const auto ret = state_owner_.TryChangeState(handle);
  if (!ret) {
    throw std::logic_error{"ServiceDiscoveryClientEventgroupStateMachine: illegal state change"};
  }
}

void ServiceDiscoveryClientEventgroupStateMachine::StartTimer(TimerDelay delay) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": delay " << delay.count();
  assert(delay.count() != 0);
  state_timer_.SetOneShot(delay);
  state_timer_.Start();
}

void ServiceDiscoveryClientEventgroupStateMachine::StopTimer() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_timer_.Stop();
}

void ServiceDiscoveryClientEventgroupStateMachine::StartTtlTimer(TimerDelay delay) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": delay " << delay.count();
  if (delay.count() == 0) {
    throw std::logic_error{"ServiceDiscoveryClientStateMachine: illegal timer delay"};
  }
  state_ttl_timer_.SetPeriod(delay);
  state_ttl_timer_.Start();
}

void ServiceDiscoveryClientEventgroupStateMachine::StopTtlTimer() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_ttl_timer_.Stop();
}

void ServiceDiscoveryClientEventgroupStateMachine::OnSubscribed() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context_->OnEventgroupSubscribed(eventgroup_id_);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnUnsubscribed() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context_->OnEventgroupUnsubscribed(eventgroup_id_);
}

void ServiceDiscoveryClientEventgroupStateMachine::OnSubscriptionPending() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context_->OnEventgroupSubscriptionPending(eventgroup_id_);
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
