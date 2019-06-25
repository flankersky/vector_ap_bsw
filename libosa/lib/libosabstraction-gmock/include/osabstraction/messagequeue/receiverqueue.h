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
/**     \file   libosabstraction-gmock/include/osabstraction/messagequeue/receiverqueue.h
 *      \brief  Mocked receiver queue.
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
#define LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vac/language/cpp14_backport.h>
#include <memory>
#include "osabstraction/messagequeue/receiverqueueinterface.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Receiver queue mock
 */
class ReceiverQueueMock : public ReceiverQueueInterface {
 public:
  /**
   * \brief Mocked IsOpen
   * @return
   */
  MOCK_CONST_METHOD0(IsOpen, bool());
  /**
   * \brief Mocked Receive
   * \return
   */
  MOCK_METHOD2(Receive, std::size_t(void* data, std::size_t size));
  /**
   * \brief Stubbed Receive
   * \param data
   * \param size
   * \param timeout
   * \return
   */
  ReceiveResult Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) {
    (void)data;
    (void)timeout;
    ReceiveResult ret_val;
    ret_val.timeout = false;
    ret_val.bytes_read = size;
    return ret_val;
  }
  /**
   * \brief Mocked IsMessageAvailable
   * \return
   */
  MOCK_CONST_METHOD0(IsMessageAvailable, bool());
};

/**
 * \brief Receiver queue wrapper
 */
class ReceiverQueue : public ReceiverQueueInterface {
 public:
  /**
   * \brief Creates an invalid message queue object.
   */
  ReceiverQueue() { mock_ = vac::language::make_unique<ReceiverQueueMock>(); }

  /**
   *
   * \param other
   */
  ReceiverQueue(ReceiverQueue&& other) = default;

  /**
   *
   * \param other
   * \return
   */
  ReceiverQueue& operator=(ReceiverQueue&& other) = default;

  bool IsOpen() const { return (mock_->IsOpen()); }

  std::size_t Receive(void* data, std::size_t size) { return mock_->Receive(data, size); }

  ReceiveResult Receive(void* data, std::size_t size, std::chrono::nanoseconds timeout) {
    (void)data;
    (void)timeout;
    ReceiveResult ret_val;
    ret_val.timeout = false;
    ret_val.bytes_read = mock_->Receive(data, size);
    return ret_val;
  }

  bool IsMessageAvailable() const { return mock_->IsMessageAvailable(); }

  /**
   * \brief Wrapped Receiver queue mock
   */
  std::unique_ptr<ReceiverQueueMock> mock_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_RECEIVERQUEUE_H_
