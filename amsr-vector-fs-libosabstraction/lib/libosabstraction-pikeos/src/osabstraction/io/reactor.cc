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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/reactor.h"
#include <unistd.h>
#include <cassert>
#include <functional>
#include <iostream>
#include <stdexcept>
#include "osabstraction/internal/sap_port_interface.h"
#include "osabstraction/io/handle_interface.h"
#include "osabstraction/io/net/socket/ip_socket.h"
#include "osabstraction/io/select/select_event.h"

namespace osabstraction {
namespace io {

Reactor::Reactor()
    : read_event_handlers_(),
      write_event_handlers_(),
      exception_event_handlers_(),
      ip_select_(),
      threads_spawned_(false) {}

Reactor::~Reactor() { Unblock(); }

void Reactor::RegisterEventHandler(HandleInterface* handle, EventHandler* event_handler, unsigned int event_type_mask,
                                   EventHandlerRemovalHandler* removal_handler) {
  if (event_handler == nullptr) {
    throw std::invalid_argument("Reactor::RegisterEventHandler: event_handler is null");
  }

  if (event_type_mask & EventType::kReadEvent) {
    auto result = read_event_handlers_.insert(EventHandlerMap::value_type(handle, {event_handler, removal_handler}));
    if (!result.second) {
      throw std::invalid_argument("Reactor::RegisterEventHandler: read_event_handler already registered for handle");
    }
  }
  if (event_type_mask & EventType::kWriteEvent) {
    auto result = write_event_handlers_.insert(EventHandlerMap::value_type(handle, {event_handler, removal_handler}));
    if (!result.second) {
      throw std::invalid_argument("Reactor::RegisterEventHandler: write_event_handler already registered for handle");
    }
  }
  if (event_type_mask & EventType::kExceptionEvent) {
    auto result =
        exception_event_handlers_.insert(EventHandlerMap::value_type(handle, {event_handler, removal_handler}));
    if (!result.second) {
      throw std::invalid_argument(
          "Reactor::RegisterEventHandler: exception_event_handler already registered for handle");
    }
  }

  switch (handle->GetType()) {
    case HandleType::kIpSocketHandle:
      ip_select_.Add(dynamic_cast<net::socket::IpSocket*>(handle), event_type_mask);
      break;
    case HandleType::kIpcSocketHandle:
      ipc_select_.Add(dynamic_cast<net::socket::IpcSocket*>(handle));
      break;
    default:
      assert(false);
  }
}

void Reactor::UnregisterEventHandler(HandleInterface* handle, unsigned int event_type_mask) {
  if (event_type_mask & EventType::kReadEvent) {
    read_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kWriteEvent) {
    write_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kExceptionEvent) {
    exception_event_handlers_.erase(handle);
  }

  switch (handle->GetType()) {
    case HandleType::kIpSocketHandle:
      ip_select_.Remove(dynamic_cast<net::socket::IpSocket*>(handle), event_type_mask);
      break;
    case HandleType::kIpcSocketHandle:
      ipc_select_.Add(dynamic_cast<net::socket::IpcSocket*>(handle));
      break;
    default:
      assert(false);
  }
}

void Reactor::IpSelectThread(const struct timeval* timeout, select::IpSelectControl& select_control,
                             std::vector<select::SelectEvent>& event_queue, std::condition_variable& condition_variable,
                             std::mutex& queue_mutex) {
  // Do forever:
  for (;;) {
    // Wait until consumer needs data.
    while (!event_queue.empty()) {
      std::unique_lock<std::mutex> lock{queue_mutex};
      condition_variable.wait(lock);
    }

    // Wait for events
    auto event = select_control.Select(timeout);

    // If there was an event:
    if (event.GetEvent() != EventType::kNoEvent) {
      // Get lock and push event into queue.
      std::unique_lock<std::mutex> lock{queue_mutex};
      event_queue.push_back(event);

      // Notify consumer.
      condition_variable.notify_all();
    }
  }
}

void Reactor::IpcSelectThread(const struct timeval* timeout, select::IpcSelectControl& select_control,
                              std::vector<select::SelectEvent>& event_queue,
                              std::condition_variable& condition_variable, std::mutex& queue_mutex) {
  // Do forever:
  for (;;) {
    // Wait until consumer needs data.
    while (!event_queue.empty()) {
      std::unique_lock<std::mutex> lock{queue_mutex};
      condition_variable.wait(lock);
    }

    // Wait for events
    // TODO(visasl): Implement timeout for IPC select.
    // TODO(visasl): Implement full set support for IPC select.
    (void)timeout;
    auto event = select_control.Select(internal::EventType::kRead | internal::EventType::kConnect);

    // If there was an event:
    if (event.GetEvent() != EventType::kNoEvent) {
      // Get lock and push event into queue.
      std::unique_lock<std::mutex> lock{queue_mutex};
      event_queue.push_back(event);

      // Notify consumer.
      condition_variable.notify_all();
    }
  }
}

void Reactor::HandleEvents(const struct timeval* timeout) {
  // If event cache is empty:
  if (cached_events_.empty()) {
    if (!threads_spawned_) {
      // Create threads to wait for select events.
      select_threads_.emplace_back(IpSelectThread, timeout, std::ref(ip_select_), std::ref(cached_events_),
                                   std::ref(select_events_cv_), std::ref(cached_events_mutex_));
      select_threads_.emplace_back(IpcSelectThread, timeout, std::ref(ipc_select_), std::ref(cached_events_),
                                   std::ref(select_events_cv_), std::ref(cached_events_mutex_));

      // TODO(visasl): Join or kill spawned threads.
      threads_spawned_ = true;
    }

    // Notify producers to create select events.
    select_events_cv_.notify_all();
  }

  // Wait for select events.
  while (cached_events_.empty()) {
    std::unique_lock<std::mutex> lock{cached_events_mutex_};
    // TODO(visasl): Wait with timeout.
    select_events_cv_.wait(lock);
  }

  // While there are cached events:
  while (!cached_events_.empty()) {
    select::SelectEvent event;
    {
      // Get lock and pop event from queue.
      std::unique_lock<std::mutex> lock{cached_events_mutex_};

      event = cached_events_.back();
      cached_events_.pop_back();
    }

    // Call event handler
    switch (event.GetEvent()) {
      case EventType::kReadEvent: {
        HandleInterface* handle = event.GetHandle();
        auto entry = read_event_handlers_[handle];
        int fd = handle->GetRawFiledescriptor();
        bool call_handler_again = entry.event_handler_->HandleRead(fd);
        if (!call_handler_again) {
          EventHandlerRemovalHandler* removal_handler = entry.removal_handler_;
          write_event_handlers_.erase(handle);
          if (removal_handler != nullptr) {
            removal_handler->EventHandlerRemoved(fd, EventType::kReadEvent);
          }
        }
      } break;
      case EventType::kWriteEvent: {
        HandleInterface* handle = event.GetHandle();
        auto entry = write_event_handlers_[handle];
        int fd = handle->GetRawFiledescriptor();
        bool call_handler_again = entry.event_handler_->HandleWrite(fd);
        if (!call_handler_again) {
          EventHandlerRemovalHandler* removal_handler = entry.removal_handler_;
          write_event_handlers_.erase(handle);
          if (removal_handler != nullptr) {
            removal_handler->EventHandlerRemoved(fd, EventType::kWriteEvent);
          }
        }
      } break;
      case EventType::kExceptionEvent: {
        HandleInterface* handle = event.GetHandle();
        auto entry = exception_event_handlers_[handle];
        int fd = handle->GetRawFiledescriptor();
        bool call_handler_again = entry.event_handler_->HandleException(fd);
        if (!call_handler_again) {
          EventHandlerRemovalHandler* removal_handler = entry.removal_handler_;
          write_event_handlers_.erase(handle);
          if (removal_handler != nullptr) {
            removal_handler->EventHandlerRemoved(fd, EventType::kExceptionEvent);
          }
        }
      } break;
      case EventType::kNoEvent:
        break;
      default:
        assert(false);
        break;
    }
  }
}

void Reactor::Unblock() {}

}  // namespace io
}  // namespace osabstraction
