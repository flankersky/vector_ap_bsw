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
 *        \brief  Abstract interface to work with stream oriented IPC server sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {

namespace address {
class IpcSocketAddress;
}  // namespace address

namespace socket {

class IpcStreamSocket;

/**
 * \brief Abstract interface to work with stream oriented IPC server sockets.
 */
class IpcStreamServerSocketInterface : public virtual SocketInterface {
 public:
  /**
   * \brief Socket type yielded by Accept() calls of this object
   */
  using ClientSocketType = IpcStreamSocket;

  /**
   * \brief Destructor.
   */
  virtual ~IpcStreamServerSocketInterface() = default;

  /**
   * \brief Activates listening for incoming connection requests.
   *
   * \param backlog The maximum length of the queue of pending connections.
   */
  virtual void Listen(int backlog) = 0;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   * \return The next connection request.
   */
  virtual ClientSocketType Accept() = 0;

  /**
   * \brief Returns the next connection request from the queue of pending connections.
   * \param remote_address The remote socket address of the next connection request.
   * \return The next connection request.
   */
  virtual ClientSocketType Accept(address::IpcSocketAddress& remote_address) = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_STREAM_SERVER_SOCKET_INTERFACE_H_
