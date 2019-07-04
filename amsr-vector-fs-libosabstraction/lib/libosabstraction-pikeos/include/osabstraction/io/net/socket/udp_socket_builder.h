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
 *        \brief  Builder implementation to create UDP sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_BUILDER_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/udp_socket_builder_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief  Implements creation of UDP sockets.
 */
class UdpSocketBuilder : public UdpSocketBuilderInterface {
 public:
  /**
   * \brief Create a UdpSocket connected to the given remote_address.
   *
   * \param[in] remote_address  The address to connect to.
   *
   * \return Created UDP socket.
   */
  UdpSocket BuildSocket(const address::IpSocketAddress& remote_address) override;

  /**
   * \brief Create a UdpSocket not connected to a remote address but with a fixed local address
   *
   * \param[in] local_address   The address to connect to.
   * \param[in] reuse_address   Tells whether the given address is allowed to be reused.
   *
   * \return Created UDP socket.
   */
  UdpSocket BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address) override;

  /**
   * \brief Create a UdpSocket connected to a remote address with a fixed local address
   *
   * \param[in] remote_address  The address to connect to.
   * \param[in] local_address   The address to open the socket at.
   * \param[in] reuse_address   Tells whether the given address is allowed to be reused.
   *
   * \return Created UDP socket.
   */
  UdpSocket BuildSocket(const address::IpSocketAddress& remote_address, const address::IpSocketAddress& local_address,
                        bool reuse_address) override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_BUILDER_H_
