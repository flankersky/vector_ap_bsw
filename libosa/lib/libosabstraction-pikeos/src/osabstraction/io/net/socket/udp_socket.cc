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
#include <netdb.h>       // addrinfo, freeaddrinfo(), gai_strerror()
#include <sys/socket.h>  // POSIX socket API
#include <cstring>       // memcpy()
#include <memory>        // unique_ptr
#include <stdexcept>     // runtime_error
#include <system_error>
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

static void addrinfo_deleter(struct addrinfo* ai) { freeaddrinfo(ai); }

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

  // #00 Parse group address
  int ret = ::getaddrinfo(group_address.data(), nullptr, &hints, &mcast_ai);
  if (ret != 0) {
    throw std::runtime_error(gai_strerror(ret));
  }
  if (mcast_ai == nullptr) {
    throw std::runtime_error("UdpSocket::JoinMulticastGroup: could not resolve multicast address " +
                             std::string(group_address.data()));
  }

  // Create smart pointer for addrinfo struct to be exception safe.
  auto mcast_ai_ptr = std::unique_ptr<struct addrinfo, decltype(&addrinfo_deleter)>(mcast_ai, &addrinfo_deleter);

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

    // Create smart pointer for addrinfo struct to be exception safe.
    auto iface_ai_ptr = std::unique_ptr<struct addrinfo, decltype(&addrinfo_deleter)>(iface_ai, &addrinfo_deleter);

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
    // TODO(visasl): UDP IPv6 multicast currently not supported.
    throw std::runtime_error("UdpSocket::JoinMulticastGroup: IPv6 multicast not supported: " +
                             std::string(interface_address.data()) + std::string(group_address.data()));
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

  // Create smart pointer for addrinfo struct to be exception safe.
  auto mcast_ai_ptr = std::unique_ptr<struct addrinfo, decltype(&addrinfo_deleter)>(mcast_ai, &addrinfo_deleter);

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

    // Create smart pointer for addrinfo struct to be exception safe.
    auto iface_ai_ptr = std::unique_ptr<struct addrinfo, decltype(&addrinfo_deleter)>(iface_ai, &addrinfo_deleter);

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
    // TODO(visasl): UDP IPv6 multicast currently not supported.
    throw std::runtime_error("UdpSocket::LeaveMulticastGroup: IPv6 multicast not supported: " +
                             std::string(interface_address.data()) + std::string(group_address.data()));
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

  // Create smart pointer for addrinfo struct to be exception safe.
  auto ai_ptr = std::unique_ptr<struct addrinfo, decltype(&addrinfo_deleter)>(ai, &addrinfo_deleter);

  if (ai->ai_family == AF_INET) {
    auto sin = reinterpret_cast<const struct sockaddr_in*>(ai->ai_addr);
    ret = ::setsockopt(handle_, IPPROTO_IP, IP_MULTICAST_IF, &sin->sin_addr, sizeof(sin->sin_addr));
    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  } else if (ai->ai_family == AF_INET6) {
    // TODO(visasl): UDP IPv6 multicast currently not supported.
    throw std::runtime_error("UdpSocket::SetMulticastSendInterface: IPv6 multicast not supported: " +
                             std::string(address.data()));
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
