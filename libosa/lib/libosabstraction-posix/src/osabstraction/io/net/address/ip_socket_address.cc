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
 *        \brief  IP socket address to string conversion functions.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/ip_socket_address.h"
#include <arpa/inet.h>  // inet_ntop(), INET_*
#include <stdexcept>    // invalid_argument()
#include <string>

namespace osabstraction {
namespace io {
namespace net {
namespace address {

std::string Ipv4AddressToString(const std::array<std::uint8_t, kIpv4AddressLength>& address) {
  char buf[INET_ADDRSTRLEN];
  const char* ret = inet_ntop(AF_INET, &address[0], buf, sizeof(buf));
  if (!ret) {
    throw std::invalid_argument("Ipv4AddressToString: Attempting to convert an invalid IPv4 address to string.");
  }
  return std::string(buf);
}

std::string Ipv6AddressToString(const std::array<std::uint8_t, kIpv6AddressLength>& address) {
  char buf[INET6_ADDRSTRLEN];
  const char* ret = inet_ntop(AF_INET6, &address[0], buf, sizeof(buf));
  if (!ret) {
    throw std::invalid_argument("Ipv6AddressToString: Attempting to convert an invalid IPv6 address to string.");
  }
  return std::string(buf);
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
