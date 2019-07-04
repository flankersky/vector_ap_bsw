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
 *        \brief  IPC socket address implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/un.h>  // sockaddr_un
#include "osabstraction/io/net/address/ipc_socket_address_interface.h"
#include "osabstraction/io/net/address/socket_address.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

/**
 * \brief  IPC socket address implementation.
 */
class IpcSocketAddress : public virtual IpcSocketAddressInterface, public SocketAddress {
 public:
  /**
   * \brief Constructor of IpcSocketAddress.
   *
   * \param sun A pointer to a Unix domain socket address.
   */
  explicit IpcSocketAddress(const struct sockaddr_un& sun) {
    *reinterpret_cast<struct sockaddr_un*>(&sockaddr_) = sun;
    sockaddr_size_ = sizeof(sun);
  }

  /**
   * \brief Converter to sockaddr_un.
   *
   * \return A pointer to sockaddr_un.
   */
  const struct sockaddr_un* toSockAddrUn() const { return reinterpret_cast<const struct sockaddr_un*>(&sockaddr_); }
};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_
