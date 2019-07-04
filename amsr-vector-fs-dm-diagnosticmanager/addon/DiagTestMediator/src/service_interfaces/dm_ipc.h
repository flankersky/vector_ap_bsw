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
/**        \file  dm_ipc.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_SERVICE_INTERFACES_DM_IPC_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_SERVICE_INTERFACES_DM_IPC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <thread>
#include <utility>

#include "vac/memory/optional.h"
#include "vac/testing/test_adapter.h"

#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton.h"
#include "data/data_manager.h"
#include "data/data_operation_factory.h"

namespace diag_test_mediator {
namespace service_interfaces {

class DM_IPC : public ara::diag::service_interfaces::dm_ipc::DM_IPCSkeleton {
 public:
  /**
   * \brief Constructor.
   * \param instance_id The instance ID
   * \param did_op_factory Factory to create DataPollingTask instances for DIDs
   * \param rid_op_factory Factory to create DataPollingTask instances for RIDs (start, stop routines, ...)
   */
  explicit DM_IPC(ara::com::InstanceIdentifier instance_id, const data::DataOperationFactory& did_operation_factory,
                  const data::DataOperationFactory& rid_operation_factory);

  /**
   * \brief Destructor.
   */
  virtual ~DM_IPC();

  DM_IPC(const DM_IPC& rhs) = delete;
  DM_IPC(DM_IPC&& rhs) = delete;
  const DM_IPC& operator=(const DM_IPC&) = delete;
  const DM_IPC& operator=(const DM_IPC&&) = delete;

  using RequestDataOutput = ara::diag::service_interfaces::dm_ipc::skeleton::methods::RequestData::Output;

  /**
   * \brief IPC entry point
   */
  ara::com::Future<RequestDataOutput> RequestData(
      const ara::diag::service_interfaces::dm_ipc::uint32& handle,
      const ara::diag::service_interfaces::dm_ipc::MetaInfoType& meta_info,
      const ara::diag::service_interfaces::dm_ipc::DataArrayType& request_data) override;

 private:
  /**
   * Structure for RequestData
   */
  struct RequestDataArgs {
    ara::diag::service_interfaces::dm_ipc::uint32 handle;
    ara::diag::service_interfaces::dm_ipc::MetaInfoType meta_info;
    ara::diag::service_interfaces::dm_ipc::DataArrayType input_data;
  };

  struct RequestDataWorkItem {
    RequestDataArgs args;
    ara::com::Promise<RequestDataOutput> promise;
  };

  /**
   * \brief Worker thread that processes the RequestData inputs.
   * The worker thread is started automatically by the constructor.
   */
  void WorkerThread();

  /**
   * \brief  Stops the worker thread.
   */
  void StopWorkerThread();

  // private helpers for processing
  void EnqueueWork(RequestDataWorkItem&& args);

  void ProcessRequest(RequestDataArgs args, ara::com::Promise<RequestDataOutput>&& promise);
  RequestDataOutput ExecuteOperation(const RequestDataArgs& args);

 private:
  /**
   * Specifies the maximum supported size of the result returned to the DM.
   */
  static constexpr std::size_t kMaxOutBufferSize = 100;
  /**
   * Data Manager - factory that creates the data polling tasks
   */
  data::DataManager data_manager_;
  /**
   * Mutex to protect the critical section (queue of one item).
   */
  std::mutex request_data_mutex_;
  /**
   * The data for the worker thread (queue of one item).
   */
  vac::memory::optional<RequestDataWorkItem> request_data_;
  /**
   * Flag that controls if the worker thread should be stopped.
   */
  bool worker_thread_running_{true};
  /**
   * Condition variable to trigger the worker thread.
   */
  std::condition_variable condition_variable_;
  /**
   * Worker thread that processes the incoming requests.
   */
  std::thread worker_thread_;
};

}  // namespace service_interfaces
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_SERVICE_INTERFACES_DM_IPC_H_
