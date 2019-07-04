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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include "osabstraction/io/net/socket/socket.h"
#include "osabstraction/io/net/socket/tcp_socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief TCP socket implementation.
 */
class TcpSocket : public virtual TcpSocketInterface, public Socket {
 public:
  TcpSocket() : Socket() {}

  /**
   * \brief Creates TCP socket from existing handle.
   *
   * \param[in,out] handle Existing TCP socket handle.
   */
  explicit TcpSocket(int handle) : Socket(handle) {}

  /**
   * \brief Creates TCP socket for a given address_family.
   *
   * \param[in] address_family  The address family for the socket.
   */
  explicit TcpSocket(address::SocketAddressFamily address_family) : Socket(address_family, SocketType::kStream) {}

  /**
   * \brief Connects a socket to specified remote address.
   *
   * \param remote_address A remote socket address to which the socket should be connected.
   */
  void Connect(const address::SocketAddress& remote_address) override;

  /**
   * \brief TCP KeepAlive parameters to set for a specific socket.
   *
   * \param keepalive_time Duration (in seconds) between two keepalive transmissions in idle condition.
   * \param keepalive_interval Duration (in seconds) between two successive keepalive retransmissions, if
   * acknowledgement to the previous keepalive transmission is not received.
   * \param retry_count Number of retransmissions to be carried out before declaring that remote end is not available.
   */
  void ParametrizeKeepAlive(std::chrono::seconds keepalive_time, std::chrono::seconds keepalive_interval,
                            int retry_count);

  /**
   * \brief Enable or disable TCP KeepAlive for this TCP socket by invoking this
   * method.
   *
   * \param enable true if the KeepAlive shall be enabled / false if KeepAlive
   * shall be disabled.
   */
  void SetKeepAlive(bool enable);
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_
