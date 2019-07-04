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
/**        \file  DataIdentifier_SWCL_A_DID_4711_skeleton.cc
 *        \brief  Skeleton for service 'DataIdentifier_SWCL_A_DID_4711'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton.h"
#include "ara/com/runtime.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

ara::com::internal::SkeletonIdObject DataIdentifier_SWCL_A_DID_4711Skeleton::skeleton_id_;

DataIdentifier_SWCL_A_DID_4711Skeleton::DataIdentifier_SWCL_A_DID_4711Skeleton(ara::com::InstanceIdentifier instance,
                                                                               ara::com::MethodCallProcessingMode mode)
    : ara::com::internal::SkeletonBase(instance, mode, "DataIdentifier_SWCL_A_DID_4711Skeleton") {}

void DataIdentifier_SWCL_A_DID_4711Skeleton::OfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  ara::com::internal::BindingPool::SkeletonImplContainer skeleton_implementations =
      binding_pool.OfferService(GetSkeletonId(), instance_id_, *this);
  // Make a downcast to the concrete ServiceSkeletonImplInterface, to allow the usage of events from the concrete
  // service.
  for (auto& skeleton_implementation : skeleton_implementations) {
    // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
    assert(skeleton_implementation.get()->GetSkeletonId() == GetSkeletonId());
    DataIdentifier_SWCL_A_DID_4711SkeletonImplInterface* concrete =
        dynamic_cast<DataIdentifier_SWCL_A_DID_4711SkeletonImplInterface*>(skeleton_implementation.get());
    // If downcasting fails, the factory created the wrong binding-specific skeleton-side during the OfferService in the
    // concrete binding.
    assert(concrete != nullptr);
    skeleton_implementation.release();
    impl_interfaces_.emplace_back(concrete);
  }
}

void DataIdentifier_SWCL_A_DID_4711Skeleton::StopOfferService() {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  impl_interfaces_.clear();
  binding_pool.StopOfferService(GetSkeletonId(), instance_id_);
}

namespace skeleton {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailedOnRead' ------------------------------------------- */

UDSServiceFailedOnRead::UDSServiceFailedOnRead(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailedOnRead::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnRead with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711";
}

const UDSResponseCodeType& UDSServiceFailedOnRead::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnWrite' ------------------------------------------- */

UDSServiceFailedOnWrite::UDSServiceFailedOnWrite(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnWrite::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnWrite with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711";
}

const UDSResponseCodeType& UDSServiceFailedOnWrite::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace skeleton
}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
