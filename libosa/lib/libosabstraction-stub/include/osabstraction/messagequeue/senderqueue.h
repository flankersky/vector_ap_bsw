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
 *      \brief  Stub sender queue.
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
#define LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/senderqueueinterface.h"

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Stub implementation.
 */
class SenderQueue : public SenderQueueInterface {
 public:
  /**
   * \brief Stub implementation.
   */
  SenderQueue() = default;

  /**
   * \brief Stub implementation.
   */
  void Send(const void* data, std::size_t size) override {
    (void)data;
    (void)size;
  }

  /**
   * \brief Stub implementation.
   */
  bool IsOpen() const override { return false; }
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_SENDERQUEUE_H_
