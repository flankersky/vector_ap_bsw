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
 *        \brief  POSIX implementation of message queue interface with receiver semantic.
 *
 *      \details  The POSIX implementation uses the message queue API.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/messagequeue.h"
#include "osabstraction/messagequeue/receiverqueueinterface.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Receiver queue.
 */
class ReceiverQueue : public ReceiverQueueInterface {
 public:
  /**
   * \brief Creates an invalid message queue object.
   */
  ReceiverQueue();

  /**
   * \brief Opens a Posix message queue.
   * \param settings The settings for the message queue.
   * \throws std::system_error   open failed.
   */
  explicit ReceiverQueue(const MessageQueueSettings& settings);

  /**
   * \brief Move constructor.
   */
  ReceiverQueue(ReceiverQueue&& other);

  /**
   * \brief Move assignment operator.
   */
  ReceiverQueue& operator=(ReceiverQueue&& other);

  /**
   * \brief Receives a message.
   *
   * \param[out]  data  Address of the buffer where the received data shall be stored.
   * \param[in]   size  Number of bytes to receive.
   *
   * \return    Returns the number of bytes which have been received.
   *
   * \throws std::system_error   Receiving failed.
   */
  std::size_t Receive(void* data, std::size_t size);

  /**
   * \brief Receive with timeout.
   *
   * \param[out]  data    Address of the buffer where the received data shall be stored.
   * \param[in]   size    Number of bytes to receive.
   * \param[in]   timeout Number of nanoseconds to wait for a message.
   *
   * \return    Returns a specific result type containing whether a timeout occurred and how many bytes have been read.
   *
   * \throws std::runtime_error   Receiving failed.
   */
  ReceiveResult Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout);

  /**
   * \brief Returns whether there are messages available to be received.
   *
   * \retval  true  There are messages available.
   * \retval  false There are no messages available.
   *
   * \throws std::runtime_error   Fetching message queue attributes failed.
   */
  bool IsMessageAvailable() const;

  /**
   * \brief Returns whether the message queue is open and can be used for communication.
   *
   * \retval true   Queue object is open.
   * \retval false  Queue object is not open.
   */
  bool IsOpen() const;

 protected:
  ReceiverQueue(const ReceiverQueue& other) = delete;
  ReceiverQueue& operator=(const ReceiverQueue& other) = delete;

  /**
   * \brief Message queue object.
   */
  MessageQueue queue_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
