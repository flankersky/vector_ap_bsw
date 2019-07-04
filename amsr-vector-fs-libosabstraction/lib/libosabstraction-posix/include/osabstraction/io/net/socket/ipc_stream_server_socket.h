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
 *        \brief  Implementation of the stream oriented IPC server socket.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/net/socket/ipc_stream_server_socket_interface.h>
#include <cassert>
#include "osabstraction/io/net/socket/socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief  Implementation of the stream oriented IPC server socket.
 */
class IpcStreamServerSocket : public virtual IpcStreamServerSocketInterface, public Socket {
 public:
  /**
   * \brief Creates an stream oriented IPC server socket.
   */
  IpcStreamServerSocket() : Socket(address::SocketAddressFamily::kIpc, SocketType::kStream) {}

  /**
   * \brief Creates an stream oriented IPC server socket for a given address_family.
   */
  explicit IpcStreamServerSocket(address::SocketAddressFamily address_family)
      : Socket(address::SocketAddressFamily::kIpc, SocketType::kStream) {
    assert(address_family == address::SocketAddressFamily::kIpc);
  }

  virtual ~IpcStreamServerSocket() { Close(); }

  /**
   * \brief Activates listening for incoming connection requests.
   *
   * \param backlog The maximum length of the queue of pending connections.
   */
  void Listen(int backlog) override;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   * \return The next connection request.
   */
  ClientSocketType Accept() override;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   * \param remote_address The remote socket address of the next connection request.
   * \return The next connection request.
   */
  ClientSocketType Accept(address::IpcSocketAddress& remote_address) override;

  /**
   * \brief Closes a socket.
   */
  void Close() override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_H_
