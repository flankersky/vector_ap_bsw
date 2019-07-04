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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_UNIX_DOMAIN_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_UNIX_DOMAIN_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "osabstraction/io/network/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace network {
namespace address {

/**
 * \brief Class for representing a Unix Domain Socket Address
 */
class UnixDomainSocketAddress : public SocketAddress {
 public:
  /**
   * \brief Constructor of SocketAddress.
   *
   * \param sun A pointer to a Unix domain socket address.
   */
  explicit UnixDomainSocketAddress(const struct sockaddr_un* sun);

  /**
   * \brief Constructor of SocketAddress.
   *
   * \param path A filesystem path for a Unix domain socket.
   */
  explicit UnixDomainSocketAddress(const std::string& path);

  /**
   * \brief Converter to sockaddr_un.
   *
   * \return A pointer to sockaddr_un.
   */
  const struct sockaddr_un* toSockAddrUn() const { return reinterpret_cast<const struct sockaddr_un*>(&sockaddr_); }
  /**
   * \brief Converter to sockaddr_un.
   *
   * \return A pointer to sockaddr_un.
   */
  struct sockaddr_un* toSockAddrUn() {
    return reinterpret_cast<struct sockaddr_un*>(&sockaddr_);
  }
};

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_UNIX_DOMAIN_SOCKET_ADDRESS_H_
