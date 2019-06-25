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
 *        \brief  Contains the implementation of the IpcSelectControl class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IPC_SELECT_CONTROL_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IPC_SELECT_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <cstdint>
#include <map>
#include "osabstraction/io/net/socket/ipc_socket.h"
#include "osabstraction/io/select/select_event.h"

namespace osabstraction {
namespace io {
namespace select {

/** Controls the select() handling for IPC sockets. */
class IpcSelectControl {
 public:
  /**
   * \brief Type of handles which is monitored by this select control.
   */
  using Handle = net::socket::IpcSocket;

  /**
   * \brief      Default constructor.
   */
  IpcSelectControl() {}

  /**
   * \brief      Adds a handle to the list of monitored handles on Select() call.
   *
   * \param[in]  handle       Handle to add.
   */
  void Add(Handle* handle);

  /**
   * \brief      Blocks the thread until an event has occurred on one of the monitored IPC sockets.
   *
   * \param[in]  event_mask   Event types the caller is interested in.
   *
   * \return     Information about the occurred event.
   */
  SelectEvent Select(std::uint32_t event_mask);

 private:
  /**
   * \brief Connections which are monitored.
   */
  std::map<std::uint32_t, Handle*> address_map_;
};

}  // namespace select
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IPC_SELECT_CONTROL_H_
