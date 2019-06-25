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
 *        \brief  Implementation of the socket interface for IP sockets.
 *
 *      \details  Based on POSIX implementation.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ip_socket.h"
#include <sys/socket.h>  // POSIX socket API
#include <sys/types.h>   // ssize_t
#include <cstddef>       // size_t
#include <cstring>       // memset()
#include <system_error>
#include <type_traits>  // make_unsigned<>
#include "osabstraction/io/net/socket/socket_eof_exception.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief Converts a socket type to the corresponding Unix socket type.
 *
 * \param[in] type A socket type.
 * \return The corresponding Unix socket type.
 */
static int ToUnixSocketType(SocketType type) {
  if (type == SocketType::kStream) {
    return SOCK_STREAM;
  } else {
    return SOCK_DGRAM;
  }
}

IpSocket::IpSocket(int handle) : FileDescriptor(handle) {
  int type = 0;
  socklen_t size = sizeof(type);
  int ret = getsockopt(handle_, SOL_SOCKET, SO_TYPE, &type, &size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

IpSocket::IpSocket(address::SocketAddressFamily family, SocketType type) {
  int ret = ::socket(address::SocketAddress::internal::toUnixSocketAddressFamily(family), ToUnixSocketType(type), 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  handle_ = ret;
}

void IpSocket::Bind(const address::SocketAddress& local_address) {
  int ret = ::bind(handle_, local_address.toSockAddr(), local_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

address::SocketAddress IpSocket::GetLocalAddress() const {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::getsockname(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return address::SocketAddress(sockaddr);
}

address::SocketAddress IpSocket::GetRemoteAddress() const {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::getpeername(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return address::SocketAddress(sockaddr);
}

std::size_t IpSocket::Peek(void* buffer, std::size_t size) { return Receive(buffer, size, MSG_PEEK); }

std::size_t IpSocket::Receive(void* buffer, std::size_t size) { return Receive(buffer, size, 0); }

std::size_t IpSocket::Receive(void* buffer, std::size_t size, int flags) {
  ssize_t ret = ::recvfrom(handle_, buffer, size, flags, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if (ret == 0) {
    throw SocketEOFException();
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t IpSocket::Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Receive(buffer, size, remote_address, MSG_PEEK);
}

std::size_t IpSocket::Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Receive(buffer, size, remote_address, 0);
}

std::size_t IpSocket::Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address, int flags) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  ssize_t ret = ::recvfrom(handle_, buffer, size, flags, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if (ret == 0) {
    throw SocketEOFException();
  }
  remote_address = address::SocketAddress(sockaddr);
  return ret;
}

std::size_t IpSocket::Peek(const IovecContainer iovec) { return Receive(iovec, MSG_PEEK); }

std::size_t IpSocket::Receive(const IovecContainer iovec) { return Receive(iovec, 0); }

std::size_t IpSocket::Receive(const IovecContainer iovec, int flags) {
  std::size_t bytes_received = 0;

  for (const auto& ioop : iovec) {
    bytes_received += Receive(ioop.iov_base, ioop.iov_len, flags);
  }

  return bytes_received;
}

std::size_t IpSocket::Peek(const IovecContainer iovec, address::SocketAddress& remote_address) {
  return Receive(iovec, remote_address, MSG_PEEK);
}

std::size_t IpSocket::Receive(const IovecContainer iovec, address::SocketAddress& remote_address) {
  return Receive(iovec, remote_address, 0);
}

std::size_t IpSocket::Receive(const IovecContainer iovec, address::SocketAddress& remote_address, int flags) {
  std::size_t bytes_received = 0;

  for (const auto& ioop : iovec) {
    bytes_received += Receive(ioop.iov_base, ioop.iov_len, remote_address, flags);
  }

  return bytes_received;
}

std::size_t IpSocket::Send(const void* buffer, std::size_t size) {
  ssize_t ret = ::sendto(handle_, buffer, size, 0, nullptr, 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t IpSocket::Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) {
  ssize_t ret = ::sendto(handle_, buffer, size, 0, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t IpSocket::Send(const IovecContainer iovec) {
  struct msghdr msg;
  std::memset(&msg, 0, sizeof(msg));
  msg.msg_iov = const_cast<struct iovec*>(iovec.data());
  msg.msg_iovlen = iovec.size();

  ssize_t ret = ::sendmsg(handle_, &msg, 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t IpSocket::Send(const IovecContainer iovec, const address::SocketAddress& remote_address) {
  struct msghdr msg;
  std::memset(&msg, 0, sizeof(msg));
  msg.msg_name = const_cast<struct sockaddr*>(remote_address.toSockAddr());
  msg.msg_namelen = remote_address.Length();
  msg.msg_iov = const_cast<struct iovec*>(iovec.data());
  msg.msg_iovlen = iovec.size();

  ssize_t ret = ::sendmsg(handle_, &msg, 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

void IpSocket::SetReuseAddress(bool reuse) {
  int flag = reuse;
  int ret = ::setsockopt(handle_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

bool IpSocket::IsConnected() {
  char dummy = 0;
  // POSIX implementation uses read(). This does not work for LwIP sockets,
  // because LwIP socket handles are different to regular file handles.
  // Therefore read() operation is not available for LwIP sockets.
  // This implementation uses recv() instead.
  // According to POSIX recv returns 0 in case of peer shutdown.
  std::size_t ret = ::recv(handle_, &dummy, 0, MSG_PEEK);
  return ret == 0;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
