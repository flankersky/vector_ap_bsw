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
#include <cassert>
#include <system_error>

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void IpcStreamServerSocket::Bind(const address::SocketAddress& local_address) {
  IpcSocket::Bind(local_address);
  port_.Open(true);
}

void IpcStreamServerSocket::Listen(int backlog) {
  assert(local_addr_.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  (void)backlog;

  port_.Listen();
  remote_addr_ = local_addr_;
}

IpcStreamSocket IpcStreamServerSocket::Accept(address::IpcSocketAddress& remote_address) {
  assert(local_addr_.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  std::pair<qport_sap_sockaddr_ipv4_t, std::uint32_t> accept_info = port_.Accept();

  remote_address = address::IpcSocketAddress(accept_info.first);

  return IpcStreamSocket(local_addr_, remote_address, accept_info.second);
}

IpcStreamSocket IpcStreamServerSocket::Accept() {
  address::IpcSocketAddress dont_care;
  return IpcStreamServerSocket::Accept(dont_care);
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
