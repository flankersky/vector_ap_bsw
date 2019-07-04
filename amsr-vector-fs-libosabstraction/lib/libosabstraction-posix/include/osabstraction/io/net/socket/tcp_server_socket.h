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
 *        \brief  Implementation of the TCP server socket.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/socket.h"
#include "osabstraction/io/net/socket/tcp_server_socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief  Implementation of the TCP server socket.
 */
class TcpServerSocket : public virtual TcpServerSocketInterface, public Socket {
 public:
  /**
   * \brief Creates a TCP server socket matching the given socket address family.
   *
   * \param[in] address_family The address familiy for the socket.
   */
  explicit TcpServerSocket(address::SocketAddressFamily address_family) : Socket(address_family, SocketType::kStream) {}

  /**
   * \brief Activates listening for incoming connection requests.
   *
   * \param backlog The maximum length of the queue of pending connections.
   */
  void Listen(int backlog) override;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   *
   * \return The next connection request.
   */
  ClientSocketType Accept() override;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   *
   * \param remote_address The remote socket address of the next connection request.
   * \return The next connection request.
   */
  ClientSocketType Accept(address::IpSocketAddress& remote_address) override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_H_
