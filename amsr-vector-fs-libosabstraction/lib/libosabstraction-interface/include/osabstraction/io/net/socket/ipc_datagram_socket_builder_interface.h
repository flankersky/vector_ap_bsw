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
 *        \brief  Abstract interface to create datagram oriented IPC sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_BUILDER_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_BUILDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace osabstraction {
namespace io {
namespace net {

namespace address {
class IpcSocketAddress;
}  // namespace address

namespace socket {

class IpcDatagramSocket;

/**
 * \brief Abstract interface to create datagram oriented IPC sockets.
 */
class IpcDatagramSocketBuilderInterface {
 public:
  virtual ~IpcDatagramSocketBuilderInterface() = default;

  /**
   * \brief Creates an IPC datagram socket from a local IPC socket address.
   * \param[in] local_address   The address to open the socket at.
   * \returns A socket not connected to any address.
   */
  virtual IpcDatagramSocket BuildSocket(const address::IpcSocketAddress& local_address) = 0;

  /**
   * \brief Creates an IPC datagram socket from a local IPC socket address, connected to the given remote address.
   * \param[in] local_address   The address to open the socket at.
   * \param[in] remote_address  The address to connect to.
   * \returns A socket connected to the given remote_address.
   */
  virtual IpcDatagramSocket BuildSocket(const address::IpcSocketAddress& local_address,
                                        const address::IpcSocketAddress& remote_address) = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_DATAGRAM_SOCKET_BUILDER_INTERFACE_H_
