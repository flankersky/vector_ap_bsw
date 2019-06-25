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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_TCP_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_TCP_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <chrono>
#include <limits>
#include "osabstraction/io/network/socket/stream_socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Represents a TCP Socket capable of initiating connections.
 */
class TCPSocket : public StreamSocket {
 public:
  TCPSocket() = default;

  /**
   * \brief Constructor that passes on an existing socket handle
   */
  explicit TCPSocket(int handle) : StreamSocket(handle) {}

  /**
   * \brief Constructor for a StreamServerSocket for a given address_family.
   */
  explicit TCPSocket(address::SocketAddressFamily address_family) : StreamSocket(address_family) {}

  /**
   * \brief TCP KeepAlive parameters to set for a specific socket.
   *
   * \param keepalive_time Duration (in seconds) between two keepalive transmissions in idle condition.
   * \param keepalive_interval Duration (in seconds) between two successive keepalive retransmissions, if
   * acknowledgement to the previous keepalive transmission is not received.
   * \param retry_count Number of retransmissions to be carried out before declaring that remote end is not
   available.
   */
  void ParametrizeKeepAlive(std::chrono::seconds keepalive_time, std::chrono::seconds keepalive_interval,
                            int retry_count) {
    if ((keepalive_time.count() > std::numeric_limits<int>::max()) ||
        (keepalive_interval.count() > std::numeric_limits<int>::max())) {
      throw std::out_of_range("KeepAlive idle time or interval are out of range.");
    }

    const int keepidle = (int)keepalive_time.count();
    const int keepintvl = (int)keepalive_interval.count();
    const int keepcnt = retry_count;

    int ret = ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
    ret &= ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
    ret &= ::setsockopt(handle_, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));

    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  }

  /**
   * \brief Enable or disable TCP KeepAlive for this TCP socket by invoking this
   * method.
   *
   * \param enable true if the KeepAlive shall be enabled / false if KeepAlive
   * shall be disabled.
   */
  void SetKeepAlive(bool enable) {
    const int keepalive{static_cast<int>(enable)};
    const int ret = ::setsockopt(handle_, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

    if (ret < 0) {
      throw std::system_error(errno, std::generic_category());
    }
  }
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_TCP_SOCKET_H_
