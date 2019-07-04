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
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp               visdap       Vector Informatik GmbH
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/address/ip_socket_address.h"

#include <netdb.h>
#include <memory>
#include <stdexcept>

#include "osabstraction/io/network/address/addrinfo_ptr.h"
#include "osabstraction/io/network/address/ipv4_socket_address.h"
#include "osabstraction/io/network/address/ipv6_socket_address.h"
#include "osabstraction/io/network/address/socket_address.h"
#include "osabstraction/io/network/socket/socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace address {

IPSocketAddress IPSocketAddress::FromHostAndPort(const std::string& hostname, const std::string& port) {
  printf("FromHostAndPort 0\n");  
  struct addrinfo hints;
  struct addrinfo* res = nullptr;
  const char* h = nullptr;
  const char* p = nullptr;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  printf("FromHostAndPort 1\n");
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
  hints.ai_protocol = 0;
  hints.ai_addrlen = 0;
  hints.ai_addr = nullptr;
  hints.ai_canonname = nullptr;
  hints.ai_next = nullptr;
  printf("FromHostAndPort 2\n");

  if (!hostname.empty()) {
    printf("FromHostAndPort 3\n");
    h = hostname.c_str();
  }

  if (!port.empty()) {
    printf("FromHostAndPort 4\n");
    p = port.c_str();
  }

  int ret = getaddrinfo(h, p, &hints, &res);
  printf("FromHostAndPort 5\n");

  if (ret != 0) {
    printf("FromHostAndPort 6\n");
    throw std::runtime_error(gai_strerror(ret));
  }

  if (res == nullptr) {
    printf("FromHostAndPort 7\n");
    throw std::runtime_error("IPSocketAddress::FromHostAndPort: could not resolve hostname " + hostname + ":" + port);
  }

  auto res_ptr = make_addrinfo_ptr(res);
  printf("FromHostAndPort 8\n");

  switch (SocketAddress::fromUnixSocketAddressFamily(res->ai_family)) {
    case SocketAddressFamily::kInet4:
        printf("FromHostAndPort 9\n");
      return IPV4SocketAddress(*res);
    case SocketAddressFamily::kInet6:
        printf("FromHostAndPort 10\n");
      return IPV6SocketAddress(*res);
    default:
        printf("FromHostAndPort 11\n");
      throw std::runtime_error("IPSocketAddress::FromHostAndPort: Address type not IPV4 or IPV6: " + hostname + ":" +
                               port);
  }
  //printf("FromHostAndPort 12\n");
}

IPSocketAddress IPSocketAddress::FromMulticastGroupAndPort(const std::string& interface_address,
                                                           const std::string& group_address, const std::string& port) {
   printf("FromMulticastGroupAndPort 0\n");                                                        
  struct addrinfo hints;
  struct addrinfo* ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  std::memset(&hints, 0, sizeof(hints));
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
  hints.ai_protocol = 0;
  hints.ai_addrlen = 0;
  hints.ai_addr = nullptr;
  hints.ai_canonname = nullptr;
  hints.ai_next = nullptr;
  printf("FromMulticastGroupAndPort 1\n");
  int ret = getaddrinfo(group_address.c_str(), port.c_str(), &hints, &ai);
  printf("FromMulticastGroupAndPort 2\n");
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (ai == nullptr) {
    throw std::runtime_error("IPSocketAddress::FromMulticastGroupAndPort: could not resolve address " + group_address +
                             ":" + port);
  }
  auto free_ai = [](struct addrinfo* ai_) { freeaddrinfo(ai_); };
  std::unique_ptr<struct addrinfo, decltype(free_ai)> ai_ptr{ai, free_ai};
  switch (SocketAddress::fromUnixSocketAddressFamily(ai->ai_family)) {
    case SocketAddressFamily::kInet4:
        printf("FromMulticastGroupAndPort kInet4\n");
      return IPV4SocketAddress(*ai);
    case SocketAddressFamily::kInet6: {
      auto index = osabstraction::io::network::socket::Socket::InterfaceAddressToIndex(interface_address);
      if (!index.first) {
        throw std::runtime_error("IPSocketAddress::FromMulticastGroupAndPort: could not resolve interface index for " +
                                 interface_address);
      }
      auto sin6 = reinterpret_cast<struct sockaddr_in6*>(ai->ai_addr);
      sin6->sin6_scope_id = index.second;
      return IPV6SocketAddress(*ai);
    }
    default:
      throw std::runtime_error("IPSocketAddress::FromMulticastGroupAndPort: Address type not IPV4 or IPV6: " +
                               group_address + ":" + port);
  }
}

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction
