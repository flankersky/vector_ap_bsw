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
/**        \file  DataIdentifier_SWCL_A_DID_4711_someip_skeleton_factory.h
 *        \brief  SOME/IP skeleton factory for service 'DataIdentifier_SWCL_A_DID_4711'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SOMEIP_SKELETON_FACTORY_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SOMEIP_SKELETON_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_factory_interface.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_someip_binding.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

/**
 * \brief Factory class to instantiate the SOME/IP backend for service 'DataIdentifier_SWCL_A_DID_4711'
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711SomeIpSkeletonFactory
    : public ara::com::someip_posix::AraComSomeIpSkeletonFactoryInterface {
 public:
  /**
   * \brief Constructor
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit DataIdentifier_SWCL_A_DID_4711SomeIpSkeletonFactory(
      ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Getter for the SOME/IP service ID.
   * \return The SOME/IP Service ID of the produced SkeletonImplInterface instances.
   */
  ::someip_posix_common::someip::ServiceId GetSomeIpServiceId() const override {
    return DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kServiceId;
  }

  /**
   * \brief Getter for the SkeletonId for which this Factory will produce SkeletonImplInterface instances.
   * \return The ara::com::internal::SkeletonId of the produced SkeletonImplInterface instances.
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override {
    return DataIdentifier_SWCL_A_DID_4711Skeleton::GetSkeletonIdStatic();
  }

  /**
   * \brief Factory method to create SOME/IP binding backend for service 'DataIdentifier_SWCL_A_DID_4711'
   * \param instance_id The SOME/IP Instance ID to use for the created SkeletonImplInterface.
   * \param skeleton The related frontend skeleton implementation
   * \return A ProxyImplInterface for service 'DataIdentifier_SWCL_A_DID_4711' for the given instance ID.
   */
  std::unique_ptr<ara::com::internal::SkeletonImplInterface> create(
      ::someip_posix_common::someip::InstanceId instance_id,
      ara::com::internal::SkeletonInterface& skeleton) const override;

 private:
  /** Reference to the SOME/IP transport binding which will be passed to the instantiated skeleton implementations */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;
};

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SOMEIP_SKELETON_FACTORY_H_
