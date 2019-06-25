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
#include <sys/un.h>   // sockaddr_un
#include <cstring>    // strncpy()
#include <stdexcept>  // invalid_argument()
#include "osabstraction/io/net/address/ipc_socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

IpcSocketAddress IpcSocketAddressBuilder::BuildFromPath(const vac::container::string_view& path) {
  if (path.empty()) {
    throw std::invalid_argument("IpcSocketAddressBuilder::BuildFromPath path is empty");
  }
  struct sockaddr_un sun;
  if (path.size() >= sizeof(sun.sun_path)) {
    throw std::invalid_argument("IpcSocketAddressBuilder:BuildFromPath path is too big");
  }
  sun.sun_family = AF_UNIX;
  std::strncpy(sun.sun_path, path.data(), sizeof(sun.sun_path));
  return IpcSocketAddress(sun);
}

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction
