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
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/socket/udp_socket.h"

#include <system_error>

#include "osabstraction/io/network/address/addrinfo_ptr.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

UDPSocket UDPSocket::CreateSocket(const address::IPSocketAddress& remote_address) {
  UDPSocket socket(remote_address.GetAddressFamily());
  socket.Connect(remote_address);
  return socket;
}

UDPSocket UDPSocket::CreateSocket(const address::IPSocketAddress& local_address, bool reuse_address) {
  UDPSocket socket(local_address.GetAddressFamily());
  socket.SetReuseAddress(reuse_address);
  socket.Bind(local_address);
  return socket;
}

UDPSocket UDPSocket::CreateSocket(const address::IPSocketAddress& remote_address,
                                  const address::IPSocketAddress& local_address, bool reuse_address) {
  UDPSocket socket = UDPSocket::CreateSocket(local_address, reuse_address);
  socket.Connect(remote_address);
  return socket;
}

void UDPSocket::JoinMulticastGroup(const std::string& interface_address, const std::string& group_address) {
  struct addrinfo hints;
  struct addrinfo* mcast_ai = nullptr;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(group_address.c_str(), nullptr, &hints, &mcast_ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (mcast_ai == nullptr) {
    throw std::runtime_error("UDPSocket::JoinMulticastGroup: could not resolve multicast address " + group_address);
  }
  auto mcast_ai_ptr = osabstraction::io::network::address::make_addrinfo_ptr(mcast_ai);
  if (mcast_ai->ai_family == AF_INET) {
    struct addrinfo* iface_ai = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICHOST;
    ret = ::getaddrinfo(interface_address.c_str(), nullptr, &hints, &iface_ai);
    if (ret != 0) {
      throw std::runtime_error(gai_strerror(ret));
    }
    if (iface_ai == nullptr) {
      throw std::runtime_error("UDPSocket::JoinMulticastGroup: could not resolve interface address " +
                               interface_address);
    }
    auto iface_ai_ptr = osabstraction::io::network::address::make_addrinfo_ptr(iface_ai);
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
    auto index = InterfaceAddressToIndex(interface_address);
    if (!index.first) {
      throw std::runtime_error("UDPSocket::JoinMulticastGroup: could not resolve interface index for " +
                               interface_address);
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
    throw std::runtime_error("UDPSocket::JoinMulticastGroup: wrong address family");
  }
}

void UDPSocket::LeaveMulticastGroup(const std::string& interface_address, const std::string& group_address) {
  struct addrinfo hints;
  struct addrinfo* mcast_ai = nullptr;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(group_address.c_str(), nullptr, &hints, &mcast_ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (mcast_ai == nullptr) {
    throw std::runtime_error("UDPSocket::LeaveMulticastGroup: could not resolve multicast address " + group_address);
  }
  auto mcast_ai_ptr = osabstraction::io::network::address::make_addrinfo_ptr(mcast_ai);
  if (mcast_ai->ai_family == AF_INET) {
    struct addrinfo* iface_ai = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_NUMERICHOST;
    ret = ::getaddrinfo(interface_address.c_str(), nullptr, &hints, &iface_ai);
    if (ret != 0) {
      throw std::runtime_error(gai_strerror(ret));
    }
    if (iface_ai == nullptr) {
      throw std::runtime_error("UDPSocket::JoinMulticastGroup: could not resolve interface address " +
                               interface_address);
    }
    auto iface_ai_ptr = osabstraction::io::network::address::make_addrinfo_ptr(iface_ai);
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
    auto index = InterfaceAddressToIndex(interface_address);
    if (!index.first) {
      throw std::runtime_error("UDPSocket::LeaveMulticastGroup: could not resolve interface index for " +
                               interface_address);
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
    throw std::runtime_error("UDPSocket::LeaveMulticastGroup: wrong address family");
  }
}

void UDPSocket::SetBroadcast(bool enable) {
  int flag = enable;
  int ret = ::setsockopt(handle_, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void UDPSocket::SetMulticastSendInterface(const std::string& address) {
  struct addrinfo hints;
  struct addrinfo* ai = nullptr;
  std::memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_NUMERICHOST;
  int ret = ::getaddrinfo(address.c_str(), nullptr, &hints, &ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (ai == nullptr) {
    throw std::runtime_error("UDPSocket::SetMulticastSendInterface: could not resolve address " + address);
  }
  auto ai_ptr = osabstraction::io::network::address::make_addrinfo_ptr(ai);
  if (ai->ai_family == AF_INET) {
    auto sin = reinterpret_cast<const struct sockaddr_in*>(ai->ai_addr);
    ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_IF, &sin->sin_addr, sizeof(sin->sin_addr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else if (ai->ai_family == AF_INET6) {
    auto index = InterfaceAddressToIndex(address);
    if (!index.first) {
      throw std::runtime_error("UDPSocket::SetMulticastSendInterface: could not resolve interface index for " +
                               address);
    }
    ret = ::setsockopt(handle_, IPPROTO_IPV6, IPV6_MULTICAST_IF, &index.second, sizeof(index.second));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else {
    throw std::runtime_error("UDPSocket::SetMulticastSendInterface: wrong address family");
  }
}

void UDPSocket::SetMulticastTTL(std::uint8_t ttl) {
  int ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void UDPSocket::SetMulticastLoop(bool enable) {
  std::uint8_t loop = enable;
  int ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction
