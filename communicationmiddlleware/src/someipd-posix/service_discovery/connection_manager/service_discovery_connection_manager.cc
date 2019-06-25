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
/**        \file  service_discovery_connection_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/connection_manager/service_discovery_connection_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryConnectionManager::ServiceDiscoveryConnectionManager(
    const configuration::Configuration* config, osabstraction::io::ReactorInterface* reactor,
    packet_router::PacketRouterInterface* packet_router, vac::timer::TimerManager* timer_manager)
    : config_(config),
      reactor_(reactor),
      packet_router_(packet_router),
      logger_(ara::log::CreateLogger("ServiceDiscoveryConnectionManager", "")),
      timer_manager_{timer_manager} {
  SetupUdpTransmissionTriggerPolicyManager(*config_);
  CreateEndpoints();
}

ServiceDiscoveryTcpSenderPtr ServiceDiscoveryConnectionManager::GetTcpSender(
    const someip_posix_common::someip::IpAddress& from_address, someip_posix_common::someip::Port from_port,
    const someip_posix_common::someip::IpAddress& to_address, someip_posix_common::someip::Port to_port) {
  auto it = FindTcpEndpoint(from_address, from_port);
  assert(it != tcp_endpoints_.end());
  return (*it)->GetSender(to_address, to_port);
}

ServiceDiscoveryTcpReceiverPtr ServiceDiscoveryConnectionManager::GetTcpReceiver(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  auto it = FindTcpEndpoint(address, port);
  assert(it != tcp_endpoints_.end());
  return (*it)->GetReceiver();
}

bool ServiceDiscoveryConnectionManager::HasTcpPassiveConnection(
    const someip_posix_common::someip::IpAddress& from_address, someip_posix_common::someip::Port from_port,
    const someip_posix_common::someip::IpAddress& to_address, someip_posix_common::someip::Port to_port) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << " to "
                     << to_address << "," << to_port;
  auto it = FindTcpEndpoint(from_address, from_port);
  if (it == tcp_endpoints_.end()) {
    return false;
  }
  return (*it)->HasPassiveConnection(to_address, to_port);
}

ServiceDiscoveryUdpSenderPtr ServiceDiscoveryConnectionManager::GetUdpSender(
    const someip_posix_common::someip::IpAddress& from_address, someip_posix_common::someip::Port from_port,
    const someip_posix_common::someip::IpAddress& to_address, someip_posix_common::someip::Port to_port) {
  auto it = FindUdpEndpoint(from_address, from_port);
  assert(it != udp_endpoints_.end());
  return (*it)->GetSender(to_address, to_port);
}

ServiceDiscoveryUdpReceiverPtr ServiceDiscoveryConnectionManager::GetUdpUnicastReceiver(
    const someip_posix_common::someip::IpAddress& address, someip_posix_common::someip::Port port) {
  auto it = FindUdpEndpoint(address, port);
  assert(it != udp_endpoints_.end());
  return (*it)->GetReceiver();
}

ServiceDiscoveryUdpReceiverPtr ServiceDiscoveryConnectionManager::GetUdpMulticastReceiver(
    const someip_posix_common::someip::IpAddress& local_address,
    const someip_posix_common::someip::IpAddress& multicast_address, someip_posix_common::someip::Port multicast_port) {
  auto it = FindUdpEndpoint(multicast_address, multicast_port);
  if (it == udp_endpoints_.end()) {
    auto ne = config_->GetNetworkEndpoint(local_address);
    assert(ne);
    udp_endpoints_.emplace_back(std::make_shared<ServiceDiscoveryUdpEndpoint>(
        reactor_, packet_router_, timer_manager_, udp_transmission_trigger_policy_manager_, local_address,
        multicast_address, multicast_port, ne->mtu_, 0U));
    it = std::prev(udp_endpoints_.end());
  }
  return (*it)->GetReceiver();
}

void ServiceDiscoveryConnectionManager::CreateEndpoints() {
  using config = configuration::Configuration;
  for (const auto& ne : config_->GetNetworkEndpoints()) {
    for (const auto& p : ne.ports_) {
      if (p.proto_ == config::Protocol::kTCP) {
        tcp_endpoints_.emplace_back(
            std::make_shared<ServiceDiscoveryTcpEndpoint>(reactor_, packet_router_, ne.address_, p.port_, p.options_));
      } else {
        someip_posix_common::someip::TransmissionTriggerBufferSize tx_trigger_buffer_size{0U};
        if (p.tx_trigger_.is_valid_) {
          tx_trigger_buffer_size = p.tx_trigger_.buffer_size_;
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": transmission trigger buffer size "
                             << tx_trigger_buffer_size;
        } else {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": using MTU as transmission trigger buffer size "
                             << ne.mtu_;
          tx_trigger_buffer_size = ne.mtu_;
        }
        udp_endpoints_.emplace_back(std::make_shared<ServiceDiscoveryUdpEndpoint>(
            reactor_, packet_router_, timer_manager_, udp_transmission_trigger_policy_manager_, ne.address_, p.port_,
            ne.mtu_, p.options_, tx_trigger_buffer_size));
      }
    }
  }
}

ServiceDiscoveryConnectionManager::ServiceDiscoveryTcpEndpointContainerIter
ServiceDiscoveryConnectionManager::FindTcpEndpoint(const someip_posix_common::someip::IpAddress& address,
                                                   someip_posix_common::someip::Port port) {
  return std::find_if(tcp_endpoints_.begin(), tcp_endpoints_.end(),
                      [&address, port](const ServiceDiscoveryTcpEndpointPtr& ep) {
                        return ep->GetAddress() == address && ep->GetPort() == port;
                      });
}

ServiceDiscoveryConnectionManager::ServiceDiscoveryUdpEndpointContainerIter
ServiceDiscoveryConnectionManager::FindUdpEndpoint(const someip_posix_common::someip::IpAddress& address,
                                                   someip_posix_common::someip::Port port) {
  return std::find_if(udp_endpoints_.begin(), udp_endpoints_.end(),
                      [&address, port](const ServiceDiscoveryUdpEndpointPtr& ep) {
                        return ep->GetAddress() == address && ep->GetPort() == port;
                      });
}

void ServiceDiscoveryConnectionManager::SetupUdpTransmissionTriggerPolicyManager(
    const configuration::Configuration& config) {
  using cfg = configuration::Configuration;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": initializing transmission trigger policy manager";
  // Iterate over each service description
  for (const auto& sc : config.GetServices()) {
    // Iterate over all methods of a given service
    for (const auto& mc : sc.methods_) {
      const auto& ttc{mc.tx_trigger_};
      if (ttc.is_valid_) {
        if (ttc.mode_ == cfg::TransmissionTriggerMode::kTransmissionTriggerModeAlways) {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": installing method policy 'always' for (" << std::hex
                             << sc.id_ << "," << mc.id_ << std::dec << ")";
          transmission_trigger::TransmissionTriggerPolicy policy{};
          policy.SetServiceId(sc.id_);
          policy.SetMethodId(mc.id_);
          udp_transmission_trigger_policy_manager_.InstallPolicy(policy);
        } else {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": installing method policy 'never' with timeout "
                             << ttc.timeout_.count() << " for (" << std::hex << sc.id_ << "," << mc.id_ << std::dec
                             << ")";
          transmission_trigger::TransmissionTriggerPolicy policy{ttc.timeout_};
          policy.SetServiceId(sc.id_);
          policy.SetMethodId(mc.id_);
          udp_transmission_trigger_policy_manager_.InstallPolicy(policy);
        }
      }
    }
    // Iterate over all events of a given service
    for (const auto& ec : sc.events_) {
      const auto& ttc{ec.tx_trigger_};
      if (ttc.is_valid_) {
        if (ttc.mode_ == cfg::TransmissionTriggerMode::kTransmissionTriggerModeAlways) {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": installing event policy 'always' for (" << std::hex
                             << sc.id_ << "," << ec.id_ << std::dec << ")";
          transmission_trigger::TransmissionTriggerPolicy policy{};
          policy.SetServiceId(sc.id_);
          policy.SetMethodId(ec.id_);
          udp_transmission_trigger_policy_manager_.InstallPolicy(policy);
        } else {
          logger_.LogDebug() << __func__ << ":" << __LINE__ << ": installing event policy 'never' with timeout "
                             << ttc.timeout_.count() << " for (" << std::hex << sc.id_ << "," << ec.id_ << std::dec
                             << ")";
          transmission_trigger::TransmissionTriggerPolicy policy{ttc.timeout_};
          policy.SetServiceId(sc.id_);
          policy.SetMethodId(ec.id_);
          udp_transmission_trigger_policy_manager_.InstallPolicy(policy);
        }
      }
    }
  }
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
