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
/**
 *      \file  polling_task.h
 *      \brief  PollingTask interface and PollingStatus definition.
 *
 *      \details  Contains the definition of the PollingTask interface and the enum PollingStatus.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_BASE_H_
#define LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/testing/test_adapter.h"

#include "common/polling_task.h"

namespace amsr {
namespace diag {
namespace common {

/**
 * \brief Abstract class with basic implementation of PollingTask interface.
 * Implements the PollingTask interface and defines templates that have to be implemented
 * by a concrete polling task implementation.
 */
class PollingTaskBase : public virtual PollingTask {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~PollingTaskBase() = default;

  /**
   * \brief Triggers first step in execution of the task.
   *
   * No action is performed if current status is different from kNotStarted.
   *
   * \returns The status of the task.
   */
  PollingStatus Start() override {
    if (status_ != PollingStatus::kNotStarted) {
      return status_;
    }
    status_ = this->StartAction();
    return status_;
  }

  /**
   * \brief Triggers the next step of performing the task.
   *
   * No action is performed if current status is different from kPending.
   *
   * \returns The status of the task.
   */
  PollingStatus Continue() override {
    // Process next step only if status is pending.
    if (status_ != PollingStatus::kPending) {
      return status_;
    }
    // Process next step
    status_ = this->ContinueAction();
    return status_;
  }

  /**
   * \brief Triggers cancellation of task.
   *
   * No action is performed if task is not yet started or task is already canceled.
   */
  void Cancel() override {
    if (status_ == PollingStatus::kNotStarted) {
      return;
    }
    if (status_ == PollingStatus::kCanceled) {
      return;
    }
    this->CancelAction();
    status_ = PollingStatus::kCanceled;
  }

  /**
   * \brief Returns the current status of the polling task.
   */
  PollingStatus GetStatus() const override { return status_; }

 protected:
  /**
   * \brief Action on first step in execution of the task.
   *
   * \returns The status of the task. Shall not be kNotStarted or kCanceled.
   */
  virtual PollingStatus StartAction() = 0;

  /**
   * \brief Action on next step of performing the task.
   *
   * \returns The status of the task. Shall not be kNotStarted or kCanceled.
   */
  virtual PollingStatus ContinueAction() = 0;

  /**
   * \brief Action on cancellation of task.
   */
  virtual void CancelAction() = 0;

 private:
  /**
   * \brief The current status of the task.
   */
  PollingStatus status_{PollingStatus::kNotStarted};

  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(PollingTaskTest, CheckStartOk);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(PollingTaskTest, CheckStartNotOk);
};

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_BASE_H_
