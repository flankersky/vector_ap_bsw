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
/**        \file  DataIdentifier_SWCL_A_DID_F190_proxy.cc
 *        \brief  Proxy for service 'DataIdentifier_SWCL_A_DID_F190'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/** Address of this member uniquely identifies the type */
ara::com::internal::ProxyIdObject DataIdentifier_SWCL_A_DID_F190Proxy::proxy_id_;

/** StartFindService observer for the DataIdentifier_SWCL_A_DID_F190Proxy */
ara::com::internal::ServiceUpdateObserver DataIdentifier_SWCL_A_DID_F190Proxy::findservice_observer_;

DataIdentifier_SWCL_A_DID_F190Proxy::DataIdentifier_SWCL_A_DID_F190Proxy(const ara::com::HandleType& handle)
    : service_interface_{std::move(ConstructInterface(handle))},
      Read{service_interface_.get()},
      Write{service_interface_.get()},
      Cancel{service_interface_.get()} {}

ara::com::FindServiceHandle DataIdentifier_SWCL_A_DID_F190Proxy::StartFindService(
    ara::com::FindServiceHandler<ara::com::HandleType> handler, ara::com::InstanceIdentifier instance) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid FindServiceHandler.");
  }
  ara::com::FindServiceHandle handle = findservice_observer_.AddObserver(GetProxyIdStatic(), instance, handler);
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StartFindService(handle, findservice_observer_);
  return handle;
}

void DataIdentifier_SWCL_A_DID_F190Proxy::StopFindService(ara::com::FindServiceHandle handle) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StopFindService(handle);
  findservice_observer_.DeleteObserver(handle);
}

ara::com::ServiceHandleContainer<ara::com::HandleType> DataIdentifier_SWCL_A_DID_F190Proxy::FindService(
    ara::com::InstanceIdentifier instance) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  return binding_pool.FindService(GetProxyIdStatic(), instance);
}

std::unique_ptr<DataIdentifier_SWCL_A_DID_F190ProxyImplInterface>
DataIdentifier_SWCL_A_DID_F190Proxy::ConstructInterface(const ara::com::HandleType& handle) {
  ara::com::internal::ServiceProxyFactoryInterface::shared_ptr factory_interface = handle.GetInterface();
  std::unique_ptr<ara::com::internal::ProxyImplInterface> proxy_interface = std::move(factory_interface->create());

  // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
  assert(proxy_interface.get()->GetProxyId() == GetProxyIdStatic());

  // TODO(visdtr) Provide a version without dynamic_cast.
  DataIdentifier_SWCL_A_DID_F190ProxyImplInterface* concrete =
      dynamic_cast<DataIdentifier_SWCL_A_DID_F190ProxyImplInterface*>(proxy_interface.get());
  // If downcasting fails, the factory created the wrong binding-specific proxy-side.
  assert(concrete != nullptr);
  proxy_interface.release();
  // Transfer the ownership of this pointer to the service_interface, this concrete proxy object can talk to.
  return std::unique_ptr<DataIdentifier_SWCL_A_DID_F190ProxyImplInterface>(concrete);
}

namespace proxy {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailedOnRead' ------------------------------------------- */

UDSServiceFailedOnRead::UDSServiceFailedOnRead(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailedOnRead::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnRead with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190";
}

const UDSResponseCodeType& UDSServiceFailedOnRead::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnWrite' ------------------------------------------- */

UDSServiceFailedOnWrite::UDSServiceFailedOnWrite(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnWrite::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnWrite with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190";
}

const UDSResponseCodeType& UDSServiceFailedOnWrite::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace proxy
}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
