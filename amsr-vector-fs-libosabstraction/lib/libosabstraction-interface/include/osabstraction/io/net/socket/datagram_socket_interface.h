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
/**        \file  datagram_socket_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_DATAGRAM_SOCKET_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_DATAGRAM_SOCKET_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/net/socket/client_socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief Abstract interface to work with datagram sockets.
 */
class DatagramSocketInterface : public virtual ClientSocketInterface {};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_DATAGRAM_SOCKET_INTERFACE_H_
