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
 *        \brief  Abstract interface for building message queues.
 *
 *      \details  This file defines an OS independent interface for building message queues.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDERINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDERINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>  // size_t

namespace osabstraction {
namespace messagequeue {

class SenderQueue;
class ReceiverQueue;

/**
 * \brief Abstract interface of a message queue builder.
 *
 * The queue builder creates message queues.
 *
 * Usage:
 *
 *     QueueBuilder builder;
 *     SenderQueue queue = builder.SetId("/mq_name")
 *                            .SetMessageSize(1024)
 *                            .SetQueueLength(23)
 *                            .SetCreate()
 *                            .BuildSenderQueue();
 *
 */
class QueueBuilderInterface {
 public:
  /**
   * \brief Constructor.
   */
  QueueBuilderInterface();

  /**
   * \brief Copy constructor.
   */
  QueueBuilderInterface(const QueueBuilderInterface& other) = default;

  /**
   * \brief Move constructor.
   */
  QueueBuilderInterface(QueueBuilderInterface&& other) = default;

  /**
   * \brief Copy assignment operator.
   */
  QueueBuilderInterface& operator=(const QueueBuilderInterface& other) = default;

  /**
   * \brief Move assignment operator.
   */
  QueueBuilderInterface& operator=(QueueBuilderInterface&& other) = default;

  /**
   * \brief Destructor.
   */
  virtual ~QueueBuilderInterface();

  /**
   * \brief Sets the ID for the message queue to be build.
   *
   * \param[in] id  ID of the message queue.
   *
   * \return Reference to queue builder.
   */
  QueueBuilderInterface& SetId(const char* id);

  /**
   * \brief Sets the maximum message size for the message queue to be build.
   *
   * \param[in] size  Maximum size of a message.
   *
   * \return Reference to queue builder.
   */
  QueueBuilderInterface& SetMessageSize(std::size_t size);

  /**
   * \brief Sets the queue length for the message queue to be build.
   *
   * \param[in] length  Length of the message queue.
   *
   * \return Reference to queue builder.
   */
  QueueBuilderInterface& SetQueueLength(int length);

  /**
   * \brief Sets that the queue shall be created.
   *
   * If this function is not called, the message queue with the configured id is expected to be existing.
   *
   * \return Reference to queue builder.
   */
  QueueBuilderInterface& SetCreate();

  /**
   * \brief Creates the configured queue for sending.
   */
  virtual SenderQueue BuildSenderQueue() = 0;

  /**
   * \brief Creates the configured queue for receiving.
   */
  virtual ReceiverQueue BuildReceiverQueue() = 0;

 protected:
  /**
   * \brief Message queue ID.
   */
  const char* id_;

  /**
   * \brief Maximum message size.
   */
  std::size_t msgsize_;

  /**
   * \brief Queue length.
   */
  int length_;

  /**
   * \brief Flag which tell whether the queue shell be freshly created or just opened.
   */
  bool create_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDERINTERFACE_H_
