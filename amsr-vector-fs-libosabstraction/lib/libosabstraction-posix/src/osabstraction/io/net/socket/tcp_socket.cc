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
 *        \brief  TCP socket implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/tcp_socket.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <system_error>
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

void TcpSocket::Connect(const address::SocketAddress& remote_address) {
  int ret = ::connect(handle_, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void TcpSocket::ParametrizeKeepAlive(std::chrono::seconds keepalive_time, std::chrono::seconds keepalive_interval,
                                     int retry_count) {
  if ((keepalive_time.count() > std::numeric_limits<int>::max()) ||
      (keepalive_interval.count() > std::numeric_limits<int>::max())) {
    throw std::out_of_range("KeepAlive idle time or interval are out of range.");
  }

  int keepidle = keepalive_time.count();
  int keepintvl = keepalive_interval.count();
  int keepcnt = retry_count;

  int ret = ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
  ret &= ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
  ret &= ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));

  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

void TcpSocket::SetKeepAlive(bool enable) {
  const int keepalive{static_cast<int>(enable)};
  const int ret = ::setsockopt(handle_, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
