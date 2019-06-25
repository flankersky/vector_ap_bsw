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
 *        \brief  Event Handlers to handle I/O events dispatched by the reactor pattern.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_EVENT_HANDLER_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_EVENT_HANDLER_H_

#include <atomic>

namespace osabstraction {
namespace io {

/**
 * \brief Reactor event handler.
 */
class EventHandler {
 public:
  /**
   * \brief Constructor
   */
  EventHandler() : event_handler_valid_(true) {}

  /**
   * \brief Destructor of EventHandler.
   */
  virtual ~EventHandler() = default;

  /**
   * \brief Read event handler.
   *
   * \param handle A file descriptor.
   * \return true in case the EventHandler can be called again, false otherwise
   *
   * This function is called by reactor whenever the file descriptor becomes readable.
   */
  virtual bool HandleRead(int handle) {
    (void)handle;  // Mark variable as "used" for compiler
    return event_handler_valid_;
  }

  /**
   * \brief Write event handler.
   *
   * \param handle A file descriptor.
   * \return true in case the EventHandler can be called again, false otherwise
   *
   * This function is called by reactor whenever the file descriptor becomes writable.
   */
  virtual bool HandleWrite(int handle) {
    (void)handle;  // Mark variable as "used" for compiler
    return event_handler_valid_;
  }

  /**
   * \brief Exception event handler.
   *
   * \param handle A file descriptor.
   * \return true in case the EventHandler can be called again, false otherwise
   *
   * This function is called by reactor whenever an exception condition occurs on the file descriptor.
   */
  virtual bool HandleException(int handle) {
    (void)handle;  // Mark variable as "used" for compiler
    return event_handler_valid_;
  }

  /**
   * \brief Determine whether this event handler is in a valid state and can be called
   *
   * \return true if the event handler can be called, false otherwise.
   */
  bool IsValid() const { return event_handler_valid_; }

 protected:
  /**
   * \brief Flag to indicate whether it is valid to call this EventHandler
   */
  std::atomic_bool event_handler_valid_;
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_EVENT_HANDLER_H_
