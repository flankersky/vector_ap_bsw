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
 *      \file  ipc_polling_task_impl.h
 *      \brief  Definition of the IpcPollingTaskImpl class.
 *
 *      \details Implementation of the IpcPollingTask interface for operations handled by the Mediator
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_IMPL_H_
#define SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

#include "ara/com/future.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy.h"
#include "common/diagnostic_service_polling_task_base.h"
#include "server/data/ipc/ipc_polling_task.h"
#include "udstransport/uds_negative_response_code.h"
#include "utility/string_helper.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace ipc {

/**
 * \brief Class for invoking a Mediator IPC as PollingTask.
 */
class IpcPollingTaskImpl : public IpcPollingTask, public common::DiagnosticServicePollingTaskBase {
 public:
  /**
   * \brief Alias for DM_IPCProxy
   */
  using DM_IPCProxy = ara::diag::service_interfaces::dm_ipc::DM_IPCProxy;

  /**
   * \brief Base class constructor.
   * \param handle The handle that identifies the operation
   * \param ipc_proxy optional containing the ipc proxy.
   */
  explicit IpcPollingTaskImpl(std::uint32_t handle, vac::memory::optional<DM_IPCProxy>& ipc_proxy)
      : handle_(handle), ipc_proxy_(ipc_proxy) {}

  virtual ~IpcPollingTaskImpl() = default;

  // Remove copy contructor/operator.
  IpcPollingTaskImpl(const IpcPollingTaskImpl&) = delete;
  IpcPollingTaskImpl& operator=(const IpcPollingTaskImpl&) = delete;

  /**
   * \brief Move constructor.
   */
  IpcPollingTaskImpl(IpcPollingTaskImpl&&) = default;

  /**
   * \brief move operator.
   */
  IpcPollingTaskImpl& operator=(IpcPollingTaskImpl&&) = default;

  void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
            const service::ServiceProcessingContext& processing_context) override {
    in_buffer_ = in_buffer;
    out_buffer_ = out_buffer;
  }

 protected:
  /**
   * \brief Alias for common::PollingStatus
   */
  using PollingStatus = amsr::diag::common::PollingStatus;

  /**
   * \brief Alias for RequestData
   */
  using RequestData = ara::diag::service_interfaces::dm_ipc::proxy::methods::RequestData;

  PollingStatus StartAction() override {
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // Request Proxy.
    if (!ipc_proxy_.has_value()) {
      ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Try to find Proxy.";
      ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
          DM_IPCProxy::FindService(ara::com::InstanceIdentifier::Any);
      if (handles.size() < 1) {
        // No Proxy found.
        ara::log::LogError() << "IpcPollingTaskImpl::" << __func__ << ": No Proxy found.";
        nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
        return PollingStatus::kFailed;
      }

      ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Proxy found.";
      ipc_proxy_.emplace(handles[0]);
    }
    // Start reading
    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Call of Read() method on Proxy.";
    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    // TODO(vismhe): Who stores out buffer? Is stack sufficient?
    ara::diag::service_interfaces::dm_ipc::DataArrayType buffer(in_buffer_.begin(), in_buffer_.end());

    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << " >> Calling RequestData with input: 0x"
                         << diag::utility::ToString(buffer, 20);

    ara::diag::service_interfaces::dm_ipc::MetaInfoType meta_info{};
    ipc_future_ = ipc_proxy_->RequestData(handle_, meta_info, buffer);

    if (!ipc_future_.valid()) {
      ara::log::LogError() << "IpcPollingTaskImpl::" << __func__ << ": Call of RequestData() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    using ara::diag::service_interfaces::dm_ipc::proxy::application_errors::Failed;
    // Check if future has value
    assert(ipc_future_.valid());
    constexpr std::chrono::nanoseconds kWaitTimeFuture(10);
    if (ara::com::FutureStatus::timeout == ipc_future_.wait_for(kWaitTimeFuture)) {
      ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }
    RequestData::Output output;
    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Try get future.";
    try {
      output = ipc_future_.get();
    } catch (const Failed& ipc_failed) {
      // Service on the application side is throwing UDSServiceFailedOnRead exception.
      ara::log::LogError() << "IpcPollingTaskImpl::" << __func__ << " : " << ipc_failed.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kGeneralReject);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogError() << "IpcPollingTaskImpl::" << __func__ << ": Reading future failed - " << ex.what();
      // TODO(visbsz): Read correct NRC from Exception object as soon as ara::com supports this feature.
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }

    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Success on reading future.";
    // Serialize output to byte stream
    if (out_buffer_.size() < output.response_data.size()) {
      ara::log::LogError() << "IpcPollingTaskImpl::" << __func__
                           << ": No or not enough buffer configured for storing result data.";
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kGeneralReject);
      return PollingStatus::kFailed;
    }

    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << " >> Result received: 0x"
                         << diag::utility::ToString(output.response_data, 20);

    std::copy_n(output.response_data.begin(), out_buffer_.size(), out_buffer_.begin());

    return PollingStatus::kComplete;
  }

  void CancelAction() override {
    ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Cancellation requested.";
    if (ipc_proxy_.has_value()) {
      ara::log::LogDebug() << "IpcPollingTaskImpl::" << __func__ << ": Call of Cancel() on Proxy.";
      // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
      // MetaInfo meta_info{};
      // Just call the method without waiting for future. No exception handling required in this case.
      // ipc_proxy_->Cancel(meta_info);
    }
  }

 private:
  /**
   * \brief The operation handle used for the IPC
   */
  std::uint32_t handle_;

  /**
   * \brief The view onto the buffer that stores the request data.
   */
  BufferTypeIn in_buffer_;

  /**
   * \brief The view onto the buffer that receives the result data.
   */
  BufferTypeOut out_buffer_;

  /**
   * \brief Proxy for Service Interface that provides the DID data to be read.
   */
  vac::memory::optional<DM_IPCProxy>& ipc_proxy_;

  /**
   * \brief Future that captures the result of reading the DID.
   */
  ara::com::Future<RequestData::Output> ipc_future_;
};

}  // namespace ipc
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_IMPL_H_
