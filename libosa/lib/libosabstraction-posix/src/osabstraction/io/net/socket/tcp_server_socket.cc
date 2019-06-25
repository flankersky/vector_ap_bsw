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
 *        \brief  Implementation of the TCP server socket.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/tcp_server_socket.h"
#include <netinet/in.h>
#include <osabstraction/io/net/socket/tcp_socket.h>
#include <sys/socket.h>
#include <system_error>
#include "osabstraction/io/net/address/ip_socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void TcpServerSocket::Listen(int backlog) {
  int ret = ::listen(handle_, backlog);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

TcpSocket TcpServerSocket::Accept() {
  int ret = ::accept(handle_, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return TcpSocket(ret);
}

TcpSocket TcpServerSocket::Accept(address::IpSocketAddress& remote_address) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::accept(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  remote_address = address::IpSocketAddress(sockaddr);
  return TcpSocket(ret);
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
