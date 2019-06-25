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
 *        \brief  Abstract interface to work with socket addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>  // sockaddr
#include <cstddef>       // size_t
#include <string>
#include <utility>  // pair

namespace osabstraction {
namespace io {
namespace net {
namespace address {

/** Socket address families. */
enum class SocketAddressFamily {
  kUnspecified, /**< Unspecified socket address family. */
  kIpc,         /**< IPC socket address family. */
  kInet4,       /**< IPv4 socket address family. */
  kInet6        /**< IPv6 socket address family. */
};

/* Forward declaration for concrete socket address. */
class SocketAddress;

/**
 * \brief Abstract interface for socket addresses.
 * Shall be implemented by \ref SocketAddress.
 */
class SocketAddressInterface {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~SocketAddressInterface() = default;

  /**
   * \brief Returns length of the socket address.
   *
   * \return The length of the socket address.
   */
  virtual std::size_t Length() const = 0;

  /**
   * \brief Returns the socket address family.
   *
   * \return The socket address family.
   */
  virtual SocketAddressFamily GetAddressFamily() const = 0;

  /**
   * \brief Returns a string with the representation of a socket address.
   *
   * \return A string representing a socket address.
   */
  virtual std::string toString() const = 0;

  /**
   * \brief Converter to sockaddr.
   * TODO(visasl): This function should be moved to IPV*SocketAddress.
   *
   * \return A pointer to sockaddr.
   */
  virtual const struct sockaddr* toSockAddr() const = 0;

  /**
   * \brief Returns a pair of strings representing a socket address.
   * TODO(visasl): This function should be moved to IPV*SocketAddress. It does not make sense for IPC sockets.
   *
   * \return A pair of strings representing a socket address.
   */
  virtual std::pair<std::string, std::string> toAddressPortStrings() const = 0;

  /**
   * \brief Compares two socket addresses.
   *
   * \param other A socket address to compare to.
   * \return true if both socket addresses are equal and false otherwise.
   */
  virtual bool operator==(const SocketAddress& other) const = 0;
};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_SOCKET_ADDRESS_INTERFACE_H_
