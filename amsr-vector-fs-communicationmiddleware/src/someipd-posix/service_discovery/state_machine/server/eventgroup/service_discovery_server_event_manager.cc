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
/**        \file  service_discovery_server_event_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/eventgroup/service_discovery_server_event_manager.h"
#include <cassert>
#include <memory>
#include <string>
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/configuration/configuration.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {
namespace eventgroup {

ServiceDiscoveryServerEventManager::ServiceDiscoveryServerEventManager(
    vac::timer::TimerManager* timer_manager, const std::string& address,
    std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance, const configuration::Configuration* config,
    const configuration::Configuration::ProvidedServiceInstance* instance_config,
    ServiceDiscoveryServerMessageBuilder& message_builder, ServiceDiscoveryMessageSender& message_sender)
    : service_instance_(service_instance),
      timer_manager_(timer_manager),
      config_(config),
      instance_config_(instance_config),
      message_builder_(message_builder),
      message_sender_(message_sender),
      logger_(ara::log::CreateLogger("EventManager", "")) {
  assert(instance_config_);
  const auto it = std::find_if(instance_config_->port_mappings_.cbegin(), instance_config_->port_mappings_.cend(),
                               [&address](const configuration::Configuration::NetworkEndpointPortMapping& nepmc) {
                                 return (address == nepmc.address_);
                               });
  assert(it != instance_config_->port_mappings_.end());
  port_mapping_config_ = &*it;
}

ServiceDiscoveryServerEventManager::~ServiceDiscoveryServerEventManager() { UnsubscribeAllEventgroupEntries(); }

void ServiceDiscoveryServerEventManager::SubscribeEventgroupEntry(const std::string& from_address,
                                                                  std::uint16_t from_port,
                                                                  const ServiceDiscoveryEntry& entry,
                                                                  const ServiceDiscoveryOptionContainer& options) {
  assert(!options.empty());
  auto it = FindEventgroupSubscription(entry, options);
  if (it == eventgroup_subscriptions_.end()) {
    NewEventgroupSubscription(from_address, from_port, entry, options);
  } else {
    RenewEventgroupSubscription(it->get(), from_address, from_port, entry);
  }
}

void ServiceDiscoveryServerEventManager::UnsubscribeEventgroupEntry(const std::string& from_address,
                                                                    std::uint16_t from_port,
                                                                    const ServiceDiscoveryEntry& entry,
                                                                    const ServiceDiscoveryOptionContainer& options) {
  CancelEventgroupSubscription(from_address, from_port, entry, options);
}

void ServiceDiscoveryServerEventManager::UnsubscribeAllEventgroupEntries() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  /* Stop all eventgroup subscriptions */
  while (!eventgroup_subscriptions_.empty()) {
    auto it = eventgroup_subscriptions_.begin();
    const auto egs = it->get();
    service_instance_->UnsubscribeEventgroup(egs->tcp_address_, egs->tcp_port_, egs->udp_address_, egs->udp_port_,
                                             egs->eventgroup_id_);
    eventgroup_subscriptions_.erase(it);
  }
}

void ServiceDiscoveryServerEventManager::OnSubscribeEventgroupExpired(EventgroupSubscription* egs) {
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  auto it = std::find_if(eventgroup_subscriptions_.begin(), eventgroup_subscriptions_.end(),
                         [egs](const std::unique_ptr<EventgroupSubscription>& other) { return (egs == other.get()); });
  assert(it != eventgroup_subscriptions_.end());
  service_instance_->UnsubscribeEventgroup(egs->tcp_address_, egs->tcp_port_, egs->udp_address_, egs->udp_port_,
                                           egs->eventgroup_id_);
  eventgroup_subscriptions_.erase(it);
}

void ServiceDiscoveryServerEventManager::NewEventgroupSubscription(const std::string& from_address,
                                                                   std::uint16_t from_port,
                                                                   const ServiceDiscoveryEntry& entry,
                                                                   const ServiceDiscoveryOptionContainer& options) {
  using cfg = someipd_posix::configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  auto egc = config_->GetEventgroup(instance_config_->service_id_, entry.eventgroup_id_);
  assert(egc != nullptr);
  assert(options.size() >= 1 && options.size() <= 2);
  std::size_t nack{0};
  const ServiceDiscoveryOption* tcp_option{nullptr};
  const ServiceDiscoveryOption* udp_option{nullptr};
  std::string tcp_address = "";
  std::uint16_t tcp_port = cfg::kInvalidPort;
  std::string udp_address = "";
  std::uint16_t udp_port = cfg::kInvalidPort;
  for (auto it = options.cbegin(); it != options.cend(); ++it) {
    switch (it->proto_) {
      case someip::SomeIpSdEndpointOptionProto::kTCP:
        tcp_option = &*it;
        tcp_address = tcp_option->address_;
        tcp_port = tcp_option->port_;
        break;
      case someip::SomeIpSdEndpointOptionProto::kUDP:
        udp_option = &*it;
        udp_address = udp_option->address_;
        udp_port = udp_option->port_;
        break;
      default:
        break;
    }
  }
  if (port_mapping_config_->tcp_port_ != cfg::kInvalidPort) {
    if (!tcp_option || !service_instance_->HasTcpConnection(tcp_option->address_, tcp_option->port_)) {
      logger_.LogDebug() << __func__ << ":" << __LINE__;
      ++nack;
    }
  }
  if (port_mapping_config_->udp_port_ != cfg::kInvalidPort && !udp_option) {
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    ++nack;
  }
  if (nack) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": nacking new subscription";
    auto entries_and_options =
        message_builder_.MakeSubscribeEventgroupNackMessage(entry.eventgroup_id_, entry.counter_);
    message_sender_.SendUnicastMessage(from_address, from_port, entries_and_options.first, entries_and_options.second);
    return;
  }
  service_instance_->SubscribeEventgroup(tcp_address, tcp_port, udp_address, udp_port, entry.eventgroup_id_);
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": acking new subscription";
  eventgroup_subscriptions_.emplace_back(new EventgroupSubscription{timer_manager_, tcp_address, tcp_port, udp_address,
                                                                    udp_port, entry.eventgroup_id_, this});
  const auto& egs = eventgroup_subscriptions_.back();
  auto entries_and_options =
      message_builder_.MakeSubscribeEventgroupAckMessage(entry.eventgroup_id_, entry.ttl_, entry.counter_);
  message_sender_.SendUnicastMessage(from_address, from_port, entries_and_options.first, entries_and_options.second);
  egs->ttl_timeout_timer_.SetOneShot(vac::timer::Timer::Clock::now() + std::chrono::seconds(entry.ttl_));
}

void ServiceDiscoveryServerEventManager::RenewEventgroupSubscription(EventgroupSubscription* egs,
                                                                     const std::string& from_address,
                                                                     std::uint16_t from_port,
                                                                     const ServiceDiscoveryEntry& entry) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  auto entries_and_options =
      message_builder_.MakeSubscribeEventgroupAckMessage(entry.eventgroup_id_, entry.ttl_, entry.counter_);
  message_sender_.SendUnicastMessage(from_address, from_port, entries_and_options.first, entries_and_options.second);
  egs->ResetTtlTimeoutTimer(entry.ttl_);
}

void ServiceDiscoveryServerEventManager::CancelEventgroupSubscription(const std::string& from_address,
                                                                      std::uint16_t from_port,
                                                                      const ServiceDiscoveryEntry& entry,
                                                                      const ServiceDiscoveryOptionContainer& options) {
  (void)from_address;
  (void)from_port;
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  auto it = FindEventgroupSubscription(entry, options);
  if (it == eventgroup_subscriptions_.end()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": unknown subscription";
    return;
  }
  const auto egs = it->get();
  egs->ttl_timeout_timer_.Stop();
  service_instance_->UnsubscribeEventgroup(egs->tcp_address_, egs->tcp_port_, egs->udp_address_, egs->udp_port_,
                                           egs->eventgroup_id_);
  eventgroup_subscriptions_.erase(it);
}

EventgroupSubscriptionContainer::iterator ServiceDiscoveryServerEventManager::FindEventgroupSubscription(
    const ServiceDiscoveryEntry& entry, const ServiceDiscoveryOptionContainer& options) {
  using cfg = configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  const ServiceDiscoveryOption* tcp_option{nullptr};
  const ServiceDiscoveryOption* udp_option{nullptr};
  for (auto it = options.cbegin(); it != options.cend(); ++it) {
    switch (it->proto_) {
      case someip::SomeIpSdEndpointOptionProto::kTCP:
        tcp_option = &*it;
        break;
      case someip::SomeIpSdEndpointOptionProto::kUDP:
        udp_option = &*it;
        break;
      default:
        break;
    }
  }
  std::string tcp_address = "";
  std::uint16_t tcp_port = cfg::kInvalidPort;
  if (tcp_option) {
    tcp_address = tcp_option->address_;
    tcp_port = tcp_option->port_;
  }
  std::string udp_address = "";
  std::uint16_t udp_port = cfg::kInvalidPort;
  if (udp_option) {
    udp_address = udp_option->address_;
    udp_port = udp_option->port_;
  }
  auto it = std::find_if(
      eventgroup_subscriptions_.begin(), eventgroup_subscriptions_.end(),
      [&tcp_address, tcp_port, &udp_address, udp_port, &entry](const std::unique_ptr<EventgroupSubscription>& egs) {
        return (egs->tcp_address_ == tcp_address && egs->tcp_port_ == tcp_port && egs->udp_address_ == udp_address &&
                egs->udp_port_ == udp_port && egs->eventgroup_id_ == entry.eventgroup_id_);
      });
  return it;
}

void ServiceDiscoveryServerEventManager::UnsubscribeAllEventgroupsOnReboot(
    const someip_posix_common::someip::IpAddress& from_address, someip_posix_common::someip::Port from_port) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  std::for_each(eventgroup_subscriptions_.cbegin(), eventgroup_subscriptions_.cend(),
                [&from_address, &from_port, this](const std::unique_ptr<EventgroupSubscription>& egs) {
                  if ((egs->tcp_address_ == from_address) || (egs->udp_address_ == from_address)) {
                    service_instance_->UnsubscribeEventgroup(egs->tcp_address_, egs->tcp_port_, egs->udp_address_,
                                                             egs->udp_port_, egs->eventgroup_id_);

                    logger_.LogDebug() << __func__ << ":"
                                       << "UnsubscribeEventgroup on reboot detection for address :" << from_address;
                  }
                });

  /* erase all event group subscriptions sent from from_address*/
  eventgroup_subscriptions_.erase(
      std::remove_if(eventgroup_subscriptions_.begin(), eventgroup_subscriptions_.end(),
                     [&from_address, &from_port](const std::unique_ptr<EventgroupSubscription>& egs) {
                       return ((egs->tcp_address_ == from_address) || (egs->udp_address_ == from_address));
                     }),
      eventgroup_subscriptions_.end());
}

}  // namespace eventgroup
}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
