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
 *        \brief  Abstract interface to work with IPC addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/address/socket_address_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

/**
 * \brief Abstract interface to work with IPC addresses.
 */
class IpcSocketAddressInterface : public virtual SocketAddressInterface {};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_INTERFACE_H_
