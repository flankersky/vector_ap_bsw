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
 *        \brief  Implements creation of datagram oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/ipc_datagram_socket_builder.h"
#include "osabstraction/io/net/address/ipc_socket_address.h"
#include "osabstraction/io/net/socket/ipc_datagram_socket.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

IpcDatagramSocket IpcDatagramSocketBuilder::BuildSocket(const address::IpcSocketAddress& local_address) {
  IpcDatagramSocket socket;
  socket.Bind(local_address);
  return socket;
}

IpcDatagramSocket IpcDatagramSocketBuilder::BuildSocket(const address::IpcSocketAddress& local_address,
                                                        const address::IpcSocketAddress& remote_address) {
  IpcDatagramSocket socket = BuildSocket(local_address);
  socket.Connect(remote_address);
  return socket;
}

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction
