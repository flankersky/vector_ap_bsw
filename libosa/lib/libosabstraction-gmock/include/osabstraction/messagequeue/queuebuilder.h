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
/**     \file   libosabstraction-gmock/include/osabstraction/messagequeue/queuebuilder.h
 *      \brief  Mock message queue interface for building message queues.
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
#define LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "osabstraction/messagequeue/queuebuilderinterface.h"
#include "osabstraction/messagequeue/receiverqueue.h"
#include "osabstraction/messagequeue/senderqueue.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Message queue builder for POSIX message queues.
 */
class QueueBuilder : public QueueBuilderInterface {
 public:
  ReceiverQueue BuildReceiverQueue() { return ReceiverQueue(); }

  SenderQueue BuildSenderQueue() { return SenderQueue(); }
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QUEUEBUILDER_H_
