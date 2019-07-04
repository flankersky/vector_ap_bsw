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
/**        \file  aracom_someip_binding_init.cc
 *        \brief  ara::com SOME/IP binding initialization
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara-someip-posix/aracom_someip_binding.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_someip_proxy_factory.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_someip_proxy_factory.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_someip_skeleton_factory.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_someip_proxy_factory.h"
#include "vac/memory/optional.h"

namespace ara {
namespace com {
namespace someip_posix {

/** Static instantiation of a skeleton factory for the service 'DM_IPC' */
vac::memory::optional<ara::diag::service_interfaces::dm_ipc::DM_IPCSomeIpSkeletonFactory>
    DM_IPC_someip_skeleton_factory_;

/** Static instantiation of a proxy factory for the service 'DataIdentifier_SWCL_A_DID_4711' */
vac::memory::optional<ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711SomeIpProxyFactory>
    DataIdentifier_SWCL_A_DID_4711_someip_proxy_factory_;
/** Static instantiation of a proxy factory for the service 'DataIdentifier_SWCL_A_DID_F190' */
vac::memory::optional<
    ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190SomeIpProxyFactory>
    DataIdentifier_SWCL_A_DID_F190_someip_proxy_factory_;
/** Static instantiation of a proxy factory for the service 'RoutineControl_SWCL_A_RID_3009' */
vac::memory::optional<
    ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009SomeIpProxyFactory>
    RoutineControl_SWCL_A_RID_3009_someip_proxy_factory_;

template <>
void AraComSomeIpBinding<::someip_posix::SomeIpPosix,
                         ::someip_posix::SomeIpPosixShutdownHandlerInterface>::Initialize() {
  // Instantiate and register a skeleton factory for the service 'DM_IPC'.
  DM_IPC_someip_skeleton_factory_.emplace(*this);
  skeleton_factories_.push_back(DM_IPC_someip_skeleton_factory_.value());

  // Instantiate and register a proxy factory for the service 'DataIdentifier_SWCL_A_DID_4711'.
  DataIdentifier_SWCL_A_DID_4711_someip_proxy_factory_.emplace(*this);
  proxy_factories_.push_back(DataIdentifier_SWCL_A_DID_4711_someip_proxy_factory_.value());

  // Instantiate and register a proxy factory for the service 'DataIdentifier_SWCL_A_DID_F190'.
  DataIdentifier_SWCL_A_DID_F190_someip_proxy_factory_.emplace(*this);
  proxy_factories_.push_back(DataIdentifier_SWCL_A_DID_F190_someip_proxy_factory_.value());

  // Instantiate and register a proxy factory for the service 'RoutineControl_SWCL_A_RID_3009'.
  RoutineControl_SWCL_A_RID_3009_someip_proxy_factory_.emplace(*this);
  proxy_factories_.push_back(RoutineControl_SWCL_A_RID_3009_someip_proxy_factory_.value());
}

}  // namespace someip_posix
}  // namespace com
}  // namespace ara
