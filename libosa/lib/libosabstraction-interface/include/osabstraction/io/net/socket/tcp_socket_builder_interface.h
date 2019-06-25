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
 *        \brief  Abstract interface to create TCP sockets with client semantics.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_BUILDER_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_BUILDER_INTERFACE_H_

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

class TcpSocket;

/**
 * \brief Abstract interface to create TCP sockets.
 */
class TcpSocketBuilderInterface {
 public:
  virtual ~TcpSocketBuilderInterface() = default;

  /**
   * \brief Create a TcpSocket connected to the given remote_address.
   * \param[in] remote_address  The address to connect to.
   * \return Created TCP socket.
   */
  virtual TcpSocket BuildSocket(const address::IpSocketAddress& remote_address) = 0;

  /**
   * \brief Create a TcpSocket not connected to a remote address but with a fixed local address
   * \param[in] local_address   The address to connect to.
   * \param[in] reuse_address   Tells whether the given address is allowed to be reused.
   * \return Created TCP socket.
   */
  virtual TcpSocket BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address) = 0;

  /**
   * \brief Create a TcpSocket connected to a remote address with a fixed local address
   * \param[in] remote_address  The address to connect to.
   * \param[in] local_address   The address to open the socket at.
   * \param[in] reuse_address   Tells whether the given address is allowed to be reused.
   * \return Created TCP socket.
   */
  virtual TcpSocket BuildSocket(const address::IpSocketAddress& remote_address,
                                const address::IpSocketAddress& local_address, bool reuse_address) = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_TCP_SOCKET_BUILDER_INTERFACE_H_
