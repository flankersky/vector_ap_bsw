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
#include "osabstraction/io/network/address/unix_domain_socket_address.h"

#include <stdexcept>

namespace osabstraction {
namespace io {
namespace network {
namespace address {

UnixDomainSocketAddress::UnixDomainSocketAddress(const struct sockaddr_un* sun) {
  *reinterpret_cast<struct sockaddr_un*>(&sockaddr_) = *sun;
  sockaddr_size_ = sizeof(*sun);
}

UnixDomainSocketAddress::UnixDomainSocketAddress(const std::string& path) {
  if (path.empty()) {
    throw std::invalid_argument("UnixDomainSocketAddress: path is empty");
  }
  struct sockaddr_un* sun = reinterpret_cast<struct sockaddr_un*>(&sockaddr_);
  if (path.size() >= sizeof(sun->sun_path)) {
    throw std::invalid_argument("UnixDomainSocketAddress: path is too big");
  }
  sun->sun_family = AF_UNIX;
  std::strncpy(sun->sun_path, path.c_str(), sizeof(sun->sun_path));
  sockaddr_size_ = sizeof(*sun);
}

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction
