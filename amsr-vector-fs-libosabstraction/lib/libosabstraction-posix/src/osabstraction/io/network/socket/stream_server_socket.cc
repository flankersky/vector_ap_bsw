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
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Damian Philipp               visdap       Vector Informatik GmbH
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/socket/stream_server_socket.h"

#include <system_error>

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

StreamServerSocket::StreamServerSocket(address::SocketAddressFamily address_family)
    : Socket(address_family, SocketType::kStream) {}

void StreamServerSocket::Listen(int backlog) {
  int ret = ::listen(handle_, backlog);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction
