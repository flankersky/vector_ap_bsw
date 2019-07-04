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
 *        \brief  POSIX message queue implementation of receiving semantic.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/receiverqueue.h"

#include <time.h>     // clock_gettime(), struct timespec
#include <cerrno>     // errno
#include <stdexcept>  // runtime_error
#include <system_error>

namespace osabstraction {
namespace messagequeue {

ReceiverQueue::ReceiverQueue() : queue_() {}

ReceiverQueue::ReceiverQueue(const MessageQueueSettings& settings) : queue_(settings) {}

ReceiverQueue::ReceiverQueue(ReceiverQueue&& other) : queue_(std::move(other.queue_)) {}

ReceiverQueue& ReceiverQueue::operator=(ReceiverQueue&& other) {
  std::swap(queue_, other.queue_);
  return *this;
}

std::size_t ReceiverQueue::Receive(void* data, std::size_t size) {
  /* #00 Receive data via message queue. */
  std::size_t msg_len = mq_receive(queue_.GetHandle(), static_cast<char*>(data), size, nullptr);
  if (msg_len == static_cast<std::size_t>(-1)) {
    throw std::system_error(errno, std::generic_category());
  }
  return msg_len;
}

ReceiveResult ReceiverQueue::Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) {
  ReceiveResult result = {0, false};
  struct timespec tm;
  constexpr std::chrono::seconds sec(1);

  clock_gettime(CLOCK_REALTIME, &tm);
  tm.tv_sec += (tm.tv_nsec + std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count()) /
               std::chrono::nanoseconds(sec).count();
  tm.tv_nsec = (tm.tv_nsec + timeout.count()) % std::chrono::nanoseconds(sec).count();

  /* #00 Receive data via message queue with given timeout. */
  std::size_t msg_len = mq_timedreceive(queue_.GetHandle(), static_cast<char*>(data), size, nullptr, &tm);

  if (msg_len == static_cast<std::size_t>(-1)) {
    if (errno != ETIMEDOUT) {
      throw std::system_error(errno, std::generic_category());
    }
    result.timeout = true;
  } else {
    result.bytes_read = msg_len;
  }

  return result;
}

bool ReceiverQueue::IsMessageAvailable() const {
  /* #00 Fetch message queue attributes. */
  struct mq_attr mq_attributes;
  if (mq_getattr(queue_.GetHandle(), &mq_attributes) != 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return mq_attributes.mq_curmsgs != 0;
}

bool ReceiverQueue::IsOpen() const { return queue_.IsOpen(); }

}  // namespace messagequeue
}  // namespace osabstraction
