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
/**        \file  service_discovery_client_message_builder.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_message_builder.h"
#include <algorithm>
#include <cassert>
#include <string>
#include "osabstraction/io/network/address/ipv4_socket_address.h"
#include "osabstraction/io/network/address/ipv6_socket_address.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

ServiceDiscoveryClientMessageBuilder::ServiceDiscoveryClientMessageBuilder(
    const configuration::Configuration::RequiredServiceInstance* instance_config)
    : instance_config_{instance_config} {
  namespace someip = someip_posix_common::someip;
  const auto& port_mapping = instance_config_->port_mapping_;
  const char* dont_care_port{"1000"};
  auto sa =
      osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(port_mapping.address_, dont_care_port);
  assert((sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) ||
         (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet6));
  if (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) {
    option_type_ = someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption;
  } else {
    option_type_ = someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption;
  }
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryClientMessageBuilder::MakeFindServiceMessage() const {
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryEntryContainer entries{};
  ServiceDiscoveryEntry entry{};
  entry.type_ = someip::SomeIpSdEntryType::kFindService;
  entry.service_id_ = instance_config_->service_id_;
  entry.instance_id_ = instance_config_->instance_id_;
  entry.major_version_ = instance_config_->major_version_;
  entry.minor_version_ = instance_config_->minor_version_;
  entry.index_1st_opts_ = 0;
  entry.index_2nd_opts_ = 0;
  entry.number_1st_opts_ = 0;
  entry.number_2nd_opts_ = 0;
  entry.ttl_ = instance_config_->service_discovery_.ttl_;
  entries.push_back(entry);
  return {entries, ServiceDiscoveryOptionContainer{}};
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryClientMessageBuilder::MakeSubscribeEventgroupMessage(std::uint16_t eventgroup_id,
                                                                     bool request_initial_events) const {
  using cfg = configuration::Configuration;
  /* Get eventgroup TTL parameter */
  auto it = std::find_if(
      instance_config_->service_discovery_.eventgroups_.cbegin(),
      instance_config_->service_discovery_.eventgroups_.cend(),
      [eventgroup_id](const cfg::RequiredServiceInstanceSdEventgroup& eg) { return eventgroup_id == eg.id_; });
  assert(it != instance_config_->service_discovery_.eventgroups_.cend());
  return MakeEventgroupMessage(eventgroup_id, it->ttl_, request_initial_events);
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryClientMessageBuilder::MakeStopSubscribeEventgroupMessage(std::uint16_t eventgroup_id) const {
  return MakeEventgroupMessage(eventgroup_id, 0, false);
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryClientMessageBuilder::MakeEventgroupMessage(std::uint16_t eventgroup_id, std::uint32_t ttl,
                                                            bool request_initial_events) const {
  using cfg = configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  // TODO(PAASR-2103)
  (void)request_initial_events;
  ServiceDiscoveryEntryContainer entries{};
  ServiceDiscoveryEntry entry{};
  entry.type_ = someip::SomeIpSdEntryType::kSubscribeEventgroup;
  entry.service_id_ = instance_config_->service_id_;
  entry.instance_id_ = instance_config_->instance_id_;
  entry.major_version_ = instance_config_->major_version_;
  entry.index_1st_opts_ = 0;
  entry.index_2nd_opts_ = 0;
  entry.number_1st_opts_ = 0;
  entry.number_2nd_opts_ = 0;
  entry.ttl_ = ttl;
  entry.counter_ = 0;
  entry.eventgroup_id_ = eventgroup_id;
  const auto& port_mapping = instance_config_->port_mapping_;
  assert((port_mapping.tcp_port_ != cfg::kInvalidPort) || (port_mapping.udp_port_ != cfg::kInvalidPort));
  ServiceDiscoveryOptionContainer options{};
  /* TCP option */
  if (port_mapping.tcp_port_ != cfg::kInvalidPort) {
    ServiceDiscoveryOption option{};
    option.type_ = option_type_;
    option.address_ = port_mapping.address_;
    option.proto_ = someip::SomeIpSdEndpointOptionProto::kTCP;
    option.port_ = port_mapping.tcp_port_;
    options.push_back(option);
    entry.number_1st_opts_++;
  }
  /* UDP option */
  if (port_mapping.udp_port_ != cfg::kInvalidPort) {
    ServiceDiscoveryOption option{};
    option.type_ = option_type_;
    option.address_ = port_mapping.address_;
    option.proto_ = someip::SomeIpSdEndpointOptionProto::kUDP;
    option.port_ = port_mapping.udp_port_;
    options.push_back(option);
    entry.number_1st_opts_++;
  }
  entries.push_back(entry);
  return {entries, options};
}

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
