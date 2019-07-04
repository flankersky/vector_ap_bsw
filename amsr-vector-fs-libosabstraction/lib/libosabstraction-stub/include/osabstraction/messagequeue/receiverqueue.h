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
/**     \file
 *      \brief  Stub receiver queue.
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
#define LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/receiverqueueinterface.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Stub implementation.
 */
class ReceiverQueue : public ReceiverQueueInterface {
 public:
  /**
   * \brief Stub implementation.
   */
  ReceiverQueue() = default;

  /**
   * \brief Stub implementation.
   */
  bool IsOpen() const override { return false; }

  /**
   * \brief Stub implementation.
   */
  std::size_t Receive(void* data, std::size_t size) override {
    (void)data;
    return size;
  }

  /**
   * \brief Stub implementation.
   */
  ReceiveResult Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) override {
    (void)data;
    (void)timeout;
    ReceiveResult ret_val;
    ret_val.timeout = false;
    ret_val.bytes_read = size;
    return ret_val;
  }

  /**
   * \brief Stub implementation.
   */
  bool IsMessageAvailable() const override { return false; }
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
