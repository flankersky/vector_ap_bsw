/**
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
/**        \file
 *        \brief  Socket address implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_
#define LIB_LIBOSABSTRACTION_PIKEOS_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/socket_address.h"
#include <sys/qport.h>   // qport_*
#include <sys/socket.h>  // sockaddr*
#include <array>
#include <cassert>    // assert()
#include <cstring>    // memcmp
#include <stdexcept>  // runtime_error()
#include "osabstraction/io/net/address/ip_socket_address_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

SocketAddress::SocketAddress() : is_ipc_(false), sockaddr_(), sockaddr_size_(0) { sockaddr_.ss_family = AF_UNSPEC; }

SocketAddress::SocketAddress(const struct sockaddr_storage& sockaddr)
    : is_ipc_(false), sockaddr_(sockaddr), sockaddr_size_(0) {
  switch (sockaddr.ss_family) {
    case AF_INET:
      sockaddr_size_ = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
      sockaddr_size_ = sizeof(struct sockaddr_in6);
      break;
    default:
      break;
  }
}

SocketAddress::SocketAddress(const struct sockaddr& sockaddr) : is_ipc_(false), sockaddr_(), sockaddr_size_(0) {
  switch (sockaddr.sa_family) {
    case AF_INET:
      *reinterpret_cast<struct sockaddr_in*>(&sockaddr_) = *reinterpret_cast<const struct sockaddr_in*>(&sockaddr);
      sockaddr_size_ = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
      *reinterpret_cast<struct sockaddr_in6*>(&sockaddr_) = *reinterpret_cast<const struct sockaddr_in6*>(&sockaddr);
      sockaddr_size_ = sizeof(struct sockaddr_in6);
      break;
    default:
      break;
  }
}

SocketAddress::SocketAddress(const qport_sap_sockaddr_ipv4_t& sockaddr)
    : is_ipc_(true), sockaddr_(), sockaddr_size_(0) {
  *reinterpret_cast<qport_sap_sockaddr_ipv4_t*>(&sockaddr_) =
      *reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(&sockaddr);
  sockaddr_size_ = sizeof(qport_sap_sockaddr_ipv4_t);
}

SocketAddressFamily SocketAddress::GetAddressFamily() const {
  if (is_ipc_ && sockaddr_.ss_family == QPORT_AF_INET4) {
    return SocketAddressFamily::kIpc;
  }
  return internal::fromUnixSocketAddressFamily(sockaddr_.ss_family);
}

std::string SocketAddress::toString() const {
  if (sockaddr_.ss_family == AF_INET || sockaddr_.ss_family == AF_INET6) {
    auto p = toAddressPortStrings();
    return p.first + "," + p.second;
  } else {
    return "";
  }
}

std::pair<std::string, std::string> SocketAddress::toAddressPortStrings() const {
  assert((is_ipc_ && sockaddr_.ss_family == QPORT_AF_INET4) || sockaddr_.ss_family == AF_INET ||
         sockaddr_.ss_family == AF_INET6);
  if (is_ipc_ && sockaddr_.ss_family == QPORT_AF_INET4) {
    const qport_sap_sockaddr_ipv4_t* sockaddr_ipc = reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(&sockaddr_);
    const in_addr* addr = reinterpret_cast<const in_addr*>(&sockaddr_ipc->address);
    std::string hostname = internal::Ipv4AddressToString(addr);
    return std::make_pair(hostname, std::to_string(sockaddr_ipc->port));
  } else if (sockaddr_.ss_family == AF_INET) {
    const sockaddr_in* sockaddr_ip4 = reinterpret_cast<const sockaddr_in*>(&sockaddr_);
    std::string hostname = internal::Ipv4AddressToString(&sockaddr_ip4->sin_addr);
    return std::make_pair(hostname, std::to_string(sockaddr_ip4->sin_port));
  } else /* if (sockaddr_.ss_family == AF_INET6) */ {
    const sockaddr_in6* sockaddr_ip6 = reinterpret_cast<const sockaddr_in6*>(&sockaddr_);
    std::string hostname = internal::Ipv6AddressToString(&sockaddr_ip6->sin6_addr);
    return std::make_pair(hostname, std::to_string(sockaddr_ip6->sin6_port));
  }
}

int SocketAddress::internal::toUnixSocketAddressFamily(SocketAddressFamily family) {
  switch (family) {
    case SocketAddressFamily::kInet4:
      return AF_INET;
      break;
    case SocketAddressFamily::kInet6:
      return AF_INET6;
      break;
    default:
      return AF_UNSPEC;
      break;
  }
}

SocketAddressFamily SocketAddress::internal::fromUnixSocketAddressFamily(int family) {
  switch (family) {
    case AF_INET:
      return SocketAddressFamily::kInet4;
      break;
    case AF_INET6:
      return SocketAddressFamily::kInet6;
      break;
    default:
      return SocketAddressFamily::kUnspecified;
      break;
  }
}

std::string SocketAddress::internal::Ipv4AddressToString(const struct in_addr* addr) {
  char buf[INET_ADDRSTRLEN];
  const char* ret = inet_ntop(AF_INET, addr, buf, sizeof(buf));
  if (!ret) {
    throw std::invalid_argument("Ipv4AddressToString: Attempting to covert an invalid IPv4 address to string.");
  }
  return std::string(buf);
}

std::string SocketAddress::internal::Ipv6AddressToString(const struct in6_addr* addr) {
  char buf[INET6_ADDRSTRLEN];
  const char* ret = inet_ntop(AF_INET6, addr, buf, sizeof(buf));
  if (!ret) {
    throw std::invalid_argument("Ipv6AddressToString: Attempting to covert an invalid IPv6 address to string.");
  }
  return std::string(buf);
}

std::string Ipv4AddressToString(const std::array<std::uint8_t, kIpv4AddressLength>& address) {
  assert(kIpv4AddressLength >= INET_ADDRSTRLEN);
  return SocketAddress::internal::Ipv4AddressToString(reinterpret_cast<const struct in_addr*>(address.data()));
}

std::string Ipv6AddressToString(const std::array<std::uint8_t, kIpv6AddressLength>& address) {
  assert(kIpv6AddressLength >= INET6_ADDRSTRLEN);
  return SocketAddress::internal::Ipv6AddressToString(reinterpret_cast<const struct in6_addr*>(address.data()));
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_
