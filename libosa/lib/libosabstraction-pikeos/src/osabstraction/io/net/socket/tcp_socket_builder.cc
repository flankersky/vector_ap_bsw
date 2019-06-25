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
 *        \brief  Builder implementation to create TCP sockets with client semantics.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/tcp_socket_builder.h"
#include "osabstraction/io/net/address/ip_socket_address.h"
#include "osabstraction/io/net/socket/tcp_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

TcpSocket TcpSocketBuilder::BuildSocket(const address::IpSocketAddress& remote_address) {
  TcpSocket tcp_socket(remote_address.GetAddressFamily());
  tcp_socket.Connect(remote_address);
  return tcp_socket;
}

TcpSocket TcpSocketBuilder::BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address) {
  TcpSocket tcp_socket(local_address.GetAddressFamily());
  tcp_socket.SetReuseAddress(reuse_address);
  tcp_socket.Bind(local_address);
  return tcp_socket;
}

TcpSocket TcpSocketBuilder::BuildSocket(const address::IpSocketAddress& remote_address,
                                        const address::IpSocketAddress& local_address, bool reuse_address) {
  TcpSocket tcp_socket = BuildSocket(local_address, reuse_address);
  tcp_socket.Connect(remote_address);
  return tcp_socket;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
