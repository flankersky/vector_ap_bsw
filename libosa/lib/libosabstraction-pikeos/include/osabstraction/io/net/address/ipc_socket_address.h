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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stddef.h>
#include <sys/qport.h>  // qport_*
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
   * \brief      Contructor creates an invalid IPC socket address.
   */
  IpcSocketAddress() = default;

  /**
   * \brief      Constructor creates an IPC socket address from
   *             qport_sap_sockaddr_ipv4_t.
   *
   * \param[in]  ipc_addr  The IPC socket address
   */
  explicit IpcSocketAddress(const qport_sap_sockaddr_ipv4_t& ipc_addr) : SocketAddress(ipc_addr) {}

  /**
   * \brief      Converter to qport_sap_sockaddr_ipv4_t.
   *
   * \return     A pointer to qport_sap_sockaddr_ipv4_t.
   */
  const qport_sap_sockaddr_ipv4_t* toIpcSockAddr() const {
    return reinterpret_cast<const qport_sap_sockaddr_ipv4_t*>(&sockaddr_);
  }
};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_H_
