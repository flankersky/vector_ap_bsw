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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_STREAM_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_STREAM_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/address/unix_domain_socket_address.h"
#include "osabstraction/io/network/socket/stream_socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Represents a UnixDomainSocket of type SocketType::kStream capable of initiating connections.
 */
class UnixDomainStreamSocket : public StreamSocket {
 public:
  UnixDomainStreamSocket() = default;

  /**
   * \brief Constructor that passes on an existing socket handle
   */
  explicit UnixDomainStreamSocket(int handle) : StreamSocket(handle) {}

  /**
   * \brief Constructor for a StreamServerSocket for a given address_family.
   */
  explicit UnixDomainStreamSocket(address::SocketAddressFamily address_family) : StreamSocket(address_family) {}

  UnixDomainStreamSocket(UnixDomainStreamSocket&&) = default;             ///< Move constructor
  UnixDomainStreamSocket& operator=(UnixDomainStreamSocket&&) = default;  ///< Move operator

  virtual ~UnixDomainStreamSocket();

  /**
   * \brief Closes a socket.
   */
  void Close() override;
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_STREAM_SOCKET_H_
