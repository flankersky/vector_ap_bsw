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
 *        \brief  Socket connector
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Alexander Egorenkov           visaev        Vector Informatik GmbH
 *  Damian Philipp                visdap        Vector Informatik GmbH
 *  Sebastian Waldvogel           visswa        Vector Informatik GmbH
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_CONNECTOR_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_CONNECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>
#include <sys/un.h>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include "osabstraction/io/network/socket/client_socket.h"
#include "osabstraction/io/reactor_interface.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Socket connector.
 *
 * An implementation of the connector pattern for sockets.
 */
template <class SocketType, class ConnectHandler, void (ConnectHandler::*HandlerFunction)(SocketType&&)>
class SocketConnector : public io::EventHandler {
 public:
  /**
   * \brief Socket file descriptor.
   */
  using SocketHandle = int;

  /**
   * \brief Constructor of SocketConnector.
   *
   * \param remote_address Remote Address to connect to
   * \param connect_handler Callback to notify on successeful/failed connection
   * \param blocking_mode Whether to connect in blocking or non-blocking mode. The socket passed to connect_handler will
   * be in the same mode.
   */
  template <class SocketAddressType>
  explicit SocketConnector(const SocketAddressType& remote_address, ConnectHandler& connect_handler, bool blocking_mode)
      : socket_(remote_address.GetAddressFamily()), connect_handler_(connect_handler) {
    socket_.SetBlockingMode(blocking_mode);
    socket_.Connect(remote_address);
  }

  /**
   * \brief Constructor of SocketConnector.
   *
   * \param remote_address Remote Address to connect to
   * \param local_address Local Address to connect from
   * \param reuse_address Whether to allow reuse of the local address
   * \param connect_handler Callback to notify on successeful/failed connection
   * \param blocking_mode Whether to connect in blocking or non-blocking mode. The socket passed to connect_handler
   * will be in the same mode.
   */
  template <class SocketAddressType>
  explicit SocketConnector(const SocketAddressType& remote_address, const SocketAddressType& local_address,
                           bool reuse_address, ConnectHandler& connect_handler, bool blocking_mode)
      : socket_(remote_address.GetAddressFamily()), connect_handler_(connect_handler) {
    socket_.SetReuseAddress(reuse_address);
    socket_.Bind(local_address);
    socket_.SetBlockingMode(blocking_mode);
    if (blocking_mode) {
      socket_.Connect(remote_address);
    } else {
      try {
        socket_.Connect(remote_address);
      } catch (std::system_error& e) {
        if (e.code().value() != EINPROGRESS) {
          // In Non-Blocking mode, EINPROGRESS is expected - the call will be completed by a later select.
          // Anything else is an actual error.
          throw e;
        }
      }
    }
  }

  /**
   * \brief Destructor of SocketConnector.
   */
  virtual ~SocketConnector() = default;

  /**
   * \brief Return the Handle of the contained socket.
   */
  int GetHandle() const { return socket_.GetHandle(); }

  /**
   * \brief Handler called by the reactor whenever the connection status changes.
   *
   * \param handle A file descriptor representing a connection. The given handle is compared to the contained socket.
   *   If the handle does not match the contained socket, no action is taken.
   * \return If the given handle matches the contained socket, returns true (one-shot handler). Returns false otherwise
   * (retry handling with possibly the correct socket).
   */
  bool HandleWrite(int handle) override {
    if (handle == socket_.GetHandle()) {
      if (socket_.IsConnected()) {
        socket_.SetBlockingMode(true);
      } else {
        socket_.Close();
      }
      (connect_handler_.*HandlerFunction)(std::move(socket_));
      return false;
    } else {
      return true;
    }
  }

 protected:
  /**
   * \brief A reactor which is used for notification about connection status.
   */
  SocketType socket_;

  /**
   * \brief Connect handlers for pending asynchronous connections.
   */
  ConnectHandler& connect_handler_;
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_CONNECTOR_H_
