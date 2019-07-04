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
/**        \file  did_data_element_writer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_WRITER_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/log/logging.hpp"
#include "common/mediator_handles.h"
#include "common/polling_task.h"
#include "server/data/did_info.h"
#include "server/data/ipc/write_did_polling_task_adapter.h"
#include "server/service/handler/service_handler.h"
#include "server/service/processor/writedataservice/write_did_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief DidDataElementWriter.
 */
class DidDataElementWriter : public service::processor::WriteDidPollingTask {
 public:
  /**
   * \brief Constructor.
   */
  explicit DidDataElementWriter(const service::ServiceProcessingContext &processing_context, std::uint32_t handle)
      : mediator_polling_task_(handle, processing_context.GetProxysPool().ipc_proxy_) {}

  // Remove copy/move constructor/assignment.
  DidDataElementWriter(const DidDataElementWriter &) = delete;
  DidDataElementWriter &operator=(const DidDataElementWriter &) = delete;

  /**
   * \brief Move constructor.
   */
  DidDataElementWriter(DidDataElementWriter &&) = default;

  /**
   * \brief Move operator.
   */
  DidDataElementWriter &operator=(DidDataElementWriter &&) = default;

  /**
   * \brief Select the delegate to use.
   * \param handler_type Type of the handler.
   */
  void Select(server::service::handler::ServiceHandler::HandlerType handler_type);

  /**
   * \brief Simplified initialization method for writeing.
   * Initializes the tasks with the buffer the DID data is written to while writeing.
   * \param in_buffer The view onto the buffer used to store the write data.
   * \param processing_context service processing context
   */
  void Init(BufferTypeIn in_buffer, const service::ServiceProcessingContext &processing_context) override {
    // Call Init method of delegate
    assert(delegate_);
    BufferTypeOut empty_out_buffer;
    this->Init(in_buffer, empty_out_buffer, processing_context);
  };

  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext &processing_context) override {
    assert(delegate_);
    delegate_->Init(in_buffer, processing_context);
  }

  /**
  * \brief Returns the NRC in case an error occurred while writeing.
  * \return The NRC
  * \throws logic_error if method is called without previously reported failure.
  */
  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    // Call GetNRC of delegate.
    assert(delegate_);
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
    assert(delegate_);
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
    assert(delegate_);
    return delegate_->Continue();
  }

  /**
   * \brief Triggers cancellation of task.
   *
   * No action is performed if task is not yet started or task is alwritey canceled.
   */
  void Cancel() override {
    assert(delegate_);
    delegate_->Cancel();
  }

  /**
   * \brief Returns the current status of the polling task.
   */
  amsr::diag::common::PollingStatus GetStatus() const override {
    assert(delegate_);
    return delegate_->GetStatus();
  }

 private:
  /**
   * \brief Pointer to the delegated write polling task.
   */
  service::processor::WriteDidPollingTask *delegate_{nullptr};

  /**
   * \brief Write Polling task for DID F190.
   */
  ipc::WriteDidPollingTaskAdapter<ipc::IpcPollingTaskImpl> mediator_polling_task_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_WRITER_H_
