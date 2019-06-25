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
 *        \brief  Implements creation of UDP sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/udp_socket_builder.h"
#include "osabstraction/io/net/address/ip_socket_address.h"
#include "osabstraction/io/net/socket/udp_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

UdpSocket UdpSocketBuilder::BuildSocket(const address::IpSocketAddress& remote_address) {
  UdpSocket udp_socket(remote_address.GetAddressFamily());
  udp_socket.Connect(remote_address);
  return udp_socket;
}

UdpSocket UdpSocketBuilder::BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address) {
  UdpSocket udp_socket(local_address.GetAddressFamily());
  udp_socket.SetReuseAddress(reuse_address);
  udp_socket.Bind(local_address);
  return udp_socket;
}

UdpSocket UdpSocketBuilder::BuildSocket(const address::IpSocketAddress& remote_address,
                                        const address::IpSocketAddress& local_address, bool reuse_address) {
  UdpSocket udp_socket = BuildSocket(local_address, reuse_address);
  udp_socket.Connect(remote_address);
  return udp_socket;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
