/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  routine_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

#include "data/data_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

/**
 * \brief Base class of all routine specific polling tasks.
 */
class RoutinePollingTask : public DataPollingTask {
 public:
  /**
   * \brief Type definition for pointer to RoutinePollingTask
   */
  using Ptr = std::unique_ptr<RoutinePollingTask>;

  virtual ~RoutinePollingTask() = default;

  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    if (!nrc_.has_value()) {
      throw std::logic_error("RoutinePollingTask::GetNRC() called without reported failure.");
    }
    return nrc_.value();
  }

  void Init(InBufferType in_buffer, OutBufferType out_buffer) override {
    ara::log::LogDebug() << "RoutinePollingTask::" << __func__ << ": InBuffer size='" << in_buffer.size()
                         << "', OutBuffer size='" << out_buffer.size() << "'";
    out_buffer_ = out_buffer;
    in_buffer_ = in_buffer;
  }

 protected:
  /**
   * \brief The view onto the buffer that contains parameter of start method.
   */
  InBufferType in_buffer_;

  /**
   * \brief The view onto the buffer that contains output values of start method.
   */
  OutBufferType out_buffer_;

  /**
   * \brief Optional for possible Negative Response Code.
   */
  vac::memory::optional<ara::diag::udstransport::UdsNegativeResponseCode> nrc_;
};

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_H_
