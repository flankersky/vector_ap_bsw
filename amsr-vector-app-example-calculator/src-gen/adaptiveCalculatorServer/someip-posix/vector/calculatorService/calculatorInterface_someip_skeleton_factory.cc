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
/**        \file  calculatorInterface_someip_skeleton_factory.cc
 *        \brief  SOME/IP skeleton factory for service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_someip_skeleton_factory.h"
#include "vac/language/cpp14_backport.h"

namespace vector {
namespace calculatorService {

CalculatorInterfaceSomeIpSkeletonFactory::CalculatorInterfaceSomeIpSkeletonFactory(
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::SkeletonImplInterface> CalculatorInterfaceSomeIpSkeletonFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id, ara::com::internal::SkeletonInterface& skeleton) const {
  // Downcast the SkeletonInterface to the concrete calculatorInterfaceSkeleton
  // TODO(visswa) Provide a version without dynamic_cast.
  CalculatorInterfaceSkeleton* concrete = dynamic_cast<CalculatorInterfaceSkeleton*>(&skeleton);
  // If downcasting fails, the factory created the wrong binding-specific skeleton-side.
  assert(concrete != nullptr);

  return vac::language::make_unique<CalculatorInterfaceSkeletonSomeIpBinding>(instance_id, someip_binding_, *concrete);
}

}  // namespace calculatorService
}  // namespace vector
