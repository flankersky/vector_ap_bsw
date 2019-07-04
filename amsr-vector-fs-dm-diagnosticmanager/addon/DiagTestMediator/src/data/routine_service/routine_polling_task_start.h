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
/**        \file  routine_polling_task_start.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_START_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_START_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/application_error_exception.h"
#include "ara/com/future.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"
#include "vac/testing/test_adapter.h"

#include "common/template_helper.h"
#include "routine_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

/**
 * \brief Template class for Starting routine from Proxy as PollingTask.
 *
 * \tparam RoutineControlProxy The Proxy class providing a Start method to start the routine.
 * \tparam RoutineControlSerializer The serializer class with static method for serialization of the Routine data.
 * \tparam MetaInfo Meta information.
 */
template <typename RoutineControlProxy, typename RoutineControlSerializer, typename UDSServiceFailed, typename MetaInfo>
class RoutinePollingTaskStart final : public RoutinePollingTask {
  static_assert(std::is_base_of<ara::com::ApplicationErrorException, UDSServiceFailed>::value,
                "UDSServiceFailed must inherit from ara::com::ApplicationErrorException");

 public:
  /**
   * \brief Alias for RoutineControlSerializer::StartOutput
   */
  using StartOutput = typename RoutineControlSerializer::StartOutput;

  /**
   * \brief Alias for RoutineControlSerializer::StartTupleRidData
   */
  using StartTupleRidData = typename RoutineControlSerializer::StartTupleRidData;

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
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__;
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // Find Proxy
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Try to find proxy.";
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        RoutineControlProxy::FindService(ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      // No Proxy found.
      ara::log::LogWarn() << "RoutinePollingTaskStart::" << __func__ << ": No Proxy found.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Proxy found.";
    routine_control_proxy_.emplace(handles[0]);

    // Start routine
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Deserialize start parameters.";
    StartTupleRidData data_input_tuple = RoutineControlSerializer::Deserialize(in_buffer_);
    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    MetaInfo meta_info{};
    auto start_parameters = std::tuple_cat(std::make_tuple(meta_info), data_input_tuple);

    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Call Start.";
    amsr::diag::common::call(routine_control_proxy_->Start, start_parameters, start_future_);

    if (!start_future_.valid()) {
      ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Call of Start() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    // Check if future has value
    assert(start_future_.valid());
    if (!start_future_.is_ready()) {
      ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }

    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Try get future.";
    StartOutput start_output;
    try {
      start_output = start_future_.get();
    } catch (const UDSServiceFailed& uds_service_failed) {
      // Service on the application side is throwing UDSServiceFailedOnWrite exception.
      ara::log::LogError() << "RoutinePollingTaskStart::" << __func__ << " : " << uds_service_failed.what();
      ara::diag::udstransport::UdsNegativeResponseCode nrc =
          static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed.geterrorContext());
      nrc_.emplace(nrc);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Getting future value failed - "
                           << ex.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Success on getting future.";
    // Serialize Data.
    ara::log::LogDebug() << "RoutinePollingTaskStart::" << __func__ << ": Serialize start output.";
    RoutineControlSerializer::Serialize(start_output, out_buffer_);
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
   * \brief Future that captures the result of start method.
   */
  ara::com::Future<StartOutput> start_future_;

  FRIEND_TEST(RoutinePollingTaskStartTestFixture, CheckInitWillSetInAndOutBuffer);
  FRIEND_TEST(RoutinePollingTaskStartTestFixture, CheckGetNRCWillNotThrowExceptionAndReturnCorrectValue);
};

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_START_H_
