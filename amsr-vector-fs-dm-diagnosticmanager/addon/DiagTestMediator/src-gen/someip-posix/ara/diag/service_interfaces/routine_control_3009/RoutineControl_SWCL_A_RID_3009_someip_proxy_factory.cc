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
/**        \file  RoutineControl_SWCL_A_RID_3009_someip_proxy_factory.cc
 *        \brief  SOME/IP proxy factory for service 'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_someip_proxy_factory.h"
#include "vac/language/cpp14_backport.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

RoutineControl_SWCL_A_RID_3009SomeIpProxyFactory::RoutineControl_SWCL_A_RID_3009SomeIpProxyFactory(
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::ProxyImplInterface> RoutineControl_SWCL_A_RID_3009SomeIpProxyFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id) const {
  return vac::language::make_unique<RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding>(instance_id, someip_binding_);
}

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
