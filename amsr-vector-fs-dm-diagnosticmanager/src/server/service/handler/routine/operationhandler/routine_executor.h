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
/**        \file  routine_executor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_EXECUTOR_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_EXECUTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/data/ipc/routine_polling_task_adapter.h"
#include "server/service/handler/routine/ridmanager/routine_info.h"
#include "server/service/processor/routinepollingtask/routine_polling_task.h"
#include "server/service/service_processing_context.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace operationhandler {

/**
 * \brief Implementation of RoutineExecutor selecting the correct polling task to use.
 */
class RoutineExecutor : public amsr::diag::common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Constructor.
   */
  explicit RoutineExecutor(const server::service::ServiceProcessingContext& processing_context,
                           std::uint32_t start_routine_handle, std::uint32_t stop_routine_handle,
                           std::uint32_t request_results_routine_handle)
      : mediator_polling_task_start(start_routine_handle, processing_context.GetProxysPool().ipc_proxy_),
        mediator_polling_task_stop(stop_routine_handle, processing_context.GetProxysPool().ipc_proxy_),
        mediator_polling_task_request_results(request_results_routine_handle,
                                              processing_context.GetProxysPool().ipc_proxy_) {}

  // Remove copy constructor/operator.
  RoutineExecutor(const RoutineExecutor&) = delete;
  RoutineExecutor& operator=(const RoutineExecutor&) = delete;

  /**
   * \brief Move constructor.
   */
  RoutineExecutor(RoutineExecutor&&) = default;

  /**
   * \brief Move operator.
   */
  RoutineExecutor& operator=(RoutineExecutor&&) = default;

  /**
   * \brief Select the delegate to use.
   * \param handler_type Type of the handler.
   * \param operation the selected operation.
   */
  void SelectPollingTask(ridmanager::RoutineInfo::HandlerType handler_type, RoutineOperationType operation) {
    // Temporary workaround only returning the polling task for the current operation.
    // TODO(PAASR-3107): Analyze if it is feasible to rework the code to use only one polling task
    switch (operation) {
      case RoutineOperationType::kStart:
        ara::log::LogDebug() << "RoutineExecutor::" << __func__ << ": polling task for start operation selected.";
        delegate_ = &mediator_polling_task_start;
        break;

      case RoutineOperationType::kStop:
        ara::log::LogDebug() << "RoutineExecutor::" << __func__ << ": polling task for stop operation selected.";
        delegate_ = &mediator_polling_task_stop;
        break;

      case RoutineOperationType::kRequestResult:
        ara::log::LogDebug() << "RoutineExecutor::" << __func__
                             << ": polling task for request results operation selected.";
        delegate_ = &mediator_polling_task_request_results;
        break;

      default:
        throw std::runtime_error("RoutineExecutor::Select: Operation is unknown.");
    }
  }

  /**
   * \brief Initializes the tasks with the input and output buffer needed for processing.
   * Before calling Init, the delegated polling task schall already have been selected by calling the SelectPollingTask
   * method.
   * \param in_buffer The view onto the buffer with input data (e. g. for write DID or routine control).
   * \remarks in_buffer is empty for read DID operations
   * \param out_buffer The view onto the buffer used to store the received data (e. g. for read DID or routine
   * control).
   * \remarks out_buffer will be empty for write DID operations
   * \param processing_context service processing context
   */
  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext& processing_context) override {
    if (!delegate_) {
      throw std::runtime_error("RoutineExecutor::Init: routine polling task has not been selected");
    }
    delegate_->Init(in_buffer, out_buffer, processing_context);
  }

  /**
   * \brief Returns the NRC in case an error occurred while reading.
   * \return The NRC
   * \throws logic_error if method is called without previously reported failure.
   */
  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    assert(delegate_);
    return delegate_->GetNRC();
  }

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
   * \brief delegated routine polling task.
   */
  processor::routinepollingtask::RoutinePollingTask* delegate_{nullptr};

  /**
   * \brief Polling task for start operation.
   */
  data::ipc::RoutinePollingTaskAdapter<data::ipc::IpcPollingTaskImpl> mediator_polling_task_start;

  /**
   * \brief Polling task for stop operation.
   */
  data::ipc::RoutinePollingTaskAdapter<data::ipc::IpcPollingTaskImpl> mediator_polling_task_stop;

  /**
   * \brief Polling task for request results operation.
   */
  data::ipc::RoutinePollingTaskAdapter<data::ipc::IpcPollingTaskImpl> mediator_polling_task_request_results;
};

}  // namespace operationhandler
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_EXECUTOR_H_
