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
/**        \file  DataIdentifier_SWCL_A_DID_F190_someip_skeleton_factory.cc
 *        \brief  SOME/IP skeleton factory for service 'DataIdentifier_SWCL_A_DID_F190'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_someip_skeleton_factory.h"
#include "vac/language/cpp14_backport.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

DataIdentifier_SWCL_A_DID_F190SomeIpSkeletonFactory::DataIdentifier_SWCL_A_DID_F190SomeIpSkeletonFactory(
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::SkeletonImplInterface> DataIdentifier_SWCL_A_DID_F190SomeIpSkeletonFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id, ara::com::internal::SkeletonInterface& skeleton) const {
  // Downcast the SkeletonInterface to the concrete DataIdentifier_SWCL_A_DID_F190Skeleton
  // TODO(visswa) Provide a version without dynamic_cast.
  DataIdentifier_SWCL_A_DID_F190Skeleton* concrete = dynamic_cast<DataIdentifier_SWCL_A_DID_F190Skeleton*>(&skeleton);
  // If downcasting fails, the factory created the wrong binding-specific skeleton-side.
  assert(concrete != nullptr);

  return vac::language::make_unique<DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding>(instance_id, someip_binding_,
                                                                                         *concrete);
}

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
