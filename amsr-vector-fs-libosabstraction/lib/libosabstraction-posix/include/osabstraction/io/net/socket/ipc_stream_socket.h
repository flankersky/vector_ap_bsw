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
 *        \brief  Implementation of stream oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/net/socket/ipc_stream_socket_interface.h>
#include <cassert>
#include "osabstraction/io/net/socket/socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief Implementation of stream oriented IPC sockets.
 */
class IpcStreamSocket : public virtual IpcStreamSocketInterface, public Socket {
 public:
  /**
   * \brief Creates an IPC socket.
   */
  IpcStreamSocket() : Socket(address::SocketAddressFamily::kIpc, SocketType::kStream) {}

  /**
   * \brief Creates IPC socket for a given address_family.
   */
  explicit IpcStreamSocket(address::SocketAddressFamily address_family)
      : Socket(address::SocketAddressFamily::kIpc, SocketType::kStream) {
    assert(address_family == address::SocketAddressFamily::kIpc);
  }

  /**
   * \brief Creates IPC socket from existing handle.
   *
   * \param[in,out] handle Existing IPC socket handle.
   */
  explicit IpcStreamSocket(int handle) : Socket(handle) {}

  virtual ~IpcStreamSocket() { Close(); }

  IpcStreamSocket(IpcStreamSocket&&) = default;             ///< Move constructor
  IpcStreamSocket& operator=(IpcStreamSocket&&) = default;  ///< Move operator

  /**
   * \brief Connects a socket to specified remote address.
   *
   * \param[in] remote_address A remote socket address to which the socket should be connected.
   */
  void Connect(const address::SocketAddress& remote_address) override;

  /**
   * \brief Closes a socket.
   */
  void Close() override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_
