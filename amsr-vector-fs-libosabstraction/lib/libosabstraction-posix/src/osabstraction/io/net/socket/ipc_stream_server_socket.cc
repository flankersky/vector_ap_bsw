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
 *        \brief  Implementation of the stream oriented IPC server socket.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/net/socket/ipc_stream_server_socket.h>
#include <osabstraction/io/net/socket/ipc_stream_socket.h>
#include <unistd.h>  // unlink()
#include <cassert>
#include <system_error>
#include "osabstraction/io/net/address/ipc_socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void IpcStreamServerSocket::Listen(int backlog) {
  int ret = ::listen(handle_, backlog);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

IpcStreamSocket IpcStreamServerSocket::Accept() {
  int ret = ::accept(handle_, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return IpcStreamSocket(ret);
}

IpcStreamSocket IpcStreamServerSocket::Accept(address::IpcSocketAddress& remote_address) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::accept(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  remote_address = address::IpcSocketAddress(*reinterpret_cast<struct sockaddr_un*>(&sockaddr));
  return IpcStreamSocket(ret);
}

void IpcStreamServerSocket::Close() {
  if (shall_unlink_ && handle_ != kInvalidFileDescriptor) {
    address::SocketAddress local_address = GetLocalAddress();
    assert(local_address.toSockAddr()->sa_family == AF_UNIX);
    address::IpcSocketAddress* udsaddr = static_cast<address::IpcSocketAddress*>(&local_address);
    ::unlink(udsaddr->toSockAddrUn()->sun_path);
  }
  FileDescriptor::Close();
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
