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
 *      \file  routine_polling_task_adapter.h
 *      \brief  Adapter that implements RoutinePollingTask
 *
 *      \details  Adapter that implements RoutinePollingTask and forwards the polling API calls to a IpcPollingTask
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_IPC_ROUTINE_POLLING_TASK_ADAPTER_H_
#define SRC_SERVER_DATA_IPC_ROUTINE_POLLING_TASK_ADAPTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <utility>

#include "vac/language/cpp14_backport.h"

#include "common/polling_task.h"
#include "server/data/ipc/ipc_polling_task.h"
#include "server/data/ipc/ipc_polling_task_impl.h"
#include "server/service/processor/routinepollingtask/routine_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace ipc {

/**
 * \brief Adapter that implements the ReadDidPollingTask interface for a IpcPollingTask.
 */
template <class IpcPollingTaskImplementation>
class RoutinePollingTaskAdapter : public server::service::processor::routinepollingtask::RoutinePollingTask {
  static_assert(std::is_base_of<IpcPollingTask, IpcPollingTaskImplementation>::value,
                "IpcPollingTaskImplementation must inherit from IpcPollingTask");
  static_assert(std::is_base_of<common::DiagnosticServicePollingTaskBase, IpcPollingTaskImplementation>::value,
                "IpcPollingTaskImplementation must inherit from common::DiagnosticServicePollingTaskBase");

 public:
  /**
   * \brief Initializes the instance
   * \param args arguments used to instantiate the delegate.
   */
  template <typename... Args>
  explicit RoutinePollingTaskAdapter(Args&&... args) : delegate_(std::forward<Args>(args)...) {}

  // no copy
  RoutinePollingTaskAdapter(const RoutinePollingTaskAdapter& other) = delete;
  RoutinePollingTaskAdapter& operator=(const RoutinePollingTaskAdapter&) = delete;

  /**
   * \brief Move constructor.
   */
  RoutinePollingTaskAdapter(RoutinePollingTaskAdapter&& other) = default;

  /**
   * \brief Move operator.
   */
  RoutinePollingTaskAdapter& operator=(RoutinePollingTaskAdapter&&) = default;

  /**
   * Destructor
   */
  virtual ~RoutinePollingTaskAdapter() = default;

  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const server::service::ServiceProcessingContext& processing_context) override {
    delegate_.Init(in_buffer, out_buffer, processing_context);
  }

  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override { return delegate_.GetNRC(); }

  amsr::diag::common::PollingStatus Start() override { return delegate_.Start(); }

  amsr::diag::common::PollingStatus Continue() override { return delegate_.Continue(); }

  void Cancel() override { delegate_.Cancel(); }

  amsr::diag::common::PollingStatus GetStatus() const override { return delegate_.GetStatus(); }

 private:
  IpcPollingTaskImplementation delegate_;
};

}  // namespace ipc
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_IPC_ROUTINE_POLLING_TASK_ADAPTER_H_
