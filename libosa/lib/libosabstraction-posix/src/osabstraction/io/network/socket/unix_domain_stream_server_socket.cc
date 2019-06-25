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
#include "osabstraction/io/network/socket/unix_domain_stream_server_socket.h"

#include <unistd.h>
#include <cassert>
#include <system_error>

#include "osabstraction/io/network/address/unix_domain_socket_address.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

UnixDomainStreamServerSocket::~UnixDomainStreamServerSocket() { Close(); }

void UnixDomainStreamServerSocket::Close() {
  if (shall_unlink_ && handle_ != kInvalidFileDescriptor) {
    address::SocketAddress local_address = GetLocalAddress();
    assert(local_address.toSockAddr()->sa_family == AF_UNIX);
    address::UnixDomainSocketAddress* udsaddr = static_cast<address::UnixDomainSocketAddress*>(&local_address);
    ::unlink(udsaddr->toSockAddrUn()->sun_path);
  }
  FileDescriptor::Close();
}

UnixDomainStreamSocket UnixDomainStreamServerSocket::Accept() {
  int ret = ::accept(handle_, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return UnixDomainStreamSocket(ret);
}

UnixDomainStreamSocket UnixDomainStreamServerSocket::Accept(address::UnixDomainSocketAddress* remote_address) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::accept(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  *remote_address = address::UnixDomainSocketAddress(reinterpret_cast<struct sockaddr_un*>(&sockaddr));
  return UnixDomainStreamSocket(ret);
}

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction
