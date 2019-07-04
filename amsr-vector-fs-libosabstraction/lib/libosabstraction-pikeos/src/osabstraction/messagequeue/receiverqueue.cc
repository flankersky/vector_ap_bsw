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
 *        \brief  PikeOS message queue implementation with receiver semantic.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/receiverqueue.h"
#include <sys/qport.h>  // ioctl()
#include <time.h>       // struct timespec
#include <unistd.h>     // read()
#include <algorithm>    // move
#include <cerrno>       // errno
#include <stdexcept>    // runtime_error
#include <utility>      // swap

namespace osabstraction {
namespace messagequeue {

ReceiverQueue::ReceiverQueue() : qport_() {}

ReceiverQueue::ReceiverQueue(const QPortSettings& settings) : qport_(settings) {}

ReceiverQueue::ReceiverQueue(ReceiverQueue&& other) : qport_(std::move(other.qport_)) {}

ReceiverQueue& ReceiverQueue::operator=(ReceiverQueue&& other) {
  std::swap(qport_, other.qport_);
  return *this;
}

std::size_t ReceiverQueue::Receive(void* data, std::size_t size) {
  /* #00 Configure qport to have no timeout. */
  qport_.ClearTimeout();

  /* #00 Receive data via qport. */
  std::size_t msg_len = read(qport_.GetHandle(), static_cast<char*>(data), size);

  /* #00 If error value is returned, check whether we really had an error. */
  if (msg_len == static_cast<std::size_t>(-1)) {
    if (errno == EINTR) {
      msg_len = 0;
    } else {
      throw std::runtime_error("Receiving message from qport failed.");
    }
  }
  return msg_len;
}

ReceiveResult ReceiverQueue::Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) {
  ReceiveResult result = {0, false};

  /* #00 Configure qport to given timeout. */
  qport_.SetTimeout(timeout);

  /* #00 Receive data via qport. */
  std::size_t msg_len = read(qport_.GetHandle(), static_cast<char*>(data), size);

  /* #00 If error value is returned, check whether we really had an error. */
  if (msg_len == static_cast<std::size_t>(-1)) {
    switch (errno) {
      case EAGAIN:
        result.timeout = true;
        break;
      case EINTR:
        break;
      default:
        throw std::runtime_error("Receiving message from qport failed.");
    }
    msg_len = 0;
  }

  result.bytes_read = msg_len;

  return result;
}

bool ReceiverQueue::IsMessageAvailable() const {
  /* #00 Check whether messages are available by using ioctl(). */
  int nr_messages = 0;
  if (ioctl(qport_.GetHandle(), FIONREAD, &nr_messages) == -1) {
    std::runtime_error("Could not fetch number of messages in qport.");
  }
  return nr_messages != 0;
}

bool ReceiverQueue::IsOpen() const { return qport_.IsOpen(); }

}  // namespace messagequeue
}  // namespace osabstraction
