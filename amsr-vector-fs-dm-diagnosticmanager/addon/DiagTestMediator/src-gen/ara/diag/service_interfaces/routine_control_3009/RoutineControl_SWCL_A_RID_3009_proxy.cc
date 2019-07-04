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
/**        \file  RoutineControl_SWCL_A_RID_3009_proxy.cc
 *        \brief  Proxy for service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/** Address of this member uniquely identifies the type */
ara::com::internal::ProxyIdObject RoutineControl_SWCL_A_RID_3009Proxy::proxy_id_;

/** StartFindService observer for the RoutineControl_SWCL_A_RID_3009Proxy */
ara::com::internal::ServiceUpdateObserver RoutineControl_SWCL_A_RID_3009Proxy::findservice_observer_;

RoutineControl_SWCL_A_RID_3009Proxy::RoutineControl_SWCL_A_RID_3009Proxy(const ara::com::HandleType& handle)
    : service_interface_{std::move(ConstructInterface(handle))},
      Start{service_interface_.get()},
      Stop{service_interface_.get()},
      RequestResults{service_interface_.get()} {}

ara::com::FindServiceHandle RoutineControl_SWCL_A_RID_3009Proxy::StartFindService(
    ara::com::FindServiceHandler<ara::com::HandleType> handler, ara::com::InstanceIdentifier instance) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid FindServiceHandler.");
  }
  ara::com::FindServiceHandle handle = findservice_observer_.AddObserver(GetProxyIdStatic(), instance, handler);
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StartFindService(handle, findservice_observer_);
  return handle;
}

void RoutineControl_SWCL_A_RID_3009Proxy::StopFindService(ara::com::FindServiceHandle handle) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StopFindService(handle);
  findservice_observer_.DeleteObserver(handle);
}

ara::com::ServiceHandleContainer<ara::com::HandleType> RoutineControl_SWCL_A_RID_3009Proxy::FindService(
    ara::com::InstanceIdentifier instance) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  return binding_pool.FindService(GetProxyIdStatic(), instance);
}

std::unique_ptr<RoutineControl_SWCL_A_RID_3009ProxyImplInterface>
RoutineControl_SWCL_A_RID_3009Proxy::ConstructInterface(const ara::com::HandleType& handle) {
  ara::com::internal::ServiceProxyFactoryInterface::shared_ptr factory_interface = handle.GetInterface();
  std::unique_ptr<ara::com::internal::ProxyImplInterface> proxy_interface = std::move(factory_interface->create());

  // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
  assert(proxy_interface.get()->GetProxyId() == GetProxyIdStatic());

  // TODO(visdtr) Provide a version without dynamic_cast.
  RoutineControl_SWCL_A_RID_3009ProxyImplInterface* concrete =
      dynamic_cast<RoutineControl_SWCL_A_RID_3009ProxyImplInterface*>(proxy_interface.get());
  // If downcasting fails, the factory created the wrong binding-specific proxy-side.
  assert(concrete != nullptr);
  proxy_interface.release();
  // Transfer the ownership of this pointer to the service_interface, this concrete proxy object can talk to.
  return std::unique_ptr<RoutineControl_SWCL_A_RID_3009ProxyImplInterface>(concrete);
}

namespace proxy {
namespace application_errors {
/* ---- ApplicationError 'UDSServiceFailedOnStart' ------------------------------------------- */

UDSServiceFailedOnStart::UDSServiceFailedOnStart(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnStart::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnStart with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnStart::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnStop' ------------------------------------------- */

UDSServiceFailedOnStop::UDSServiceFailedOnStop(const UDSResponseCodeType& errorContext) : errorContext_{errorContext} {}

const char* UDSServiceFailedOnStop::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnStop with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnStop::geterrorContext() const { return errorContext_; }

/* ---- ApplicationError 'UDSServiceFailedOnRequestResults' ------------------------------------------- */

UDSServiceFailedOnRequestResults::UDSServiceFailedOnRequestResults(const UDSResponseCodeType& errorContext)
    : errorContext_{errorContext} {}

const char* UDSServiceFailedOnRequestResults::what() const noexcept {
  return "ApplicationError UDSServiceFailedOnRequestResults with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009";
}

const UDSResponseCodeType& UDSServiceFailedOnRequestResults::geterrorContext() const { return errorContext_; }

}  // namespace application_errors
}  // namespace proxy
}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
