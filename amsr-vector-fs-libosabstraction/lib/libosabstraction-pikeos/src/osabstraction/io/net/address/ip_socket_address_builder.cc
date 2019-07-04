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
#include <lwip/netdb.h>  // getaddrinfo(), freeaddrinfo(), gai_strerror()
#include "osabstraction/io/net/address/ip_socket_address.h"

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
  hints.ai_socktype = SOCK_DGRAM;

  if (!hostname.empty()) {
    h = hostname.data();
  }

  if (!port.empty()) {
    p = port.data();
  }

  /* #00 Parse address. */
  int ret = getaddrinfo(h, p, &hints, &res);

  /* #00 If parsing fails, throw exception. */
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }

  if (res == nullptr) {
    throw std::runtime_error("IpSocketAddressBuilder::BuildFromHostAndPort: could not resolve hostname " +
                             std::string(hostname.data()) + ":" + std::string(port.data()));
  }

  /* #00 Create socket depending on address family. */
  /* #00 If address family is not IPv4 or IPv6, throw exception. */
  switch (res->ai_family) {
    case AF_INET:
    case AF_INET6:
      return IpSocketAddress(*res);
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
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICHOST | AI_NUMERICSERV;

  /* #00 Parse address. */
  int ret = getaddrinfo(group_address.data(), port.data(), &hints, &ai);

  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (ai == nullptr) {
    throw std::runtime_error("IpSocketAddressBuilder::BuildFromMulticastGroupAndPort: could not resolve address " +
                             std::string(group_address.data()) + ":" + std::string(port.data()));
  }

  /* #00 Create socket depending on address family. */
  /* #00 Multicast only supported for IPv4. Throw exception in case of other address family. */
  switch (ai->ai_family) {
    case AF_INET:
      return IpSocketAddress(*ai);
    case AF_INET6: {
      throw std::runtime_error(
          "IpSocketAddressBuilder::BuildFromMulticastGroupAndPort: IPv6 multicast not supported: " +
          std::string(interface_address.data()) + std::string(group_address.data()) + ":" + std::string(port.data()));
    }
    default:
      throw std::runtime_error("IPSocketAddress::FromMulticastGroupAndPort: Address type not IPV4 or IPV6: " +
                               std::string(interface_address.data()) + std::string(group_address.data()) + ":" +
                               std::string(port.data()));
  }
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
