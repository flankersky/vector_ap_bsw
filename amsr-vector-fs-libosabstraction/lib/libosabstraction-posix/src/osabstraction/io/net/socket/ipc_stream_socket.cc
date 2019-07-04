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
 *        \brief  Implementation for stream oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ipc_stream_socket.h"
#include <unistd.h>  // unlink()
#include <cassert>
#include <system_error>
#include "osabstraction/io/net/address/ipc_socket_address.h"
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void IpcStreamSocket::Connect(const address::SocketAddress& remote_address) {
  int ret = ::connect(handle_, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void IpcStreamSocket::Close() {
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
