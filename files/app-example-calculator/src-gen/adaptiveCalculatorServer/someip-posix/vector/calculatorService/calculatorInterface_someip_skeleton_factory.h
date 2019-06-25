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
/**        \file  calculatorInterface_someip_skeleton_factory.h
 *        \brief  SOME/IP skeleton factory for service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_SKELETON_FACTORY_H_
#define ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_SKELETON_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_factory_interface.h"
#include "vector/calculatorService/calculatorInterface_skeleton.h"
#include "vector/calculatorService/calculatorInterface_skeleton_someip_binding.h"

namespace vector {
namespace calculatorService {

/**
 * \brief Factory class to instantiate the SOME/IP backend for service 'calculatorInterface'
 * \remark generated
 */
class CalculatorInterfaceSomeIpSkeletonFactory : public ara::com::someip_posix::AraComSomeIpSkeletonFactoryInterface {
 public:
  /**
   * \brief Constructor
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit CalculatorInterfaceSomeIpSkeletonFactory(
      ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Getter for the SOME/IP service ID.
   * \return The SOME/IP Service ID of the produced SkeletonImplInterface instances.
   */
  ::someip_posix_common::someip::ServiceId GetSomeIpServiceId() const override {
    return CalculatorInterfaceSkeletonSomeIpBinding::kServiceId;
  }

  /**
   * \brief Getter for the SkeletonId for which this Factory will produce SkeletonImplInterface instances.
   * \return The ara::com::internal::SkeletonId of the produced SkeletonImplInterface instances.
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override {
    return CalculatorInterfaceSkeleton::GetSkeletonIdStatic();
  }

  /**
   * \brief Factory method to create SOME/IP binding backend for service 'calculatorInterface'
   * \param instance_id The SOME/IP Instance ID to use for the created SkeletonImplInterface.
   * \param skeleton The related frontend skeleton implementation
   * \return A ProxyImplInterface for service 'calculatorInterface' for the given instance ID.
   */
  std::unique_ptr<ara::com::internal::SkeletonImplInterface> create(
      ::someip_posix_common::someip::InstanceId instance_id,
      ara::com::internal::SkeletonInterface& skeleton) const override;

 private:
  /** Reference to the SOME/IP transport binding which will be passed to the instantiated skeleton implementations */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SOMEIP_SKELETON_FACTORY_H_
