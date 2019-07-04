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
/**        \file  service_discovery_endpoint.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_endpoint.h"
#include <arpa/inet.h>
#include <algorithm>
#include <cassert>
#include "osabstraction/io/network/address/ipv4_socket_address.h"
#include "osabstraction/io/network/address/ipv6_socket_address.h"

namespace someipd_posix {
namespace service_discovery {

ServiceDiscoveryEndpoint::ServiceDiscoveryEndpoint(osabstraction::io::ReactorInterface* reactor,
                                                   const std::string& unicast_address,
                                                   const std::string& multicast_address, std::uint16_t port,
                                                   std::size_t max_datagram_length,
                                                   ServiceDiscoveryEndpointObserver& sd_endpoint_observer)
    : reactor_(reactor),
      logger_(ara::log::CreateLogger("ServiceDiscoveryEndpoint", "")),
      ucast_address_(unicast_address),
      mcast_address_(multicast_address),
      port_(port),
      max_dgram_length_(max_datagram_length),
      ucast_socket_address_(
          osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(ucast_address_, std::to_string(port_))),
      mcast_socket_address_(osabstraction::io::network::address::IPSocketAddress::FromMulticastGroupAndPort(
          ucast_address_, mcast_address_, std::to_string(port_))),
      ucast_socket_(osabstraction::io::network::socket::UDPSocket::CreateSocket(ucast_socket_address_, true)),
      mcast_socket_(osabstraction::io::network::socket::UDPSocket::CreateSocket(mcast_socket_address_, true)),
      sd_endpoint_observer_(sd_endpoint_observer),
      message_reader_(max_dgram_length_),
      send_mcast_dgram_buf_(max_dgram_length_),
      send_mcast_dgram_buf_pos_(send_mcast_dgram_buf_.begin()),
      send_ucast_dgram_buf_(max_dgram_length_) {
      printf("ServiceDiscoveryEndpoint 1\n");
  ucast_socket_.SetMulticastSendInterface(ucast_address_);
  mcast_socket_.JoinMulticastGroup(ucast_address_, mcast_address_);
  reactor_->RegisterEventHandler(ucast_socket_.GetHandle(), this, osabstraction::io::kReadEvent, nullptr);
  reactor_->RegisterEventHandler(mcast_socket_.GetHandle(), this, osabstraction::io::kReadEvent, nullptr);
}

ServiceDiscoveryEndpoint::~ServiceDiscoveryEndpoint() {
  reactor_->UnregisterEventHandler(ucast_socket_.GetHandle(), osabstraction::io::kReadEvent);
  reactor_->UnregisterEventHandler(mcast_socket_.GetHandle(), osabstraction::io::kReadEvent);
  mcast_socket_.LeaveMulticastGroup(ucast_address_, mcast_address_);
}

void ServiceDiscoveryEndpoint::SendUnicastMessage(const std::string& to_address, std::uint16_t to_port,
                                                  const ServiceDiscoveryEntryContainer& entries,
                                                  const ServiceDiscoveryOptionContainer& options) {
  namespace someip = someip_posix_common::someip;
  auto header_pos = send_ucast_dgram_buf_.begin();
  auto body_pos = header_pos + sizeof(someip::SomeIpMessageHeader);
  assert(body_pos - header_pos == sizeof(someip::SomeIpMessageHeader));

  // Add the message receiver details to reboot_management_map.
  auto address_pair = std::make_pair(to_address, to_port);

  auto next_reboot_param = reboot_manager_.GetNextUnicastSenderSessionIdAndRebootFlag(address_pair);

  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": next_reboot_param "
                     << " session_id :" << next_reboot_param.session_id_
                     << " reboot_flag :" << next_reboot_param.reboot_flag_;

  auto pos = SerializeSdMessage(body_pos, entries, options, next_reboot_param.reboot_flag_);
  auto body_length = static_cast<std::uint32_t>(pos - body_pos);
  auto dgram_length = static_cast<std::size_t>(pos - send_ucast_dgram_buf_.begin());

  SerializeSdMessageHeader(header_pos, body_length, next_reboot_param.session_id_);
  ucast_socket_.Send(
      send_ucast_dgram_buf_.data(), dgram_length,
      osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(to_address, std::to_string(to_port)));
}

void ServiceDiscoveryEndpoint::SendMulticastMessage(const ServiceDiscoveryEntryContainer& entries,
                                                    const ServiceDiscoveryOptionContainer& options) {
  namespace someip = someip_posix_common::someip;
  send_mcast_dgram_buf_pos_ = send_mcast_dgram_buf_.begin();
  auto header_pos = send_mcast_dgram_buf_pos_;
  auto body_pos = header_pos + sizeof(someip::SomeIpMessageHeader);
  assert(body_pos - header_pos == sizeof(someip::SomeIpMessageHeader));

  // Add the reboot_management_ details.
  auto next_reboot_param = reboot_manager_.GetNextMulticastSenderSessionIdAndRebootFlag();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": next_reboot_param "
                     << " session_id :" << next_reboot_param.session_id_
                     << " reboot_flag :" << next_reboot_param.reboot_flag_;
  send_mcast_dgram_buf_pos_ = SerializeSdMessage(body_pos, entries, options, next_reboot_param.reboot_flag_);
  auto body_length = static_cast<std::uint32_t>(send_mcast_dgram_buf_pos_ - body_pos);
  auto dgram_length = static_cast<std::size_t>(send_mcast_dgram_buf_pos_ - send_mcast_dgram_buf_.begin());

  SerializeSdMessageHeader(header_pos, body_length, next_reboot_param.session_id_);
  ucast_socket_.Send(send_mcast_dgram_buf_.data(), dgram_length, mcast_socket_address_);
}

// TODO(PAASR-1188)
void ServiceDiscoveryEndpoint::ProcessSdMessage(SomeIpMessage&& packet, bool is_multicast) {
  namespace someip = someip_posix_common::someip;
  auto header = packet.GetHeader();
  std::size_t bytes_left = packet.GetBodySize();
  /* Header sanity checks */
  const auto service_id = header.service_id_;
  const auto method_id = header.method_id_;
  if ((service_id != someip::kSdServiceId) || (method_id != someip::kSdMethodId) ||
      (header.protocol_version_ != someip::kSdProtocolVersion) ||
      (header.interface_version_ != someip::kSdInterfaceVersion) ||
      (header.length_ < someip::kSdMinimumMessageLength) || (header.client_id_ != someip::kSdClientId) ||
      (header.message_type_ != someip::SomeIpMessageType::kNotification) ||
      (header.return_code_ != someip::SomeIpReturnCode::kOk)) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString()
                       << " received a malformed SD Message (0x" << std::hex << service_id << ", 0x" << method_id
                       << ", " << std::dec << header.protocol_version_ << ", " << header.interface_version_ << ", "
                       << header.length_ << ", 0x" << std::hex << header.client_id_ << ", 0x" << header.session_id_
                       << ", 0x" << header.message_type_ << ", 0x" << header.return_code_ << std::dec
                       << "): Message discarded.";
    return;
  }

  auto ptr = packet.GetBodyPtr();
  /* Parse flags */
  const std::uint8_t flags = *ptr;
  const bool reboot_flag = (flags >> someip::kSomeIpSdRebootFlagOffset) & someip::kSomeIpSdFlagMask;

  ptr += sizeof(std::uint32_t);
  bytes_left -= sizeof(std::uint32_t);
  /* Parse entries */
  if (bytes_left < sizeof(std::uint32_t)) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), entry length field invalid, bytes left " << bytes_left << " expected at least "
                       << sizeof(std::uint32_t);
    return;
  }
  std::uint32_t length_of_entries = ntohl(*reinterpret_cast<const std::uint32_t*>(ptr));
  ptr += sizeof(std::uint32_t);
  bytes_left -= sizeof(std::uint32_t);
  if (bytes_left < length_of_entries) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), invalid entries length, bytes left " << bytes_left << " expected at least "
                       << length_of_entries;
    return;
  }
  auto entries = ParseSdEntries(ptr, length_of_entries);
  if (!entries.first) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), failed to parse entries";
    return;
  }
  ptr += length_of_entries;
  bytes_left -= length_of_entries;
  /* Parse options */
  if (bytes_left < sizeof(std::uint32_t)) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), options length field invalid, bytes left " << bytes_left << " expected at least "
                       << sizeof(std::uint32_t);
    return;
  }
  std::uint32_t length_of_options = ntohl(*reinterpret_cast<const std::uint32_t*>(ptr));
  ptr += sizeof(std::uint32_t);
  bytes_left -= sizeof(std::uint32_t);
  if (bytes_left < length_of_options) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), invalid options length, bytes left " << bytes_left << " expected at least "
                       << length_of_options;
    return;
  }
  auto options = ParseSdOptions(ptr, length_of_options);
  if (!options.first) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": from " << packet.GetFromAddress().first.toString() << " ("
                       << std::hex << header.client_id_ << "," << header.session_id_ << std::dec
                       << "), failed to parse options";
    return;
  }
  ptr += length_of_options;
  bytes_left -= length_of_options;
  if (bytes_left > 0) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": bytes left " << bytes_left;
  }
  /* Invoke receive handler and pass all entries and options */
  const auto& from_address = packet.GetFromAddress().first;
  auto from_address_pair = from_address.toAddressPortStrings();
  std::string from_address_str = from_address_pair.first;
  std::uint16_t from_port = static_cast<std::uint16_t>(std::stoul(from_address_pair.second));

  if (is_multicast) {
    // Add the message receiver details to reboot_management_map.
    auto address_pair = std::make_pair(from_address_str, from_port);

    const RebootParam old_reboot_param =
        reboot_manager_.GetLastMulticastReceiverSessionIdAndRebootFlag(address_pair, header.session_id_, reboot_flag);

    RebootParam new_reboot_param;
    new_reboot_param.session_id_ = header.session_id_;
    new_reboot_param.reboot_flag_ = reboot_flag;

    // Process reboot detection.
    const bool is_reboot_detected = IsRebootDetected(old_reboot_param, new_reboot_param);

    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": is_reboot_detected for multicast " << is_reboot_detected
                       << " old_session_id :" << old_reboot_param.session_id_
                       << " new_session_id :" << new_reboot_param.session_id_
                       << " old_reboot_flag :" << old_reboot_param.reboot_flag_
                       << " new_reboot_flag :" << new_reboot_param.reboot_flag_;
    if (is_reboot_detected == true) {
      // Reset the session id and reboot flag for same provider in unicast receiver map.
      reboot_manager_.ResetUnicastReceiverRebootParam(address_pair);
      sd_endpoint_observer_.OnRebootDetected(ucast_address_, from_address_str, from_port);
    }
    sd_endpoint_observer_.OnMulticastSdMessage(ucast_address_, from_address_str, from_port, entries.second,
                                               options.second);
  } else {
    // Add the message receiver details to reboot_management_map.
    auto address_pair = std::make_pair(from_address_str, from_port);

    const RebootParam old_reboot_param =
        reboot_manager_.GetLastUnicastReceiverSessionIdAndRebootFlag(address_pair, header.session_id_, reboot_flag);

    RebootParam new_reboot_param;
    new_reboot_param.session_id_ = header.session_id_;
    new_reboot_param.reboot_flag_ = reboot_flag;

    // Process reboot detection.
    const bool is_reboot_detected = IsRebootDetected(old_reboot_param, new_reboot_param);

    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": is_reboot_detected for unicast " << is_reboot_detected
                       << " old_session_id :" << old_reboot_param.session_id_
                       << " new_session_id :" << new_reboot_param.session_id_
                       << " old_reboot_flag :" << old_reboot_param.reboot_flag_
                       << " new_reboot_flag :" << new_reboot_param.reboot_flag_;
    if (is_reboot_detected == true) {
      // Reset the session id and reboot flag for same provider in multicast receiver map.
      reboot_manager_.ResetMulticastReceiverRebootParam(address_pair);
      sd_endpoint_observer_.OnRebootDetected(ucast_address_, from_address_str, from_port);
    }
    sd_endpoint_observer_.OnUnicastSdMessage(ucast_address_, from_address_str, from_port, entries.second,
                                             options.second);
  }
}

std::pair<bool, ServiceDiscoveryEntryContainer> ServiceDiscoveryEndpoint::ParseSdEntries(const std::uint8_t* buffer,
                                                                                         std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryEntryContainer entries;
  while (buffer_length > 0) {
    if (buffer_length < sizeof(std::uint8_t)) {
      return std::make_pair(false, ServiceDiscoveryEntryContainer{});
    }
    std::uint8_t type = *buffer;
    if (type == someip::SomeIpSdEntryType::kFindService) {
      auto ret = ParseSdFindServiceEntry(buffer, buffer_length);
      if (ret.first) {
        entries.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdFindServiceEntry);
        buffer_length -= sizeof(someip::SomeIpSdFindServiceEntry);
      } else {
        return std::make_pair(false, ServiceDiscoveryEntryContainer{});
      }
    } else if (type == someip::SomeIpSdEntryType::kOfferService) {
      auto ret = ParseSdOfferServiceEntry(buffer, buffer_length);
      if (ret.first) {
        entries.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdOfferServiceEntry);
        buffer_length -= sizeof(someip::SomeIpSdOfferServiceEntry);
      } else {
        return std::make_pair(false, ServiceDiscoveryEntryContainer{});
      }
    } else if (type == someip::SomeIpSdEntryType::kSubscribeEventgroup) {
      auto ret = ParseSdSubscribeEventgroupEntry(buffer, buffer_length);
      if (ret.first) {
        entries.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdSubscribeEventgroupEntry);
        buffer_length -= sizeof(someip::SomeIpSdSubscribeEventgroupEntry);
      } else {
        return std::make_pair(false, ServiceDiscoveryEntryContainer{});
      }
    } else if (type == someip::SomeIpSdEntryType::kSubscribeEventgroupAck) {
      auto ret = ParseSdSubscribeEventgroupAckEntry(buffer, buffer_length);
      if (ret.first) {
        entries.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdSubscribeEventgroupAckEntry);
        buffer_length -= sizeof(someip::SomeIpSdSubscribeEventgroupAckEntry);
      } else {
        return std::make_pair(false, ServiceDiscoveryEntryContainer{});
      }
    } else {
      logger_.LogError() << __func__ << ":" << __LINE__ << ": unknown entry type " << std::hex << static_cast<int>(type)
                         << std::dec;
      return std::make_pair(false, ServiceDiscoveryEntryContainer{});
    }
  }
  return std::make_pair(true, entries);
}

std::pair<bool, ServiceDiscoveryOptionContainer> ServiceDiscoveryEndpoint::ParseSdOptions(const std::uint8_t* buffer,
                                                                                          std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryOptionContainer options;
  while (buffer_length > 0) {
    if (buffer_length < (sizeof(std::uint16_t) + sizeof(std::uint8_t))) {
      return std::make_pair(false, ServiceDiscoveryOptionContainer{});
    }
    std::uint8_t type = *(buffer + sizeof(std::uint16_t));
    if (type == someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption ||
        type == someip::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption) {
      auto ret = ParseSdIPv4EndpointOption(buffer, buffer_length);
      if (ret.first) {
        options.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdIPv4EndpointOption);
        buffer_length -= sizeof(someip::SomeIpSdIPv4EndpointOption);
      } else {
        return std::make_pair(false, ServiceDiscoveryOptionContainer{});
      }
    } else if (type == someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption ||
               type == someip::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption) {
      auto ret = ParseSdIPv6EndpointOption(buffer, buffer_length);
      if (ret.first) {
        options.push_back(ret.second);
        buffer += sizeof(someip::SomeIpSdIPv6EndpointOption);
        buffer_length -= sizeof(someip::SomeIpSdIPv6EndpointOption);
      } else {
        return std::make_pair(false, ServiceDiscoveryOptionContainer{});
      }
    } else {
      logger_.LogError() << __func__ << ":" << __LINE__ << ": unknown option type " << std::hex
                         << static_cast<int>(type) << std::dec;
      return std::make_pair(false, ServiceDiscoveryOptionContainer{});
    }
  }
  return std::make_pair(true, options);
}

std::pair<bool, ServiceDiscoveryEntry> ServiceDiscoveryEndpoint::ParseSdFindServiceEntry(const std::uint8_t* buffer,
                                                                                         std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdFindServiceEntry)) {
    return std::make_pair(false, ServiceDiscoveryEntry{});
  }
  auto sd_entry = reinterpret_cast<const someip::SomeIpSdFindServiceEntry*>(buffer);
  ServiceDiscoveryEntry entry{};
  entry.type_ = sd_entry->type_;
  entry.service_id_ = ntohs(sd_entry->service_id_);
  entry.instance_id_ = ntohs(sd_entry->instance_id_);
  std::uint32_t major_version_ttl = ntohl(sd_entry->major_version_ttl_);
  entry.major_version_ = static_cast<std::uint8_t>((major_version_ttl >> someip::kSomeIpSdEntryMajorVersionShift) &
                                                   someip::kSomeIpSdEntryMajorVersionMask);
  entry.ttl_ = major_version_ttl & someip::kSomeIpSdEntryTtlMask;
  entry.minor_version_ = ntohl(sd_entry->minor_version_);
  entry.index_1st_opts_ = sd_entry->index_1st_opts_;
  entry.index_2nd_opts_ = sd_entry->index_2nd_opts_;
  entry.number_1st_opts_ =
      (sd_entry->number_of_opts_ >> someip::kSomeIpSdEntryNumberFirstOptsShift) & someip::kSomeIpSdEntryNumberOptsMask;
  entry.number_2nd_opts_ = sd_entry->number_of_opts_ & someip::kSomeIpSdEntryNumberOptsMask;
  return std::make_pair(true, entry);
}

std::pair<bool, ServiceDiscoveryEntry> ServiceDiscoveryEndpoint::ParseSdOfferServiceEntry(const std::uint8_t* buffer,
                                                                                          std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdOfferServiceEntry)) {
    return std::make_pair(false, ServiceDiscoveryEntry{});
  }
  auto sd_entry = reinterpret_cast<const someip::SomeIpSdOfferServiceEntry*>(buffer);
  ServiceDiscoveryEntry entry{};
  entry.type_ = sd_entry->type_;
  entry.service_id_ = ntohs(sd_entry->service_id_);
  entry.instance_id_ = ntohs(sd_entry->instance_id_);
  std::uint32_t major_version_ttl = ntohl(sd_entry->major_version_ttl_);
  entry.major_version_ = static_cast<std::uint8_t>((major_version_ttl >> someip::kSomeIpSdEntryMajorVersionShift) &
                                                   someip::kSomeIpSdEntryMajorVersionMask);
  entry.ttl_ = major_version_ttl & someip::kSomeIpSdEntryTtlMask;
  entry.minor_version_ = ntohl(sd_entry->minor_version_);
  entry.index_1st_opts_ = sd_entry->index_1st_opts_;
  entry.index_2nd_opts_ = sd_entry->index_2nd_opts_;
  entry.number_1st_opts_ =
      (sd_entry->number_of_opts_ >> someip::kSomeIpSdEntryNumberFirstOptsShift) & someip::kSomeIpSdEntryNumberOptsMask;
  entry.number_2nd_opts_ = sd_entry->number_of_opts_ & someip::kSomeIpSdEntryNumberOptsMask;
  return std::make_pair(true, entry);
}

std::pair<bool, ServiceDiscoveryEntry> ServiceDiscoveryEndpoint::ParseSdSubscribeEventgroupEntry(
    const std::uint8_t* buffer, std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdSubscribeEventgroupEntry)) {
    return std::make_pair(false, ServiceDiscoveryEntry{});
  }
  auto sd_entry = reinterpret_cast<const someip::SomeIpSdSubscribeEventgroupEntry*>(buffer);
  ServiceDiscoveryEntry entry{};
  entry.type_ = sd_entry->type_;
  entry.service_id_ = ntohs(sd_entry->service_id_);
  entry.instance_id_ = ntohs(sd_entry->instance_id_);
  std::uint32_t major_version_ttl = ntohl(sd_entry->major_version_ttl_);
  entry.major_version_ = static_cast<std::uint8_t>((major_version_ttl >> someip::kSomeIpSdEntryMajorVersionShift) &
                                                   someip::kSomeIpSdEntryMajorVersionMask);
  entry.ttl_ = major_version_ttl & someip::kSomeIpSdEntryTtlMask;
  entry.index_1st_opts_ = sd_entry->index_1st_opts_;
  entry.index_2nd_opts_ = sd_entry->index_2nd_opts_;
  entry.number_1st_opts_ =
      (sd_entry->number_of_opts_ >> someip::kSomeIpSdEntryNumberFirstOptsShift) & someip::kSomeIpSdEntryNumberOptsMask;
  entry.number_2nd_opts_ = sd_entry->number_of_opts_ & someip::kSomeIpSdEntryNumberOptsMask;
  entry.counter_ = sd_entry->counter_;
  entry.eventgroup_id_ = ntohs(sd_entry->eventgroup_id_);
  return std::make_pair(true, entry);
}

std::pair<bool, ServiceDiscoveryEntry> ServiceDiscoveryEndpoint::ParseSdSubscribeEventgroupAckEntry(
    const std::uint8_t* buffer, std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdSubscribeEventgroupAckEntry)) {
    return std::make_pair(false, ServiceDiscoveryEntry{});
  }
  auto sd_entry = reinterpret_cast<const someip::SomeIpSdSubscribeEventgroupAckEntry*>(buffer);
  ServiceDiscoveryEntry entry{};
  entry.type_ = sd_entry->type_;
  entry.service_id_ = ntohs(sd_entry->service_id_);
  entry.instance_id_ = ntohs(sd_entry->instance_id_);
  std::uint32_t major_version_ttl = ntohl(sd_entry->major_version_ttl_);
  entry.major_version_ = static_cast<std::uint8_t>((major_version_ttl >> someip::kSomeIpSdEntryMajorVersionShift) &
                                                   someip::kSomeIpSdEntryMajorVersionMask);
  entry.ttl_ = major_version_ttl & someip::kSomeIpSdEntryTtlMask;
  entry.index_1st_opts_ = sd_entry->index_1st_opts_;
  entry.index_2nd_opts_ = sd_entry->index_2nd_opts_;
  entry.number_1st_opts_ =
      (sd_entry->number_of_opts_ >> someip::kSomeIpSdEntryNumberFirstOptsShift) & someip::kSomeIpSdEntryNumberOptsMask;
  entry.number_2nd_opts_ = sd_entry->number_of_opts_ & someip::kSomeIpSdEntryNumberOptsMask;
  entry.counter_ = sd_entry->counter_;
  entry.eventgroup_id_ = ntohs(sd_entry->eventgroup_id_);
  return std::make_pair(true, entry);
}

std::pair<bool, ServiceDiscoveryOption> ServiceDiscoveryEndpoint::ParseSdIPv4EndpointOption(const std::uint8_t* buffer,
                                                                                            std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdIPv4EndpointOption)) {
    return std::make_pair(false, ServiceDiscoveryOption{});
  }
  auto sd_option = reinterpret_cast<const someip::SomeIpSdIPv4EndpointOption*>(buffer);
  if (ntohs(sd_option->length_) != someip::kSomeIpSdIPv4EndpointOptionLength ||
      (sd_option->type_ != someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption &&
       sd_option->type_ != someip::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption)) {
    return std::make_pair(false, ServiceDiscoveryOption{});
  }
  ServiceDiscoveryOption option{};
  option.type_ = sd_option->type_;
  option.proto_ = sd_option->proto_;
  option.port_ = ntohs(sd_option->port_);
  std::array<std::uint8_t, osabstraction::io::network::address::IPV4SocketAddress::kAddressLength> address;
  std::copy(std::begin(sd_option->address_), std::end(sd_option->address_), std::begin(address));
  option.address_ = osabstraction::io::network::address::IPV4SocketAddress::AddressToString(address);
  return std::make_pair(true, option);
}

std::pair<bool, ServiceDiscoveryOption> ServiceDiscoveryEndpoint::ParseSdIPv6EndpointOption(const std::uint8_t* buffer,
                                                                                            std::size_t buffer_length) {
  namespace someip = someip_posix_common::someip;
  if (buffer_length < sizeof(someip::SomeIpSdIPv6EndpointOption)) {
    return std::make_pair(false, ServiceDiscoveryOption{});
  }
  auto sd_option = reinterpret_cast<const someip::SomeIpSdIPv6EndpointOption*>(buffer);
  if (ntohs(sd_option->length_) != someip::kSomeIpSdIPv6EndpointOptionLength ||
      (sd_option->type_ != someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption &&
       sd_option->type_ != someip::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption)) {
    return std::make_pair(false, ServiceDiscoveryOption{});
  }
  ServiceDiscoveryOption option{};
  option.type_ = sd_option->type_;
  option.proto_ = sd_option->proto_;
  option.port_ = ntohs(sd_option->port_);
  std::array<std::uint8_t, osabstraction::io::network::address::IPV6SocketAddress::kAddressLength> address;
  std::copy(std::begin(sd_option->address_), std::end(sd_option->address_), std::begin(address));
  option.address_ = osabstraction::io::network::address::IPV6SocketAddress::AddressToString(address);
  return std::make_pair(true, option);
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdMessageHeader(
    DatagramBufferIterator pos, std::size_t body_length, someip_posix_common::someip::SessionId session_id) {
  namespace someip = someip_posix_common::someip;
  /* Header */
  auto header = reinterpret_cast<someip::SomeIpMessageHeader*>(&*pos);
  pos += sizeof(someip::SomeIpMessageHeader);
  header->service_id_ = htons(someip::kSdServiceId);
  header->method_id_ = htons(someip::kSdMethodId);
  const someip_posix_common::someip::LengthField length_field_value =
      static_cast<someip_posix_common::someip::LengthField>(
          someip::CalculateSomeIpMessageHeaderLengthField(body_length));
  header->length_ = htonl(length_field_value);
  header->client_id_ = someip::kSdClientId;
  header->session_id_ = htons(session_id);
  header->protocol_version_ = someip::kSdProtocolVersion;
  header->interface_version_ = someip::kSdInterfaceVersion;
  header->message_type_ = someip::SomeIpMessageType::kNotification;
  header->return_code_ = someip::SomeIpReturnCode::kOk;
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdMessage(
    DatagramBufferIterator pos, const ServiceDiscoveryEntryContainer& entries,
    const ServiceDiscoveryOptionContainer& options, bool reboot_flag) {
  namespace someip = someip_posix_common::someip;
  /* Flags */
  auto flags = reinterpret_cast<std::uint8_t*>(&*pos);
  pos += sizeof(std::uint32_t);
  *flags = someip::SomeIpSdFlag::kUnicast;

  if (reboot_flag) {
    *flags |= someip::SomeIpSdFlag::kReboot;
  }
  /* Entries */
  auto length_of_entries = reinterpret_cast<std::uint32_t*>(&*pos);
  pos += sizeof(std::uint32_t);
  auto first_entry_pos = pos;
  for (const auto& entry : entries) {
    switch (entry.type_) {
      case someip::SomeIpSdEntryType::kFindService:
        pos = SerializeSdFindServiceEntry(pos, entry);
        break;
      case someip::SomeIpSdEntryType::kOfferService:
        pos = SerializeSdOfferServiceEntry(pos, entry);
        break;
      case someip::SomeIpSdEntryType::kSubscribeEventgroup:
        pos = SerializeSdSubscribeEventgroupEntry(pos, entry);
        break;
      case someip::SomeIpSdEntryType::kSubscribeEventgroupAck:
        pos = SerializeSdSubscribeEventgroupAckEntry(pos, entry);
        break;
      default:
        assert(0);
        break;
    }
  }
  *length_of_entries = htonl(static_cast<std::uint32_t>(pos - first_entry_pos));
  /* Options */
  auto length_of_options = reinterpret_cast<std::uint32_t*>(&*pos);
  pos += sizeof(std::uint32_t);
  auto first_option_pos = pos;
  for (const auto& option : options) {
    switch (option.type_) {
      case someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption:
      case someip::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption:
        pos = SerializeSdIPv4EndpointOption(pos, option);
        break;
      case someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption:
      case someip::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption:
        pos = SerializeSdIPv6EndpointOption(pos, option);
        break;
      default:
        assert(0);
        break;
    }
  }
  *length_of_options = htonl(static_cast<std::uint32_t>(pos - first_option_pos));
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdFindServiceEntry(
    DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry) {
  namespace someip = someip_posix_common::someip;
  assert(entry.type_ == someip::SomeIpSdEntryType::kFindService);
  auto sd_entry = reinterpret_cast<someip::SomeIpSdFindServiceEntry*>(&*pos);
  pos += sizeof(someip::SomeIpSdFindServiceEntry);
  sd_entry->type_ = someip::SomeIpSdEntryType::kFindService;
  sd_entry->index_1st_opts_ = entry.index_1st_opts_;
  sd_entry->index_2nd_opts_ = entry.index_2nd_opts_;
  sd_entry->number_of_opts_ = static_cast<std::uint8_t>(
      ((entry.number_1st_opts_ & someip::kSomeIpSdEntryNumberOptsMask) << someip::kSomeIpSdEntryNumberFirstOptsShift) |
      (entry.number_2nd_opts_ & someip::kSomeIpSdEntryNumberOptsMask));
  sd_entry->service_id_ = htons(entry.service_id_);
  sd_entry->instance_id_ = htons(entry.instance_id_);
  sd_entry->major_version_ttl_ = htonl(static_cast<std::uint32_t>(
      (((entry.major_version_ & someip::kSomeIpSdEntryMajorVersionMask) << someip::kSomeIpSdEntryMajorVersionShift) |
       (entry.ttl_ & someip::kSomeIpSdEntryTtlMask))));
  sd_entry->minor_version_ = htonl(entry.minor_version_);
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdOfferServiceEntry(
    DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry) {
  namespace someip = someip_posix_common::someip;
  assert(entry.type_ == someip::SomeIpSdEntryType::kOfferService);
  auto sd_entry = reinterpret_cast<someip::SomeIpSdOfferServiceEntry*>(&*pos);
  pos += sizeof(someip::SomeIpSdOfferServiceEntry);
  sd_entry->type_ = someip::SomeIpSdEntryType::kOfferService;
  sd_entry->index_1st_opts_ = entry.index_1st_opts_;
  sd_entry->index_2nd_opts_ = entry.index_2nd_opts_;
  sd_entry->number_of_opts_ = static_cast<std::uint8_t>(
      ((entry.number_1st_opts_ & someip::kSomeIpSdEntryNumberOptsMask) << someip::kSomeIpSdEntryNumberFirstOptsShift) |
      (entry.number_2nd_opts_ & someip::kSomeIpSdEntryNumberOptsMask));
  sd_entry->service_id_ = htons(entry.service_id_);
  sd_entry->instance_id_ = htons(entry.instance_id_);
  sd_entry->major_version_ttl_ = htonl(static_cast<std::uint32_t>(
      (((entry.major_version_ & someip::kSomeIpSdEntryMajorVersionMask) << someip::kSomeIpSdEntryMajorVersionShift) |
       (entry.ttl_ & someip::kSomeIpSdEntryTtlMask))));
  sd_entry->minor_version_ = htonl(entry.minor_version_);
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdSubscribeEventgroupEntry(
    DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry) {
  namespace someip = someip_posix_common::someip;
  assert(entry.type_ == someip::SomeIpSdEntryType::kSubscribeEventgroup);
  auto sd_entry = reinterpret_cast<someip::SomeIpSdSubscribeEventgroupEntry*>(&*pos);
  pos += sizeof(someip::SomeIpSdSubscribeEventgroupEntry);
  sd_entry->type_ = someip::SomeIpSdEntryType::kSubscribeEventgroup;
  sd_entry->index_1st_opts_ = entry.index_1st_opts_;
  sd_entry->index_2nd_opts_ = entry.index_2nd_opts_;
  sd_entry->number_of_opts_ = static_cast<std::uint8_t>(
      ((entry.number_1st_opts_ & someip::kSomeIpSdEntryNumberOptsMask) << someip::kSomeIpSdEntryNumberFirstOptsShift) |
      (entry.number_2nd_opts_ & someip::kSomeIpSdEntryNumberOptsMask));
  sd_entry->service_id_ = htons(entry.service_id_);
  sd_entry->instance_id_ = htons(entry.instance_id_);
  sd_entry->major_version_ttl_ = htonl(static_cast<std::uint32_t>(
      (((entry.major_version_ & someip::kSomeIpSdEntryMajorVersionMask) << someip::kSomeIpSdEntryMajorVersionShift) |
       (entry.ttl_ & someip::kSomeIpSdEntryTtlMask))));
  sd_entry->counter_ = entry.counter_;
  sd_entry->eventgroup_id_ = htons(entry.eventgroup_id_);
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdSubscribeEventgroupAckEntry(
    DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry) {
  namespace someip = someip_posix_common::someip;
  assert(entry.type_ == someip::SomeIpSdEntryType::kSubscribeEventgroupAck);
  auto sd_entry = reinterpret_cast<someip::SomeIpSdSubscribeEventgroupAckEntry*>(&*pos);
  pos += sizeof(someip::SomeIpSdSubscribeEventgroupAckEntry);
  sd_entry->type_ = someip::SomeIpSdEntryType::kSubscribeEventgroupAck;
  sd_entry->index_1st_opts_ = entry.index_1st_opts_;
  sd_entry->index_2nd_opts_ = entry.index_2nd_opts_;
  sd_entry->number_of_opts_ = static_cast<std::uint8_t>(
      ((entry.number_1st_opts_ & someip::kSomeIpSdEntryNumberOptsMask) << someip::kSomeIpSdEntryNumberFirstOptsShift) |
      (entry.number_2nd_opts_ & someip::kSomeIpSdEntryNumberOptsMask));
  sd_entry->service_id_ = htons(entry.service_id_);
  sd_entry->instance_id_ = htons(entry.instance_id_);
  sd_entry->major_version_ttl_ = htonl(static_cast<std::uint32_t>(
      (((entry.major_version_ & someip::kSomeIpSdEntryMajorVersionMask) << someip::kSomeIpSdEntryMajorVersionShift) |
       (entry.ttl_ & someip::kSomeIpSdEntryTtlMask))));
  sd_entry->counter_ = entry.counter_;
  sd_entry->eventgroup_id_ = htons(entry.eventgroup_id_);
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdIPv4EndpointOption(
    DatagramBufferIterator pos, const ServiceDiscoveryOption& option) {
  namespace someip = someip_posix_common::someip;
  assert(option.type_ == someip::SomeIpSdEndpointOptionType::kIPv4UnicastEndpointOption ||
         option.type_ == someip::SomeIpSdEndpointOptionType::kIPv4MulticastEndpointOption);
  auto sd_option = reinterpret_cast<someip::SomeIpSdIPv4EndpointOption*>(&*pos);
  pos += sizeof(someip::SomeIpSdIPv4EndpointOption);
  sd_option->length_ = htons(someip::kSomeIpSdIPv4EndpointOptionLength);
  sd_option->type_ = option.type_;
  sd_option->reserved1_ = 0x00;
  sd_option->reserved2_ = 0x00;
  sd_option->proto_ = option.proto_;
  sd_option->port_ = htons(option.port_);
  // TODO(visaev) This cast is an atrocity :-/
  auto address = osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(option.address_,
                                                                                       std::to_string(option.port_));
  auto ipv4_addr =
      reinterpret_cast<const std::uint8_t*>(&(reinterpret_cast<const sockaddr_in*>(address.toSockAddr())->sin_addr));
  std::copy(ipv4_addr, ipv4_addr + sizeof(sd_option->address_), sd_option->address_);
  return pos;
}

ServiceDiscoveryEndpoint::DatagramBufferIterator ServiceDiscoveryEndpoint::SerializeSdIPv6EndpointOption(
    DatagramBufferIterator pos, const ServiceDiscoveryOption& option) {
  namespace someip = someip_posix_common::someip;
  assert(option.type_ == someip::SomeIpSdEndpointOptionType::kIPv6UnicastEndpointOption ||
         option.type_ == someip::SomeIpSdEndpointOptionType::kIPv6MulticastEndpointOption);
  auto sd_option = reinterpret_cast<someip::SomeIpSdIPv6EndpointOption*>(&*pos);
  pos += sizeof(someip::SomeIpSdIPv6EndpointOption);
  sd_option->length_ = htons(someip::kSomeIpSdIPv6EndpointOptionLength);
  sd_option->type_ = option.type_;
  sd_option->reserved1_ = 0x00;
  sd_option->reserved2_ = 0x00;
  sd_option->proto_ = option.proto_;
  sd_option->port_ = htons(option.port_);
  // TODO(visaev) This cast is an atrocity :-/
  auto address = osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(option.address_,
                                                                                       std::to_string(option.port_));
  auto ipv6_addr =
      reinterpret_cast<const std::uint8_t*>(&(reinterpret_cast<const sockaddr_in6*>(address.toSockAddr())->sin6_addr));
  std::copy(ipv6_addr, ipv6_addr + sizeof(sd_option->address_), sd_option->address_);
  return pos;
}

bool ServiceDiscoveryEndpoint::HandleRead(int handle) {
  assert(handle == ucast_socket_.GetHandle() || handle == mcast_socket_.GetHandle());
  bool is_multicast = (handle == mcast_socket_.GetHandle());
  if (is_multicast) {
    message_reader_.Read(&mcast_socket_);
  } else {
    message_reader_.Read(&ucast_socket_);
  }
  while (message_reader_.IsMessageAvailable()) {
    auto someip_packet = message_reader_.GetNextMessage();
    // TODO(visaev) Drop my own datagram
    assert(someip_packet.GetFromAddress().second);
    const auto& from_address = someip_packet.GetFromAddress().first;
    if (!(from_address == ucast_socket_address_)) {
      ProcessSdMessage(std::move(someip_packet), is_multicast);
    }
  }
  return EventHandler::HandleRead(handle);
}

bool ServiceDiscoveryEndpoint::IsRebootDetected(const RebootParam& old_reboot_param,
                                                const RebootParam& new_reboot_param) const {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " old_session_id :" << old_reboot_param.session_id_
                     << " new_session_id :" << new_reboot_param.session_id_
                     << " old_reboot_flag :" << old_reboot_param.reboot_flag_
                     << " new_reboot_flag :" << new_reboot_param.reboot_flag_;
  bool reboot_detected{false};
  if (((old_reboot_param.reboot_flag_ == false) && (new_reboot_param.reboot_flag_ == true)) ||
      ((old_reboot_param.reboot_flag_ == true) && (new_reboot_param.reboot_flag_ == true) &&
       (old_reboot_param.session_id_ >= new_reboot_param.session_id_))) {
    reboot_detected = true;
  }

  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": reboot_detected " << reboot_detected;
  return reboot_detected;
}

}  // namespace service_discovery
}  // namespace someipd_posix
