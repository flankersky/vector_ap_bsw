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
 *        \brief  Abstract interface to work with UDP sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>  // uint8_t
#include "osabstraction/io/net/socket/datagram_socket_interface.h"
#include "osabstraction/io/net/socket/socket_interface.h"
#include "vac/container/string_view.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief Abstract interface to work with stream oriented IPC client sockets.
 */
class UdpSocketInterface : public virtual DatagramSocketInterface {
 public:
  /**
   * \brief Joins a multicast group.
   *
   * \param[in] interface_address An interface IP address.
   * \param[in] group_address A multicast group address that will be subscribed.
   */
  virtual void JoinMulticastGroup(const vac::container::string_view& interface_address,
                                  const vac::container::string_view& group_address) = 0;

  /**
   * \brief Leaves a multicast group.
   *
   * \param[in] interface_address An interface IP address.
   * \param[in] group_address A multicast group address that will be unsubscribed.
   */
  virtual void LeaveMulticastGroup(const vac::container::string_view& interface_address,
                                   const vac::container::string_view& group_address) = 0;

  /**
   * \brief Enable socket to broadcast
   *
   * \param[in] enable A switch to enable/disable the broadcasting permission of the socket
   */
  virtual void SetBroadcast(bool enable) = 0;

  /**
   * \brief Sets an interface on which outgoing multicast packets will be sent.
   *
   * \param[in] address An IP address identifying a network interface.
   */
  virtual void SetMulticastSendInterface(const vac::container::string_view& address) = 0;

  /**
   * \brief Sets a TTL value for all outgoing multicast datagrams.
   *
   * \param[in] ttl A TTL value.
   */
  virtual void SetMulticastTTL(std::uint8_t ttl) = 0;

  /**
   * \brief Decides whether the sent data is looped back to the sending host or not.
   *
   * \param[in] enable Indicates whether the looping back will be enabled or not.
   */
  virtual void SetMulticastLoop(bool enable) = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_UDP_SOCKET_INTERFACE_H_
