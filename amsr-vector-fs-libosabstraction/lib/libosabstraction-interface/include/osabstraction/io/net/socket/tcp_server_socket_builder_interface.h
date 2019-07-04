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
 *        \brief  Interface to create TCP sockets with server semantics.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_BUILDER_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_BUILDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace osabstraction {
namespace io {
namespace net {

namespace address {
class IpSocketAddress;
}  // namespace address

namespace socket {

class TcpServerSocket;

/**
 * \brief Abstract interface to create TCP server sockets.
 */
class TcpServerSocketBuilderInterface {
 public:
  virtual ~TcpServerSocketBuilderInterface() = default;

  /**
   * \brief Create a TcpServerSocket listening at the given local address.
   * \param[in] local_address The address to bind to.
   * \param[in] reuse_address Tells whether the given address is allowed to be reused.
   * \param[in] backlog       The maximum length of the queue of pending connections.
   * \return Created TCP server socket.
   */
  virtual TcpServerSocket BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address,
                                      int backlog) = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SERVER_SOCKET_BUILDER_INTERFACE_H_
