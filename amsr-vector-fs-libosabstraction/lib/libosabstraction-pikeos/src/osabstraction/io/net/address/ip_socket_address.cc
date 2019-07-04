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
 *        \brief  IP socket address implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/ip_socket_address.h"
#include <sys/socket.h>
#include <cassert>  // assert()
#include <cstring>  // memcpy()

namespace osabstraction {
namespace io {
namespace net {
namespace address {

IpSocketAddress::IpSocketAddress(const struct addrinfo& addr) {
  assert((addr.ai_family == AF_INET) || (addr.ai_family == AF_INET6));

  std::memcpy(&sockaddr_, addr.ai_addr, addr.ai_addrlen);
  sockaddr_size_ = addr.ai_addrlen;
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
