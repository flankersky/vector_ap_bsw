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
 *        \brief  Implementation of the UDP socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/udp_socket.h"
#include <netdb.h>  // addrinfo
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>    // memcpy()
#include <stdexcept>  // runtime_error
#include <system_error>
#include "osabstraction/io/net/address/internal/addrinfo_ptr.h"
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void UdpSocket::Connect(const address::SocketAddress& remote_address) {
  int ret = ::connect(handle_, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void UdpSocket::JoinMulticastGroup(const vac::container::string_view& interface_address,
                                   const vac::container::string_view& group_address) {
  struct addrinfo hints {};
  struct addrinfo* mcast_ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(group_address.data(), nullptr, &hints, &mcast_ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (mcast_ai == nullptr) {
    throw std::runtime_error("UdpSocket::JoinMulticastGroup: could not resolve multicast address " +
                             std::string(group_address.data()));
  }
  auto mcast_ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(mcast_ai);
  if (mcast_ai->ai_family == AF_INET) {
    struct addrinfo* iface_ai = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICHOST;
    ret = ::getaddrinfo(interface_address.data(), nullptr, &hints, &iface_ai);
    if (ret != 0) {
      throw std::runtime_error(gai_strerror(ret));
    }
    if (iface_ai == nullptr) {
      throw std::runtime_error("UdpSocket::JoinMulticastGroup: could not resolve interface address " +
                               std::string(interface_address.data()));
    }
    auto iface_ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(iface_ai);
    static struct ip_mreq mr;
    auto mcast_sin = reinterpret_cast<const struct sockaddr_in*>(mcast_ai->ai_addr);
    auto iface_sin = reinterpret_cast<const struct sockaddr_in*>(iface_ai->ai_addr);
    std::memcpy(&mr.imr_multiaddr.s_addr, &mcast_sin->sin_addr, sizeof(mcast_sin->sin_addr));
    std::memcpy(&mr.imr_interface.s_addr, &iface_sin->sin_addr, sizeof(iface_sin->sin_addr));
    ret = ::setsockopt(handle_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mr, sizeof(mr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else if (mcast_ai->ai_family == AF_INET6) {
    auto index = internal::InterfaceAddressToIndex(interface_address);
    if (!index.first) {
      throw std::runtime_error("UdpSocket::JoinMulticastGroup: could not resolve interface index for " +
                               std::string(interface_address.data()));
    }
    struct ipv6_mreq mr;
    auto sin6 = reinterpret_cast<const struct sockaddr_in6*>(mcast_ai->ai_addr);
    std::memcpy(&mr.ipv6mr_multiaddr, &sin6->sin6_addr, sizeof(sin6->sin6_addr));
    mr.ipv6mr_interface = index.second;
    ret = ::setsockopt(handle_, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mr, sizeof(mr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else {
    throw std::runtime_error("UdpSocket::JoinMulticastGroup: wrong address family");
  }
}

void UdpSocket::LeaveMulticastGroup(const vac::container::string_view& interface_address,
                                    const vac::container::string_view& group_address) {
  struct addrinfo hints {};
  struct addrinfo* mcast_ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(group_address.data(), nullptr, &hints, &mcast_ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (mcast_ai == nullptr) {
    throw std::runtime_error("UdpSocket::LeaveMulticastGroup: could not resolve multicast address " +
                             std::string(group_address.data()));
  }
  auto mcast_ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(mcast_ai);
  if (mcast_ai->ai_family == AF_INET) {
    struct addrinfo* iface_ai = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICHOST;
    ret = ::getaddrinfo(interface_address.data(), nullptr, &hints, &iface_ai);
    if (ret != 0) {
      throw std::runtime_error(gai_strerror(ret));
    }
    if (iface_ai == nullptr) {
      throw std::runtime_error("UdpSocket::JoinMulticastGroup: could not resolve interface address " +
                               std::string(interface_address.data()));
    }
    auto iface_ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(iface_ai);
    static struct ip_mreq mr;
    auto mcast_sin = reinterpret_cast<const struct sockaddr_in*>(mcast_ai->ai_addr);
    auto iface_sin = reinterpret_cast<const struct sockaddr_in*>(iface_ai->ai_addr);
    std::memcpy(&mr.imr_multiaddr.s_addr, &mcast_sin->sin_addr, sizeof(mcast_sin->sin_addr));
    std::memcpy(&mr.imr_interface.s_addr, &iface_sin->sin_addr, sizeof(iface_sin->sin_addr));
    ret = ::setsockopt(handle_, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mr, sizeof(mr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else if (mcast_ai->ai_family == AF_INET6) {
    auto index = internal::InterfaceAddressToIndex(interface_address);
    if (!index.first) {
      throw std::runtime_error("UdpSocket::LeaveMulticastGroup: could not resolve interface index for " +
                               std::string(interface_address.data()));
    }
    struct ipv6_mreq mr;
    auto sin6 = reinterpret_cast<const struct sockaddr_in6*>(mcast_ai->ai_addr);
    std::memcpy(&mr.ipv6mr_multiaddr, &sin6->sin6_addr, sizeof(sin6->sin6_addr));
    mr.ipv6mr_interface = index.second;
    ret = ::setsockopt(handle_, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &mr, sizeof(mr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else {
    throw std::runtime_error("UdpSocket::LeaveMulticastGroup: wrong address family");
  }
}

void UdpSocket::SetBroadcast(bool enable) {
  int flag = enable;
  int ret = ::setsockopt(handle_, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void UdpSocket::SetMulticastSendInterface(const vac::container::string_view& address) {
  struct addrinfo hints {};
  struct addrinfo* ai = nullptr;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(address.data(), nullptr, &hints, &ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (ai == nullptr) {
    throw std::runtime_error("UdpSocket::SetMulticastSendInterface: could not resolve address " +
                             std::string(address.data()));
  }
  auto ai_ptr = osabstraction::io::net::address::internal::make_addrinfo_ptr(ai);
  if (ai->ai_family == AF_INET) {
    auto sin = reinterpret_cast<const struct sockaddr_in*>(ai->ai_addr);
    ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_IF, &sin->sin_addr, sizeof(sin->sin_addr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else if (ai->ai_family == AF_INET6) {
    auto index = internal::InterfaceAddressToIndex(address);
    if (!index.first) {
      throw std::runtime_error("UdpSocket::SetMulticastSendInterface: could not resolve interface index for " +
                               std::string(address.data()));
    }
    ret = ::setsockopt(handle_, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index.second, sizeof(index.second));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else {
    throw std::runtime_error("UdpSocket::SetMulticastSendInterface: wrong address family");
  }
}

void UdpSocket::SetMulticastTTL(std::uint8_t ttl) {
  int ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void UdpSocket::SetMulticastLoop(bool enable) {
  std::uint8_t loop = enable;
  int ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
