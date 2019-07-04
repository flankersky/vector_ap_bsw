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
#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UDP_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UDP_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "osabstraction/io/network/address/socket_address.h"
#include "osabstraction/io/network/socket/datagram_socket.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Represents a UDP Socket capable.
 */
class UDPSocket : public DatagramSocket {
 public:
  /**
   * \brief Create a UDPSocket connected to the given remote_address.
   */
  static UDPSocket CreateSocket(const address::IPSocketAddress& remote_address);

  /**
   * \brief Create a UDPSocket not connected to a remote address but with a fixed local address
   */
  static UDPSocket CreateSocket(const address::IPSocketAddress& local_address, bool reuse_address);

  /**
   * \brief Create a UDPSocket connected to a remote address with a fixed local address
   */
  static UDPSocket CreateSocket(const address::IPSocketAddress& remote_address,
                                const address::IPSocketAddress& ip_address, bool reuse_address);

  /**
   * \brief Joins a multicast group.
   *
   * \param interface_address An interface IP address.
   * \param group_address A multicast group address that will be subscribed.
   */
  virtual void JoinMulticastGroup(const std::string& interface_address, const std::string& group_address);
  /**
   * \brief Leaves a multicast group.
   *
   * \param interface_address An interface IP address.
   * \param group_address A multicast group address that will be unsubscribed.
   */
  virtual void LeaveMulticastGroup(const std::string& interface_address, const std::string& group_address);
  /**
   * \brief Enable socket to broadcast
   *
   * \param enable A switch to enable/disable the broadcasting permission of the socket
   */
  virtual void SetBroadcast(bool enable);
  /**
   * \brief Sets an interface on which outgoing multicast packets will be sent.
   *
   * \param address An IP address identifying a network interface.
   */
  virtual void SetMulticastSendInterface(const std::string& address);
  /**
   * \brief Sets a TTL value for all outgoing multicast datagrams.
   *
   * \param ttl A TTL value.
   */
  virtual void SetMulticastTTL(std::uint8_t ttl);
  /**
   * \brief Decides whether the sent data is looped back to the sending host or not.
   *
   * \param enable Indicates whether the looping back will be enabled or not.
   */
  virtual void SetMulticastLoop(bool enable);

 protected:
  UDPSocket() = default;

  /**
   * \brief Constructor for a UDPSocket of a given Address Family.
   */
  explicit UDPSocket(address::SocketAddressFamily address_family) : DatagramSocket(address_family) {}
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_UDP_SOCKET_H_
