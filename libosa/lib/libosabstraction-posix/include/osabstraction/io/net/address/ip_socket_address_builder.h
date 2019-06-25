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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_BUILDER_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/ip_socket_address_builder_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

/**
 * \brief  Implements creation of IP socket addresses.
 */
class IpSocketAddressBuilder : public IpSocketAddressBuilderInterface {
 public:
  /**
   * \brief Creates an IP socket address from host name and port.
   * \param[in] hostname  An IPv4/IPv6 address or a DNS name of a host.
   * \param[in] port      A socket port number.
   * \return An IP socket address.
   */
  IpSocketAddress BuildFromHostAndPort(const vac::container::string_view& hostname,
                                       const vac::container::string_view& port) override;

  /**
   * \brief Creates an IP socket address from an interface address, group address and port.
   * \param interface_address   An unicast IPv4/IPv6 address which identifies a network interface.
   * \param group_address       A multicast IPv4/IPv6 address.
   * \param port                A socket port number.
   * \return An IP socket address
   */
  IpSocketAddress BuildFromMulticastGroupAndPort(const vac::container::string_view& interface_address,
                                                 const vac::container::string_view& group_address,
                                                 const vac::container::string_view& port) override;
};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_BUILDER_H_
