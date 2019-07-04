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
 *        \brief  Implementation of the PikeOS message queue builder
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/queuebuilder.h"
#include <fcntl.h>  // O_*
#include "osabstraction/messagequeue/senderqueue.h"

namespace osabstraction {
namespace messagequeue {

SenderQueue QueueBuilder::BuildSenderQueue() {
  const QPortSettings settings = {id_, create_, O_WRONLY, length_, msgsize_};

  return SenderQueue(settings);
}

ReceiverQueue QueueBuilder::BuildReceiverQueue() {
  const QPortSettings settings = {id_, create_, O_RDONLY, length_, msgsize_};

  return ReceiverQueue(settings);
}

}  // namespace messagequeue
}  // namespace osabstraction
