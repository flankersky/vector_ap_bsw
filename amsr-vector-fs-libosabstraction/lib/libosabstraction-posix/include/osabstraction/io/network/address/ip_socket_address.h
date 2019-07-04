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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IP_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IP_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "osabstraction/io/network/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace network {
namespace address {

/**
 * \brief Abstract class for sockaddrs representing any sort of IP address
 */
class IPSocketAddress : public SocketAddress {
 public:
  using SocketAddress::SocketAddress;

  /**
   * \brief Constructor of IPSocketAddress.
   *
   * \param hostname An IPv4/IPv6 address or a DNS name of a host.
   * \param port A socket port number.
   * \return An IPSocketAddress object
   */
  static IPSocketAddress FromHostAndPort(const std::string& hostname, const std::string& port);

  /**
   * \brief Constructor of IPSocketAddress.
   *
   * \param interface_address An unicast IPv4/IPv6 address which identifies a network interface.
   * \param group_address A multicast IPv4/IPv6 address.
   * \param port A socket port number.
   * \return An IPSocketAddress object
   */
  static IPSocketAddress FromMulticastGroupAndPort(const std::string& interface_address,
                                                   const std::string& group_address, const std::string& port);
};

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IP_SOCKET_ADDRESS_H_
