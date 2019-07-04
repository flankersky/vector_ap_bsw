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
/**        \file  polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_H_
#define LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace diag {
namespace common {

/**
 * \brief Enum for reporting the status of the task.
 */
enum PollingStatus { kNotStarted, kComplete, kFailed, kPending, kCanceled };

/**
 * Returns string representation of the PollingStatus
 * \param status polling status
 * \return string representation.
 */
inline const char* ToString(PollingStatus status) {
  switch (status) {
    case kNotStarted:
      return "<NotStarted>";
    case kComplete:
      return "<Complete>";
    case kFailed:
      return "<Failed>";
    case kPending:
      return "<Pending>";
    case kCanceled:
      return "<Canceled>";
    default:
      return "<unknown PollingTaskStatus>";
  }
}

/**
 * \brief Interface for tasks that are processed by polling mechanism.
 */
class PollingTask {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~PollingTask() = default;

  /**
   * \brief Triggers first step in execution of the task.
   *
   * No action is performed if current status is different from kNotStarted.
   *
   * \returns The status of the task.
   */
  virtual PollingStatus Start() = 0;

  /**
   * \brief Triggers the next step of performing the task.
   *
   * No action is performed if current status is different from kPending.
   *
   * \returns The status of the task.
   */
  virtual PollingStatus Continue() = 0;

  /**
   * \brief Triggers cancellation of task.
   *
   * No action is performed if task is not yet started or task is already canceled.
   */
  virtual void Cancel() = 0;

  /**
   * \brief Returns the current status of the polling task.
   */
  virtual PollingStatus GetStatus() const = 0;
};

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_POLLING_TASK_H_
