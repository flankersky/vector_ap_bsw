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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ip_socket.h"
#include "osabstraction/io/net/socket/tcp_socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief TCP socket implementation.
 */
class TcpSocket : public virtual TcpSocketInterface, public IpSocket {
 public:
  TcpSocket() : IpSocket() {}

  /**
   * \brief Creates TCP socket from existing handle.
   *
   * \param[in,out] handle  Existing TCP socket handle.
   */
  explicit TcpSocket(int handle) : IpSocket(handle) {}

  /**
   * \brief Creates TCP socket for a given address_family.
   *
   * \param[in] address_family The address family for the socket.
   */
  explicit TcpSocket(address::SocketAddressFamily address_family) : IpSocket(address_family, SocketType::kStream) {}

  /**
    * \brief Destructor.
    */
  virtual ~TcpSocket() {}

  /**
    * \brief Move constructor.
    */
  TcpSocket(TcpSocket&& other) = default;

  /**
    * \brief Move assignment operator.
    */
  TcpSocket& operator=(TcpSocket&& other) = default;

  /**
   * \brief Connects a socket to specified remote address.
   *
   * \param remote_address A remote socket address to which the socket should be connected.
   */
  void Connect(const address::SocketAddress& remote_address) override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_H_
