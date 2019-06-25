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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/net/socket/ipc_stream_socket_interface.h>
#include <cassert>
#include "osabstraction/io/net/socket/ipc_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief      Implementation of stream oriented IPC sockets.
 */
class IpcStreamSocket : public virtual IpcStreamSocketInterface, public IpcSocket {
 public:
  /**
   * \brief      Default constructor.
   */
  IpcStreamSocket() : IpcSocket() {}

  /**
   * \brief      Copy contstructor.
   */
  IpcStreamSocket(const IpcStreamSocket&) = delete;

  /**
   * \brief      Copy assignment operator.
   */
  IpcStreamSocket& operator=(const IpcStreamSocket&) = delete;

  /**
   * \brief      Move constructor.
   */
  IpcStreamSocket(IpcStreamSocket&&) = default;

  /**
   * \brief      Move assignment operator.
   */
  IpcStreamSocket& operator=(IpcStreamSocket&&) = default;

  /**
   * \brief Creates IPC stream socket for a given address_family.
   */
  explicit IpcStreamSocket(address::SocketAddressFamily address_family) : IpcSocket() {
    assert(address_family == address::SocketAddressFamily::kIpc);
  }

  /**
   * \brief      Creates a IPC stream socket from local and remote address.
   *
   * \param[in]  local_address   The local socket address.
   * \param[in]  remote_address  The remote socket address.
   * \param[in]  id              The identifier of the new connection.
   */
  IpcStreamSocket(const address::SocketAddress& local_address, const address::SocketAddress& remote_address,
                  std::uint32_t id)
      : IpcSocket(local_address, remote_address, id) {
    port_.Open(true);
  }

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

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SOCKET_H_
