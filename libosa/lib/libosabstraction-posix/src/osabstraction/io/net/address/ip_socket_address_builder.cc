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
/**        \file
 *        \brief  Implements creation of IP socket addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/ip_socket_address_builder.h"
#include <netdb.h>  // addrinfo, getaddrinfo()
#include <netinet/in.h>
#include <sys/socket.h>  // AF_*
#include "osabstraction/io/net/address/internal/addrinfo_ptr.h"
#include "osabstraction/io/net/address/internal/ipv4_socket_address.h"
#include "osabstraction/io/net/address/internal/ipv6_socket_address.h"
#include "osabstraction/io/net/address/ip_socket_address.h"
#include "osabstraction/io/net/socket/socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

IpSocketAddress IpSocketAddressBuilder::BuildFromHostAndPort(const vac::container::string_view& hostname,
                                                             const vac::container::string_view& port) {
  struct addrinfo hints {};
  struct addrinfo* res = nullptr;
  const char* h = nullptr;
  const char* p = nullptr;

  hints.ai_family = AF_UNSPEC;
  /*
   * Use socket type SOCK_DGRAM.
   *
   * This workaround is required for QNX.
   * It doesn't matter on Linux because the socket type is never used after address resolution.
   *
   * Excerpt from NetBSD's getaddrinfo man page:
   *   If you specify a numeric servname, while leaving ai_socktype and ai_protocol unspecified, getaddrinfo() will
   *   raise an error. This is because the numeric servname does not identify any socket type, and getaddrinfo()
   *   is not allowed to glob the argument in such case.
   */
  hints.ai_socktype = SOCK_DGRAM;

  if (!hostname.empty()) {
    h = hostname.data();
  }

  if (!port.empty()) {
    p = port.data();
  }

  int ret = getaddrinfo(h, p, &hints, &res);

  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }

  if (res == nullptr) {
    throw std::runtime_error("IpSocketAddressBuilder::BuildFromHostAndPort: could not resolve hostname " +
                             std::string(hostname.data()) + ":" + std::string(port.data()));
  }

  auto res_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(res);

  switch (SocketAddress::internal::fromUnixSocketAddressFamily(res->ai_family)) {
    case SocketAddressFamily::kInet4:
      return internal::Ipv4SocketAddress(*res);
    case SocketAddressFamily::kInet6:
      return internal::Ipv6SocketAddress(*res);
    default:
      throw std::runtime_error("IpSocketAddressBuilder::BuildFromHostAndPort: Address type not IPV4 or IPV6: " +
                               std::string(hostname.data()) + ":" + std::string(port.data()));
  }
}

IpSocketAddress IpSocketAddressBuilder::BuildFromMulticastGroupAndPort(
    const vac::container::string_view& interface_address, const vac::container::string_view& group_address,
    const vac::container::string_view& port) {
  struct addrinfo hints {};
  struct addrinfo* ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  /*
   * Use socket type SOCK_DGRAM.
   *
   * This workaround is required for QNX.
   * It doesn't matter on Linux because the socket type is never used after address resolution.
   *
   * Excerpt from NetBSD's getaddrinfo man page:
   *   If you specify a numeric servname, while leaving ai_socktype and ai_protocol unspecified, getaddrinfo() will
   *   raise an error. This is because the numeric servname does not identify any socket type, and getaddrinfo()
   *   is not allowed to glob the argument in such case.
   */
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;
  int ret = getaddrinfo(group_address.data(), port.data(), &hints, &ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (ai == nullptr) {
    throw std::runtime_error("IpSocketAddressBuilder::BuildFromMulticastGroupAndPort: could not resolve address " +
                             std::string(group_address.data()) + ":" + std::string(port.data()));
  }
  std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> ai_ptr{ai, &freeaddrinfo};
  switch (SocketAddress::internal::fromUnixSocketAddressFamily(ai->ai_family)) {
    case SocketAddressFamily::kInet4:
      return internal::Ipv4SocketAddress(*ai);
    case SocketAddressFamily::kInet6: {
      auto index = osabstraction::io::net::socket::Socket::internal::InterfaceAddressToIndex(interface_address);
      if (!index.first) {
        throw std::runtime_error(
            "IpSocketAddressBuilder::BuildFromMulticastGroupAndPort: could not resolve interface index for " +
            std::string(interface_address.data()));
      }
      auto sin6 = reinterpret_cast<struct sockaddr_in6*>(ai->ai_addr);
      sin6->sin6_scope_id = index.second;
      return internal::Ipv6SocketAddress(*ai);
    }
    default:
      throw std::runtime_error("IPSocketAddress::FromMulticastGroupAndPort: Address type not IPV4 or IPV6: " +
                               std::string(group_address.data()) + ":" + std::string(port.data()));
  }
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
