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
#include <iostream>
#include <stdexcept>

namespace osabstraction {
namespace io {

Reactor::Reactor()
    : wakeup_pipe_(),
      dispatching_(false),
      rfds_(),
      wfds_(),
      efds_(),
      read_event_handlers_(),
      write_event_handlers_(),
      exception_event_handlers_() {
  /* Create a pipe for unblocking the select system call */
  int ret = pipe(wakeup_pipe_.data());
  if (ret != 0) {
    std::runtime_error("pipe");
  }
}

Reactor::~Reactor() {
  Unblock();
  // TODO(PAASR-606): Verify that no method of Reactor is still running when the object is destroyed.
  ::close(wakeup_pipe_[0]);
  ::close(wakeup_pipe_[1]);
}

void Reactor::RegisterEventHandler(int handle, EventHandler* event_handler, unsigned int event_type_mask,
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
  if (!dispatching_) {
    SendWakeup();
  }
}

void Reactor::UnregisterEventHandler(int handle, unsigned int event_type_mask) {
  size_t count = 0;
  if (event_type_mask & EventType::kReadEvent) {
    count += read_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kWriteEvent) {
    count += write_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kExceptionEvent) {
    count += exception_event_handlers_.erase(handle);
  }
  if (count > 0 && !dispatching_) {
    SendWakeup();
  }
}

void Reactor::HandleEvents(const struct timeval* timeout) {
  struct timeval tv;
  struct timeval* tv_ptr = nullptr;
  if (timeout != nullptr) {
    // select() on Linux modifies the object under tv_ptr, therefore we copy the passed data.
    tv = *timeout;
    tv_ptr = &tv;
  }

  int maxfd = SetupFdSets();

  int count = select(maxfd + 1, &rfds_, &wfds_, &efds_, tv_ptr);
  if (count > 0) {
    DispatchEventHandlers();
  }
}

void Reactor::Unblock() { SendWakeup(); }

int Reactor::SetupFdSets() {
  FD_ZERO(&rfds_);
  FD_ZERO(&wfds_);
  FD_ZERO(&efds_);

  FD_SET(wakeup_pipe_[0], &rfds_);
  int maxfd = wakeup_pipe_[0];

  for (auto it = read_event_handlers_.begin(); it != read_event_handlers_.end();) {
    if (it->second.event_handler_->IsValid()) {
      FD_SET(it->first, &rfds_);
      maxfd = std::max(maxfd, it->first);
      ++it;
    } else {
      EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
      int handle = it->first;
      it = read_event_handlers_.erase(it);
      if (removal_handler != nullptr) {
        removal_handler->EventHandlerRemoved(handle, EventType::kReadEvent);
      }
    }
  }

  for (auto it = write_event_handlers_.begin(); it != write_event_handlers_.end();) {
    if (it->second.event_handler_->IsValid()) {
      FD_SET(it->first, &wfds_);
      maxfd = std::max(maxfd, it->first);
      ++it;
    } else {
      EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
      int handle = it->first;
      it = write_event_handlers_.erase(it);
      if (removal_handler != nullptr) {
        removal_handler->EventHandlerRemoved(handle, EventType::kWriteEvent);
      }
    }
  }

  for (auto it = exception_event_handlers_.begin(); it != exception_event_handlers_.end();) {
    if (it->second.event_handler_->IsValid()) {
      FD_SET(it->first, &efds_);
      maxfd = std::max(maxfd, it->first);
      ++it;
    } else {
      EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
      int handle = it->first;
      it = exception_event_handlers_.erase(it);
      if (removal_handler != nullptr) {
        removal_handler->EventHandlerRemoved(handle, EventType::kExceptionEvent);
      }
    }
  }

  return maxfd;
}

void Reactor::DispatchEventHandlers() {
  dispatching_ = true;

  if (FD_ISSET(wakeup_pipe_[0], &rfds_)) {
    HandleWakeup();
  }

  for (auto it = read_event_handlers_.begin(); it != read_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &rfds_)) {
      if (entry.event_handler_->IsValid() && entry.event_handler_->HandleRead(handle)) {
        ++it;
      } else {
        EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
        it = read_event_handlers_.erase(it);
        if (removal_handler != nullptr) {
          removal_handler->EventHandlerRemoved(handle, EventType::kReadEvent);
        }
      }
    } else {
      ++it;
    }
  }

  for (auto it = write_event_handlers_.begin(); it != write_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &wfds_)) {
      if (entry.event_handler_->IsValid() && entry.event_handler_->HandleWrite(handle)) {
        ++it;
      } else {
        EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
        it = write_event_handlers_.erase(it);
        if (removal_handler != nullptr) {
          removal_handler->EventHandlerRemoved(handle, EventType::kWriteEvent);
        }
      }
    } else {
      ++it;
    }
  }

  for (auto it = exception_event_handlers_.begin(); it != exception_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &efds_)) {
      if (entry.event_handler_->IsValid() && entry.event_handler_->HandleException(handle)) {
        ++it;
      } else {
        EventHandlerRemovalHandler* removal_handler = it->second.removal_handler_;
        it = exception_event_handlers_.erase(it);
        if (removal_handler != nullptr) {
          removal_handler->EventHandlerRemoved(handle, EventType::kExceptionEvent);
        }
      }
    } else {
      ++it;
    }
  }

  dispatching_ = false;
}

void Reactor::SendWakeup() {
  char dummy;

  ssize_t ret = ::write(wakeup_pipe_[1], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("write");
  }
}

void Reactor::HandleWakeup() {
  char dummy;

  ssize_t ret = ::read(wakeup_pipe_[0], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("read");
  }
}

}  // namespace io
}  // namespace osabstraction
