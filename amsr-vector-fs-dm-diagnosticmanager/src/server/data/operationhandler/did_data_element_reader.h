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
/**        \file  did_data_element_reader.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_READER_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/log/logging.hpp"
#include "common/mediator_handles.h"
#include "common/polling_task.h"
#include "server/data/did_info.h"
#include "server/data/ipc/internal_data_polling_task.h"
#include "server/data/ipc/ipc_polling_task_impl.h"
#include "server/data/ipc/read_did_polling_task_adapter.h"
#include "server/service/handler/service_handler.h"
#include "server/service/processor/readdataservice/read_did_polling_task.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {
/**
 * Implementation of SimpleReadDidPollingTask that retrieves the ActiveDiagnosticSession.
 */
class ReadActiveDiagnosticSessionPollingTask : public ipc::InternalDataPollingTask<1> {
  /**
   * Helper function that implements the ReadActiveDiagnosticSession action.
   */
  DidData RetrieveDidData(const service::ServiceProcessingContext &processing_context,
                          bool &cancellation_requested) override {
    // get current session and return it as DidData
    std::uint8_t current_session =
        processing_context.GetAccessState(conversation::access::AccessCategory::kSession).Get();
    return ipc::InternalDataPollingTask<1>::DidData{current_session};
  }
};

/**
 * \brief DidDataElementReader.
 */
class DidDataElementReader : public service::processor::ReadDidPollingTask {
 public:
  /**
   * \brief Constructor.
   */
  DidDataElementReader(const service::ServiceProcessingContext &processing_context, std::uint32_t handle)
      : mediator_polling_task_(handle, processing_context.GetProxysPool().ipc_proxy_) {}

  // Remove copy/move constructor/assignment.
  DidDataElementReader(const DidDataElementReader &) = delete;
  DidDataElementReader &operator=(const DidDataElementReader &) = delete;

  /**
   * \brief Move constructor.
   */
  DidDataElementReader(DidDataElementReader &&) = default;

  /**
   * \brief Move operator.
   */
  DidDataElementReader &operator=(DidDataElementReader &&) = default;

  /**
   * \brief Select the delegate to use.
   * \param handler_type Type of the handler.
   */
  void Select(server::service::handler::ServiceHandler::HandlerType handler_type);

  /**
   * \brief Simplified initialization method for reading.
   * Initializes the tasks with the buffer the DID data is written to while reading.
   * \param out_buffer The view onto the buffer used to store the read data.
   * \param processing_context service processing context
   */
  void Init(BufferTypeOut out_buffer, const service::ServiceProcessingContext &processing_context) override {
    // Call Init method of delegate
    service::processor::ReadDidPollingTask::BufferTypeIn empty_in_buffer;
    this->Init(empty_in_buffer, out_buffer, processing_context);
  };

  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext &processing_context) override {
    assert(delegate_);
    delegate_->Init(out_buffer, processing_context);
  }

  /**
  * \brief Returns the NRC in case an error occurred while reading.
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
   * No action is performed if task is not yet started or task is already canceled.
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
   * \brief Pointer to the delegated read polling task.
   */
  service::processor::ReadDidPollingTask *delegate_{nullptr};

  /**
   * \brief Read Polling task for mediator. The request is transferred to the mediator.
   */
  ipc::ReadDidPollingTaskAdapter<ipc::IpcPollingTaskImpl> mediator_polling_task_;

  /**
   * \brief Adapter for the ipc_polling_task reading active sessions.
   */
  ipc::ReadDidPollingTaskAdapter<ReadActiveDiagnosticSessionPollingTask> read_session_polling_task_adapter_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_DID_DATA_ELEMENT_READER_H_
