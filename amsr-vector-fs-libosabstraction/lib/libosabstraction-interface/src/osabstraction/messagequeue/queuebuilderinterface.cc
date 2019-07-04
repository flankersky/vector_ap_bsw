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
 *        \brief  Implementation of OS independent parts of the message queue builder interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/queuebuilderinterface.h"

namespace osabstraction {
namespace messagequeue {

QueueBuilderInterface::QueueBuilderInterface() : id_(nullptr), msgsize_(0), length_(0), create_(false) {}

QueueBuilderInterface::~QueueBuilderInterface() {}

QueueBuilderInterface& QueueBuilderInterface::SetId(const char* id) {
  id_ = id;
  return *this;
}

QueueBuilderInterface& QueueBuilderInterface::SetMessageSize(std::size_t size) {
  msgsize_ = size;
  return *this;
}
QueueBuilderInterface& QueueBuilderInterface::SetQueueLength(int length) {
  length_ = length;
  return *this;
}

QueueBuilderInterface& QueueBuilderInterface::SetCreate() {
  create_ = true;
  return *this;
}

}  // namespace messagequeue
}  // namespace osabstraction
