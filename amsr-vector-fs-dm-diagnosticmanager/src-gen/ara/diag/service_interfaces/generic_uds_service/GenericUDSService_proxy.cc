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
/**        \file  GenericUDSService_proxy.cc
 *        \brief  Proxy for service 'GenericUDSService'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/** Address of this member uniquely identifies the type */
ara::com::internal::ProxyIdObject GenericUDSServiceProxy::proxy_id_;

/** StartFindService observer for the GenericUDSServiceProxy */
ara::com::internal::ServiceUpdateObserver GenericUDSServiceProxy::findservice_observer_;

GenericUDSServiceProxy::GenericUDSServiceProxy(const ara::com::HandleType& handle)
    : service_interface_{std::move(ConstructInterface(handle))}, Service{service_interface_.get()} {}

ara::com::FindServiceHandle GenericUDSServiceProxy::StartFindService(
    ara::com::FindServiceHandler<ara::com::HandleType> handler, ara::com::InstanceIdentifier instance) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid FindServiceHandler.");
  }
  ara::com::FindServiceHandle handle = findservice_observer_.AddObserver(GetProxyIdStatic(), instance, handler);
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StartFindService(handle, findservice_observer_);
  return handle;
}

void GenericUDSServiceProxy::StopFindService(ara::com::FindServiceHandle handle) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StopFindService(handle);
  findservice_observer_.DeleteObserver(handle);
}

ara::com::ServiceHandleContainer<ara::com::HandleType> GenericUDSServiceProxy::FindService(
    ara::com::InstanceIdentifier instance) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  return binding_pool.FindService(GetProxyIdStatic(), instance);
}

std::unique_ptr<GenericUDSServiceProxyImplInterface> GenericUDSServiceProxy::ConstructInterface(
    const ara::com::HandleType& handle) {
  ara::com::internal::ServiceProxyFactoryInterface::shared_ptr factory_interface = handle.GetInterface();
  std::unique_ptr<ara::com::internal::ProxyImplInterface> proxy_interface = std::move(factory_interface->create());

  // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
  assert(proxy_interface.get()->GetProxyId() == GetProxyIdStatic());

  // TODO(visdtr) Provide a version without dynamic_cast.
  GenericUDSServiceProxyImplInterface* concrete =
      dynamic_cast<GenericUDSServiceProxyImplInterface*>(proxy_interface.get());
  // If downcasting fails, the factory created the wrong binding-specific proxy-side.
  assert(concrete != nullptr);
  proxy_interface.release();
  // Transfer the ownership of this pointer to the service_interface, this concrete proxy object can talk to.
  return std::unique_ptr<GenericUDSServiceProxyImplInterface>(concrete);
}

namespace proxy {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailed' ------------------------------------------- */

UDSServiceFailed::UDSServiceFailed(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailed::what() const noexcept {
  return "ApplicationError UDSServiceFailed with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::generic_uds_service::GenericUDSService";
}

const UDSResponseCodeType& UDSServiceFailed::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace proxy
}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
