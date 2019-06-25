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
 *        \brief  Implementation of the socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/socket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>  // memset
#include <system_error>
#include "osabstraction/io/net/address/internal/addrinfo_ptr.h"
#include "osabstraction/io/net/address/internal/ifaddrs_ptr.h"
#include "osabstraction/io/net/address/socket_address.h"
#include "osabstraction/io/net/socket/socket_eof_exception.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

Socket::Socket(int handle) : FileDescriptor(handle), shall_unlink_(false) {
  int type = 0;
  socklen_t size = sizeof(type);
  int ret = getsockopt(handle_, SOL_SOCKET, SO_TYPE, &type, &size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  type_ = Socket::toSocketType(type);
}

Socket::Socket(address::SocketAddressFamily family, SocketType type) : type_{type} {
  int ret =
      ::socket(address::SocketAddress::internal::toUnixSocketAddressFamily(family), Socket::toUnixSocketType(type), 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  handle_ = ret;
}

void Socket::Bind(const address::SocketAddress& local_address) {
  int ret = ::bind(handle_, local_address.toSockAddr(), local_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void Socket::Close() { FileDescriptor::Close(); }

address::SocketAddress Socket::GetLocalAddress() const {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::getsockname(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return address::SocketAddress(sockaddr);
}

address::SocketAddress Socket::GetRemoteAddress() const {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  int ret = ::getpeername(handle_, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return address::SocketAddress(sockaddr);
}

std::size_t Socket::Peek(void* buffer, std::size_t size) { return Receive(buffer, size, MSG_PEEK); }

std::size_t Socket::Receive(void* buffer, std::size_t size) { return Receive(buffer, size, 0); }

std::size_t Socket::Receive(void* buffer, std::size_t size, int flags) {
  ssize_t ret = ::recvfrom(handle_, buffer, size, flags, nullptr, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if ((type_ == SocketType::kStream) && (ret == 0)) {
    throw SocketEOFException();
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t Socket::Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Receive(buffer, size, remote_address, MSG_PEEK);
}

std::size_t Socket::Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) {
  return Receive(buffer, size, remote_address, 0);
}

std::size_t Socket::Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address, int flags) {
  struct sockaddr_storage sockaddr;
  socklen_t sockaddr_size = sizeof(sockaddr);
  ssize_t ret = ::recvfrom(handle_, buffer, size, flags, reinterpret_cast<struct sockaddr*>(&sockaddr), &sockaddr_size);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if ((type_ == SocketType::kStream) && (ret == 0)) {
    throw SocketEOFException();
  }
  remote_address = address::SocketAddress(sockaddr);
  return ret;
}

std::size_t Socket::Peek(const IovecContainer iovec) { return Receive(iovec, MSG_PEEK); }

std::size_t Socket::Receive(const IovecContainer iovec) { return Receive(iovec, 0); }

std::size_t Socket::Receive(const IovecContainer iovec, int flags) {
  struct msghdr msg;
  std::memset(&msg, 0, sizeof(msg));
  msg.msg_iov = const_cast<struct iovec*>(iovec.data());
  msg.msg_iovlen = iovec.size();
  ssize_t ret = ::recvmsg(handle_, &msg, flags);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if ((type_ == SocketType::kStream) && (ret == 0)) {
    throw SocketEOFException();
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t Socket::Peek(const IovecContainer iovec, address::SocketAddress& remote_address) {
  return Receive(iovec, remote_address, MSG_PEEK);
}

std::size_t Socket::Receive(const IovecContainer iovec, address::SocketAddress& remote_address) {
  return Receive(iovec, remote_address, 0);
}

std::size_t Socket::Receive(const IovecContainer iovec, address::SocketAddress& remote_address, int flags) {
  struct msghdr msg;
  struct sockaddr_storage sockaddr;
  std::memset(&msg, 0, sizeof(msg));
  msg.msg_name = &sockaddr;
  msg.msg_namelen = sizeof(sockaddr);
  msg.msg_iov = const_cast<struct iovec*>(iovec.data());
  msg.msg_iovlen = iovec.size();
  ssize_t ret = ::recvmsg(handle_, &msg, flags);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  } else if ((type_ == SocketType::kStream) && (ret == 0)) {
    throw SocketEOFException();
  }
  remote_address = address::SocketAddress(sockaddr);
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t Socket::Send(const void* buffer, std::size_t size) {
  ssize_t ret = ::sendto(handle_, buffer, size, 0, nullptr, 0);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t Socket::Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) {
  ssize_t ret = ::sendto(handle_, buffer, size, 0, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return static_cast<typename std::make_unsigned<ssize_t>::type>(ret);
}

std::size_t Socket::Send(const IovecContainer iovec) {
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

std::size_t Socket::Send(const IovecContainer iovec, const address::SocketAddress& remote_address) {
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

void Socket::SetReuseAddress(bool reuse) {
  int flag = reuse;
  int ret = ::setsockopt(handle_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

bool Socket::IsConnected() {
  char dummy = 0;
  ssize_t ret = ::read(handle_, &dummy, 0);
  return ret == 0;
}

std::pair<bool, unsigned int> Socket::internal::InterfaceAddressToIndex(const vac::container::string_view& address) {
  struct addrinfo hints {};
  struct addrinfo* ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = getaddrinfo(address.data(), nullptr, &hints, &ai);
  if (ret != 0) {
    return {false, 0};
  }
  if (ai == nullptr) {
    return {false, 0};
  }
  auto ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(ai);
  struct ifaddrs* ifa;
  ret = getifaddrs(&ifa);
  if (ret < 0) {
    return {false, 0};
  }
  auto ifa_ptr = osabstraction::io::net::address::internal::make_ifaddrs_ptr(ifa);
  for (; ifa != nullptr; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) {
      continue;
    }
    if (ifa->ifa_addr->sa_family != ai->ai_family) {
      continue;
    }
    if (std::memcmp(ifa->ifa_addr, ai->ai_addr, ai->ai_addrlen) == 0) {
      return {true, if_nametoindex(ifa->ifa_name)};
    }
  }
  return {false, 0};
}

void Socket::SetPriority(const int priority) {
  const int ret = ::setsockopt(handle_, SOL_SOCKET, SO_PRIORITY, &priority, sizeof(priority));

  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
