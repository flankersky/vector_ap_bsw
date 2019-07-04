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
 *        \brief  Contains the implementation of the IpSelectControl class.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/select/ip_select_control.h"
#include <algorithm>  // std::max()
#include <cassert>
#include <iostream>
#include "osabstraction/io/reactor_interface.h"

namespace osabstraction {
namespace io {
namespace select {

/**
 * \brief Invalid file descriptor ID .
 */
constexpr int kInvalidFileDescriptor = -1;

IpSelectControl::IpSelectControl() {}

void IpSelectControl::Add(Handle* handle, std::uint32_t event_types) {
  assert(handle != nullptr);
  assert(event_types != 0);

  int file_descriptor = handle->GetRawFiledescriptor();
  assert(file_descriptor != kInvalidFileDescriptor);

  // Add file descriptor to the event type related file descriptor sets.
  if (event_types & EventType::kReadEvent) {
    read_handles_.insert(HandleMap::value_type(file_descriptor, handle));
  }

  if (event_types & EventType::kWriteEvent) {
    write_handles_.insert(HandleMap::value_type(file_descriptor, handle));
  }

  if (event_types & EventType::kExceptionEvent) {
    exception_handles_.insert(HandleMap::value_type(file_descriptor, handle));
  }
}

void IpSelectControl::Remove(Handle* handle, std::uint32_t event_types) {
  assert(handle != nullptr);
  assert(event_types != 0);

  const int file_descriptor = handle->GetRawFiledescriptor();
  assert(file_descriptor != kInvalidFileDescriptor);

  // Add file descriptor to the event type related file descriptor sets.
  if (event_types & EventType::kReadEvent) {
    read_handles_.erase(file_descriptor);
  }

  if (event_types & EventType::kWriteEvent) {
    write_handles_.erase(file_descriptor);
  }

  if (event_types & EventType::kExceptionEvent) {
    exception_handles_.erase(file_descriptor);
  }
}

SelectEvent IpSelectControl::Select(const struct timeval* timeout) {
  int maximum_fd = kInvalidFileDescriptor;
  struct timeval tv;
  struct timeval* tv_ptr = nullptr;

  // If no cached events available, wait for events.
  if (cached_events_.empty()) {
    if (timeout != nullptr) {
      // select() on Linux modifies the object under tv_ptr, therefore we copy the passed data.
      tv = *timeout;
      tv_ptr = &tv;
    }

    {
      int tmp_max_fd;
      tmp_max_fd = SetupFdSet(read_handles_, read_fd_set_);
      maximum_fd = std::max(tmp_max_fd, maximum_fd);

      tmp_max_fd = SetupFdSet(write_handles_, write_fd_set_);
      maximum_fd = std::max(tmp_max_fd, maximum_fd);

      tmp_max_fd = SetupFdSet(exception_handles_, exception_fd_set_);
      maximum_fd = std::max(tmp_max_fd, maximum_fd);
    }

    int count = ::select(maximum_fd + 1, &read_fd_set_, &write_fd_set_, &exception_fd_set_, tv_ptr);

    if (count > 0) {
      // Create cached events from select() call.
      for (const auto it : read_handles_) {
        if (FD_ISSET(it.first, &read_fd_set_)) {
          HandleInterface* handle = it.second;
          cached_events_.push_back({EventType::kReadEvent, handle});
        }
      }

      for (const auto it : write_handles_) {
        if (FD_ISSET(it.first, &write_fd_set_)) {
          HandleInterface* handle = it.second;
          cached_events_.push_back({EventType::kWriteEvent, handle});
        }
      }

      for (const auto it : exception_handles_) {
        if (FD_ISSET(it.first, &exception_fd_set_)) {
          HandleInterface* handle = it.second;
          cached_events_.push_back({EventType::kExceptionEvent, handle});
        }
      }
    }
  }

  if (!cached_events_.empty()) {
    SelectEvent event = cached_events_.back();
    cached_events_.pop_back();
    return event;
  } else {
    return SelectEvent{EventType::kNoEvent, nullptr};
  }
}

int IpSelectControl::SetupFdSet(HandleMap& handles, fd_set& set) {
  int maximum_fd = kInvalidFileDescriptor;

  FD_ZERO(&set);

  for (const auto& it : handles) {
    FD_SET(it.first, &set);
    maximum_fd = std::max(it.first, maximum_fd);
  }

  return maximum_fd;
}

}  // namespace select
}  // namespace io
}  // namespace osabstraction
