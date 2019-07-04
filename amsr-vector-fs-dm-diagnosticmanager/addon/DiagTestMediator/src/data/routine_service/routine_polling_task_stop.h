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
/**        \file  routine_polling_task_stop.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_STOP_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_STOP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/application_error_exception.h"
#include "ara/com/future.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

#include "common/template_helper.h"
#include "routine_polling_task.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

/**
 * \brief Template class for Stoping routine from Proxy as PollingTask.
 *
 * \tparam RoutineControlProxy The Proxy class providing a Stop method to stop the routine.
 * \tparam RoutineControlSerializer The serializer class with static method for serialization of the Routine data.
 * \tparam MetaInfo Meta information.
 */
template <typename RoutineControlProxy, typename RoutineControlSerializer, typename UDSServiceFailed, typename MetaInfo>
class RoutinePollingTaskStop final : public RoutinePollingTask {
  static_assert(std::is_base_of<ara::com::ApplicationErrorException, UDSServiceFailed>::value,
                "UDSServiceFailed must inherit from ara::com::ApplicationErrorException");

 public:
  /**
   * \brief Alias for RoutineControlSerializer::StopOutput
   */
  using StopOutput = typename RoutineControlSerializer::StopOutput;

  /**
   * \brief Alias for RoutineControlSerializer::StopTupleRidData
   */
  using StopTupleRidData = typename RoutineControlSerializer::StopTupleRidData;

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
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__;
    using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

    // Find Proxy
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Try to find proxy.";
    ara::com::ServiceHandleContainer<ara::com::HandleType> handles =
        RoutineControlProxy::FindService(ara::com::InstanceIdentifier::Any);
    if (handles.size() < 1) {
      // No Proxy found.
      ara::log::LogWarn() << "RoutinePollingTaskStop::" << __func__ << ": No Proxy found.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Proxy found.";
    routine_control_proxy_.emplace(handles[0]);

    // Stop routine
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Deserialize stop parameters.";
    StopTupleRidData data_input_tuple = RoutineControlSerializer::Deserialize(in_buffer_);
    // TODO(visbsz): Replace the following dummy meta-info by meta-info provided by request UdsMessage
    MetaInfo meta_info{};
    auto stop_parameters = std::tuple_cat(std::make_tuple(meta_info), data_input_tuple);

    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Call Stop.";
    amsr::diag::common::call(routine_control_proxy_->Stop, stop_parameters, stop_future_);

    if (!stop_future_.valid()) {
      ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Call of Stop() returned invalid future.";
      nrc_.emplace(UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    return PollingStatus::kPending;
  }

  PollingStatus ContinueAction() override {
    // Check if future has value
    assert(stop_future_.valid());
    if (!stop_future_.is_ready()) {
      ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Still pending for future's return.";
      // no value available
      return PollingStatus::kPending;
    }

    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Try get future.";
    StopOutput stop_output;
    try {
      stop_output = stop_future_.get();
    } catch (const UDSServiceFailed& uds_service_failed) {
      // Service on the application side is throwing UDSServiceFailedOnWrite exception.
      ara::log::LogError() << "RoutinePollingTaskStop::" << __func__ << " : " << uds_service_failed.what();
      ara::diag::udstransport::UdsNegativeResponseCode nrc =
          static_cast<ara::diag::udstransport::UdsNegativeResponseCode>(uds_service_failed.geterrorContext());
      nrc_.emplace(nrc);
      return PollingStatus::kFailed;
    } catch (const std::exception& ex) {
      ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Getting future value failed - " << ex.what();
      nrc_.emplace(ara::diag::udstransport::UdsNegativeResponseCode::kConditionsNotCorrect);
      return PollingStatus::kFailed;
    }
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Success on getting future.";
    // Serialize Data.
    ara::log::LogDebug() << "RoutinePollingTaskStop::" << __func__ << ": Serialize stop output.";
    RoutineControlSerializer::Serialize(stop_output, out_buffer_);
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
   * \brief Future that captures the result of stop method.
   */
  ara::com::Future<StopOutput> stop_future_;

  FRIEND_TEST(RoutinePollingTaskStopTestFixture, CheckInitWillSetInAndOutBuffer);
  FRIEND_TEST(RoutinePollingTaskStopTestFixture, CheckGetNRCWillNotThrowExceptionAndReturnCorrectValue);
};

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_POLLING_TASK_STOP_H_
