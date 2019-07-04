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
/**        \file  dm_ipc.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"

#include "service_interfaces/dm_ipc.h"

namespace diag_test_mediator {
namespace service_interfaces {

using ara::diag::service_interfaces::dm_ipc::skeleton::methods::RequestData;
using ara::diag::service_interfaces::dm_ipc::uint32;
using ara::diag::service_interfaces::dm_ipc::DataArrayType;
using ara::com::Promise;
using ara::com::Future;

using diag_test_mediator::data::DataHandler;
using diag_test_mediator::data::DataPollingTask;

static std::string ToHexString(const DataArrayType& data) {
  std::stringstream stream;
  stream << std::hex << std::setw(2) << std::setfill('0');
  for (std::stringstream::char_type byte : data) {
    // print the byte as std::uint16_t to force a readable number (std::uint8_t is interpreted as character)
    stream << static_cast<std::uint16_t>(byte);
  }
  return stream.str();
}

DM_IPC::DM_IPC(const ara::com::InstanceIdentifier instance_id, const data::DataOperationFactory& did_operation_factory,
               const data::DataOperationFactory& rid_operation_factory)
    : ara::diag::service_interfaces::dm_ipc::DM_IPCSkeleton(instance_id, ara::com::MethodCallProcessingMode::kPoll),
      data_manager_{did_operation_factory, rid_operation_factory},
      worker_thread_(&DM_IPC::WorkerThread, this) {}

DM_IPC::~DM_IPC() { StopWorkerThread(); }

void DM_IPC::StopWorkerThread() {
  worker_thread_running_ = false;
  // signal conditional variable (critical section)
  {
    std::lock_guard<std::mutex> lk(request_data_mutex_);
    condition_variable_.notify_one();
  }
  // join thread if thread is joinable
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  } else {
    // thread was stopped by an exception or never started
    ara::log::LogError() << "DM_IPC::Shutdown: Worker thread is not joinable!";
  }
}

Future<RequestData::Output> DM_IPC::RequestData(
    const ara::diag::service_interfaces::dm_ipc::uint32& handle,
    const ara::diag::service_interfaces::dm_ipc::MetaInfoType& meta_info,
    const ara::diag::service_interfaces::dm_ipc::DataArrayType& input_data) {
  Promise<RequestData::Output> promise;
  Future<RequestData::Output> future = promise.get_future();

  ara::log::LogDebug() << __func__ << " >> handle='" << std::to_string(handle) << "', input_data='"
                       << ToHexString(input_data) << "'";

  if (data_manager_.IsHandleSupported(handle)) {
    EnqueueWork(RequestDataWorkItem{handle, meta_info, input_data, std::move(promise)});
  } else {
    std::runtime_error error("DM_IPC::RequestData: Not supported handle '" + std::to_string(handle) + "'!");
    promise.set_exception(std::make_exception_ptr(error));
  }

  return future;
}

void DM_IPC::EnqueueWork(RequestDataWorkItem&& args) {
  // critical section for work data (caller will be blocked when in use)
  {
    // TODO(PAASR-2951): this could lead to subtle errors when one thread is waiting for a ara::com response
    // and the next is blocked because it wants to push data, but currently only one thread (kPoll mode) will
    // call this method so this might need to be reworked when the current implementation(/the workaround) is changed
    std::lock_guard<std::mutex> lk(request_data_mutex_);
    request_data_.emplace(std::move(args));
    condition_variable_.notify_one();
  }
}

void DM_IPC::WorkerThread() {
  try {
    while (worker_thread_running_) {
      std::unique_lock<std::mutex> lk(request_data_mutex_);
      // wait for the next message
      condition_variable_.wait(
          lk, [this]() { return (this->worker_thread_running_ == false || request_data_.has_value()); });
      if (worker_thread_running_) {
        assert(request_data_.has_value());

        RequestDataWorkItem work_item = std::move(request_data_.value());
        request_data_.reset();
        lk.unlock();
        ProcessRequest(work_item.args, std::move(work_item.promise));
      }
    }
  } catch (const std::exception& ex) {
    ara::log::LogError() << "DM_IPC::" << __func__ << " >> Worker thread interrupted by exception: " << ex.what();
  }
}

void DM_IPC::ProcessRequest(RequestDataArgs args, ara::com::Promise<RequestDataOutput>&& promise) {
  try {
    RequestData::Output result = ExecuteOperation(args);

    // provide the result to the caller
    promise.set_value(result);

    // return to caller
  } catch (const std::exception& ex) {
    ara::log::LogDebug() << "DM_IPC::" << __func__ << " >> Exception while processing Request: " << ex.what();
    promise.set_exception(std::make_exception_ptr(ex));
  }
}

DM_IPC::RequestDataOutput DM_IPC::ExecuteOperation(const RequestDataArgs& args) {
  RequestData::Output result;

  vac::memory::optional<diag_test_mediator::data::DataHandler> data_handle =
      data_manager_.CreateDataHandlerIfAvailable(args.handle);
  // if the handle is not supported stop further processing
  if (false == data_handle.has_value()) {
    throw std::runtime_error("DM_IPC::ExecuteOperation: Not supported handle '" + std::to_string(args.handle) + "'!");
  }

  /* TODO(PAASR-2686) how is this supposed to work - has to be a std::vector... */
  result.response_data.resize(data_handle->GetOutputDataSize());

  vac::container::const_array_view<std::uint8_t> task_input_data{args.input_data};
  vac::container::array_view<std::uint8_t> task_output_buffer{result.response_data};

  ara::diag::udstransport::UdsMessage::MetaInfoMap temp_meta_info;
  // TODO(PAASR-2683): use the meta information when a usable version is available
  DataPollingTask::Ptr polling_task = data_handle->StartOperation(temp_meta_info, task_input_data, task_output_buffer);

  // run polling task
  while (polling_task->GetStatus() == amsr::diag::common::PollingStatus::kPending) {
    // TODO(PAASR-2388, PAASR-1926): support cancellation
    polling_task->Continue();
    // Notice: the called ContinueAction uses the future::wait_for with a short timeout
    // to free the resources occupied by the current thread and to give other threads the CPU
  }

  // TODO(PAASR-2955, PAASR-2999): Currently NRCs which might be provided by the called application are not forwarded to
  // the DM, because DM_IPC::RequestData dosen't support NRCs/Errors until now (shall be added with PAASR-2999)

  return result;
}

}  // namespace service_interfaces
}  // namespace diag_test_mediator
