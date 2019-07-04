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
/**        \file  DM_IPC_proxy.cc
 *        \brief  Proxy for service 'DM_IPC'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/** Address of this member uniquely identifies the type */
ara::com::internal::ProxyIdObject DM_IPCProxy::proxy_id_;

/** StartFindService observer for the DM_IPCProxy */
ara::com::internal::ServiceUpdateObserver DM_IPCProxy::findservice_observer_;

DM_IPCProxy::DM_IPCProxy(const ara::com::HandleType& handle)
    : service_interface_{std::move(ConstructInterface(handle))}, RequestData{service_interface_.get()} {}

ara::com::FindServiceHandle DM_IPCProxy::StartFindService(ara::com::FindServiceHandler<ara::com::HandleType> handler,
                                                          ara::com::InstanceIdentifier instance) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid FindServiceHandler.");
  }
  ara::com::FindServiceHandle handle = findservice_observer_.AddObserver(GetProxyIdStatic(), instance, handler);
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StartFindService(handle, findservice_observer_);
  return handle;
}

void DM_IPCProxy::StopFindService(ara::com::FindServiceHandle handle) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StopFindService(handle);
  findservice_observer_.DeleteObserver(handle);
}

ara::com::ServiceHandleContainer<ara::com::HandleType> DM_IPCProxy::FindService(ara::com::InstanceIdentifier instance) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  return binding_pool.FindService(GetProxyIdStatic(), instance);
}

std::unique_ptr<DM_IPCProxyImplInterface> DM_IPCProxy::ConstructInterface(const ara::com::HandleType& handle) {
  ara::com::internal::ServiceProxyFactoryInterface::shared_ptr factory_interface = handle.GetInterface();
  std::unique_ptr<ara::com::internal::ProxyImplInterface> proxy_interface = std::move(factory_interface->create());

  // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
  assert(proxy_interface.get()->GetProxyId() == GetProxyIdStatic());

  // TODO(visdtr) Provide a version without dynamic_cast.
  DM_IPCProxyImplInterface* concrete = dynamic_cast<DM_IPCProxyImplInterface*>(proxy_interface.get());
  // If downcasting fails, the factory created the wrong binding-specific proxy-side.
  assert(concrete != nullptr);
  proxy_interface.release();
  // Transfer the ownership of this pointer to the service_interface, this concrete proxy object can talk to.
  return std::unique_ptr<DM_IPCProxyImplInterface>(concrete);
}

namespace proxy {
namespace application_errors {
/* ---- ApplicationError 'Failed' ------------------------------------------- */

Failed::Failed() {}

const char* Failed::what() const noexcept {
  return "ApplicationError Failed with error code 1 occurred for service interface "
         "ara::diag::service_interfaces::dm_ipc::DM_IPC";
}

}  // namespace application_errors
}  // namespace proxy
}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
