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
 *        \brief  Abstract interface of a message queue with sender semantic.
 *
 *      \details  This file defines an OS independent message queues interface for sending.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUEINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUEINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>  // size_t

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Message queue with sender semantic.
 */
class SenderQueueInterface {
 public:
  /**
   * \brief Opens a message queue for sending.
   */
  SenderQueueInterface() = default;

  /**
   * \brief Closes a message queue.
   */
  virtual ~SenderQueueInterface() = default;

  /**
   * \brief Returns whether the message queue is open and can be used for communication.
   *
   * \retval true   Queue object is open.
   * \retval false  Queue object is not open.
   */
  virtual bool IsOpen() const = 0;

  /**
   * \brief Sends the given data.
   *
   * If the queue is full this function blocks.
   *
   * \param[in] data      Address of the data to send.
   * \param[in] size      Number of bytes to send.
   */
  virtual void Send(const void* data, std::size_t size) = 0;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUEINTERFACE_H_
