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
/**        \file  service_discovery_server_message_builder.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include <string>
#include <utility>
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/service_discovery/message/service_discovery_message_builder.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerMessageBuilder::ServiceDiscoveryServerMessageBuilder(
    const std::string& address, const configuration::Configuration::ProvidedServiceInstance* config)
    : instance_config_(config) {
  namespace someip = someip_posix_common::someip;
  assert(instance_config_);
  using cfg = configuration::Configuration;
  const auto it =
      std::find_if(instance_config_->port_mappings_.cbegin(), instance_config_->port_mappings_.cend(),
                   [&address](const cfg::NetworkEndpointPortMapping& nepmc) { return (address == nepmc.address_); });
  assert(it != instance_config_->port_mappings_.end());
  port_mapping_config_ = &*it;
  if (port_mapping_config_->tcp_port_ != cfg::kInvalidPort) {
    assert(port_mapping_config_->address_ != cfg::kInvalidAddress);
    auto sa = osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
        port_mapping_config_->address_, std::to_string(port_mapping_config_->tcp_port_));
    assert((sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) ||
           (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet6));

    if (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) {
      tcp_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption;
    } else {
      tcp_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption;
    }
  }
  if (port_mapping_config_->udp_port_ != cfg::kInvalidPort) {
    assert(port_mapping_config_->address_ != cfg::kInvalidAddress);
    auto sa = osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
        port_mapping_config_->address_, std::to_string(port_mapping_config_->udp_port_));
    assert((sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) ||
           (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet6));

    if (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) {
      udp_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption;
    } else {
      udp_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption;
    }
  }
  if ((port_mapping_config_->event_multicast_port_ != cfg::kInvalidPort) &&
      (port_mapping_config_->event_multicast_address_ != cfg::kInvalidAddress)) {
    auto sa = osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
        port_mapping_config_->event_multicast_address_, std::to_string(port_mapping_config_->event_multicast_port_));
    assert((sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) ||
           (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet6));

    if (sa.GetAddressFamily() == osabstraction::io::network::address::SocketAddressFamily::kInet4) {
      multicast_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption;
    } else {
      multicast_option_type_ = someip::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption;
    }
  }
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryServerMessageBuilder::MakeOfferServiceMessage() {
  return MakeOfferServiceMessage(instance_config_->service_discovery_.ttl_);
}
std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryServerMessageBuilder::MakeStopOfferServiceMessage() {
  return MakeOfferServiceMessage(0);
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryServerMessageBuilder::MakeOfferServiceMessage(std::uint32_t ttl) {
  using cfg = configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryEntryContainer entries{};
  ServiceDiscoveryEntry entry{};  // Entry Type 1
  entry.type_ = someip::SomeIpSdEntryType::kOfferService;
  entry.service_id_ = instance_config_->service_id_;
  entry.instance_id_ = instance_config_->instance_id_;
  entry.major_version_ = instance_config_->major_version_;
  entry.minor_version_ = instance_config_->minor_version_;
  entry.index_1st_opts_ = 0;
  entry.index_2nd_opts_ = 0;
  entry.number_1st_opts_ = 0;
  entry.number_2nd_opts_ = 0;
  entry.ttl_ = ttl;
  ServiceDiscoveryOptionContainer options{};
  if (port_mapping_config_->tcp_port_ != cfg::kInvalidPort) {
    ServiceDiscoveryOption option{};
    option.type_ = tcp_option_type_;
    option.address_ = port_mapping_config_->address_;
    option.proto_ = someip::SomeIpSdEndpointOptionProto::kTCP;
    option.port_ = port_mapping_config_->tcp_port_;
    options.push_back(option);
    entry.number_1st_opts_++;
  }
  if (port_mapping_config_->udp_port_ != cfg::kInvalidPort) {
    ServiceDiscoveryOption option{};
    option.type_ = udp_option_type_;
    option.address_ = port_mapping_config_->address_;
    option.proto_ = someip::SomeIpSdEndpointOptionProto::kUDP;
    option.port_ = port_mapping_config_->udp_port_;
    options.push_back(option);
    entry.number_1st_opts_++;
  }
  entries.push_back(entry);
  return std::make_pair(entries, options);
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryServerMessageBuilder::MakeSubscribeEventgroupAckMessage(std::uint16_t eventgroup_id, std::uint32_t ttl,
                                                                        std::uint8_t counter) {
  using cfg = configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryEntryContainer entries{};
  ServiceDiscoveryEntry entry{};
  entry.type_ = someip::SomeIpSdEntryType::kSubscribeEventgroupAck;
  entry.service_id_ = instance_config_->service_id_;
  entry.instance_id_ = instance_config_->instance_id_;
  entry.major_version_ = instance_config_->major_version_;
  entry.index_1st_opts_ = 0;
  entry.index_2nd_opts_ = 0;
  entry.number_1st_opts_ = 0;
  entry.number_2nd_opts_ = 0;
  entry.ttl_ = ttl;
  entry.counter_ = counter;
  entry.eventgroup_id_ = eventgroup_id;
  ServiceDiscoveryOptionContainer options{};
  if ((port_mapping_config_->event_multicast_port_ != cfg::kInvalidPort) &&
      (port_mapping_config_->event_multicast_address_ != cfg::kInvalidAddress)) {
    ServiceDiscoveryOption option{};
    option.type_ = multicast_option_type_;
    option.address_ = port_mapping_config_->event_multicast_address_;
    option.proto_ = someip::SomeIpSdEndpointOptionProto::kUDP;
    option.port_ = port_mapping_config_->event_multicast_port_;
    options.push_back(option);
    entry.number_1st_opts_++;
  }
  entries.push_back(entry);
  return std::make_pair(entries, options);
}

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryServerMessageBuilder::MakeSubscribeEventgroupNackMessage(std::uint16_t eventgroup_id,
                                                                         std::uint8_t counter) {
  return ServiceDiscoveryMessageBuilder::MakeSubscribeEventgroupNackMessage(
      instance_config_->service_id_, instance_config_->instance_id_, instance_config_->major_version_, eventgroup_id,
      counter);
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
