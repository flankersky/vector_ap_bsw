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
/**        \file  calculatorInterface_someip_proxy_factory.cc
 *        \brief  SOME/IP proxy factory for service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_someip_proxy_factory.h"
#include "vac/language/cpp14_backport.h"

namespace vector {
namespace calculatorService {

CalculatorInterfaceSomeIpProxyFactory::CalculatorInterfaceSomeIpProxyFactory(
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding)
    : someip_binding_(someip_binding) {}

std::unique_ptr<ara::com::internal::ProxyImplInterface> CalculatorInterfaceSomeIpProxyFactory::create(
    ::someip_posix_common::someip::InstanceId instance_id) const {
  return vac::language::make_unique<CalculatorInterfaceProxySomeIpBinding>(instance_id, someip_binding_);
}

}  // namespace calculatorService
}  // namespace vector
