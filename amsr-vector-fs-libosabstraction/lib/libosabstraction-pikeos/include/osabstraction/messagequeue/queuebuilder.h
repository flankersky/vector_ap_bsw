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
 *        \brief  PikeOS message queue interface for building message queues.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/queuebuilderinterface.h"
#include "osabstraction/messagequeue/receiverqueue.h"
#include "osabstraction/messagequeue/senderqueue.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Message queue builder for PikeOS message queues.
 */
class QueueBuilder : public QueueBuilderInterface {
 public:
  /**
   * \brief Creates a message queue for sending.
   */
  SenderQueue BuildSenderQueue();

  /**
   * \brief Creates a message queue for receiving.
   */
  ReceiverQueue BuildReceiverQueue();
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
