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
 *        \brief  Implementation for datagram oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ipc_datagram_socket.h"
#include <cassert>
#include <system_error>
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void IpcDatagramSocket::Bind(const address::SocketAddress& local_address) {
  IpcSocket::Bind(local_address);
  connected_ = true;
  port_.Open(false);
}

void IpcDatagramSocket::Connect(const address::SocketAddress& remote_address) {
  assert(local_addr_.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  assert(remote_address.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  remote_addr_ = remote_address;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
