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
#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IPV6_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IPV6_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <netdb.h>
#include <array>
#include <string>
#include "osabstraction/io/network/address/ip_socket_address.h"

namespace osabstraction {
namespace io {
namespace network {
namespace address {

/**
 * \brief Class for representing an IPv6 Address
 */
class IPV6SocketAddress : public IPSocketAddress {
 public:
  /**
   * \brief Constructor of SocketAddress.
   *
   * \param sin6 A pointer to a IPv6 socket address.
   */
  explicit IPV6SocketAddress(const struct sockaddr_in6* sin6);

  /**
   * \brief Constructor of SocketAddress.
   *
   * \param addr A pointer to an IPv6 addrinfo struct.
   */
  explicit IPV6SocketAddress(const struct ::addrinfo& addr);

  /**
   * \brief Converter to sockaddr_in6.
   *
   * \return A pointer to sockaddr_in6.
   */
  const struct sockaddr_in6* toSockAddrIn6() const { return reinterpret_cast<const struct sockaddr_in6*>(&sockaddr_); }

  /**
   * \brief Converter to sockaddr_in6.
   *
   * \return A pointer to sockaddr_in6.
   */
  struct sockaddr_in6* toSockAddrIn6() {
    return reinterpret_cast<struct sockaddr_in6*>(&sockaddr_);
  }
  /**
   * \brief Length of IPv6 address in bytes.
   */
  static constexpr std::size_t kAddressLength = 16;
  /**
   * \brief Converts an IPv6 address to string.
   *
   * \param address An IPv6 address.
   * \return String representation of the given IPv6 address.
   */
  static std::string AddressToString(const std::array<std::uint8_t, kAddressLength>& address);
};

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IPV6_SOCKET_ADDRESS_H_
