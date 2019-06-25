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
 *        \brief  POSIX implementation of the message queue interface.
 *
 *      \details  This file contains message queue descriptor handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_MESSAGEQUEUE_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_MESSAGEQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mqueue.h>  // mqd_t

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Settings to create a message queue.
 */
struct MessageQueueSettings {
  /**
   * \brief Path to the message queue.
   */
  const char* id;

  /**
   * \brief Flags to be used to open the queue.
   */
  int flags;

  /**
   * \brief Access mode of the queue.
   *
   * Only relevant if the queue is created.
   */
  mode_t mode;

  /**
   * \brief Attributes of the queue.
   *
   * Only relevant if the queue is created.
   */
  struct mq_attr* attributes;
};

/**
 * \brief Manages message queue descriptor handling.
 */
class MessageQueue {
 public:
  /**
   * \brief Creates an invalid message queue object.
   */
  MessageQueue();

  /**
   * \brief Opens a Posix message queue.
   *
   * \param settings The settings for the message queue.
   * \throws std::system_error   open failed.
   */
  explicit MessageQueue(const MessageQueueSettings& settings);

  /**
   * \brief Closes the message queue.
   */
  virtual ~MessageQueue();

  /**
   * \brief Move constructor.
   */
  MessageQueue(MessageQueue&& other);

  /**
   * \brief Move assignment operator.
   */
  MessageQueue& operator=(MessageQueue&& other);

  /**
   * \brief Returns whether the message queue is open and can be used for communication.
   *
   * \retval true   Queue object is open.
   * \retval false  Queue object is not open.
   */
  bool IsOpen() const;

  /**
   * \brief Returns the message queue handle.
   *
   * \return  Message queue handle.
   */
  mqd_t GetHandle() const;

 private:
  MessageQueue(const MessageQueue& other) = delete;
  MessageQueue& operator=(const MessageQueue& other) = delete;

  /**
   * \brief Message queue descriptor used for mq_*() APIs.
   */
  mqd_t msgqueue_descriptor_;

  /**
   * \brief Tells whether the queue has been created (true) or just opened (false).
   */
  bool created_;

  /**
   * \brief Path to the message queue.
   */
  const char* id_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_MESSAGEQUEUE_H_
