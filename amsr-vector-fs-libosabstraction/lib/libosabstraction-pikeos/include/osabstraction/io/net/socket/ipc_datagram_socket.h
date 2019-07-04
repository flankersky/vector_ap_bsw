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
 *        \brief  Implementation for datagram oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ipc_datagram_socket_interface.h"
#include "osabstraction/io/net/socket/ipc_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief      Implementation for datagram oriented IPC sockets.
 */
class IpcDatagramSocket : public virtual IpcDatagramSocketInterface, public IpcSocket {
 public:
  /**
   * \brief      Creates an IPC socket.
   */
  IpcDatagramSocket() : IpcSocket() {}

  /**
   * \brief      Binds a socket to specified local address.
   *
   * \param[in]  local_address  A local socket address to which the socket should be bound.
   */
  void Bind(const address::SocketAddress& local_address) override;

  /**
   * \brief      Connects a socket to specified remote address.
   *
   * \param[in]  remote_address  A remote socket address to which the socket
   *                             should be connected.
   */
  void Connect(const address::SocketAddress& remote_address) override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_H_
