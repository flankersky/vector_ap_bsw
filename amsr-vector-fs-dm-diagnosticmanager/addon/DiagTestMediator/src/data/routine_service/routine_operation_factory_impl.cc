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
/**        \file  did_operation_factory_impl.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "data/routine_service/routine_operation_factory_impl.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_serializer_request_results.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_serializer_start.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_serializer_stop.h"
#include "common/mediator_handles.h"
#include "data/routine_service/routine_polling_task_request_results.h"
#include "data/routine_service/routine_polling_task_start.h"
#include "data/routine_service/routine_polling_task_stop.h"
#include "vac/language/cpp14_backport.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

namespace handles = amsr::diag::common::handles;
namespace routine_control_3009 = ara::diag::service_interfaces::routine_control_3009;

bool RoutineOperationFactoryImpl::CanCreatePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kStartRoutineRid0x3009:
    case handles::kStopRoutineRid0x3009:
    case handles::kRequestRoutineResultsRid0x3009:
      return true;
    default:
      return false;
  }
}

DataPollingTask::Ptr RoutineOperationFactoryImpl::CreatePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kStartRoutineRid0x3009:
      return CreateStartRoutinePollingTask(handle);

    case handles::kStopRoutineRid0x3009:
      return CreateStopRoutinePollingTask(handle);

    case handles::kRequestRoutineResultsRid0x3009:
      return CreateRequestRoutineResultsPollingTask(handle);

    default:
      ara::log::LogError() << "RoutineOperationFactoryImpl::" << __func__ << ": handle '" << handle
                           << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

using RoutineControl_SWCL_A_RID_3009SerializerStart =
    routine_control_3009::serializer::RoutineControl_SWCL_A_RID_3009SerializerStart;

DataPollingTask::Ptr RoutineOperationFactoryImpl::CreateStartRoutinePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kStartRoutineRid0x3009:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<RoutinePollingTaskStart<
          routine_control_3009::RoutineControl_SWCL_A_RID_3009Proxy, RoutineControl_SWCL_A_RID_3009SerializerStart,
          routine_control_3009::proxy::application_errors::UDSServiceFailedOnStart,
          routine_control_3009::MetaInfoType>>();
    default:
      ara::log::LogError() << "RoutineOperationFactoryImpl::" << __func__ << ": handle '" << handle
                           << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

using RoutineControl_SWCL_A_RID_3009SerializerStop =
    routine_control_3009::serializer::RoutineControl_SWCL_A_RID_3009SerializerStop;

DataPollingTask::Ptr RoutineOperationFactoryImpl::CreateStopRoutinePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kStopRoutineRid0x3009:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<RoutinePollingTaskStop<
          routine_control_3009::RoutineControl_SWCL_A_RID_3009Proxy, RoutineControl_SWCL_A_RID_3009SerializerStop,
          routine_control_3009::proxy::application_errors::UDSServiceFailedOnStop,
          routine_control_3009::MetaInfoType>>();
    default:
      ara::log::LogError() << "RoutineOperationFactoryImpl::" << __func__ << ": handle '" << handle
                           << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

using RoutineControl_SWCL_A_RID_3009SerializerRequestResults = ara::diag::service_interfaces::routine_control_3009::
    serializer::RoutineControl_SWCL_A_RID_3009SerializerRequestResults;

DataPollingTask::Ptr RoutineOperationFactoryImpl::CreateRequestRoutineResultsPollingTask(Handle handle) const {
  switch (handle) {
    case handles::kRequestRoutineResultsRid0x3009:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<RoutinePollingTaskRequestResults<
          ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009Proxy,
          RoutineControl_SWCL_A_RID_3009SerializerRequestResults,
          routine_control_3009::proxy::application_errors::UDSServiceFailedOnRequestResults,
          routine_control_3009::MetaInfoType>>();
    default:
      ara::log::LogError() << "RoutineOperationFactoryImpl::" << __func__ << ": handle '" << handle
                           << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

std::size_t RoutineOperationFactoryImpl::GetOutputDataSize(Handle handle) const {
  switch (handle) {
    case handles::kStartRoutineRid0x3009:
      return RoutineControl_SWCL_A_RID_3009SerializerStart::GetOutputDataSize();
    case handles::kStopRoutineRid0x3009:
      return RoutineControl_SWCL_A_RID_3009SerializerStop::GetOutputDataSize();
    case handles::kRequestRoutineResultsRid0x3009:
      return RoutineControl_SWCL_A_RID_3009SerializerRequestResults::GetOutputDataSize();
    default:
      ara::log::LogError() << "DidOperationFactoryImpl::" << __func__ << ": handle '" << handle << "' not supported!";
      return 0;
  }
}

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator
