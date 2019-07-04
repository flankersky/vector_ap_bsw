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
 *        \brief  Implements class to work with IPv4 socket addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_IPV4_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_IPV4_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <netdb.h>  // addrinfo
#include <cstring>  // memcpy()
#include "osabstraction/io/net/address/ip_socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {
namespace internal {

/**
 * \brief Implements class to work with IPv4 socket addresses.
 */
class Ipv4SocketAddress : public IpSocketAddress {
 public:
  /**
   * \brief Create IPv4 socket address.
   *
   * \param addr A pointer to an IPv4 addrinfo struct.
   */
  explicit Ipv4SocketAddress(const struct addrinfo& addr) {
    if (SocketAddress::internal::fromUnixSocketAddressFamily(addr.ai_family) == SocketAddressFamily::kInet4) {
      std::memcpy(&sockaddr_, addr.ai_addr, addr.ai_addrlen);
      sockaddr_size_ = addr.ai_addrlen;
    } else {
      throw std::invalid_argument("Ipv4SocketAddress: Attempting to construct from addrinfo of the wrong family.");
    }
  }

  /**
   * \brief Converter to sockaddr_in.
   *
   * \return A pointer to sockaddr_in.
   */
  const struct sockaddr_in* toSockAddrIn() const { return reinterpret_cast<const struct sockaddr_in*>(&sockaddr_); }
};

}  // namespace internal
}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_IPV4_SOCKET_ADDRESS_H_
