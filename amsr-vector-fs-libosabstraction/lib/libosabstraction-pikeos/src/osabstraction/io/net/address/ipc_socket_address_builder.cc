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
 *        \brief  Implements creation of IPC socket addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/ipc_socket_address_builder.h"
#include <stddef.h>
#include <sys/qport.h>
#include <sys/socket.h>
#include <stdexcept>  // invalid_argument()
#include "osabstraction/io/net/address/ipc_socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

IpcSocketAddress IpcSocketAddressBuilder::BuildFromPath(const vac::container::string_view& path) {
  if (path.empty()) {
    throw std::invalid_argument("IpcSocketAddressBuilder::BuildFromPath path is empty.");
  }

  struct in_addr addr;
  int ret = inet_aton(path.data(), &addr);
  if (ret == 0) {
    throw std::invalid_argument("IpcSocketAddressBuilder::BuildFromPath path is invalid.");
  }

  qport_sap_sockaddr_ipv4_t ipc_addr;
  ipc_addr.length = sizeof(ipc_addr);
  ipc_addr.family = QPORT_AF_INET4;
  ipc_addr.port = 0;
  *reinterpret_cast<struct in_addr*>(&ipc_addr.address) = addr;

  return IpcSocketAddress(ipc_addr);
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
