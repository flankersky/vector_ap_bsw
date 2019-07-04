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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IP_SELECT_CONTROL_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IP_SELECT_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/select.h>
#include <cstdint>
#include <map>
#include <vector>
#include "osabstraction/io/net/socket/ip_socket.h"
#include "osabstraction/io/select/select_event.h"

namespace osabstraction {
namespace io {
namespace select {

/** Controls the select() handling for IP socket handles. */
class IpSelectControl {
 public:
  /**
   * \brief Type of handles which is monitored by this select control.
   */
  using Handle = net::socket::IpSocket;

  /**
   * \brief      Default constructor.
   */
  IpSelectControl();

  /**
   * \brief      Adds a handle to the list of monitored handles on Select() call.
   *
   * \param[in]  handle       Handle to add.
   * \param[in]  event_types  Event types to monitor.
   */
  void Add(Handle* handle, std::uint32_t event_types);

  /**
   * \brief      Removes a handle to the list of monitored handles on Select() call.
   *
   * \param[in]  handle       Handle to remove.
   * \param[in]  event_types  Event types to monitor.
   */
  void Remove(Handle* handle, std::uint32_t event_types);

  /**
   * \brief      Blocks the thread until an event has occurred on one of the monitored handles.
   * \param[in]  timeout  Blocking will end if the given timeout occurs.
   * \return     Information about the occurred event.
   */
  SelectEvent Select(const struct timeval* timeout);

 private:
  using HandleMap = std::map<int, Handle*>;
  /**
   * \brief      Clears lists of monitored handles.
   */
  void Clear();

  /**
   * \brief Sets up file descriptor set required for the select system call.
   *
   * \param[in]   handles   Handles which shall be set in the given file descriptor set.
   * \param[out]  set       File descriptor set to change.
   *
   * \return Returns the maximum file descriptor.
   */
  int SetupFdSet(HandleMap& handles, fd_set& set);

  /**
   * \brief File descriptor sets required for the select() system call.
   */
  fd_set read_fd_set_;

  /**
   * \brief File descriptor sets required for the select() system call.
   */
  fd_set write_fd_set_;

  /**
   * \brief File descriptor sets required for the select() system call.
   */
  fd_set exception_fd_set_;

  /**
   * \brief File descriptors which are monitored.
   */
  HandleMap read_handles_;

  /**
   * \brief File descriptors which are monitored.
   */
  HandleMap write_handles_;

  /**
   * \brief File descriptors which are monitored.
   */
  HandleMap exception_handles_;

  /**
   * \brief Select events which are known to be ready.
   */
  std::vector<SelectEvent> cached_events_;
};

}  // namespace select
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_IP_SELECT_CONTROL_H_
