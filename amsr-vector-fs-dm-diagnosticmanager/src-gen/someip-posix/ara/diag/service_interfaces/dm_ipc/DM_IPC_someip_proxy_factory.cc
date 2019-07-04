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
/**        \file  DM_IPC_someip_proxy_factory.cc
 *        \brief  SOME/IP proxy factory for service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_someip_proxy_factory.h"
#include "vac/language/cpp14_backport.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

DM_IPCSomeIpProxyFactory::DM_IPCSomeIpProxyFactory(ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::ProxyImplInterface> DM_IPCSomeIpProxyFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id) const {
  return vac::language::make_unique<DM_IPCProxySomeIpBinding>(instance_id, someip_binding_);
}

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
