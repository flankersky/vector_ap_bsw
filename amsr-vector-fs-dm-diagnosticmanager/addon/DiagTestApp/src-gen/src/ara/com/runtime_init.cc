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
/**        \file  runtime_init.cc
 *        \brief  Runtime initialization with all deployment bindings
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/runtime.h"
#include "ara-someip-posix/aracom_someip_binding.h"
#include "ara/com/configuration/configuration.h"
#include "vac/memory/optional.h"

namespace ara {
namespace com {

/** Static instantiation of binding 'SomeIp' */
vac::memory::optional<ara::com::someip_posix::AraComSomeIpBinding<::someip_posix::SomeIpPosix,
                                                                  ::someip_posix::SomeIpPosixShutdownHandlerInterface>>
    aracom_someip_binding_;

void Runtime::InitializeBindings() {
  // Initialize binding 'SomeIp'
  aracom_someip_binding_.emplace(config_);
  aracom_someip_binding_.value().Initialize();
  binding_pool_->AddBinding(aracom_someip_binding_.value());
}

void Runtime::DeinitializeBindings() {
  // Deinitialize binding 'SomeIp'
  binding_pool_->RemoveBinding(aracom_someip_binding_.value());
  aracom_someip_binding_.reset();
}

}  // namespace com
}  // namespace ara
