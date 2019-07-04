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
/** \file   libosabstraction-gmock/include/osabstraction/messagequeue/senderqueue.h
 *  \brief  Mocked sender queue
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
#define LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include "osabstraction/messagequeue/senderqueueinterface.h"
#include "vac/language/cpp14_backport.h"
/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Sender queue mock
 */
class SenderQueueMock : public SenderQueueInterface {
 public:
  /**
   * \brief Mocked send
   * \return
   */
  MOCK_METHOD2(Send, void(const void* data, std::size_t size));
  /**
   * \brief Mocked IsOpen
   * \return
   */
  MOCK_CONST_METHOD0(IsOpen, bool());
};

/**
 * Sender Queue wrapper
 */
class SenderQueue : public SenderQueueInterface {
 public:
  SenderQueue() { mock_ = vac::language::make_unique<SenderQueueMock>(); }

  void Send(const void* data, std::size_t size) override { mock_->Send(data, size); }
  bool IsOpen() const override { return mock_->IsOpen(); }

  /**
   * Wrapped Sender queue mock
   */
  std::unique_ptr<SenderQueueMock> mock_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
