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
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/address/ipv4_socket_address.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>

namespace osabstraction {
namespace io {
namespace network {
namespace address {

IPV4SocketAddress::IPV4SocketAddress(const struct sockaddr_in* sin) {
  *reinterpret_cast<struct sockaddr_in*>(&sockaddr_) = *sin;
  sockaddr_size_ = sizeof(*sin);
}

IPV4SocketAddress::IPV4SocketAddress(const struct ::addrinfo& addr) {
  if (SocketAddress::fromUnixSocketAddressFamily(addr.ai_family) == SocketAddressFamily::kInet4) {
    std::memcpy(&sockaddr_, addr.ai_addr, addr.ai_addrlen);
    sockaddr_size_ = addr.ai_addrlen;
  } else {
    throw std::invalid_argument("IPV4SocketAddress: Attempting to construct from addrinfo of the wrong family.");
  }
}

std::string IPV4SocketAddress::AddressToString(const std::array<std::uint8_t, kAddressLength>& address) {
  char buf[INET_ADDRSTRLEN];
  const char* ret = inet_ntop(AF_INET, &address[0], buf, sizeof(buf));
  if (!ret) {
    throw std::invalid_argument("IPV4SocketAddress: Attempting to convert an invalid IPv4 address to string.");
  }
  return std::string(buf);
}

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction
