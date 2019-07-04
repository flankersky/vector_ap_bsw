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
/**        \file  service_discovery_server_service_instance.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include <cassert>
#include <utility>
#include "someipd-posix/service_discovery/connection_manager/service_discovery_connection_manager.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {

ServiceDiscoveryServerServiceInstance::ServiceDiscoveryServerServiceInstance(
    std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version, std::uint32_t minor_version,
    const std::string& address, packet_router::PacketRouterInterface* packet_router,
    connection_manager::ServiceDiscoveryConnectionManager* conman, const configuration::Configuration* config)
    : packet_router_(packet_router),
      conman_(conman),
      config_(config),
      instance_config_(config->GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version)),
      logger_(ara::log::CreateLogger("ServiceDiscoveryServerServiceInstance", "")),
      tcp_receiver_(nullptr),
      udp_receiver_(nullptr),
      udp_mcast_sender_(nullptr) {
  using cfg = configuration::Configuration;
  assert(instance_config_);
  const auto it =
      std::find_if(instance_config_->port_mappings_.cbegin(), instance_config_->port_mappings_.cend(),
                   [&address](const cfg::NetworkEndpointPortMapping& nepmc) { return (address == nepmc.address_); });
  assert(it != instance_config_->port_mappings_.end());
  port_mapping_config_ = &*it;
}

void ServiceDiscoveryServerServiceInstance::Start() {
  using cfg = configuration::Configuration;
  namespace conman = connection_manager;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (port_mapping_config_->tcp_port_ != cfg::kInvalidPort) {
    tcp_receiver_ = conman_->GetTcpReceiver(port_mapping_config_->address_, port_mapping_config_->tcp_port_);
    tcp_receiver_->RegisterProvidedServiceInstance(instance_config_->service_id_, instance_config_->instance_id_);
  }
  if (port_mapping_config_->udp_port_ != cfg::kInvalidPort) {
    udp_receiver_ = conman_->GetUdpUnicastReceiver(port_mapping_config_->address_, port_mapping_config_->udp_port_);
    udp_receiver_->RegisterProvidedServiceInstance(instance_config_->service_id_, instance_config_->instance_id_);
  }
  if (port_mapping_config_->event_multicast_port_ != cfg::kInvalidPort) {
    udp_mcast_sender_ = conman_->GetUdpSender(port_mapping_config_->address_, port_mapping_config_->udp_port_,
                                              port_mapping_config_->event_multicast_address_,
                                              port_mapping_config_->event_multicast_port_);
  }
}

void ServiceDiscoveryServerServiceInstance::Stop() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  tcp_receiver_.reset(nullptr);
  udp_receiver_.reset(nullptr);
  udp_mcast_sender_.reset(nullptr);
  tcp_eventgroup_subscriptions_.clear();
  udp_eventgroup_subscriptions_.clear();
}

void ServiceDiscoveryServerServiceInstance::SubscribeEventgroup(const std::string& tcp_address, std::uint16_t tcp_port,
                                                                const std::string& udp_address, std::uint16_t udp_port,
                                                                std::uint16_t eventgroup_id) {
  namespace conman = connection_manager;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": tcp " << tcp_address << "," << tcp_port << " udp "
                     << udp_address << "," << udp_port << " eventgroup id " << std::hex << eventgroup_id << std::dec;

  connection_manager::ServiceDiscoveryTcpSenderPtr tcp_sender{nullptr};
  connection_manager::ServiceDiscoveryUdpSenderPtr udp_sender{nullptr};
  if (tcp_port != conman::kInvalidPort) {
    tcp_sender =
        conman_->GetTcpSender(port_mapping_config_->address_, port_mapping_config_->tcp_port_, tcp_address, tcp_port);
    tcp_sender->SetConnectionStateChangeHandler(this);
  }
  if (udp_port != conman::kInvalidPort) {
    udp_sender =
        conman_->GetUdpSender(port_mapping_config_->address_, port_mapping_config_->udp_port_, udp_address, udp_port);
  }
  assert(tcp_sender || udp_sender);

  auto subscriptions =
      tcp_eventgroup_subscriptions_[eventgroup_id].size() + udp_eventgroup_subscriptions_[eventgroup_id].size();
  if (subscriptions == 0) {
    packet_router_->AddEventgroupRoute(instance_config_->service_id_, instance_config_->instance_id_, eventgroup_id,
                                       shared_from_this());

    // Send initial field event
    packet_router_->SendInitialEvents(instance_config_->service_id_, instance_config_->instance_id_, eventgroup_id,
                                      tcp_sender.get(), udp_sender.get());
  }
  if (tcp_sender) {
    tcp_eventgroup_subscriptions_[eventgroup_id].emplace_back(std::move(tcp_sender));
  }
  if (udp_sender) {
    udp_eventgroup_subscriptions_[eventgroup_id].emplace_back(std::move(udp_sender));
  }
}

void ServiceDiscoveryServerServiceInstance::UnsubscribeEventgroup(const std::string& tcp_address,
                                                                  std::uint16_t tcp_port,
                                                                  const std::string& udp_address,
                                                                  std::uint16_t udp_port, std::uint16_t eventgroup_id) {
  namespace conman = connection_manager;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": tcp " << tcp_address << "," << tcp_port << " udp "
                     << udp_address << "," << udp_port << " eventgroup id " << std::hex << eventgroup_id << std::dec;
  if (tcp_port != conman::kInvalidPort) {
    auto it = std::find_if(tcp_eventgroup_subscriptions_[eventgroup_id].begin(),
                           tcp_eventgroup_subscriptions_[eventgroup_id].end(),
                           [&tcp_address, tcp_port](const conman::ServiceDiscoveryTcpSenderPtr& s) {
                             return s->GetRemoteAddress() == tcp_address && s->GetRemotePort() == tcp_port;
                           });
    assert(it != tcp_eventgroup_subscriptions_[eventgroup_id].end());
    tcp_eventgroup_subscriptions_[eventgroup_id].erase(it);
  }
  if (udp_port != conman::kInvalidPort) {
    auto it = std::find_if(udp_eventgroup_subscriptions_[eventgroup_id].begin(),
                           udp_eventgroup_subscriptions_[eventgroup_id].end(),
                           [&udp_address, udp_port](const conman::ServiceDiscoveryUdpSenderPtr& s) {
                             return s->GetRemoteAddress() == udp_address && s->GetRemotePort() == udp_port;
                           });
    assert(it != udp_eventgroup_subscriptions_[eventgroup_id].end());
    udp_eventgroup_subscriptions_[eventgroup_id].erase(it);
  }
  auto subscriptions =
      tcp_eventgroup_subscriptions_[eventgroup_id].size() + udp_eventgroup_subscriptions_[eventgroup_id].size();
  if (subscriptions == 0) {
    packet_router_->DeleteEventgroupRoute(instance_config_->service_id_, instance_config_->instance_id_, eventgroup_id,
                                          shared_from_this());
  }
}

bool ServiceDiscoveryServerServiceInstance::HasTcpConnection(const std::string& address, std::uint16_t port) const {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": " << address << "," << port;
  return conman_->HasTcpPassiveConnection(port_mapping_config_->address_, port_mapping_config_->tcp_port_, address,
                                          port);
}

void ServiceDiscoveryServerServiceInstance::Forward(std::uint16_t instance_id, packet_router::Packet packet) {
  namespace someip = someip_posix_common::someip;
  using cfg = configuration::Configuration;
  namespace conman = connection_manager;
  auto header = packet->GetHeader();
  const auto event_id = header.method_id_;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << instance_config_->service_id_ << ","
                     << instance_config_->instance_id_ << "," << event_id << std::dec;
  auto ev = config_->GetEvent(instance_config_->service_id_, event_id);
  assert(ev);
  if (ev->proto_ == cfg::Protocol::kTCP) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": sending via TCP";
    for (auto egid : config_->EventToEventgroups(instance_config_->service_id_, event_id)) {
      std::for_each(
          tcp_eventgroup_subscriptions_[egid].begin(), tcp_eventgroup_subscriptions_[egid].end(),
          [instance_id, packet](conman::ServiceDiscoveryTcpSenderPtr& s) { s->Forward(instance_id, packet); });
    }
  } else {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": sending via UDP";
    for (auto egid : config_->EventToEventgroups(instance_config_->service_id_, event_id)) {
      if (udp_mcast_sender_) {
        auto eg = config_->GetProvidedServiceInstanceSdEventgroup(
            instance_config_->service_id_, instance_config_->instance_id_, instance_config_->major_version_,
            instance_config_->minor_version_, egid);
        assert(eg);
        logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup id " << egid << " threshold "
                           << eg->event_multicast_threshold_ << " subscribers "
                           << udp_eventgroup_subscriptions_[egid].size();
        /* TPS_MANI_03018 */
        if ((eg->event_multicast_threshold_ == cfg::kEventMulticastThresholdUseOnlyUnicast) ||
            (eg->event_multicast_threshold_ > udp_eventgroup_subscriptions_[egid].size())) {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": sending via unicast";
          std::for_each(
              udp_eventgroup_subscriptions_[egid].begin(), udp_eventgroup_subscriptions_[egid].end(),
              [instance_id, packet](conman::ServiceDiscoveryUdpSenderPtr& s) { s->Forward(instance_id, packet); });
        } else {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": sending via multicast";
          udp_mcast_sender_->Forward(instance_id, packet);
        }
      } else {
        logger_.LogDebug() << __func__ << ":" << __LINE__ << ": sending via unicast";
        std::for_each(
            udp_eventgroup_subscriptions_[egid].begin(), udp_eventgroup_subscriptions_[egid].end(),
            [instance_id, packet](conman::ServiceDiscoveryUdpSenderPtr& s) { s->Forward(instance_id, packet); });
      }
    }
  }
}

void ServiceDiscoveryServerServiceInstance::OnConnect(connection_manager::ServiceDiscoveryTcpSender* sender) {
  (void)sender;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryServerServiceInstance::OnDisconnect(connection_manager::ServiceDiscoveryTcpSender* sender) {
  (void)sender;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

}  // namespace service_discovery
}  // namespace someipd_posix
