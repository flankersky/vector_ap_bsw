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
/**        \file  service_discovery_client_service_instance.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_client_service_instance.h"
#include <cassert>
#include <utility>
#include "someipd-posix/service_discovery/service_discovery.h"

namespace someipd_posix {
namespace service_discovery {

ServiceDiscoveryClientServiceInstance::ServiceDiscoveryClientServiceInstance(
    std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version, std::uint32_t minor_version,
    ServiceDiscovery* service_discovery, connection_manager::ServiceDiscoveryConnectionManager* conman,
    const configuration::Configuration* config)
    : service_discovery_(service_discovery),
      conman_(conman),
      config_(config),
      instance_config_(config->GetRequiredServiceInstance(service_id, instance_id, major_version, minor_version)),
      logger_(ara::log::CreateLogger("ServiceDiscoveryClientServiceInstance", "")),
      tcp_address_(connection_manager::kInvalidAddress),
      tcp_port_(connection_manager::kInvalidPort),
      udp_address_(connection_manager::kInvalidAddress),
      udp_port_(connection_manager::kInvalidPort),
      tcp_sender_(nullptr),
      udp_sender_(nullptr),
      udp_receiver_(nullptr),
      eventgroup_subscriptions_(0),
      is_offered_(false) {}

void ServiceDiscoveryClientServiceInstance::Connect(const std::string& tcp_address, std::uint16_t tcp_port,
                                                    const std::string& udp_address, std::uint16_t udp_port) {
  namespace conman = connection_manager;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": tcp " << tcp_address << "," << tcp_port << " udp "
                     << udp_address << "," << udp_port;
  tcp_address_ = tcp_address;
  tcp_port_ = tcp_port;
  udp_address_ = udp_address;
  udp_port_ = udp_port;
  if (tcp_port != conman::kInvalidPort) {
    assert(instance_config_->port_mapping_.tcp_port_ != configuration::Configuration::kInvalidPort);
    tcp_sender_ = conman_->GetTcpSender(instance_config_->port_mapping_.address_,
                                        instance_config_->port_mapping_.tcp_port_, tcp_address, tcp_port);
    tcp_sender_->SetConnectionStateChangeHandler(this);
    tcp_sender_->RegisterRequiredServiceInstance(instance_config_->service_id_, instance_config_->instance_id_);
  }
  if (udp_port != conman::kInvalidPort) {
    assert(instance_config_->port_mapping_.udp_port_ != configuration::Configuration::kInvalidPort);
    udp_sender_ = conman_->GetUdpSender(instance_config_->port_mapping_.address_,
                                        instance_config_->port_mapping_.udp_port_, udp_address, udp_port);
    udp_sender_->RegisterRequiredServiceInstance(instance_config_->service_id_, instance_config_->instance_id_);
  }
  if (tcp_port == conman::kInvalidPort) {
    service_discovery_->OfferService(instance_config_->service_id_, instance_config_->instance_id_, shared_from_this());
    is_offered_ = true;
  }
}

void ServiceDiscoveryClientServiceInstance::Disconnect() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (is_offered_) {
    service_discovery_->StopOfferService(instance_config_->service_id_, instance_config_->instance_id_);
    is_offered_ = false;
  }
  tcp_sender_.reset(nullptr);
  udp_sender_.reset(nullptr);
  udp_receiver_.reset(nullptr);
  eventgroup_subscriptions_ = 0;
}

void ServiceDiscoveryClientServiceInstance::StartListenForMulticastEventgroup(const std::string& address,
                                                                              std::uint16_t port,
                                                                              std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": address " << address << "," << std::to_string(port)
                     << " eventgroup id " << std::hex << eventgroup_id << std::dec;
  if (++eventgroup_subscriptions_ == 1) {
    assert(!udp_receiver_);
    udp_receiver_ = conman_->GetUdpMulticastReceiver(instance_config_->port_mapping_.address_, address, port);
    udp_receiver_->RegisterRequiredServiceInstance(instance_config_->service_id_, instance_config_->instance_id_);
  }
}

void ServiceDiscoveryClientServiceInstance::StopListenForMulticastEventgroup(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup id " << std::hex << eventgroup_id << std::dec;

  // Bug logged for assertion failure. PAASR-2997
  if (eventgroup_subscriptions_ > 0) {
    if (--eventgroup_subscriptions_ == 0) {
      udp_receiver_.reset(nullptr);
    }
  }
}

void ServiceDiscoveryClientServiceInstance::Forward(std::uint16_t instance_id, packet_router::Packet packet) {
  namespace someip = someip_posix_common::someip;
  using cfg = configuration::Configuration;
  auto header = packet->GetHeader();
  const auto method_id = header.method_id_;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << instance_config_->service_id_ << ","
                     << instance_config_->instance_id_ << "," << method_id << std::dec << ")";
  auto method = config_->GetMethod(instance_config_->service_id_, method_id);
  assert(method);
  if (method->proto_ == cfg::Protocol::kTCP) {
    assert(tcp_sender_);
    tcp_sender_->Forward(instance_id, std::move(packet));
  } else {
    assert(udp_sender_);
    udp_sender_->Forward(instance_id, std::move(packet));
  }
}

void ServiceDiscoveryClientServiceInstance::OnConnect(connection_manager::ServiceDiscoveryTcpSender* sender) {
  (void)sender;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  service_discovery_->OfferService(instance_config_->service_id_, instance_config_->instance_id_, shared_from_this());
  is_offered_ = true;
}

void ServiceDiscoveryClientServiceInstance::OnDisconnect(connection_manager::ServiceDiscoveryTcpSender* sender) {
  (void)sender;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  service_discovery_->StopOfferService(instance_config_->service_id_, instance_config_->instance_id_);
  is_offered_ = false;
}

}  // namespace service_discovery
}  // namespace someipd_posix
