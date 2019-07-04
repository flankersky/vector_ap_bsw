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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton.cc
 *        \brief  Skeleton for service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton.h"
#include "ara/com/runtime.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

ara::com::internal::SkeletonIdObject RoutineControl_SWCL_A_RID_3009Skeleton::skeleton_id_;

RoutineControl_SWCL_A_RID_3009Skeleton::RoutineControl_SWCL_A_RID_3009Skeleton(ara::com::InstanceIdentifier instance,
                                                                               ara::com::MethodCallProcessingMode mode)
    : ara::com::internal::SkeletonBase(instance, mode, "RoutineControl_SWCL_A_RID_3009Skeleton") {}

void RoutineControl_SWCL_A_RID_3009Skeleton::OfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  ara::com::internal::BindingPool::SkeletonImplContainer skeleton_implementations =
      binding_pool.OfferService(GetSkeletonId(), instance_id_, *this);
  // Make a downcast to the concrete ServiceSkeletonImplInterface, to allow the usage of events from the concrete
  // service.
  for (auto& skeleton_implementation : skeleton_implementations) {
    // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
    assert(skeleton_implementation.get()->GetSkeletonId() == GetSkeletonId());
    RoutineControl_SWCL_A_RID_3009SkeletonImplInterface* concrete =
        dynamic_cast<RoutineControl_SWCL_A_RID_3009SkeletonImplInterface*>(skeleton_implementation.get());
    // If downcasting fails, the factory created the wrong binding-specific skeleton-side during the OfferService in the
    // concrete binding.
    assert(concrete != nullptr);
    skeleton_implementation.release();
    impl_interfaces_.emplace_back(concrete);
  }
}

void RoutineControl_SWCL_A_RID_3009Skeleton::StopOfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  impl_interfaces_.clear();
  binding_pool.StopOfferService(GetSkeletonId(), instance_id_);
}

namespace skeleton {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailedOnStart' ------------------------------------------- */

UDSServiceFailedOnStart::UDSServiceFailedOnStart(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnStart::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnStart with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnStart::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnStop' ------------------------------------------- */

UDSServiceFailedOnStop::UDSServiceFailedOnStop(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailedOnStop::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnStop with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnStop::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnRequestResults' ------------------------------------------- */

UDSServiceFailedOnRequestResults::UDSServiceFailedOnRequestResults(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnRequestResults::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnRequestResults with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnRequestResults::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace skeleton
}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
