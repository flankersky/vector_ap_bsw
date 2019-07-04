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
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_someip_proxy_factory.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_someip_proxy_factory.h"
#include "vac/memory/optional.h"

namespace ara {
namespace com {
namespace someip_posix {

/** Static instantiation of a proxy factory for the service 'DM_IPC' */
vac::memory::optional<ara::diag::service_interfaces::dm_ipc::DM_IPCSomeIpProxyFactory> DM_IPC_someip_proxy_factory_;
/** Static instantiation of a proxy factory for the service 'GenericUDSService' */
vac::memory::optional<ara::diag::service_interfaces::generic_uds_service::GenericUDSServiceSomeIpProxyFactory>
    GenericUDSService_someip_proxy_factory_;

template <>
void AraComSomeIpBinding<::someip_posix::SomeIpPosix,
                         ::someip_posix::SomeIpPosixShutdownHandlerInterface>::Initialize() {
  // Instantiate and register a proxy factory for the service 'DM_IPC'.
  DM_IPC_someip_proxy_factory_.emplace(*this);
  proxy_factories_.push_back(DM_IPC_someip_proxy_factory_.value());

  // Instantiate and register a proxy factory for the service 'GenericUDSService'.
  GenericUDSService_someip_proxy_factory_.emplace(*this);
  proxy_factories_.push_back(GenericUDSService_someip_proxy_factory_.value());
}

}  // namespace someip_posix
}  // namespace com
}  // namespace ara
