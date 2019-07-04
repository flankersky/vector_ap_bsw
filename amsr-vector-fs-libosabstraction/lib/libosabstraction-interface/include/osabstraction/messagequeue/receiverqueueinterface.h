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
 *        \brief  Abstract interface of a message queue with receiver semantic.
 *
 *      \details  This file defines an OS independent message queues interface for receiving.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUEINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUEINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>   // nanoseconds
#include <cstddef>  // size_t

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Result type for receiving data with timeout.
 */
struct ReceiveResult {
  /**
   * \brief Number of bytes which have been received.
   */
  std::size_t bytes_read;

  /**
   * \brief Tells whether a timeout happened.
   *
   * True if timeout happened, false if not.
   */
  bool timeout;
};

/**
 * \brief Message queue with receiver semantic.
 */
class ReceiverQueueInterface {
 public:
  /**
   * \brief Opens a message queue for receiving.
   */
  ReceiverQueueInterface() = default;

  /**
   * \brief Closes a message queue.
   */
  virtual ~ReceiverQueueInterface() = default;

  /**
   * \brief Returns whether the message queue is open and can be used for communication.
   *
   * \retval true   Queue object is open.
   * \retval false  Queue object is not open.
   */
  virtual bool IsOpen() const = 0;

  /**
   * \brief Receives data and stores it at the given pointer.
   *
   * If the queue is empty this function blocks.
   *
   * \param[out]  data  Address of the buffer where the received data shall be stored.
   * \param[in]   size  Number of bytes to receive.
   *
   * \return    Returns the number of bytes which have been received.
   */
  virtual std::size_t Receive(void* data, std::size_t size) = 0;

  /**
   * \brief Receive with timeout.
   *
   * If the queue is empty this function blocks until the time specified by timeout is passed.
   *
   * \param[out]  data    Address of the buffer where the received data shall be stored.
   * \param[in]   size    Number of bytes to receive.
   * \param[in]   timeout Number of nanoseconds to wait for a message.
   *
   * \return    Returns a specific result type containing whether a timeout occurred and how many bytes have been read.
   */
  virtual ReceiveResult Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) = 0;

  /**
   * \brief Returns whether there are messages available to be received.
   *
   * \retval  true  There are messages available.
   * \retval  false There are no messages available.
   */
  virtual bool IsMessageAvailable() const = 0;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUEINTERFACE_H_
