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
/**        \file  write_did_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_POLLING_TASK_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "common/mediator_handles.h"
#include "common/polling_task.h"
#include "did_data_element_writer.h"
#include "server/data/did_info.h"
#include "server/service/processor/writedataservice/write_did_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief Abstract class for operation handlers.
 */
class WriteDidOperationHandlerPollingTask : public service::processor::WriteDidPollingTask {
 public:
  /**
   * \brief Handler for internal service 0xF186.
   */
  static constexpr amsr::diag::common::handles::Handle kInternalWriteDid0xF186 = 0x2EF186ul;

  /**
   * \brief Constructor.
   */
  WriteDidOperationHandlerPollingTask(const server::data::DidInfo::DataElementInfoList &data_element_info_list,
                                      DidInfo::Did did)
      : service::processor::WriteDidPollingTask(), data_element_info_list_(data_element_info_list), did_(did) {}

  // Remove copy constructor/assignment.
  WriteDidOperationHandlerPollingTask(const WriteDidOperationHandlerPollingTask &) = delete;
  WriteDidOperationHandlerPollingTask &operator=(const WriteDidOperationHandlerPollingTask &) = delete;

  /**
   * \brief Move constructor.
   */
  WriteDidOperationHandlerPollingTask(WriteDidOperationHandlerPollingTask &&) = default;

  /**
   * \brief Move operator.
   */
  WriteDidOperationHandlerPollingTask &operator=(WriteDidOperationHandlerPollingTask &&) = default;

  /**
   * \brief Simplified initialization method for writing.
   * Initializes the tasks with the buffer the DID data is written to while writeing.
   * \param in_buffer The view onto the buffer used to store the write data.
   * \param processing_context service processing context
   */
  void Init(BufferTypeIn in_buffer, const service::ServiceProcessingContext &processing_context) override {
    // Get the handle to be passed to the Mediator
    amsr::diag::common::handles::Handle handle = GetHandle();

    // Instantiate delegate;
    delegate_.emplace(processing_context, handle);

    // Select the polling task to use.
    assert(data_element_info_list_.size() >= 1);
    delegate_->Select(data_element_info_list_.front().GetHandlerType());

    // Call Init method of delegate
    server::service::processor::ReadDidPollingTask::BufferTypeOut empty_out_buffer;
    this->Init(in_buffer, empty_out_buffer, processing_context);
  }

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
  * \brief Returns the NRC in case an error occurred while writeing.
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
   * No action is performed if task is not yet started or task is alwritey canceled.
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
        return amsr::diag::common::handles::kWriteDid0x4711;
      case 0xF190:
        return amsr::diag::common::handles::kWriteDid0xF190;
      case 0xF186:
        return kInternalWriteDid0xF186;
      default:
        throw std::logic_error(
            "WriteDidOperationHandlerPollingTask::GetMediatorHandle() not supported DID"
            " - no Mediator handle available!");
    }
  }

 private:
  /**
   * \brief pointer to the delegated write polling task.
   */
  vac::memory::optional<DidDataElementWriter> delegate_;

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

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_POLLING_TASK_H_
