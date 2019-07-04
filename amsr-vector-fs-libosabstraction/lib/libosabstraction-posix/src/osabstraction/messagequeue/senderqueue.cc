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
 *        \brief  POSIX message queue implementation of sending semantic.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/senderqueue.h"

#include <algorithm>  // move
#include <stdexcept>  // runtime_error
#include <system_error>
#include <utility>  // swap

namespace osabstraction {
namespace messagequeue {

SenderQueue::SenderQueue() : queue_() {}

SenderQueue::SenderQueue(const MessageQueueSettings& settings) : queue_(settings) {}

SenderQueue::SenderQueue(SenderQueue&& other) : queue_(std::move(other.queue_)) {}

SenderQueue& SenderQueue::operator=(SenderQueue&& other) {
  std::swap(queue_, other.queue_);
  return *this;
}

void SenderQueue::Send(const void* data, std::size_t size) {
  /* #00 Send given data via message queue. */
  if (mq_send(queue_.GetHandle(), static_cast<const char*>(data), size, 0) != 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

bool SenderQueue::IsOpen() const { return queue_.IsOpen(); }

}  // namespace messagequeue
}  // namespace osabstraction
