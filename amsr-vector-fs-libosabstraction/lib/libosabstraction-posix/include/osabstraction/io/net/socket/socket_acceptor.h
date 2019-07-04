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
 *        \brief  Socket acceptor
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_ACCEPTOR_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_ACCEPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include "osabstraction/io/event_handler.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief Socket acceptor.
 *
 * An implementation of the acceptor pattern for stream sockets.
 */
template <class ServerSocketType, class AcceptHandler,
          void (AcceptHandler::*HandlerFunction)(typename ServerSocketType::ClientSocketType&&)>
class SocketAcceptor : public io::EventHandler {
 public:
  /**
   * \brief Constructor of SocketAcceptor.
   *
   * \param local_address A local socket address on which the acceptor will listen for incoming connections.
   * \param accept_handler A function to which a file descriptor representing a new established connection is passed.
   */
  template <class SocketAddressType>
  SocketAcceptor(const SocketAddressType& local_address, AcceptHandler& accept_handler)
      : server_socket_(local_address.GetAddressFamily()), accept_handler_(accept_handler) {
    server_socket_.SetReuseAddress(true);
    server_socket_.Bind(local_address);
    server_socket_.Listen(kListenBacklog);
  }

  /**
   * \brief Destructor of SocketAcceptor.
   */
  virtual ~SocketAcceptor() = default;

  /**
   * \brief Return the Handle of the contained socket.
   */
  int GetHandle() const { return server_socket_.GetHandle(); }

 protected:
  /**
   * \brief Maximum length of the queue for requested connections.
   */
  static const int kListenBacklog = 5;

  /**
   * \brief Handler that is called by reactor whenever a new connection can be accepted.
   *
   * \param handle The accepting socket handle.
   * \return always returns true
   */
  bool HandleRead(int handle) override {
    (accept_handler_.*HandlerFunction)(std::move(server_socket_.Accept()));
    return EventHandler::HandleRead(handle);
  }

  /**
   * \brief Unix domain socket.
   */
  ServerSocketType server_socket_;

  /**
   * \brief Handler to which each established connection represented by a file descriptor is passed.
   */
  AcceptHandler& accept_handler_;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_ACCEPTOR_H_
