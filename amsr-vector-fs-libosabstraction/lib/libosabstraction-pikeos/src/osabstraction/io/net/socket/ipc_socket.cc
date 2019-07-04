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
 *        \brief  Implementation of the IPC socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ipc_socket.h"
#include <stddef.h>     // size_t
#include <sys/qport.h>  // qport_sap_*
#include <unistd.h>     // close()
#include <cassert>
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

IpcSocket::IpcSocket() : FileDescriptor(kInvalidFileDescriptor) {}

IpcSocket::IpcSocket(const address::SocketAddress& local_address, const address::SocketAddress& remote_address,
                     std::uint32_t id)
    : FileDescriptor(kInvalidFileDescriptor),
      port_(id),
      local_addr_(local_address),
      remote_addr_(remote_address),
      connected_(true) {}

void IpcSocket::Bind(const address::SocketAddress& local_address) {
  assert(local_address.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  local_addr_ = local_address;
  assert(local_addr_.GetAddressFamily() == address::SocketAddressFamily::kIpc);
}

address::SocketAddress IpcSocket::GetLocalAddress() const { return address::SocketAddress{local_addr_}; }

address::SocketAddress IpcSocket::GetRemoteAddress() const { return address::SocketAddress{remote_addr_}; }

std::size_t IpcSocket::Peek(void* buffer, std::size_t size) {
  address::SocketAddress dummy;
  return Read(buffer, size, dummy, true);
}

std::size_t IpcSocket::Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Read(buffer, size, remote_address, true);
}

std::size_t IpcSocket::Peek(const IovecContainer iovec) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Peek(ioop.iov_base, ioop.iov_len);
  }
  return result;
}

std::size_t IpcSocket::Peek(const IovecContainer iovec, address::SocketAddress& remote_address) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Peek(ioop.iov_base, ioop.iov_len, remote_address);
  }
  return result;
}

std::size_t IpcSocket::Receive(void* buffer, std::size_t size) {
  address::SocketAddress dummy;
  return Read(buffer, size, dummy, false);
}

std::size_t IpcSocket::Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Read(buffer, size, remote_address, false);
}

std::size_t IpcSocket::Receive(const IovecContainer iovec) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Receive(ioop.iov_base, ioop.iov_len);
  }
  return result;
}

std::size_t IpcSocket::Receive(const IovecContainer iovec, address::SocketAddress& remote_address) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Receive(ioop.iov_base, ioop.iov_len, remote_address);
  }
  return result;
}

std::size_t IpcSocket::Send(const void* buffer, std::size_t size) { return Send(buffer, size, remote_addr_); }

std::size_t IpcSocket::Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) {
  assert(remote_address.GetAddressFamily() == address::SocketAddressFamily::kIpc);
  assert(connected_);

  return port_.Send(buffer, size, *reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(remote_address.toSockAddr()));
}

std::size_t IpcSocket::Send(const IovecContainer iovec) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Send(ioop.iov_base, ioop.iov_len);
  }
  return result;
}

std::size_t IpcSocket::Send(const IovecContainer iovec, const address::SocketAddress& remote_address) {
  std::size_t result = 0;
  for (const auto& ioop : iovec) {
    result += Send(ioop.iov_base, ioop.iov_len, remote_address);
  }
  return result;
}

std::size_t IpcSocket::Read(void* buffer, std::size_t size, address::SocketAddress& remote_address, bool peek) {
  assert(connected_);

  std::size_t ret;

  if (peek) {
    ret = port_.Peek(buffer, size, *reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(remote_addr_.toSockAddr()),
                     *reinterpret_cast<qport_sap_sockaddr_ipv4_t*>(const_cast<sockaddr*>(remote_address.toSockAddr())));
  } else {
    ret = port_.Receive(
        buffer, size, *reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(remote_addr_.toSockAddr()),
        *reinterpret_cast<qport_sap_sockaddr_ipv4_t*>(const_cast<sockaddr*>(remote_address.toSockAddr())));
  }

  return ret;
}

void IpcSocket::SetBlockingMode(bool blocking) { port_.SetBlocking(blocking); }

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
