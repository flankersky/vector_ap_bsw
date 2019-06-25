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
 *        \brief  Contains the implementation of the SelectEvent class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_SELECT_EVENT_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_SELECT_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/reactor_interface.h"

namespace osabstraction {
namespace io {

class Handle;

namespace select {

/**
 * \brief      Class to describe a select event.
 */
class SelectEvent {
 public:
  /**
   * \brief      Constructor.
   */
  SelectEvent() : event_(EventType::kNoEvent), handle_(nullptr) {}

  /**
   * \brief      Constructor.
   *
   * \param[in]  event   Occurred event.
   * \param[in]  handle  Handle which triggered the event.
   */
  SelectEvent(EventType event, HandleInterface* handle) : event_(event), handle_(handle) {}

  /**
   * \brief      Event getter.
   *
   * \return     The event.
   */
  EventType GetEvent() { return event_; }

  /**
   * \brief      Socket getter.
   *
   * \return     The socket.
   */
  HandleInterface* GetHandle() { return handle_; }

 private:
  /**
   * Type of the event.
   */
  EventType event_;

  /**
   * Handle which triggered the event.
   */
  HandleInterface* handle_;
};

}  // namespace select
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_SELECT_SELECT_EVENT_H_
