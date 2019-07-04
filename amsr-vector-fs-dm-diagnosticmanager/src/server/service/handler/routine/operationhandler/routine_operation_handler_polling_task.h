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
/**        \file  routine_operation_handler_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_POLLING_TASK_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "common/diagnostic_service_polling_task.h"
#include "common/mediator_handles.h"
#include "routine_executor.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace operationhandler {
/**
 * \brief The routine polling task used by the routine operation handler.
 */
class RoutineOperationHandlerPollingTask : public common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Constructor.
   */
  RoutineOperationHandlerPollingTask(RoutineOperationType operation_type,
                                     ridmanager::RoutineInfo::HandlerType handler_type)
      : operation_type_(operation_type), handler_type_(handler_type) {}

  // Remove copy ctor/operator.
  RoutineOperationHandlerPollingTask(const RoutineOperationHandlerPollingTask&) = delete;
  RoutineOperationHandlerPollingTask& operator=(const RoutineOperationHandlerPollingTask&) = delete;

  /**
   * \brief Move constructor.
   */
  RoutineOperationHandlerPollingTask(RoutineOperationHandlerPollingTask&&) = default;

  /**
   * \brief Move operator.
   */
  RoutineOperationHandlerPollingTask& operator=(RoutineOperationHandlerPollingTask&&) = default;

  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext& processing_context) override {
    // Get the handles to be passed to the Mediator

    // TODO(PAASR-2714): this is only a temporary solution and has to be changed when the the handles are generated
    // e. g. the handles will be provided by the configuration and will be available with the DataElement, ...
    using MediatorHandle = amsr::diag::common::handles::Handle;
    MediatorHandle start_routine_handle = amsr::diag::common::handles::kStartRoutineRid0x3009;
    MediatorHandle stop_routine_handle = amsr::diag::common::handles::kStopRoutineRid0x3009;
    MediatorHandle request_results_routine_handle = amsr::diag::common::handles::kRequestRoutineResultsRid0x3009;

    // Emplace the routine executor.
    routine_executor_.emplace(processing_context, start_routine_handle, stop_routine_handle,
                              request_results_routine_handle);

    // Select the polling task to use.
    routine_executor_->SelectPollingTask(handler_type_, operation_type_);

    // Call Init method of delegate.
    routine_executor_->Init(in_buffer, out_buffer, processing_context);
  }

  /**
   * \brief Returns the NRC in case an error occurred while reading.
   * \return The NRC
   * \throws logic_error if method is called without previously reported failure.
   */
  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    assert(routine_executor_.has_value());
    return routine_executor_->GetNRC();
  }

  /**
     * \brief Triggers first step in execution of the task.
     *
     * No action is performed if current status is different from kNotStarted.
     *
     * \returns The status of the task.
     */
  amsr::diag::common::PollingStatus Start() override {
    assert(routine_executor_.has_value());
    return routine_executor_->Start();
  }

  /**
   * \brief Triggers the next step of performing the task.
   *
   * No action is performed if current status is different from kPending.
   *
   * \returns The status of the task.
   */
  amsr::diag::common::PollingStatus Continue() override {
    assert(routine_executor_.has_value());
    return routine_executor_->Continue();
  }

  /**
   * \brief Triggers cancellation of task.
   *
   * No action is performed if task is not yet started or task is alwritey canceled.
   */
  void Cancel() override {
    assert(routine_executor_.has_value());
    routine_executor_->Cancel();
  }

  /**
   * \brief Returns the current status of the polling task.
   */
  amsr::diag::common::PollingStatus GetStatus() const override {
    assert(routine_executor_.has_value());
    return routine_executor_->GetStatus();
  }

 private:
  /**
   * \brief The routine executor containing the different polling tasks (mediator and generic UDS).
   */
  vac::memory::optional<RoutineExecutor> routine_executor_;

  /**
   * \brief current operation type.
   */
  RoutineOperationType operation_type_;

  /**
   * \brief the handler type
   */
  ridmanager::RoutineInfo::HandlerType handler_type_;
};

}  // namespace operationhandler
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_POLLING_TASK_H_
