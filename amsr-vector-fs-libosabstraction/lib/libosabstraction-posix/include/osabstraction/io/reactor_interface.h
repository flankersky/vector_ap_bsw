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
 *        \brief  Abstract interface to a reactor pattern.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>  // struct timeval
#include <functional>
#include "osabstraction/io/event_handler.h"
#include "vac/timer/timer_reactor_interface.h"

namespace osabstraction {
namespace io {

/**
 * \brief Reactor event type.
 */
enum EventType { kReadEvent = 1, kWriteEvent = 2, kExceptionEvent = 4 };

/**
 * \brief Forward declaration
 */
class EventHandlerRemovalHandler;

/**
 * \brief Reactor Interface.
 *
 * An implementation of the reactor design pattern.
 */
class ReactorInterface : public vac::timer::TimerReactorInterface {
 public:
  /**
   * \brief Destructor of Reactor.
   */
  virtual ~ReactorInterface() = default;

  /**
   * \brief Typedef for the handle used for event handlers
   */
  using HandleType = int;

  /**
   * \brief Registers an event handler for a file descriptor.
   *
   * \param[in]   handle            A file descriptor.
   * \param[in]   event_handler     A pointer to an event handler.
   * \param[in]   event_type_mask   A mask of event types for which the passed event handler should be registered.
   *
   * \param removal_handler A callback that is notified after the successful removal of the event handler.
   */
  virtual void RegisterEventHandler(HandleType handle, EventHandler* event_handler, unsigned int event_type_mask,
                                    EventHandlerRemovalHandler* removal_handler) = 0;
  /**
   * \brief Unregisters a previously registered event handler for a file descriptor.
   *
   * \param[in]   handle            A file descriptor.
   * \param[in]   event_type_mask   A mask of event types for which the event handler should be unregistered.
   */
  virtual void UnregisterEventHandler(HandleType handle, unsigned int event_type_mask) = 0;

  /**
   * \brief Lets the reactor check whether some events are pending on any registered file descriptors and dispatch
   *        the corresponding event handlers.
   *
   * \param[in]   timeout   A pointer to a time value to wait for events. In case of the null pointer the
   *                        reactora will not block waiting for events.
   */
  virtual void HandleEvents(const struct timeval* timeout = nullptr) = 0;

  /**
   * \brief Causes the reactor to return from the blocking call of the function handle_events.
   */
  virtual void Unblock() = 0;
};

/**
 * \brief Interface for notifications about the successful removal of an event_handler
 */
class EventHandlerRemovalHandler {
 public:
  virtual ~EventHandlerRemovalHandler() = default;

  /**
   * \brief Callback to notify about the successful removal of an event_handler from the reactor.
   */
  virtual void EventHandlerRemoved(ReactorInterface::HandleType removed_handle, EventType event_type) = 0;
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_INTERFACE_H_
