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
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_REACTOR_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_REACTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include "osabstraction/io/event_handler.h"
#include "osabstraction/io/reactor_interface.h"
#include "osabstraction/io/select/ip_select_control.h"
#include "osabstraction/io/select/ipc_select_control.h"

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
   * \param[in]   handle            A handle.
   * \param[in]   event_handler     A pointer to an event handler.
   * \param[in]   event_type_mask   A mask of event types for which the passed event handler should be registered.
   *
   * \param removal_handler A callback that is notified after the successful removal of the event handler.
   */
  void RegisterEventHandler(HandleInterface* handle, EventHandler* event_handler, unsigned int event_type_mask,
                            EventHandlerRemovalHandler* removal_handler) override;

  /**
   * \brief Unregisters a previously registered event handler for a file descriptor.
   *
   * \param[in]   handle            A handle.
   * \param[in]   event_type_mask   A mask of event types for which the event handler should be unregistered.
   */
  void UnregisterEventHandler(HandleInterface* handle, unsigned int event_type_mask) override;

  /**
   * \brief Lets the reactor check whether some events are pending on any registered file descriptors and dispatch
   *        the corresponding event handlers.
   *
   * \param[in]   timeout           A pointer to a time value to wait for events. In case of the null pointer the
   *                                reactor will not block waiting for events.
   */
  void HandleEvents(const struct timeval* timeout = nullptr) override;

  /**
   * \brief Causes the reactor to return from the blocking call of the function handle_events.
   */
  void Unblock() override;

 private:
  /**
   * \brief Function which is executed in the IP select thread.
   *
   * \param[in]     timeout             A pointer to a time value to wait for events.
   * \param[in,out] select_control      Select control which shall be used.
   * \param[in,out] event_queue         Event queue which shall be used.
   * \param[in,out] condition_variable  Condition variable to sync with.
   * \param[in,out] queue_mutex         Mutex which shall be used to protect the event queue from race conditions.
   */
  static void IpSelectThread(const struct timeval* timeout, select::IpSelectControl& select_control,
                             std::vector<select::SelectEvent>& event_queue, std::condition_variable& condition_variable,
                             std::mutex& queue_mutex);

  /**
   * \brief Function which is executed in the IPC select thread.
   *
   * \param[in]     timeout             A pointer to a time value to wait for events.
   * \param[in,out] select_control      Select control which shall be used.
   * \param[in,out] event_queue         Event queue which shall be used.
   * \param[in,out] condition_variable  Condition variable to sync with.
   * \param[in,out] queue_mutex         Mutex which shall be used to protect the event queue from race conditions.
   */
  static void IpcSelectThread(const struct timeval* timeout, select::IpcSelectControl& select_control,
                              std::vector<select::SelectEvent>& event_queue,
                              std::condition_variable& condition_variable, std::mutex& queue_mutex);

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
   * \brief Type for storing EventHandlers.
   */
  using EventHandlerMap = std::map<HandleInterface*, EventHandlerMapEntry>;

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

  /**
   * \brief Select controller for IP sockets.
   */
  select::IpSelectControl ip_select_;

  /**
   * \brief Select controller for IPC sockets.
   */
  select::IpcSelectControl ipc_select_;

  /**
   * \brief Select events which are known to be ready.
   */
  std::vector<select::SelectEvent> cached_events_;

  /**
   * \brief Mutex to protect access to cached_events queue.
   */
  std::mutex cached_events_mutex_;

  /**
   * \brief Condition variable to coordinate select event producing threads with consuming thread.
   */
  std::condition_variable select_events_cv_;

  /**
   * \brief Tells whether select threads have been spawned.
   */
  std::vector<std::thread> select_threads_;

  /**
   * \brief Tells whether select threads have been spawned.
   */
  bool threads_spawned_;
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_REACTOR_H_
