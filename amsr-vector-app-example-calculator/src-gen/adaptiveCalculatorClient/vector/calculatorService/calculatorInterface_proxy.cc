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
/**        \file  calculatorInterface_proxy.cc
 *        \brief  Proxy for service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_proxy.h"
#include "vector/calculatorService/calculatorInterface_proxy_impl_interface.h"

namespace vector {
namespace calculatorService {

/** Address of this member uniquely identifies the type */
ara::com::internal::ProxyIdObject CalculatorInterfaceProxy::proxy_id_;

/** StartFindService observer for the calculatorInterfaceProxy */
ara::com::internal::ServiceUpdateObserver CalculatorInterfaceProxy::findservice_observer_;

CalculatorInterfaceProxy::CalculatorInterfaceProxy(const ara::com::HandleType& handle)
    : service_interface_{std::move(ConstructInterface(handle))},
      subtract{service_interface_.get()},
      add{service_interface_.get()},
      divide{service_interface_.get()},
      divisionByZero{service_interface_.get()},
      divideResult{service_interface_.get()} {}

ara::com::FindServiceHandle CalculatorInterfaceProxy::StartFindService(
    ara::com::FindServiceHandler<ara::com::HandleType> handler, ara::com::InstanceIdentifier instance) {
  if (handler == nullptr) {
    throw std::runtime_error("Invalid FindServiceHandler.");
  }
  ara::com::FindServiceHandle handle = findservice_observer_.AddObserver(GetProxyIdStatic(), instance, handler);
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StartFindService(handle, findservice_observer_);
  return handle;
}

void CalculatorInterfaceProxy::StopFindService(ara::com::FindServiceHandle handle) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  binding_pool.StopFindService(handle);
  findservice_observer_.DeleteObserver(handle);
}

ara::com::ServiceHandleContainer<ara::com::HandleType> CalculatorInterfaceProxy::FindService(
    ara::com::InstanceIdentifier instance) {
  ara::com::internal::BindingPool& binding_pool = ara::com::Runtime::getInstance().GetBindingPool();
  return binding_pool.FindService(GetProxyIdStatic(), instance);
}

std::unique_ptr<CalculatorInterfaceProxyImplInterface> CalculatorInterfaceProxy::ConstructInterface(
    const ara::com::HandleType& handle) {
  ara::com::internal::ServiceProxyFactoryInterface::shared_ptr factory_interface = handle.GetInterface();
  std::unique_ptr<ara::com::internal::ProxyImplInterface> proxy_interface = std::move(factory_interface->create());

  // Check if constructed skeleton backend matches the expected SkeletonId of the frontend
  assert(proxy_interface.get()->GetProxyId() == GetProxyIdStatic());

  // TODO(visdtr) Provide a version without dynamic_cast.
  CalculatorInterfaceProxyImplInterface* concrete =
      dynamic_cast<CalculatorInterfaceProxyImplInterface*>(proxy_interface.get());
  // If downcasting fails, the factory created the wrong binding-specific proxy-side.
  assert(concrete != nullptr);
  proxy_interface.release();
  // Transfer the ownership of this pointer to the service_interface, this concrete proxy object can talk to.
  return std::unique_ptr<CalculatorInterfaceProxyImplInterface>(concrete);
}
}  // namespace calculatorService
}  // namespace vector
