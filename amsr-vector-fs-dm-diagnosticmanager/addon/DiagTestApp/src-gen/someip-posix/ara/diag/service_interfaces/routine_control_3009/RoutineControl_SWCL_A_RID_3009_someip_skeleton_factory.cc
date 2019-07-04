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
/**        \file  RoutineControl_SWCL_A_RID_3009_someip_skeleton_factory.cc
 *        \brief  SOME/IP skeleton factory for service 'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_someip_skeleton_factory.h"
#include "vac/language/cpp14_backport.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

RoutineControl_SWCL_A_RID_3009SomeIpSkeletonFactory::RoutineControl_SWCL_A_RID_3009SomeIpSkeletonFactory(
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::SkeletonImplInterface> RoutineControl_SWCL_A_RID_3009SomeIpSkeletonFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id, ara::com::internal::SkeletonInterface& skeleton) const {
  // Downcast the SkeletonInterface to the concrete RoutineControl_SWCL_A_RID_3009Skeleton
  // TODO(visswa) Provide a version without dynamic_cast.
  RoutineControl_SWCL_A_RID_3009Skeleton* concrete = dynamic_cast<RoutineControl_SWCL_A_RID_3009Skeleton*>(&skeleton);
  // If downcasting fails, the factory created the wrong binding-specific skeleton-side.
  assert(concrete != nullptr);

  return vac::language::make_unique<RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding>(instance_id, someip_binding_,
                                                                                         *concrete);
}

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
