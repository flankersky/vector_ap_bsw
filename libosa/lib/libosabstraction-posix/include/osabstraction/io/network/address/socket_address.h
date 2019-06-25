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
 *        \brief  Socket address
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <cstring>
#include <string>
#include <utility>

namespace osabstraction {
namespace io {
namespace network {
namespace address {

/**
 * \brief Socket address families.
 */
enum class SocketAddressFamily {
  /**
   * \brief Unspecified socket address family.
   */
  kUnspecified,
  /**
   * \brief Unix domain socket address family.
   */
  kUnix,
  /**
   * \brief IPv4 socket address family.
   */
  kInet4,
  /**
   * \brief IPv6 socket address family.
   */
  kInet6
};

/**
 * \brief Base Class for representations of the Socket API type sockaddr
 */
class SocketAddress {
 public:
  /**
   * \brief Constructor of SocketAddress.
   */
  SocketAddress();

  /**
   * \brief Constructor of SocketAddress.
   *
   * \param sockaddr A pointer to a socket address storage.
   */
  explicit SocketAddress(const struct sockaddr_storage* sockaddr);

  /**
   * \brief Constructor of SocketAddress.
   *
   * \param sockaddr A pointer to a socket address.
   */
  explicit SocketAddress(const struct sockaddr* sockaddr);

  virtual ~SocketAddress() = default;

  /**
   * \brief Compares two socket addresses.
   *
   * \param other A socket address to compare to.
   * \return true if both socket addresses are equal and false otherwise.
   */
  bool operator==(const SocketAddress& other) const {
    if (sockaddr_.ss_family != other.sockaddr_.ss_family) {
      return false;
    }
    if (sockaddr_.ss_family == AF_UNIX) {
      auto sun = reinterpret_cast<const struct sockaddr_un*>(&sockaddr_);
      auto other_sun = reinterpret_cast<const struct sockaddr_un*>(&other.sockaddr_);
      return (std::strncmp(sun->sun_path, other_sun->sun_path, sizeof(sun->sun_path)) == 0);
    } else {
      return (std::memcmp(&sockaddr_, &other.sockaddr_, sockaddr_size_) == 0);
    }
  }

  /**
   * \brief Converter to sockaddr.
   *
   * \return A pointer to sockaddr.
   */
  const struct sockaddr* toSockAddr() const { return reinterpret_cast<const struct sockaddr*>(&sockaddr_); }

  /**
   * \brief Converter to sockaddr.
   *
   * \return A pointer to sockaddr.
   */
  struct sockaddr* toSockAddr() {
    return reinterpret_cast<struct sockaddr*>(&sockaddr_);
  }

  /**
   * \brief Returns length of the socket address.
   *
   * \return The length of the socket address.
   */
  socklen_t Length() const { return sockaddr_size_; }

  /**
   * \brief Returns the socket address family.
   *
   * \return The socket address family.
   */
  SocketAddressFamily GetAddressFamily() const { return fromUnixSocketAddressFamily(sockaddr_.ss_family); }

  /**
   * \brief Returns a string with the representation of a socket address.
   *
   * \return A string representing a socket address.
   */
  std::string toString() const;

  /**
   * \brief Returns a pair of strings representing a socket address.
   *
   * \return A pair of strings representing a socket address.
   */
  std::pair<std::string, std::string> toAddressPortStrings() const;

  /**
   * \brief Converts to the corresponding Unix socket address family.
   *
   * \param family A socket address family.
   * \return The corresponding Unix socket address family.
   */
  static int toUnixSocketAddressFamily(SocketAddressFamily family);

  /**
   * \brief Converts to the corresponding socket address family.
   *
   * \param family A Unix socket address family.
   * \return The corresponding socket address family.
   */
  static SocketAddressFamily fromUnixSocketAddressFamily(int family);

 protected:
  /**
   * \brief Socket address storage.
   */
  struct sockaddr_storage sockaddr_;

  /**
   * \brief Length of socket address.
   */
  socklen_t sockaddr_size_;
};

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_SOCKET_ADDRESS_H_
