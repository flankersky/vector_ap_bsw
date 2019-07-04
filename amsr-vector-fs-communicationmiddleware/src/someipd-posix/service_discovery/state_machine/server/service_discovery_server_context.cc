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
/**        \file  service_discovery_server_context.cc
 *        \brief  Service Discovery Server State Machine Context
 *
 *      \details  Context class contains all required interfaces to be called by any state machine state
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_context.h"
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "osabstraction/io/network/address/ip_socket_address.h"
#include "someip-posix-common/someip/message.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerContext::ServiceDiscoveryServerContext(
    ServiceDiscoveryServerStateOwner& state_owner, const std::string& address,
    const configuration::Configuration::ProvidedServiceInstance* config,
    std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance,
    eventgroup::ServiceDiscoveryServerEventManager& event_manager,
    ServiceDiscoveryServerMessageBuilder& message_builder, ServiceDiscoveryMessageSender& message_sender,
    ServiceDiscoveryServerOfferServiceSendQueue& message_send_queue)
    : state_owner_(state_owner),
      service_instance_(service_instance),
      instance_config_(config),
      event_manager_(event_manager),
      message_builder_(message_builder),
      message_sender_(message_sender),
      message_send_queue_(message_send_queue) {
  assert(instance_config_);
  const auto it = std::find_if(instance_config_->port_mappings_.cbegin(), instance_config_->port_mappings_.cend(),
                               [&address](const configuration::Configuration::NetworkEndpointPortMapping& nepmc) {
                                 return (address == nepmc.address_);
                               });
  assert(it != instance_config_->port_mappings_.end());
  port_mapping_config_ = &*it;

  // calculate random value between InitialDelayMin & InitialDelayMax
  if (instance_config_->service_discovery_.initial_delay_min_ns_ ==
      instance_config_->service_discovery_.initial_delay_max_ns_) {
    initial_delay_random_ = std::chrono::nanoseconds(instance_config_->service_discovery_.initial_delay_min_ns_);
  } else {
    std::random_device rd;   // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<decltype(instance_config_->service_discovery_.initial_delay_min_ns_)> dis(
        instance_config_->service_discovery_.initial_delay_min_ns_,
        instance_config_->service_discovery_.initial_delay_max_ns_);
    initial_delay_random_ = std::chrono::nanoseconds(dis(gen));
  }

  // calculate random value between RequestResponseDelayMin & RequestResponseDelayMax
  if (instance_config_->service_discovery_.request_response_delay_min_ns_ ==
      instance_config_->service_discovery_.request_response_delay_max_ns_) {
    request_response_delay_random_ =
        std::chrono::nanoseconds(instance_config_->service_discovery_.request_response_delay_min_ns_);
  } else {
    std::random_device rd;   // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<decltype(instance_config_->service_discovery_.initial_delay_min_ns_)> dis(
        instance_config_->service_discovery_.request_response_delay_min_ns_,
        instance_config_->service_discovery_.request_response_delay_max_ns_);
    request_response_delay_random_ = std::chrono::nanoseconds(dis(gen));
  }
}

/// State Control/Monitor

bool ServiceDiscoveryServerContext::IsNetworkUp() const { return state_owner_.IsNetworkUp(); }

bool ServiceDiscoveryServerContext::IsServiceUp() const { return state_owner_.IsServiceUp(); }

void ServiceDiscoveryServerContext::RequestStateChange(const ServiceDiscoveryServerStateHandle handle) {
  requested_next_state_ = handle;
}

ServiceDiscoveryServerStateHandle ServiceDiscoveryServerContext::GetRequestedNextState() const {
  return requested_next_state_;
}

void ServiceDiscoveryServerContext::StartTimer(vac::timer::Timer::Clock::duration period, bool periodic) {
  state_owner_.StartTimer(period, periodic);
}

void ServiceDiscoveryServerContext::StopTimer() { state_owner_.StopTimer(); }

/// Configuration Info

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetInitialDelayMin() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.initial_delay_min_ns_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetInitialDelayMax() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.initial_delay_max_ns_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetInitialDelayRandom() const {
  return initial_delay_random_;
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetInitialRepetitionBaseDelay() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.initial_repetitions_base_delay_ns_);
}

std::uint32_t ServiceDiscoveryServerContext::GetInitialRepetitionMaxCount() const {
  return static_cast<decltype(instance_config_->service_discovery_.initial_repetitions_max_)>(
      instance_config_->service_discovery_.initial_repetitions_max_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetCyclicOfferServiceDelay() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.cyclic_offer_delay_ns_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetRequestResponseDelayMax() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.request_response_delay_max_ns_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetRequestResponseDelayMin() const {
  return std::chrono::nanoseconds(instance_config_->service_discovery_.request_response_delay_min_ns_);
}

vac::timer::Timer::Clock::duration ServiceDiscoveryServerContext::GetRequestResponseDelayRandom() const {
  return request_response_delay_random_;
}

/// Actions

void ServiceDiscoveryServerContext::StartServiceInstance() { service_instance_->Start(); }

void ServiceDiscoveryServerContext::StopServiceInstance() { service_instance_->Stop(); }

void ServiceDiscoveryServerContext::SendMulticastOfferServiceMessage(vac::timer::Timer::Clock::duration delay) {
  message_send_queue_.AddToMulticastSendQueue(delay);
}

void ServiceDiscoveryServerContext::SendUnicastOfferServiceMessage(const std::string& to_address, std::uint16_t to_port,
                                                                   vac::timer::Timer::Clock::duration delay) {
  message_send_queue_.AddToUnicastSendQueue(to_address, to_port, delay);
}

void ServiceDiscoveryServerContext::SendMulticastStopOfferServiceMessage() {
  auto entries_and_options = message_builder_.MakeStopOfferServiceMessage();
  message_sender_.SendMulticastMessage(entries_and_options.first, entries_and_options.second);
}

void ServiceDiscoveryServerContext::SendUnicastStopOfferServiceMessage(const std::string& to_address,
                                                                       std::uint16_t to_port) {
  auto entries_and_options = message_builder_.MakeStopOfferServiceMessage();
  message_sender_.SendUnicastMessage(to_address, to_port, entries_and_options.first, entries_and_options.second);
}

void ServiceDiscoveryServerContext::SubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                                             const ServiceDiscoveryEntry& entry,
                                                             const ServiceDiscoveryOptionContainer& options) {
  event_manager_.SubscribeEventgroupEntry(from_address, from_port, entry, options);
}

void ServiceDiscoveryServerContext::UnsubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                                               const ServiceDiscoveryEntry& entry,
                                                               const ServiceDiscoveryOptionContainer& options) {
  event_manager_.UnsubscribeEventgroupEntry(from_address, from_port, entry, options);
}

void ServiceDiscoveryServerContext::UnsubscribeAllEventgroupEntries() {
  event_manager_.UnsubscribeAllEventgroupEntries();
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
