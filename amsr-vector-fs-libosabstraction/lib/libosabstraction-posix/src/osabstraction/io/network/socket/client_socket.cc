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
#include "osabstraction/io/network/socket/client_socket.h"

#include <system_error>

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

void ClientSocket::Connect(const address::SocketAddress& remote_address) {
  int ret = ::connect(handle_, remote_address.toSockAddr(), remote_address.Length());
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction
