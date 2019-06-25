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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_DATAGRAM_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_DATAGRAM_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/address/unix_domain_socket_address.h"
#include "osabstraction/io/network/socket/datagram_socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Represents a UnixDomainSocket of type SocketType::kDatagram.
 */
class UnixDomainDatagramSocket : public DatagramSocket {
 public:
  /**
   * \brief Named Constructor for a datagram socket
   *
   * \param local_address the address to open the socket at.
   * \returns A UnixDomainDatagramSocket not connected to any address.
   */
  static UnixDomainDatagramSocket CreateSocket(const address::UnixDomainSocketAddress& local_address);

  /**
   * \brief Named Constructor for a datagram socket
   *
   * \param local_address the address to open the socket at.
   * \param remote_address the address to connect tos.
   * \returns A UnixDomainDatagramSocket connected to the given socket_address
   */
  static UnixDomainDatagramSocket CreateSocket(const address::UnixDomainSocketAddress& local_address,
                                               const address::UnixDomainSocketAddress& remote_address);

  virtual ~UnixDomainDatagramSocket();

  /**
   * \brief Closes a socket.
   */
  void Close() override;

  UnixDomainDatagramSocket(UnixDomainDatagramSocket&&) = default;             ///< Move constructor
  UnixDomainDatagramSocket& operator=(UnixDomainDatagramSocket&&) = default;  ///< Move operator

 protected:
  UnixDomainDatagramSocket() = default;

  /**
   * \brief Constructor for a UDPSocket of a given Address Family.
   */
  explicit UnixDomainDatagramSocket(address::SocketAddressFamily address_family) : DatagramSocket(address_family) {}
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UNIX_DOMAIN_DATAGRAM_SOCKET_H_
