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
 *        \brief  Abstract interface to create IPC socket addresses.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_BUILDER_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_BUILDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/string_view.h"

namespace osabstraction {
namespace io {
namespace net {
namespace address {

class IpcSocketAddress;

/**
 * \brief Abstract interface to create IPC socket addresses.
 */
class IpcSocketAddressBuilderInterface {
 public:
  virtual ~IpcSocketAddressBuilderInterface() = default;

  /**
   * \brief Creates IPC socket address from a given path.
   * \param[in] path  A path for an IPC socket address.
   * \return Created IPC socket address object.
   */
  virtual IpcSocketAddress BuildFromPath(const vac::container::string_view& path) = 0;
};

}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_IPC_SOCKET_ADDRESS_BUILDER_INTERFACE_H_
