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
/**        \file  GenericUDSService_someip_proxy_factory.h
 *        \brief  SOME/IP proxy factory for service 'GenericUDSService'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SOMEIP_PROXY_FACTORY_H_
#define SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SOMEIP_PROXY_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_factory_interface.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy_someip_binding.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/**
 * \brief Factory class to instantiate the SOME/IP backend for service 'GenericUDSService'
 * \remark generated
 */
class GenericUDSServiceSomeIpProxyFactory : public ara::com::someip_posix::AraComSomeIpProxyFactoryInterface {
 public:
  /**
   * \brief Constructor
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit GenericUDSServiceSomeIpProxyFactory(ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Getter for the SOME/IP service ID.
   * \return The SOME/IP Service ID of the produced ProxyImplInterface instances.
   */
  ::someip_posix_common::someip::ServiceId GetSomeIpServiceId() const override {
    return GenericUDSServiceProxySomeIpBinding::kServiceId;
  }

  /**
   * \brief Getter for the ProxyId for which this Factory will produce ProxyImplInterface instances.
   * \return The ara::com::internal::ProxyId of the produced ProxyImplInterface instances.
   */
  ara::com::internal::ProxyId GetProxyId() const override { return GenericUDSServiceProxy::GetProxyIdStatic(); }

  /**
   * \brief Factory method to create SOME/IP binding backend for service 'GenericUDSService'
   * \param instance_id The SOME/IP Instance ID to use for the created ProxyImplInterface.
   * \return A ProxyImplInterface for service 'GenericUDSService' for the given instance ID.
   */
  std::unique_ptr<ara::com::internal::ProxyImplInterface> create(
      ::someip_posix_common::someip::InstanceId instance_id) const override;

 private:
  /** Reference to the SOME/IP transport binding which will be passed to the instantiated proxy implementations */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;
};

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SOMEIP_PROXY_FACTORY_H_
