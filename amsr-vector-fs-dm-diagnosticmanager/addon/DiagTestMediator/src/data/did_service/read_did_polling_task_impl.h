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
/**        \file  read_did_polling_task_impl.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_READ_DID_POLLING_TASK_IMPL_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_READ_DID_POLLING_TASK_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"
#include "vac/testing/test_adapter.h"

#include "data/data_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_proxy.h"

namespace diag_test_mediator {
namespace data {
namespace did_service {

/**
 * \brief Template class for Reading DID from Proxy as PollingTask.
 *
 * \tparam ReadDidProxy The Proxy class providing a Read method to read DID data.
 * \tparam DidSerializer The serializer class with static method for serialization of the DID data.
 */
template <typename ReadDidProxy, typename DidSerializer, typename MetaInfo>
class ReadDidPollingTaskImpl : public DataPollingTask {
 public:
  /**
   * \brief Alias for DidSerializer::ReadOutput
   */
  using ReadOutput = typename DidSerializer::ReadOutput;
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
    if (out_buffer.size() < required_size) {
      ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__
                           << ": Called with invalid output buffer size! (Required: '" << std::to_string(required_size)
                           << "', provided: '" << std::to_string(out_buffer.size()) << "')";
      throw std::length_error("ReadDidPollingTask::Init called with output buffer of not supported size!");
    }
    out_buffer_ = out_buffer;
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
    ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Try to find Proxy.";
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        ReadDidProxy::FindService(ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      // No Proxy found.
      ara::log::LogWarn() << "ReadDidPollingTaskImpl::" << __func__ << ": No Proxy found.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << __func__ << ": Proxy found.";
    read_did_proxy_.emplace(handles[0]);
    // Start reading
    ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Call of Read() method on Proxy.";
    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    MetaInfo meta_info{};
    reading_future_ = read_did_proxy_->Read(meta_info);
    if (!reading_future_.valid()) {
      ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Call of Read() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    using UDSServiceFailedOnRead =
        ara::diag::service_interfaces::data_identifier::proxy::application_errors::UDSServiceFailedOnRead;
    // Check if future has value
    assert(reading_future_.valid());
    if (ara::com::FutureStatus::timeout == reading_future_.wait_for(kWaitTimeFuture)) {
      ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }

    ReadOutput output;
    ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Try get result of future.";
    try {
      output = reading_future_.get();
    } catch (const UDSServiceFailedOnRead& uds_service_failed_on_read) {
      // Service on the application side is throwing UDSServiceFailedOnRead exception.
      ara::log::LogError() << "ReadDidPollingTaskImpl::" << __func__ << " : " << uds_service_failed_on_read.what();
      ara::diag::udstransport::UdsNegativeResponseCode nrc =
          static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed_on_read.geterrorContext());
      nrc_.emplace(nrc);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogError() << "ReadDidPollingTaskImpl::" << __func__ << ": Reading future failed - " << ex.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }

    ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Success on reading future.";
    // Serialize output to byte stream
    if (out_buffer_.empty()) {
      ara::log::LogError() << "ReadDidPollingTaskImpl::" << __func__ << ": No buffer configured for storing DID data.";
    } else {
      DidSerializer::Serialize(output, out_buffer_);
      ara::log::LogDebug() << "ReadDidPollingTaskImpl::" << __func__ << ": Serialized '"
                           << std::to_string(out_buffer_.size()) << "' bytes.";
    }
    return PollingStatus::kComplete;
  }

  void CancelAction() override {
    ara::log::LogDebug() << __func__ << ": Cancellation requested.";
    if (read_did_proxy_.has_value()) {
      ara::log::LogDebug() << __func__ << ": Call of Cancel() on Proxy.";
      // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
      MetaInfo meta_info{};
      // Just call the method without waiting for future. No exception handling required in this case.
      read_did_proxy_->Cancel(meta_info);
    }
  }

 private:
  /**
   * \brief The view onto the buffer that stores the read DID data.
   */
  OutBufferType out_buffer_;

  /**
   * \brief Optional for possible Negative Response Code.
   */
  vac::memory::optional<ara::diag::udstransport::UdsNegativeResponseCode> nrc_;

  /**
   * \brief Proxy for Service Interface that provides the DID data to be read.
   */
  vac::memory::optional<ReadDidProxy> read_did_proxy_;

  /**
   * \brief Future that captures the result of reading the DID.
   */
  ara::com::Future<ReadOutput> reading_future_;

  FRIEND_TEST(ReadDidPollingTaskImplTestFixture, CheckInitWillSetOutBuffer);
  FRIEND_TEST(ReadDidPollingTaskImplTestFixture, CheckGetNRCWillNotThrowExceptionAndReturnCorrectValue);
};

}  // namespace did_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_READ_DID_POLLING_TASK_IMPL_H_
