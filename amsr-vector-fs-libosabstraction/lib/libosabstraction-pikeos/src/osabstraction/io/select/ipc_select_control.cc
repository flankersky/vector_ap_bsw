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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/select/ipc_select_control.h"
#include <iostream>
#include "osabstraction/internal/sap_port.h"
#include "osabstraction/io/net/socket/ipc_socket.h"
#include "osabstraction/io/reactor_interface.h"
#include "osabstraction/io/select/select_event.h"

namespace osabstraction {
namespace io {
namespace select {

static EventType toReactorEventType(internal::EventType event_type) {
  EventType result;
  switch (event_type) {
    case internal::EventType::kNone:
      result = EventType::kNoEvent;
      break;
    case internal::EventType::kRead:
    case internal::EventType::kConnect:
      result = EventType::kReadEvent;
      break;
    case internal::EventType::kWrite:
    case internal::EventType::kClose:
      result = EventType::kWriteEvent;
      break;
    default:
      assert(false);
  }
  return result;
}

void IpcSelectControl::Add(Handle* handle) {
  std::uint32_t id = handle->GetSapPort().GetId();

  address_map_[id] = handle;
}

SelectEvent IpcSelectControl::Select(std::uint32_t event_mask) {
  if (address_map_.empty()) {
    return SelectEvent{EventType::kNoEvent, nullptr};
  }
  internal::SapPort& port = address_map_.begin()->second->GetSapPort();

  std::pair<internal::EventType, std::uint32_t> event = port.Wait(event_mask);

  // TODO(virbiv): Check if the reported id exists in the map.

  SelectEvent result{toReactorEventType(event.first), address_map_[event.second]};
  return result;
}

}  // namespace select
}  // namespace io
}  // namespace osabstraction
