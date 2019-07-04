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
/**        \file  calculatorInterface_someip_proxy_factory.h
 *        \brief  SOME/IP proxy factory for service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_PROXY_FACTORY_H_
#define ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_PROXY_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_factory_interface.h"
#include "vector/calculatorService/calculatorInterface_proxy.h"
#include "vector/calculatorService/calculatorInterface_proxy_someip_binding.h"

namespace vector {
namespace calculatorService {

/**
 * \brief Factory class to instantiate the SOME/IP backend for service 'calculatorInterface'
 * \remark generated
 */
class CalculatorInterfaceSomeIpProxyFactory : public ara::com::someip_posix::AraComSomeIpProxyFactoryInterface {
 public:
  /**
   * \brief Constructor
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit CalculatorInterfaceSomeIpProxyFactory(ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Getter for the SOME/IP service ID.
   * \return The SOME/IP Service ID of the produced ProxyImplInterface instances.
   */
  ::someip_posix_common::someip::ServiceId GetSomeIpServiceId() const override {
    return CalculatorInterfaceProxySomeIpBinding::kServiceId;
  }

  /**
   * \brief Getter for the ProxyId for which this Factory will produce ProxyImplInterface instances.
   * \return The ara::com::internal::ProxyId of the produced ProxyImplInterface instances.
   */
  ara::com::internal::ProxyId GetProxyId() const override { return CalculatorInterfaceProxy::GetProxyIdStatic(); }

  /**
   * \brief Factory method to create SOME/IP binding backend for service 'calculatorInterface'
   * \param instance_id The SOME/IP Instance ID to use for the created ProxyImplInterface.
   * \return A ProxyImplInterface for service 'calculatorInterface' for the given instance ID.
   */
  std::unique_ptr<ara::com::internal::ProxyImplInterface> create(
      ::someip_posix_common::someip::InstanceId instance_id) const override;

 private:
  /** Reference to the SOME/IP transport binding which will be passed to the instantiated proxy implementations */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_PROXY_FACTORY_H_
