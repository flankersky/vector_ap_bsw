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
#include "vac/memory/optional.h"
#include "vector/calculatorService/calculatorInterface_someip_skeleton_factory.h"

namespace ara {
namespace com {
namespace someip_posix {

/** Static instantiation of a skeleton factory for the service 'calculatorInterface' */
vac::memory::optional<vector::calculatorService::CalculatorInterfaceSomeIpSkeletonFactory>
    calculatorInterface_someip_skeleton_factory_;

template <>
void AraComSomeIpBinding<::someip_posix::SomeIpPosix,
                         ::someip_posix::SomeIpPosixShutdownHandlerInterface>::Initialize() {
  // Instantiate and register a skeleton factory for the service 'calculatorInterface'.
  calculatorInterface_someip_skeleton_factory_.emplace(*this);
  skeleton_factories_.push_back(calculatorInterface_someip_skeleton_factory_.value());
}

}  // namespace someip_posix
}  // namespace com
}  // namespace ara
