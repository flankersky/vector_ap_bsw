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
 *        \brief  PikeOS message queue implementation with sender semantic.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/senderqueue.h"
#include <unistd.h>   // write()
#include <algorithm>  // move
#include <stdexcept>  // runtime_error
#include <utility>    // swap

namespace osabstraction {
namespace messagequeue {

SenderQueue::SenderQueue() : qport_() {}

SenderQueue::SenderQueue(const QPortSettings& settings) : qport_(settings) {}

SenderQueue::SenderQueue(SenderQueue&& other) : qport_(std::move(other.qport_)) {}

SenderQueue& SenderQueue::operator=(SenderQueue&& other) {
  std::swap(qport_, other.qport_);
  return *this;
}

void SenderQueue::Send(const void* data, std::size_t size) {
  /* #00 Send given data via qport. */
  if (write(qport_.GetHandle(), static_cast<const char*>(data), size) != static_cast<int>(size)) {
    throw std::runtime_error("Sending message via qport failed.");
  }
}

bool SenderQueue::IsOpen() const { return qport_.IsOpen(); }

}  // namespace messagequeue
}  // namespace osabstraction
