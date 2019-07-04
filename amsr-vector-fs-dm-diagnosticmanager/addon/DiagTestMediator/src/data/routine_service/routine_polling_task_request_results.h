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
/**        \file  routine_polling_task_request_results.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_REQUEST_RESULTS_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_REQUEST_RESULTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

#include "ara/log/logging.hpp"

#include "ara/com/application_error_exception.h"
#include "common/template_helper.h"
#include "routine_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

/**
 * \brief Template class for RequestResultsing routine from Proxy as PollingTask.
 *
 * \tparam RoutineControlProxy The Proxy class providing a RequestResults method to request_results the routine.
 * \tparam RoutineControlSerializer The serializer class with static method for serialization of the Routine data.
 * \tparam MetaInfo Meta information.
 */
template <typename RoutineControlProxy, typename RoutineControlSerializer, typename UDSServiceFailed, typename MetaInfo>
class RoutinePollingTaskRequestResults final : public RoutinePollingTask {
  static_assert(std::is_base_of<ara::com::ApplicationErrorException, UDSServiceFailed>::value,
                "UDSServiceFailed must inherit from ara::com::ApplicationErrorException");

 public:
  /**
   * \brief Alias for RoutineControlSerializer::RequestResultsOutput
   */
  using RequestResultsOutput = typename RoutineControlSerializer::RequestResultsOutput;

  /**
   * \brief Alias for RoutineControlSerializer::RequestResultsTupleRidData
   */
  using RequestResultsTupleRidData = typename RoutineControlSerializer::RequestResultsTupleRidData;

  /**
   * \brief Base class constructors.
   */
  using RoutinePollingTask::RoutinePollingTask;

 protected:
  /**
     * \brief Alias for common::PollingStatus
     */
  using PollingStatus = amsr::diag::common::PollingStatus;

  PollingStatus StartAction() override {
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__;
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // Find Proxy
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Try to find proxy.";
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        RoutineControlProxy::FindService(ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      // No Proxy found.
      ara::log::LogWarn() << "RoutinePollingTaskRequestResults::" << __func__ << ": No Proxy found.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Proxy found.";
    routine_control_proxy_.emplace(handles[0]);

    // RequestResults routine
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__
                         << ": Deserialize request_results parameters.";
    RequestResultsTupleRidData data_input_tuple = RoutineControlSerializer::Deserialize(in_buffer_);
    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    MetaInfo meta_info{};
    auto request_results_parameters = std::tuple_cat(std::make_tuple(meta_info), data_input_tuple);

    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Call RequestResults.";
    amsr::diag::common::call(routine_control_proxy_->RequestResults, request_results_parameters,
                             request_results_future_);

    if (!request_results_future_.valid()) {
      ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__
                           << ": Call of RequestResults() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    // Check if future has value
    assert(request_results_future_.valid());
    if (!request_results_future_.is_ready()) {
      ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__
                           << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }

    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Try get future.";
    RequestResultsOutput request_results_output;
    try {
      request_results_output = request_results_future_.get();
    } catch (const UDSServiceFailed& uds_service_failed) {
      // Service on the application side is throwing UDSServiceFailedOnWrite exception.
      ara::log::LogError() << "RoutinePollingTaskRequestResults::" << __func__ << " : " << uds_service_failed.what();
      ara::diag::udstransport::UdsNegativeResponseCode nrc =
          static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed.geterrorContext());
      nrc_.emplace(nrc);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Getting future value failed - "
                           << ex.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Success on getting future.";
    // Serialize Data.
    ara::log::LogDebug() << "RoutinePollingTaskRequestResults::" << __func__ << ": Serialize request results output.";
    RoutineControlSerializer::Serialize(request_results_output, out_buffer_);
    return PollingStatus::kComplete;
  }

  void CancelAction() override {  // Doing nothing
  }

 protected:
  /**
   * \brief Proxy for Service Interface that provides the Routine control methods.
   */
  vac::memory::optional<RoutineControlProxy> routine_control_proxy_;

  /**
   * \brief Future that captures the result of request_results method.
   */
  ara::com::Future<RequestResultsOutput> request_results_future_;

  FRIEND_TEST(RoutinePollingTaskRequestResultsTestFixture, CheckInitWillSetInAndOutBuffer);
  FRIEND_TEST(RoutinePollingTaskRequestResultsTestFixture, CheckGetNRCWillNotThrowExceptionAndReturnCorrectValue);
};

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_REQUEST_RESULTS_H_
