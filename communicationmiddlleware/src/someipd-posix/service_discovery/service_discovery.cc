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
/**        \file  service_discovery.cc
 *        \brief  Service discovery
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery.h"

#include <algorithm>
#include <utility>

#include "someipd-posix/service_discovery/service_discovery_endpoint_observer.h"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state_observer.h"
#include "someipd-posix/service_discovery/service_discovery_offer_service_observer.h"
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server.h"

namespace someipd_posix {
namespace service_discovery {

ServiceDiscovery::ServiceDiscovery(const configuration::Configuration* config,
                                   osabstraction::io::ReactorInterface* reactor,
                                   vac::timer::TimerManager* timer_manager,
                                   packet_router::PacketRouterInterface* packet_router)
    : config_(config),
      reactor_(reactor),
      timer_manager_(timer_manager),
      packet_router_(packet_router),
      logger_(ara::log::CreateLogger("ServiceDiscovery", "")),
      conman_(config, reactor, packet_router, timer_manager) {
  CreateSdEndpoints();
  CreateServerServiceInstanceStateMachines();
  CreateClientServiceInstanceStateMachines();
  CreateStaticClientServiceInstances();
  StartServiceInstanceStateMachines();
}

ServiceDiscovery::~ServiceDiscovery() { StopServiceInstanceStateMachines(); }

void ServiceDiscovery::OfferService(std::uint16_t service_id, std::uint16_t instance_id,
                                    std::shared_ptr<packet_router::PacketSink> to) {
  logger_.LogDebug() << "offer service (" << std::hex << service_id << ", " << instance_id << ")" << std::dec;

  auto it = FindOfferedService(service_id, instance_id);
  if (it == offered_services_.end()) {
    offered_services_.push_back({service_id, instance_id});
    packet_router_->AddRequestRoute(service_id, instance_id, to);
    for (auto& observer : offer_service_observers_) {
      observer->OnStartOfferServiceInstance(service_id, instance_id);
    }

    auto sit = FindServerStateMachine(service_id, instance_id);
    if (sit != sd_server_service_instance_state_machines_.end()) {
      (*sit)->OnServiceUp();
    } else {
      logger_.LogError() << " offered service (" << std::hex << service_id << ", " << instance_id << std::dec
                         << ") could not be found in configuration";
    }

    UpdateServiceInstanceEventsSubscriptionState(service_id, instance_id,
                                                 ServiceDiscoveryEventSubscriptionState::kSubscribed);
  } else {
    throw std::invalid_argument("ServiceDiscovery::OfferService: service (" + std::to_string(service_id) + ", " +
                                std::to_string(instance_id) + ") is already offered");
  }
}

void ServiceDiscovery::StopOfferService(std::uint16_t service_id, std::uint16_t instance_id) {
  logger_.LogDebug() << "stop offer service (" << std::hex << service_id << ", " << instance_id << ")" << std::dec;

  auto it = FindOfferedService(service_id, instance_id);
  if (it != offered_services_.end()) {
    offered_services_.erase(it);
    packet_router_->DeleteRequestRoute(service_id, instance_id);
    for (auto& observer : offer_service_observers_) {
      observer->OnStopOfferServiceInstance(service_id, instance_id);
    }
    auto sit = FindServerStateMachine(service_id, instance_id);
    if (sit != sd_server_service_instance_state_machines_.end()) {
      (*sit)->OnServiceDown();
    }
  } else {
    throw std::invalid_argument("ServiceDiscovery::StopOfferService: service (" + std::to_string(service_id) + ", " +
                                std::to_string(instance_id) + ") is not offered");
  }

  UpdateServiceInstanceEventsSubscriptionState(service_id, instance_id,
                                               ServiceDiscoveryEventSubscriptionState::kSubscriptionPending);
}

ServiceDiscovery::OfferedServiceContainer ServiceDiscovery::GetOfferedServices() const {
  OfferedServiceContainer result = offered_services_;
  return result;
}

void ServiceDiscovery::SubscribeEvent(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id,
                                      std::shared_ptr<packet_router::PacketSink> to) {
  packet_router_->AddEventRoute(service_id, instance_id, event_id, to);

  auto it = FindClientStateMachine(service_id, instance_id);
  if (it == sd_client_service_instance_state_machines_.end()) {
    // Local event, subscription is done.
    UpdateEventSubscriptionState(service_id, instance_id, event_id,
                                 ServiceDiscoveryEventSubscriptionState::kSubscribed);
  } else {
    // Remote event
    SubscribeRemoteEventgroup(service_id, instance_id, event_id);
  }

  // For Field Event, an immediate notification is required.
  assert(to.get());
  packet_router_->SendInitialEvent(service_id, instance_id, event_id, *(to.get()));
}

void ServiceDiscovery::UnsubscribeEvent(std::uint16_t service_id, std::uint16_t instance_id, std::uint16_t event_id,
                                        std::shared_ptr<packet_router::PacketSink> to) {
  packet_router_->DeleteEventRoute(service_id, instance_id, event_id, to);

  auto it = FindClientStateMachine(service_id, instance_id);
  if (it == sd_client_service_instance_state_machines_.end()) {
    // Local event, unsubscription is done.
    // No need to send subscription state notification because ara::com knows already about the unsubscription.
  } else {
    // Remote event
    UnsubscribeRemoteEventgroup(service_id, instance_id, event_id);
  }
}

void ServiceDiscovery::RequestService(std::uint16_t service_id, std::uint16_t instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (" << std::hex << service_id << "," << instance_id << std::dec
                     << ")";
  auto sit = FindClientStateMachine(service_id, instance_id);
  if (sit != sd_client_service_instance_state_machines_.end()) {
    (*sit)->OnServiceRequested();
  } else {
    // No client state machine exists for services which are offered only locally
    logger_.LogError() << __func__ << ":" << __LINE__ << ": no state machine found for (" << service_id << ","
                       << instance_id << ")";
  }
}

void ServiceDiscovery::ReleaseService(std::uint16_t service_id, std::uint16_t instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (" << std::hex << service_id << "," << instance_id << std::dec
                     << ")";
  auto sit = FindClientStateMachine(service_id, instance_id);
  if (sit != sd_client_service_instance_state_machines_.end()) {
    (*sit)->OnServiceReleased();
  } else {
    // No client state machine exists for services which are offered only locally
    logger_.LogError() << __func__ << ":" << __LINE__ << ": no state machine found for (" << service_id << ","
                       << instance_id << ")";
  }
}

void ServiceDiscovery::RegisterServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver* observer) {
  offer_service_observers_.push_back(observer);
}

void ServiceDiscovery::UnRegisterServiceDiscoveryOfferServiceObserver(ServiceDiscoveryOfferServiceObserver* observer) {
  auto it = std::find(offer_service_observers_.begin(), offer_service_observers_.end(), observer);
  if (it != offer_service_observers_.end()) {
    offer_service_observers_.erase(it);
  } else {
    logger_.LogError() << __func__ << ": no such observer found.";
  }
}

void ServiceDiscovery::RegisterServiceDiscoveryEventSubscriptionStateObserver(
    ServiceDiscoveryEventSubscriptionStateObserver* observer) {
  event_subscription_observers_.push_back(observer);
}

void ServiceDiscovery::UnRegisterServiceDiscoveryEventSubscriptionStateObserver(
    ServiceDiscoveryEventSubscriptionStateObserver* observer) {
  auto it = std::find(event_subscription_observers_.begin(), event_subscription_observers_.end(), observer);
  if (it != event_subscription_observers_.end()) {
    event_subscription_observers_.erase(it);
  } else {
    logger_.LogError() << __func__ << ": no such observer found.";
  }
}

void ServiceDiscovery::CreateSdEndpoints() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (const auto& nec : config_->GetNetworkEndpoints()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": address " << nec.address_;
    auto address = nec.address_;
    sd_endpoints_.emplace_back(new ServiceDiscoveryEndpoint(
        reactor_, address, nec.service_discovery_.multicast_address_, nec.service_discovery_.port_, nec.mtu_, *this));
  }
}

void ServiceDiscovery::CreateServerServiceInstanceStateMachines() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (const auto& psic : config_->GetProvidedServiceInstances()) {
    for (const auto& pmc : psic.port_mappings_) {
      // For each configured end-point, the same service is provided with a separate state machine.
      auto it = std::find_if(
          sd_endpoints_.begin(), sd_endpoints_.end(),
          [&pmc](std::unique_ptr<ServiceDiscoveryEndpoint>& e) { return pmc.address_ == e->GetUnicastAddress(); });
      assert(it != sd_endpoints_.end());
      auto service_instance = std::make_shared<ServiceDiscoveryServerServiceInstance>(
          psic.service_id_, psic.instance_id_, psic.major_version_, psic.minor_version_, pmc.address_, packet_router_,
          &conman_, config_);
      auto service_instance_state_machine = new state_machine::server::ServiceDiscoveryServer(
          psic.service_id_, psic.instance_id_, psic.major_version_, psic.minor_version_, pmc.address_, timer_manager_,
          it->get(), service_instance, config_);

      sd_service_instance_state_machine_map_.insert({pmc.address_, service_instance_state_machine});
      sd_server_service_instance_state_machines_.emplace_back(service_instance_state_machine);
    }
  }
}

void ServiceDiscovery::CreateClientServiceInstanceStateMachines() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (!config_->GetStaticServiceDiscovery().enable_) {
    for (const auto& rsic : config_->GetRequiredServiceInstances()) {
      // Is a state machine for this required service instance already running ?
      auto state_machine = std::find_if(
          sd_client_service_instance_state_machines_.cbegin(), sd_client_service_instance_state_machines_.cend(),
          [&rsic](const std::unique_ptr<state_machine::client::ServiceDiscoveryClientServiceInstanceStateMachine>& sm) {
            return (rsic.service_id_ == sm->GetServiceId()) && (rsic.instance_id_ == sm->GetInstanceId()) &&
                   (rsic.major_version_ == sm->GetMajorVersion()) && (rsic.minor_version_ == sm->GetMinorVersion());
          });
      // Skip duplicates
      if (state_machine == sd_client_service_instance_state_machines_.cend()) {
        auto it = std::find_if(sd_endpoints_.begin(), sd_endpoints_.end(),
                               [&rsic](std::unique_ptr<ServiceDiscoveryEndpoint>& e) {
                                 return rsic.port_mapping_.address_ == e->GetUnicastAddress();
                               });
        assert(it != sd_endpoints_.end());
        auto service_instance = std::make_shared<ServiceDiscoveryClientServiceInstance>(
            rsic.service_id_, rsic.instance_id_, rsic.major_version_, rsic.minor_version_, this, &conman_, config_);
        auto service_instance_state_machine = new state_machine::client::ServiceDiscoveryClient(
            rsic.service_id_, rsic.instance_id_, rsic.major_version_, rsic.minor_version_, timer_manager_, it->get(),
            service_instance, config_, this);
        sd_service_instance_state_machine_map_.insert({rsic.port_mapping_.address_, service_instance_state_machine});
        sd_client_service_instance_state_machines_.emplace_back(service_instance_state_machine);
      } else {
        logger_.LogError() << __func__ << ":" << __LINE__ << ": duplicate required service instance (" << std::hex
                           << rsic.service_id_ << "," << rsic.instance_id_ << ","
                           << static_cast<std::size_t>(rsic.major_version_) << "," << rsic.minor_version_ << std::dec
                           << ")";
      }
    }
  }
}

void ServiceDiscovery::CreateStaticClientServiceInstances() {
  using cfg = configuration::Configuration;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (config_->GetStaticServiceDiscovery().enable_) {
    logger_.LogInfo() << __func__ << ":" << __LINE__ << ": static SD is enabled";
    for (const auto& rne : config_->GetStaticServiceDiscovery().endpoints_) {
      /* We need to find the local IP address and port because static SD entries define only the address of the remote
       * side */
      for (const auto& rsi : rne.required_service_instances_) {
        /* Find corresponding required service instance */
        const auto& it = std::find_if(
            config_->GetRequiredServiceInstances().cbegin(), config_->GetRequiredServiceInstances().cend(),
            [&rsi](const cfg::RequiredServiceInstance& other) {
              return (rsi.service_id_ == other.service_id_) && (rsi.instance_id_ == other.instance_id_) &&
                     (rsi.major_version_ == other.major_version_) && (rsi.minor_version_ == other.minor_version_);
            });
        if (it != config_->GetRequiredServiceInstances().end()) {
          logger_.LogInfo() << __func__ << ":" << __LINE__ << ": establishing a static required service instance ("
                            << std::hex << rsi.service_id_ << "," << rsi.instance_id_ << ") from " << std::dec
                            << it->port_mapping_.address_ << "," << it->port_mapping_.tcp_port_ << " to "
                            << rne.address_ << "," << rne.tcp_port_ << "," << rne.udp_port_;
          assert((it->port_mapping_.tcp_port_ != cfg::kInvalidPort) ||
                 (it->port_mapping_.udp_port_ != cfg::kInvalidPort));
          auto service_instance = std::make_shared<ServiceDiscoveryClientServiceInstance>(
              rsi.service_id_, rsi.instance_id_, rsi.major_version_, rsi.minor_version_, this, &conman_, config_);
          static_client_service_instances_.emplace_back(service_instance);
          service_instance->Connect(rne.address_, rne.tcp_port_, rne.address_, rne.udp_port_);
        }
      }
    }
  }
}

void ServiceDiscovery::StartServiceInstanceStateMachines() {
  std::for_each(sd_server_service_instance_state_machines_.begin(), sd_server_service_instance_state_machines_.end(),
                [](std::unique_ptr<state_machine::server::ServiceDiscoveryServerServiceInstanceStateMachine>& sism) {
                  sism->OnStart();
                  sism->OnNetworkUp();
                });
  std::for_each(sd_client_service_instance_state_machines_.begin(), sd_client_service_instance_state_machines_.end(),
                [](std::unique_ptr<state_machine::client::ServiceDiscoveryClientServiceInstanceStateMachine>& sism) {
                  sism->OnStart();
                  sism->OnNetworkUp();
                  // TODO(PAASR-1310) Required until background service lookup is implemented
                  sism->OnServiceRequested();
                });
}

void ServiceDiscovery::StopServiceInstanceStateMachines() {
  std::for_each(sd_client_service_instance_state_machines_.begin(), sd_client_service_instance_state_machines_.end(),
                [](std::unique_ptr<state_machine::client::ServiceDiscoveryClientServiceInstanceStateMachine>& sism) {
                  // TODO(PAASR-1310) Required until background service lookup is implemented
                  sism->OnServiceReleased();
                  sism->OnNetworkDown();
                  sism->OnStop();
                });
  std::for_each(sd_server_service_instance_state_machines_.begin(), sd_server_service_instance_state_machines_.end(),
                [](std::unique_ptr<state_machine::server::ServiceDiscoveryServerServiceInstanceStateMachine>& sism) {
                  sism->OnNetworkDown();
                  sism->OnStop();
                });
}

void ServiceDiscovery::SubscribeRemoteEventgroup(std::uint16_t service_id, std::uint16_t instance_id,
                                                 std::uint16_t event_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << service_id << ", " << instance_id << ", "
                     << event_id << ")" << std::dec;

  auto it = FindClientStateMachine(service_id, instance_id);
  if (it == sd_client_service_instance_state_machines_.end()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": no service instance found";
    return;
  }
  auto eventgroup = FindEventgroup(service_id, event_id);
  if (eventgroup.first) {
    auto eventgroup_id = eventgroup.second;
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;

    // Notify current state directly
    auto eg_subscription_state = it->get()->GetEventgroupSubscriptionState(eventgroup_id);
    if ((eg_subscription_state == ServiceDiscoveryEventSubscriptionState::kNotSubscribed) &&
        (it->get()->GetEventgroupSubscribersCount(eventgroup_id) > 0)) {
      // In this case, the server stopped offering the service, and once it re-offers it, the events will be
      // re-subscribed.
      eg_subscription_state = ServiceDiscoveryEventSubscriptionState::kSubscriptionPending;
    }
    UpdateEventSubscriptionState(service_id, instance_id, event_id, eg_subscription_state);

    // Note that if more than one client have subscribed to the same event, they will all get again notified here.
    // ara::com must then only notify the proxy that has the subscription state changed.

    (*it)->OnSubscribeEventgroup(eventgroup_id);
  } else {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": Event " << std::hex << event_id << std::dec
                       << " is not located in any eventgroup !";
  }
}

void ServiceDiscovery::UnsubscribeRemoteEventgroup(std::uint16_t service_id, std::uint16_t instance_id,
                                                   std::uint16_t event_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << service_id << ", " << instance_id << ", "
                     << event_id << ")" << std::dec;

  auto it = FindClientStateMachine(service_id, instance_id);
  if (it == sd_client_service_instance_state_machines_.end()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": no service instance found";
    return;
  }
  auto eventgroup = FindEventgroup(service_id, event_id);
  if (eventgroup.first) {
    auto eventgroup_id = eventgroup.second;
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
    (*it)->OnStopSubscribeEventgroup(eventgroup_id);
  } else {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": Event " << std::hex << event_id << std::dec
                       << " is not located in any eventgroup !";
  }

  // No need to send subscription state notification because ara::com knows already about the unsubscription.
}

void ServiceDiscovery::OnUnicastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                                          const someip_posix_common::someip::IpAddress& from_address,
                                          someip_posix_common::someip::Port from_port,
                                          const ServiceDiscoveryEntryContainer& entries,
                                          const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " my address " << my_address << " from address " << from_address
                     << "," << from_port;
  auto range = sd_service_instance_state_machine_map_.equal_range(my_address);
  for (auto it = range.first; it != range.second; ++it) {
    it->second->OnUnicastMessage(from_address, from_port, entries, options);
  }

  // For Subscribe Eventgroup Entries, NACK is required in case no target state machine is found.
  std::for_each(entries.begin(), entries.end(), [this, &my_address, &from_address, from_port,
                                                 &options](const ServiceDiscoveryEntry& entry) {
    if ((entry.type_ == someip_posix_common::someip::SomeIpSdEntryType::kSubscribeEventgroup) && (entry.ttl_ > 0U)) {
      auto it = FindStateMachine(my_address, entry.service_id_, entry.instance_id_, entry.major_version_);
      if (it == sd_service_instance_state_machine_map_.end()) {
        // TODO(PAASR-3054) Send NACK on incorrect subscribe eventgroup entry
      }
    }
  });
}

void ServiceDiscovery::OnMulticastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                                            const someip_posix_common::someip::IpAddress& from_address,
                                            someip_posix_common::someip::Port from_port,
                                            const ServiceDiscoveryEntryContainer& entries,
                                            const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " my address " << my_address << " from address " << from_address
                     << "," << from_port;
  auto range = sd_service_instance_state_machine_map_.equal_range(my_address);
  for (auto it = range.first; it != range.second; ++it) {
    it->second->OnMulticastMessage(from_address, from_port, entries, options);
  }
}

void ServiceDiscovery::OnRebootDetected(const someip_posix_common::someip::IpAddress& my_address,
                                        const someip_posix_common::someip::IpAddress& from_address,
                                        someip_posix_common::someip::Port from_port) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " from address " << from_address;
  auto range = sd_service_instance_state_machine_map_.equal_range(my_address);
  for (auto it = range.first; it != range.second; ++it) {
    it->second->OnRebootDetected(from_address, from_port);
  }
}

ServiceDiscovery::OfferedServiceContainer::iterator ServiceDiscovery::FindOfferedService(std::uint16_t service_id,
                                                                                         std::uint16_t instance_id) {
  return std::find_if(offered_services_.begin(), offered_services_.end(),
                      [service_id, instance_id](const OfferedService& other) {
                        return (service_id == other.service_id_) && (instance_id == other.instance_id_);
                      });
}

ServiceDiscovery::ServiceDiscoveryServiceInstanceStateMachineContainer::iterator ServiceDiscovery::FindStateMachine(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::ServiceId service_id,
    someip_posix_common::someip::InstanceId instance_id, someip_posix_common::someip::MajorVersion major_version) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " (" << std::hex << service_id << ", " << instance_id << std::dec
                     << ")";
  auto range = sd_service_instance_state_machine_map_.equal_range(address);
  auto it = find_if(
      range.first, range.second, [service_id, instance_id, major_version](
                                     const ServiceDiscoveryServiceInstanceStateMachineContainer::value_type sism) {
        return (sism.second->GetServiceId() == service_id) && (sism.second->GetInstanceId() == instance_id) &&
               (sism.second->GetMajorVersion() == major_version);
      });

  return it;
}

ServiceDiscovery::ServiceDiscoveryServiceInstanceClientStateMachineContainer::iterator
ServiceDiscovery::FindClientStateMachine(std::uint16_t service_id, std::uint16_t instance_id) {
  return std::find_if(
      sd_client_service_instance_state_machines_.begin(), sd_client_service_instance_state_machines_.end(),
      [service_id, instance_id](
          const std::unique_ptr<state_machine::client::ServiceDiscoveryClientServiceInstanceStateMachine>& sism) {
        return (sism->GetServiceId() == service_id) && (sism->GetInstanceId() == instance_id);
      });
}

ServiceDiscovery::ServiceDiscoveryServiceInstanceServerStateMachineContainer::iterator
ServiceDiscovery::FindServerStateMachine(std::uint16_t service_id, std::uint16_t instance_id) {
  return std::find_if(
      sd_server_service_instance_state_machines_.begin(), sd_server_service_instance_state_machines_.end(),
      [service_id, instance_id](
          const std::unique_ptr<state_machine::server::ServiceDiscoveryServerServiceInstanceStateMachine>& sism) {
        return (sism->GetServiceId() == service_id) && (sism->GetInstanceId() == instance_id);
      });
}

void ServiceDiscovery::OnEventgroupSubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                                              const std::uint16_t eventgroup_id) {
  UpdateEventgroupSubscriptionState(service_id, instance_id, eventgroup_id,
                                    ServiceDiscoveryEventSubscriptionState::kSubscribed);
}

void ServiceDiscovery::OnEventgroupUnsubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                                                const std::uint16_t eventgroup_id) {
  // Complete eventgroup is unsubscribed, that might be due to server shutdown, we must notify the subscription state
  // change.
  auto it = FindClientStateMachine(service_id, instance_id);
  if (it == sd_client_service_instance_state_machines_.end()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": no service instance found";
    return;
  }
  if (it->get()->GetEventgroupSubscribersCount(eventgroup_id) > 0) {
    // In this case, the server stopped offering the service, and once it re-offers it, the events will be
    // re-subscribed.
    UpdateEventgroupSubscriptionState(service_id, instance_id, eventgroup_id,
                                      ServiceDiscoveryEventSubscriptionState::kSubscriptionPending);
  } else {
    UpdateEventgroupSubscriptionState(service_id, instance_id, eventgroup_id,
                                      ServiceDiscoveryEventSubscriptionState::kNotSubscribed);
  }
}

void ServiceDiscovery::OnEventgroupSubscriptionPending(const std::uint16_t service_id, const std::uint16_t instance_id,
                                                       const std::uint16_t eventgroup_id) {
  UpdateEventgroupSubscriptionState(service_id, instance_id, eventgroup_id,
                                    ServiceDiscoveryEventSubscriptionState::kSubscriptionPending);
}

void ServiceDiscovery::UpdateEventSubscriptionState(const std::uint16_t service_id, const std::uint16_t instance_id,
                                                    const std::uint16_t event_id,
                                                    const ServiceDiscoveryEventSubscriptionState new_state) {
  for (auto& observer : event_subscription_observers_) {
    observer->OnEventSubscriptionStateChange(service_id, instance_id, event_id, new_state);
  }
}

void ServiceDiscovery::UpdateEventgroupSubscriptionState(const std::uint16_t service_id,
                                                         const std::uint16_t instance_id,
                                                         const std::uint16_t eventgroup_id,
                                                         const ServiceDiscoveryEventSubscriptionState new_state) {
  auto eventgroup = config_->GetEventgroup(service_id, eventgroup_id);
  if (eventgroup) {
    for (const auto event_id : eventgroup->events_) {
      UpdateEventSubscriptionState(service_id, instance_id, event_id, new_state);
    }
  }
}

void ServiceDiscovery::UpdateServiceInstanceEventsSubscriptionState(
    const std::uint16_t service_id, const std::uint16_t instance_id,
    const ServiceDiscoveryEventSubscriptionState new_state) {
  using Event = configuration::ConfigurationTypesAndDefs::Event;
  auto service = config_->GetService(service_id);
  assert(service);
  std::for_each(service->events_.begin(), service->events_.end(),
                [this, service_id, instance_id, new_state](const Event& event) {
                  UpdateEventSubscriptionState(service_id, instance_id, event.id_, new_state);
                });
}

std::pair<bool, someip_posix_common::someip::EventgroupId> ServiceDiscovery::FindEventgroup(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::EventId event_id) {
  auto eventgroup_ids = config_->EventToEventgroups(service_id, event_id);
  if (!eventgroup_ids.empty()) {
    return {true, eventgroup_ids[0]};
  }

  return {false, 0U};
}
}  // namespace service_discovery
}  // namespace someipd_posix
