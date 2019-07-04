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
 *        \brief  Socket address implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_
#define LIB_LIBOSABSTRACTION_POSIX_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/socket_address.h"
#include <netdb.h>       // NI_*
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // sockaddr_storage, AF_*
#include <sys/un.h>      // sockaddr_un
#include <cassert>       // assert()
#include <stdexcept>     // runtime_error()

namespace osabstraction {
namespace io {
namespace net {
namespace address {

SocketAddress::SocketAddress() : sockaddr_(), sockaddr_size_(0) { sockaddr_.ss_family = AF_UNSPEC; }

SocketAddress::SocketAddress(const struct sockaddr_storage& sockaddr) : sockaddr_(sockaddr), sockaddr_size_(0) {
  switch (sockaddr.ss_family) {
    case AF_UNIX:
      sockaddr_size_ = sizeof(struct sockaddr_un);
      break;
    case AF_INET:
      sockaddr_size_ = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
      sockaddr_size_ = sizeof(struct sockaddr_in6);
      break;
    default:
      sockaddr_size_ = 0;
      break;
  }
}

SocketAddress::SocketAddress(const struct sockaddr& sockaddr) : sockaddr_(), sockaddr_size_(0) {
  switch (sockaddr.sa_family) {
    case AF_UNIX:
      *reinterpret_cast<struct sockaddr_un*>(&sockaddr_) = *reinterpret_cast<const struct sockaddr_un*>(&sockaddr);
      sockaddr_size_ = sizeof(struct sockaddr_un);
      break;
    case AF_INET:
      *reinterpret_cast<struct sockaddr_in*>(&sockaddr_) = *reinterpret_cast<const struct sockaddr_in*>(&sockaddr);
      sockaddr_size_ = sizeof(struct sockaddr_in);
      break;
    case AF_INET6:
      *reinterpret_cast<struct sockaddr_in6*>(&sockaddr_) = *reinterpret_cast<const struct sockaddr_in6*>(&sockaddr);
      sockaddr_size_ = sizeof(struct sockaddr_in6);
      break;
    default:
      break;
  }
}

SocketAddressFamily SocketAddress::GetAddressFamily() const {
  return internal::fromUnixSocketAddressFamily(sockaddr_.ss_family);
}

std::string SocketAddress::toString() const {
  if (sockaddr_.ss_family == AF_UNIX) {
    auto sun = reinterpret_cast<const struct sockaddr_un*>(&sockaddr_);
    static_assert(std::is_array<decltype(sun->sun_path)>::value, "SocketAddress: sun_path isn't a char array");
    return sun->sun_path;
  } else if (sockaddr_.ss_family == AF_INET || sockaddr_.ss_family == AF_INET6) {
    auto p = toAddressPortStrings();
    return p.first + "," + p.second;
  } else {
    return "";
  }
}

std::pair<std::string, std::string> SocketAddress::toAddressPortStrings() const {
  assert(sockaddr_.ss_family == AF_INET || sockaddr_.ss_family == AF_INET6);
  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
  int ret =
      getnameinfo(toSockAddr(), Length(), hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  return std::make_pair(std::string(hbuf), std::string(sbuf));
}

int SocketAddress::internal::toUnixSocketAddressFamily(SocketAddressFamily family) {
  switch (family) {
    case SocketAddressFamily::kIpc:
      return AF_UNIX;
      break;
    case SocketAddressFamily::kInet4:
      return AF_INET;
      break;
    case SocketAddressFamily::kInet6:
      return AF_INET6;
      break;
    default:
      return AF_UNSPEC;
      break;
  }
}

SocketAddressFamily SocketAddress::internal::fromUnixSocketAddressFamily(int family) {
  switch (family) {
    case AF_UNIX:
      return SocketAddressFamily::kIpc;
      break;
    case AF_INET:
      return SocketAddressFamily::kInet4;
      break;
    case AF_INET6:
      return SocketAddressFamily::kInet6;
      break;
    default:
      return SocketAddressFamily::kUnspecified;
      break;
  }
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_SRC_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_CC_
