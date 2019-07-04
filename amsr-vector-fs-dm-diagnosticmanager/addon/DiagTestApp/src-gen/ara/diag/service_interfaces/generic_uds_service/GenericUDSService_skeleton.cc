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
/**        \file  GenericUDSService_skeleton.cc
 *        \brief  Skeleton for service 'GenericUDSService'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton.h"
#include "ara/com/runtime.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

ara::com::internal::SkeletonIdObject GenericUDSServiceSkeleton::skeleton_id_;

GenericUDSServiceSkeleton::GenericUDSServiceSkeleton(ara::com::InstanceIdentifier instance,
                                                     ara::com::MethodCallProcessingMode mode)
    : ara::com::internal::SkeletonBase(instance, mode, "GenericUDSServiceSkeleton") {}

void GenericUDSServiceSkeleton::OfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  ara::com::internal::BindingPool::SkeletonImplContainer skeleton_implementations =
      binding_pool.OfferService(GetSkeletonId(), instance_id_, *this);
  // Make a downcast to the concrete ServiceSkeletonImplInterface, to allow the usage of events from the concrete
  // service.
  for (auto& skeleton_implementation : skeleton_implementations) {
    // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
    assert(skeleton_implementation.get()->GetSkeletonId() == GetSkeletonId());
    GenericUDSServiceSkeletonImplInterface* concrete =
        dynamic_cast<GenericUDSServiceSkeletonImplInterface*>(skeleton_implementation.get());
    // If downcasting fails, the factory created the wrong binding-specific skeleton-side during the OfferService in the
    // concrete binding.
    assert(concrete != nullptr);
    skeleton_implementation.release();
    impl_interfaces_.emplace_back(concrete);
  }
}

void GenericUDSServiceSkeleton::StopOfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  impl_interfaces_.clear();
  binding_pool.StopOfferService(GetSkeletonId(), instance_id_);
}

namespace skeleton {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailed' ------------------------------------------- */

UDSServiceFailed::UDSServiceFailed(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailed::what() const noexcept {
  return "ApplicationError UDSServiceFailed with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::generic_uds_service::GenericUDSService";
}

const UDSResponseCodeType& UDSServiceFailed::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace skeleton
}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
