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
 *        \brief  POSIX implementation of message queue interface for building message queues.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/queuebuilderinterface.h"
#include "osabstraction/messagequeue/receiverqueue.h"
#include "osabstraction/messagequeue/senderqueue.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Message queue builder for mocked message queues.
 */
class QueueBuilder : public QueueBuilderInterface {
 public:
  /**
   * \brief Creates a message queue for sending.
   */
  SenderQueue BuildSenderQueue() {
    struct mq_attr attributes;
    MessageQueueSettings settings = {id_, O_WRONLY | (create_ ? O_CREAT : 0), S_IRWXU | S_IRWXG | S_IRWXO, nullptr};

    if (length_ || msgsize_) {
      attributes.mq_maxmsg = length_;
      attributes.mq_msgsize = msgsize_;
      settings.attributes = &attributes;
    }
    return SenderQueue(settings);
  }

  /**
   * \brief Creates a message queue for receiving.
   */
  ReceiverQueue BuildReceiverQueue() {
    struct mq_attr attributes;
    MessageQueueSettings settings = {id_, O_RDONLY | (create_ ? O_CREAT : 0), S_IRWXU | S_IRWXG | S_IRWXO, nullptr};

    if (length_ || msgsize_) {
      attributes.mq_maxmsg = length_;
      attributes.mq_msgsize = msgsize_;
      settings.attributes = &attributes;
    }

    return ReceiverQueue(settings);
  }
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
