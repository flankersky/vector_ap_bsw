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
/**        \file  read_did_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_POLLING_TASK_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "common/mediator_handles.h"
#include "common/multi_byte_type.h"
#include "common/polling_task.h"
#include "did_data_element_reader.h"
#include "server/data/did_info.h"
#include "server/service/processor/readdataservice/read_did_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief Read did operation handler.
 */
class ReadDidOperationHandlerPollingTask : public service::processor::ReadDidPollingTask {
 public:
  /**
   * \brief Handler for internal service 0xF186.
   */
  static constexpr amsr::diag::common::handles::Handle kInternalReadDid0xF186 = 0x22F186ul;

  /**
   * \brief Constructor.
   */
  ReadDidOperationHandlerPollingTask(const server::data::DidInfo::DataElementInfoList &data_element_info_list,
                                     DidInfo::Did did)
      : service::processor::ReadDidPollingTask(), data_element_info_list_(data_element_info_list), did_(did) {}

  // Remove copy constructor/assignment.
  ReadDidOperationHandlerPollingTask(const ReadDidOperationHandlerPollingTask &) = delete;
  ReadDidOperationHandlerPollingTask &operator=(const ReadDidOperationHandlerPollingTask &) = delete;

  /**
   * \brief Move constructor.
   */
  ReadDidOperationHandlerPollingTask(ReadDidOperationHandlerPollingTask &&) = default;

  /**
   * \brief Move operator.
   */
  ReadDidOperationHandlerPollingTask &operator=(ReadDidOperationHandlerPollingTask &&) = default;

  /**
   * \brief Simplified initialization method for reading.
   *
   * Initializes the tasks with the buffer the DID data is written to while reading.
   * \param out_buffer The view onto the buffer used to store the read data.
   * \param processing_context service processing context
   */
  void Init(BufferTypeOut out_buffer, const service::ServiceProcessingContext &processing_context) override {
    // Get the handle to be passed to the Mediator
    amsr::diag::common::handles::Handle handle = GetHandle();

    // Instantiate delegate;
    delegate_.emplace(processing_context, handle);

    // Select the polling task to use.
    assert(data_element_info_list_.size() >= 1);
    delegate_->Select(data_element_info_list_.front().GetHandlerType());

    // Call Init method of delegate
    server::service::processor::ReadDidPollingTask::BufferTypeIn empty_in_buffer;
    this->Init(empty_in_buffer, out_buffer, processing_context);
  };

  /**
   * \brief Initializes the tasks with the input and output buffer needed for processing.
   * \param in_buffer The view onto the buffer with input data (e. g. for write DID or routine control).
   * \remarks in_buffer is empty for read DID operations
   * \param out_buffer The view onto the buffer used to store the received data (e. g. for read DID or routine control).
   * \remarks out_buffer will be empty for write DID operations
   * \param processing_context service processing context
   */
  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext &processing_context) override {
    assert(delegate_.has_value());
    delegate_->Init(in_buffer, out_buffer, processing_context);
  }

  /**
  * \brief Returns the NRC in case an error occurred while reading.
  * \return The NRC
  * \throws logic_error if method is called without previously reported failure.
  */
  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    // Call GetNRC of delegate.
    assert(delegate_.has_value());
    return delegate_->GetNRC();
  };

  /**
   * \brief Triggers first step in execution of the task.
   *
   * No action is performed if current status is different from kNotStarted.
   *
   * \returns The status of the task.
   */
  amsr::diag::common::PollingStatus Start() override {
    assert(delegate_.has_value());
    return delegate_->Start();
  }

  /**
   * \brief Triggers the next step of performing the task.
   *
   * No action is performed if current status is different from kPending.
   *
   * \returns The status of the task.
   */
  amsr::diag::common::PollingStatus Continue() override {
    assert(delegate_.has_value());
    return delegate_->Continue();
  }

  /**
   * \brief Triggers cancellation of task.
   *
   * No action is performed if task is not yet started or task is already canceled.
   */
  void Cancel() override {
    assert(delegate_.has_value());
    delegate_->Cancel();
  }

  /**
   * \brief Returns the current status of the polling task.
   */
  amsr::diag::common::PollingStatus GetStatus() const override {
    assert(delegate_.has_value());
    return delegate_->GetStatus();
  }

 private:
  amsr::diag::common::handles::Handle GetHandle() const {
    // TODO(PAASR-2714): this is only a temporary solution and has to be changed when the the handles are generated
    // e. g. the handles will be provided by the configuration and will be available with the DataElement, ...
    switch (did_) {
      case 0x4711:
        return amsr::diag::common::handles::kReadDid0x4711;
      case 0xF190:
        return amsr::diag::common::handles::kReadDid0xF190;
      case 0xF186:
        return kInternalReadDid0xF186;
      default:
        throw std::logic_error(
            "ReadDidOperationHandlerPollingTask::GetMediatorHandle() not supported DID"
            " - no Mediator handle available!");
    }
  }

 private:
  /**
   * \brief pointer to the delegated read polling task.
   */
  vac::memory::optional<DidDataElementReader> delegate_;

  /**
   * \brief The list of Did data elements information.
   */
  const server::data::DidInfo::DataElementInfoList &data_element_info_list_;

  /**
   * \brief Did the polling task has been created for.
   */
  const DidInfo::Did did_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_POLLING_TASK_H_
