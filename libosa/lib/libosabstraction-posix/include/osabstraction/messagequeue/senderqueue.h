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
 *        \brief  POSIX implementation message queue interface with sender semantic.
 *
 *      \details  The POSIX implementation uses the message queue API.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/messagequeue/messagequeue.h>
#include "osabstraction/messagequeue/senderqueueinterface.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Sender queue.
 */
class SenderQueue : public SenderQueueInterface {
 public:
  /**
   * \brief Creates an invalid message queue object.
   */
  SenderQueue();

  /**
   * \brief Opens a POSIX message queue.
   *
   * \param settings The settings for the message queue.
   * \throws std::system_error   open failed.
   */
  explicit SenderQueue(const MessageQueueSettings& settings);

  /**
   * \brief Move constructor.
   */
  SenderQueue(SenderQueue&& other);

  /**
   * \brief Move assignment operator.
   */
  SenderQueue& operator=(SenderQueue&& other);

  /**
   * \brief Sends a message.
   *
   * \param[in] data      Address of the data to send.
   * \param[in] size      Number of bytes to send.
   *
   * \throws std::system_error   Sending was not successful.
   */
  void Send(const void* data, std::size_t size);

  /**
   * \brief Returns whether the message queue is open and can be used for communication.
   *
   * \retval true   Queue object is open.
   * \retval false  Queue object is not open.
   */
  bool IsOpen() const;

 protected:
  SenderQueue(const SenderQueue& other) = delete;
  SenderQueue& operator=(const SenderQueue& other) = delete;

  /**
   * \brief Message queue object.
   */
  MessageQueue queue_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
