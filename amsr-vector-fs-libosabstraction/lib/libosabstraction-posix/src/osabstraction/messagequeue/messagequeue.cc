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
 *        \brief  POSIX message queue implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/messagequeue.h"

#include <fcntl.h>    // O_WRONLY, O_*
#include <algorithm>  // move
#include <stdexcept>
#include <system_error>

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Invalid message queue descriptor.
 */
constexpr const int kInvalidDescriptor = -1;

MessageQueue::MessageQueue() : msgqueue_descriptor_(kInvalidDescriptor), created_(false), id_(nullptr) {}

MessageQueue::MessageQueue(const MessageQueueSettings& settings)
    : msgqueue_descriptor_(kInvalidDescriptor), created_(false), id_(nullptr) {
  /* #00 If a queue with the given Id already exists delete it. */
  if (settings.flags & O_CREAT) {
    mq_unlink(settings.id);
    /* #00 Remember that the queue shall be created. */
    created_ = true;
    id_ = settings.id;
  }

  /* #00 Open queue with prepared settings. */
  msgqueue_descriptor_ = mq_open(settings.id, settings.flags, settings.mode, settings.attributes);

  /* #00 If open failed, throw exception. */
  if (msgqueue_descriptor_ == kInvalidDescriptor) {
    throw std::system_error(errno, std::generic_category());
  }
}

MessageQueue::~MessageQueue() {
  /* #00 If message queue is open, close it. */
  if (msgqueue_descriptor_ != kInvalidDescriptor) {
    mq_close(msgqueue_descriptor_);
    /* #00 If the queue has been created, delete it again. */
    if (created_) {
      mq_unlink(id_);
    }
  }
}

MessageQueue::MessageQueue(MessageQueue&& other)
    : msgqueue_descriptor_(std::move(other.msgqueue_descriptor_)),
      created_(std::move(other.created_)),
      id_(std::move(other.id_)) {
  /* #00 Move message queue descriptor. Set other descriptor to invalid. */
  other.msgqueue_descriptor_ = kInvalidDescriptor;
}

MessageQueue& MessageQueue::operator=(MessageQueue&& other) {
  /* #00 Swap members. */
  std::swap(msgqueue_descriptor_, other.msgqueue_descriptor_);
  std::swap(created_, other.created_);
  std::swap(id_, other.id_);
  return *this;
}

bool MessageQueue::IsOpen() const {
  /* #00 Return whether the msgqueue_descriptor_ is valid. */
  return msgqueue_descriptor_ != kInvalidDescriptor;
}

mqd_t MessageQueue::GetHandle() const { return msgqueue_descriptor_; }

}  // namespace messagequeue
}  // namespace osabstraction
