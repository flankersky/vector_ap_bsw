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
#include "osabstraction/io/network/socket/tcp_server_socket.h"

#include <system_error>

#include "osabstraction/io/network/socket/tcp_socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

TCPSocket TCPServerSocket::Accept() {
  int ret = ::accept(handle_, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return TCPSocket(ret);
}

TCPSocket TCPServerSocket::Accept(address::IPSocketAddress* remote_address) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::accept(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  *remote_address = address::IPSocketAddress(&sockaddr);
  return TCPSocket(ret);
}

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction
