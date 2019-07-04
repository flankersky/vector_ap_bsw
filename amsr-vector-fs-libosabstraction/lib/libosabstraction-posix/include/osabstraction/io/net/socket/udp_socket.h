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
 *        \brief  Implementation of the UDP socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>  // uint8_t
#include "osabstraction/io/net/socket/socket.h"
#include "osabstraction/io/net/socket/udp_socket_interface.h"
#include "vac/container/string_view.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief  Implementation of the UDP socket interface.
 */
class UdpSocket : public virtual UdpSocketInterface, public Socket {
 public:
  /**
   * \brief Creates a UDP socket matching the given socket address family.
   */
  explicit UdpSocket(address::SocketAddressFamily address_family) : Socket(address_family, SocketType::kDatagram) {}

  /**
   * \brief Connects a socket to specified remote address.
   *
   * \param[in] remote_address A remote socket address to which the socket should be connected.
   */
  void Connect(const address::SocketAddress& remote_address) override;

  /**
   * \brief Joins a multicast group.
   *
   * \param[in] interface_address An interface IP address.
   * \param[in] group_address A multicast group address that will be subscribed.
   */
  void JoinMulticastGroup(const vac::container::string_view& interface_address,
                          const vac::container::string_view& group_address) override;

  /**
   * \brief Leaves a multicast group.
   *
   * \param[in] interface_address An interface IP address.
   * \param[in] group_address A multicast group address that will be unsubscribed.
   */
  void LeaveMulticastGroup(const vac::container::string_view& interface_address,
                           const vac::container::string_view& group_address) override;

  /**
   * \brief Enable socket to broadcast
   *
   * \param[in] enable A switch to enable/disable the broadcasting permission of the socket
   */
  void SetBroadcast(bool enable) override;

  /**
   * \brief Sets an interface on which outgoing multicast packets will be sent.
   *
   * \param[in] address An IP address identifying a network interface.
   */
  void SetMulticastSendInterface(const vac::container::string_view& address) override;

  /**
   * \brief Sets a TTL value for all outgoing multicast datagrams.
   *
   * \param[in] ttl A TTL value.
   */
  void SetMulticastTTL(std::uint8_t ttl) override;

  /**
   * \brief Decides whether the sent data is looped back to the sending host or not.
   *
   * \param[in] enable Indicates whether the looping back will be enabled or not.
   */
  void SetMulticastLoop(bool enable) override;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_H_
