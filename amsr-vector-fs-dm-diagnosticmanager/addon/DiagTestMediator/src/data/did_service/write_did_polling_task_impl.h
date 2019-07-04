/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  write_did_polling_task_impl.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_WRITE_DID_POLLING_TASK_IMPL_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_WRITE_DID_POLLING_TASK_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"
#include "vac/testing/test_adapter.h"

#include "common/template_helper.h"
#include "data/data_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {
namespace did_service {

/**
 * \brief Template class for Reading DID from Proxy as PollingTask.
 *
 * \tparam WriteDidProxy The Proxy class providing a Write method to write DID data.
 * \tparam DidSerializer The serializer class with static method for serialization of the DID data.
 */
template <typename WriteDidProxy, typename DidSerializer, typename MetaInfo>
class WriteDidPollingTaskImpl : public DataPollingTask {
 public:
  /**
   * \brief Alias for DidSerializer::WriteOutput
   */
  using WriteOutput = typename DidSerializer::WriteOutput;
  /**
   * \brief Alias for DidSerializer::TupleDidData
   */
  using TupleDidData = typename DidSerializer::TupleDidData;
  /**
   * \brief Base class constructors.
   */
  using DataPollingTask::DataPollingTask;

  /**
   * \brief Initializes the tasks with the buffer the DID data is written to while reading.
   * TODO(vismhe) param meta_info meta info to pass to the application
   * \param in_buffer The view onto the buffer used pass to the mediator.
   * \param out_buffer The view onto the buffer used to store the received data.
   * \param processing_context service processing context
   */
  void Init(InBufferType in_buffer, OutBufferType out_buffer) override {
    const std::size_t required_size = DidSerializer::GetDidDataSize();
    if (in_buffer.size() < required_size) {
      ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__
                           << ": Called with invalid input buffer size! (Required: '" << std::to_string(required_size)
                           << "', provided: '" << std::to_string(in_buffer.size()) << "')";
      throw std::length_error("WriteDidPollingTaskImpl::Init called with input buffer of not supported size!");
    }
    in_buffer_ = in_buffer;
  }

  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    if (!nrc_.has_value()) {
      throw std::logic_error("ReadDidPollingTask::GetNRC() called without reported failure.");
    }
    return nrc_.value();
  }

 protected:
  /**
   * \brief Alias for common::PollingStatus
   */
  using PollingStatus = amsr::diag::common::PollingStatus;

  PollingStatus StartAction() override {
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // Request Proxy.
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Try to find Proxy.";
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        WriteDidProxy::FindService(ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      // No Proxy found.
      ara::log::LogDebug() << __func__ << ": No Proxy found.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Proxy found.";
    write_did_proxy_.emplace(handles[0]);
    TupleDidData data_input_tuple = DidSerializer::Deserialize(in_buffer_);
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Deserialized '"
                         << std::to_string(in_buffer_.size()) << "' bytes.";

    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    MetaInfo meta_info{};
    auto write_input_tuple = std::tuple_cat(std::make_tuple(meta_info), data_input_tuple);

    // Start writing
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Call of Write() method on Proxy.";
    amsr::diag::common::call(write_did_proxy_->Write, write_input_tuple, writing_future_);
    if (!writing_future_.valid()) {
      ara::log::LogError() << "WriteDidPollingTaskImpl::" << __func__ << ": Call of Write() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    using UDSServiceFailedOnWrite =
        ara::diag::service_interfaces::data_identifier::proxy::application_errors::UDSServiceFailedOnWrite;
    // Check if future has value
    assert(writing_future_.valid());
    if (ara::com::FutureStatus::timeout == writing_future_.wait_for(kWaitTimeFuture)) {
      ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }

    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Try get value of future.";
    try {
      writing_future_.get();
    } catch (const UDSServiceFailedOnWrite& uds_service_failed_on_write) {
      // Service on the application side is throwing UDSServiceFailedOnWrite exception.
      ara::log::LogError() << "WriteDidPollingTaskImpl::" << __func__ << " : " << uds_service_failed_on_write.what();
      ara::diag::udstransport::UdsNegativeResponseCode nrc =
          static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed_on_write.geterrorContext());
      nrc_.emplace(nrc);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__
                           << ": Getting future value failed with exception - " << ex.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Success on getting future.";
    // Writing action was successful, no action pending.
    return PollingStatus::kComplete;
  }

  void CancelAction() override {
    ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Cancellation requested.";
    if (write_did_proxy_.has_value()) {
      ara::log::LogDebug() << "WriteDidPollingTaskImpl::" << __func__ << ": Call of Cancel() on Proxy.";
      // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
      MetaInfo meta_info{};
      // Just call the method without waiting for future. No exception handling required in this case.
      write_did_proxy_->Cancel(meta_info);
    }
  }

 private:
  /**
   * \brief The view onto the buffer that stores the DID data to be written.
   */
  InBufferType in_buffer_;

  /**
   * \brief Optional for possible Negative Response Code.
   */
  vac::memory::optional<ara::diag::udstransport::UdsNegativeResponseCode> nrc_;

  /**
   * \brief Proxy for Service Interface that provides write method.
   */
  vac::memory::optional<WriteDidProxy> write_did_proxy_;

  /**
   * \brief Future that captures the result of writing the DID.
   */
  ara::com::Future<WriteOutput> writing_future_;

  FRIEND_TEST(WriteDidPollingTaskImplTestFixture, CheckInitWillSetInBuffer);
  FRIEND_TEST(WriteDidPollingTaskImplTestFixture, CheckGetNRCWillNotThrowExceptionAndReturnCorrectValue);
};

}  // namespace did_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_WRITE_DID_POLLING_TASK_IMPL_H_
