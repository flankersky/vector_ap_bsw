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
 *        \brief  Implementation of the reactor pattern interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/select.h>

#include <array>
#include <atomic>
#include <map>
#include "vac/testing/test_adapter.h"

#include "osabstraction/io/event_handler.h"
#include "osabstraction/io/reactor_interface.h"

namespace osabstraction {
namespace io {

/**
 * \brief Reactor.
 *
 * An implementation of the reactor design pattern.
 */
class Reactor : public ReactorInterface {
 public:
  /**
   * \brief Constructor of Reactor.
   */
  Reactor();

  /**
   * \brief Destructor of Reactor.
   */
  ~Reactor();

  /**
   * \brief Registers an event handler for a file descriptor.
   *
   * \param handle A file descriptor.
   * \param event_handler A pointer to an event handler.
   * \param event_type_mask A mask of event types for which the passed event handler should be registered.
   * \param removal_handler A callback that is notified after the successful removal of the event handler.
   */
  void RegisterEventHandler(HandleType handle, EventHandler* event_handler, unsigned int event_type_mask,
                            EventHandlerRemovalHandler* removal_handler) override;

  /**
   * \brief Unregisters a previously registered event handler for a file descriptor.
   *
   * \param handle A file descriptor.
   * \param event_type_mask A mask of event types for which the event handler should be unregistered.
   */
  void UnregisterEventHandler(HandleType handle, unsigned int event_type_mask) override;

  /**
   * \brief Lets the reactor check whether some events are pending on any registered file descriptors and dispatch
   *        the corresponding event handlers.
   *
   * \param timeout A pointer to a time value to wait for events. In case of the null pointer the reactor
   *                will not block waiting for events.
   */
  void HandleEvents(const struct timeval* timeout = nullptr) override;

  /**
   * \brief Causes the reactor to return from the blocking call of the function handle_events.
   */
  void Unblock() override;

 protected:
  /**
   * \brief Structure for storing registered event handlers.
   */
  struct EventHandlerMapEntry {
    /**
     * \brief EventHandler which is called in case of an I/O event on a file descriptor.
     */
    EventHandler* event_handler_;

    /**
     * \brief Handler which is called if the event handler has decided to be removed from the reactor.
     */
    EventHandlerRemovalHandler* removal_handler_;
  };

  /**
   * \brief Type for storing EventHandlerRegistrations
   */
  using EventHandlerMap = std::map<HandleType, EventHandlerMapEntry>;

  /**
   * \brief Notifies registered event handlers for which events are available.
   */
  void DispatchEventHandlers();

  /**
   * \brief Sets up file descriptor sets required for the select system call.
   */
  int SetupFdSets();

  /**
   * \brief Sends wakeup signal to unblocks the select system call being executed in the function handle_events.
   */
  void SendWakeup();

  /**
   * \brief Handles wakeup signal sent by the function send_wakeup.
   */
  void HandleWakeup();

  /**
   * \brief Pipe used for unblocking a blocked select call.
   */
  std::array<int, 2> wakeup_pipe_;

  /**
   * \brief Indicates whether the reactor is currently dispatching occurred events.
   */
  std::atomic_bool dispatching_;

  /**
   * \brief File descriptor sets required for the select system call.
   */
  fd_set rfds_;

  /**
   * \brief File descriptor sets required for the select system call.
   */
  fd_set wfds_;

  /**
   * \brief File descriptor sets required for the select system call.
   */
  fd_set efds_;

  /**
   * \brief Event handlers for read events.
   */
  EventHandlerMap read_event_handlers_;

  /**
   * \brief Event handlers for write events.
   */
  EventHandlerMap write_event_handlers_;

  /**
   * \brief Event handlers for exception events.
   */
  EventHandlerMap exception_event_handlers_;

 private:
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_NullHandler);
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_NonNullHandler_NoRegistration);
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_NonNullHandler_AllRegistration);
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_MultipleHandler);
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_MultipleHandler_Redefine);
  FRIEND_TEST(ReactorTestFixture, RegisterEventHandler_MultiplySubmittedHandler);
  FRIEND_TEST(Reactor, RegisterEventHandler_SecondThread);
  FRIEND_TEST(Reactor, RegisterEventHandler_SecondThreadMultipleHandlers);
  FRIEND_TEST(Reactor, RemoveEventHandler_Read);
  FRIEND_TEST(Reactor, RemoveEventHandler_Write);
  FRIEND_TEST(Reactor, RemoveEventHandler_Exception);
  FRIEND_TEST(Reactor, InsertElementsDuringDispatchEventHandlers);
  FRIEND_TEST(Reactor, RegisterEventHandler_InvokedInNewThread);
  FRIEND_TEST(Reactor, RegisterEventHandler_SingleThread);
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_REACTOR_H_
