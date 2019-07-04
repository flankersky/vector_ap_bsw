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
 *      \file  internal_data_polling_task.h
 *      \brief  Contains the implementation of InternalDataPollingTask
 *
 *      \details Contains a implementation of InternalDataPollingTask that takes a functor with the logic.
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_IPC_INTERNAL_DATA_POLLING_TASK_H_
#define SRC_SERVER_DATA_IPC_INTERNAL_DATA_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <algorithm>
#include <array>
#include <functional>

#include "ara/com/future.h"
#include "ara/com/promise.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

#include "common/diagnostic_service_polling_task_base.h"
#include "server/data/ipc/ipc_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace ipc {

/**
 * \brief Implementation of ReadDidPollingTask for operations that can be finished immediately.
 * The SimpleReadDidPollingTask provides a basic implementation of ReadDidPollingTask that requires the
 * implementation of
 */
template <std::size_t DidDataSize>
class InternalDataPollingTask : public virtual IpcPollingTask, public common::DiagnosticServicePollingTaskBase {
 public:
  /**
   * The DID data type.
   */
  using DidData = std::array<std::uint8_t, DidDataSize>;

  InternalDataPollingTask() = default;
  virtual ~InternalDataPollingTask() = default;

  // no copy or move
  InternalDataPollingTask(const InternalDataPollingTask& other) = delete;
  InternalDataPollingTask& operator=(const InternalDataPollingTask&) = delete;

  /**
   * \brief Move Constructor.
   */
  InternalDataPollingTask(InternalDataPollingTask&& other) = default;

  /**
   * \brief Move operator.
   */
  InternalDataPollingTask& operator=(InternalDataPollingTask&&) = default;

  /**
   * \brief Initializes the tasks with the buffer the DID data is written to while reading.
   * \param in_buffer No input data is expected given that this instance is only used to
   * implement DM internally processed DIDs.
   * \param out_buffer The view onto the buffer used to store the read data.
   * \remarks The provided buffer has to be of the size specified by DidDataSize.
   * \throws logic_error when the size of the provided array view does not match DidDataSize.
   * \param processing_context service processing context
   */
  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const service::ServiceProcessingContext& processing_context) override {
    if (in_buffer.size() > 0) {
      throw std::logic_error("SimpleReadDidPollingTask::Init called with request data which is not supported!");
    }
    // first check if the DID data buffer is sufficient
    if (out_buffer.size() != DidDataSize) {
      throw std::logic_error("SimpleReadDidPollingTask::Init called with not supported response buffer size!");
    }

    did_data_out_buffer_ = out_buffer;
    processing_context_ = &processing_context;
  }

 protected:
  /**
   * \brief Alias for common::PollingStatus
   */
  using PollingStatus = amsr::diag::common::PollingStatus;

  /**
   * \brief Action on first step in execution of the task.
   * The implementation checks if a valid processing context was provided and calls
   * the template method RetrieveDidData to fetch the data of the DID which is checked
   * and copied to the output buffer if the size is as expected.
   * \returns The status of the task: kComplete when the result was fetched; otherwise kFailed.
   */
  PollingStatus StartAction() final {
    DidData did_data = RetrieveDidData(*processing_context_, cancellation_requested_);

    if (did_data.size() != DidDataSize) {
      ara::log::LogError() << "InternalReadDidPollingTaskImpl::" << __func__ << ": RetrieveDidData returned '"
                           << std::to_string(DidDataSize) << "' bytes, but '" << std::to_string(DidDataSize)
                           << "' bytes are expected!";
      return PollingStatus::kFailed;
    }

    assert(did_data_out_buffer_.size() == DidDataSize);  //< already checked by Init
    assert(did_data_out_buffer_.size() == did_data.size());

    std::copy_n(did_data.begin(), did_data_out_buffer_.size(), did_data_out_buffer_.begin());
    return PollingStatus::kComplete;
  }

  /**
   * Always returns kFailed, because this method should never be called for this type.
   * \returns kFailed - method shouldn't be called, because StartAction returns kFailed or kComplete.
   */
  PollingStatus ContinueAction() final {
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // a SimpleReadDidPollingTask has to be finished when StartAction returns,
    // so a call of this method is an error
    nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
    return PollingStatus::kFailed;
  }

  /**
   * Sets the cancellation_requested_ flag that is provided to the template method RetrieveDidData
   */
  void CancelAction() final { cancellation_requested_ = true; }

 protected:
  /**
   * Template method to be implemented by derived classes.
   * \param processing_context The processing context
   * \param cancellation_requested The flag signals cancellation (should be checked at start of the implementation)
   * \returns The DID data
   */
  virtual DidData RetrieveDidData(const service::ServiceProcessingContext& processing_context,
                                  bool& cancellation_requested) = 0;

 private:
  /**
   * \brief The view onto the buffer that stores the DID data.
   */
  BufferTypeOut did_data_out_buffer_;

  /**
   * Flag to signal cancellation.
   */
  bool cancellation_requested_{false};

  /**
  * The currently active processing context.
  */
  const service::ServiceProcessingContext* processing_context_{nullptr};
};

}  // namespace ipc
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_IPC_INTERNAL_DATA_POLLING_TASK_H_
