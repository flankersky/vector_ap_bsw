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
 *        \brief  Implementation of the builder interface to create TCP sockets with server semantics.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/tcp_server_socket_builder.h"
#include "osabstraction/io/net/address/ip_socket_address.h"
#include "osabstraction/io/net/socket/tcp_server_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

TcpServerSocket TcpServerSocketBuilder::BuildSocket(const address::IpSocketAddress& local_address, bool reuse_address,
                                                    int backlog) {
  TcpServerSocket server_socket(local_address.GetAddressFamily());
  server_socket.SetReuseAddress(reuse_address);
  server_socket.Bind(local_address);
  server_socket.Listen(backlog);

  return server_socket;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
