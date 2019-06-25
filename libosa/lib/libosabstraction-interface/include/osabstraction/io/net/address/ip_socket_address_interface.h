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
 *        \brief  Abstract interface to work with IP addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstddef>  // size_t
#include <cstdint>  // uint8_t
#include <string>
#include "osabstraction/io/net/address/socket_address_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

/**
 * \brief Abstract interface to work with IP addresses.
 */
class IpSocketAddressInterface : public virtual SocketAddressInterface {};

/**
 * \brief Length of IPv4 address in bytes.
 */
static constexpr std::size_t kIpv4AddressLength = 16;

/**
 * \brief Converts an IPv4 address to string.
 * \param[in] address An IPv4 address.
 * \return String representation of the given IPv4 address.
 */
std::string Ipv4AddressToString(const std::array<std::uint8_t, kIpv4AddressLength>& address);

/**
 * \brief Length of IPv6 address in bytes.
 */
static constexpr std::size_t kIpv6AddressLength = 46;

/**
 * \brief Converts an IPv6 address to string.
 * \param[in] address An IPv6 address.
 * \return String representation of the given IPv6 address.
 */
std::string Ipv6AddressToString(const std::array<std::uint8_t, kIpv6AddressLength>& address);

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IP_SOCKET_ADDRESS_INTERFACE_H_
