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
/**        \file  calculatorInterface_proxy.h
 *        \brief  Proxy for service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_H_
#define ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/com/handle_type.h"
#include "ara/com/internal/method.h"
#include "ara/com/internal/proxy_event.h"
#include "ara/com/internal/proxy_field.h"
#include "ara/com/internal/service_update_observer.h"
#include "ara/com/runtime.h"
#include "vector/calculatorService/calculatorInterface_proxy_impl_interface.h"

namespace vector {
namespace calculatorService {
namespace proxy {
namespace methods {

/**
 * \brief Type-alias for service method 'subtract', that is part of the proxy.
 */
using MethodSubtract = ara::com::internal::MethodParameters<uint32&, uint32&>::ProxyMethod<
    CalculatorInterfaceProxyImplInterface, proxy::methods::Subtract,
    &CalculatorInterfaceProxyImplInterface::MethodSubtract>;

/**
 * \brief Type-alias for service method 'add', that is part of the proxy.
 */
using MethodAdd = ara::com::internal::MethodParameters<uint32&, uint32&>::ProxyMethod<
    CalculatorInterfaceProxyImplInterface, proxy::methods::Add, &CalculatorInterfaceProxyImplInterface::MethodAdd>;

/**
 * \brief Type-alias for service method 'divide', that is part of the proxy.
 */
using MethodDivide = ara::com::internal::MethodParameters<uint32&, uint32&>::ProxyMethod<
    CalculatorInterfaceProxyImplInterface, proxy::methods::Divide,
    &CalculatorInterfaceProxyImplInterface::MethodDivide>;

}  // namespace methods

namespace events {

/**
 * \brief Type-alias for service event 'divisionByZero', that is part of the proxy.
 */
using EventDivisionByZero =
    ara::com::internal::ProxyEvent<CalculatorInterfaceProxyImplInterface, vector::calculatorService::boolean,
                                   ara::com::internal::ProxyImplEvent<vector::calculatorService::boolean>,
                                   &CalculatorInterfaceProxyImplInterface::GetEventManagerDivisionByZero>;

}  // namespace events

namespace fields {

/* ---- Type-aliases for field 'divideResult' ---- */

/**
 * \brief Type-alias for the notifier of service field 'divideResult'.
 */
using FieldNotifierDivideResult =
    ara::com::internal::ProxyEvent<CalculatorInterfaceProxyImplInterface, vector::calculatorService::uint32,
                                   ara::com::internal::ProxyImplEvent<vector::calculatorService::uint32>,
                                   &CalculatorInterfaceProxyImplInterface::GetFieldNotifierDivideResult>;
/**
 * \brief Type-alias for the notifier configuration of field 'divideResult'.
 */
using FieldNotifierConfigDivideResult = ara::com::internal::HasNotifier<true, FieldNotifierDivideResult>;
/**
 * \brief Type-alias for the getter method of service field 'divideResult'.
 */
using FieldMethodGetDivideResult = ara::com::internal::MethodParameters<void>::ProxyMethod<
    CalculatorInterfaceProxyImplInterface, proxy::fields::DivideResult,
    &CalculatorInterfaceProxyImplInterface::FieldDivideResultMethodGet>;

/**
 * \brief Type-alias for the getter configuration of field 'divideResult'.
 */
using FieldGetterConfigDivideResult = ara::com::internal::HasGetter<true, FieldMethodGetDivideResult>;

/**
 * \brief Type-alias for the setter method of service field 'divideResult'.
 */
using FieldMethodSetDivideResult =
    ara::com::internal::MethodParameters<vector::calculatorService::uint32&>::ProxyMethod<
        CalculatorInterfaceProxyImplInterface, proxy::fields::DivideResult,
        &CalculatorInterfaceProxyImplInterface::FieldDivideResultMethodSet>;
/**
 * \brief Type-alias for the setter configuration of field 'divideResult'.
 */
using FieldSetterConfigDivideResult = ara::com::internal::HasSetter<true, FieldMethodSetDivideResult>;

/**
 * \brief Type-alias for the parameterization of field 'divideResult'.
 */
using FieldConfigDivideResult =
    ara::com::internal::FieldParameters<CalculatorInterfaceProxyImplInterface, FieldNotifierConfigDivideResult,
                                        FieldGetterConfigDivideResult, FieldSetterConfigDivideResult>;

/**
 * \brief Type-alias for the field 'divideResult'.
 */
using FieldDivideResult = ara::com::internal::ProxyField<vector::calculatorService::uint32, FieldConfigDivideResult>;

}  // namespace fields

}  // namespace proxy

/**
 * \brief Proxy class for the service 'calculatorInterface'.
 * \remark generated
 */
class CalculatorInterfaceProxy {
 public:
  /**
   * \brief Constructor of calculatorInterfaceProxy. The communication management shall provide a constructor for each
   * specific ServiceProxy class taking a handle returned by any FindService method of the ServiceProxy class to get a
   * valid ServiceProxy based on the handles returned by FindService.
   * \param handle The identification of the service the proxy should represent. Handle are generated by synchronous
   * calculatorInterfaceProxy::FindService or asynchronous calculatorInterfaceProxy::StartFindService.
   */
  explicit CalculatorInterfaceProxy(const ara::com::HandleType& handle);

  /**
   * \brief The application developer must ensure, that any existing FindService jobs must be terminated in the
   * bindings for this ServiceProxy and he ensures that all resources are freed!
   * Calling something in the binding-pool within the destructor may lead to runtime exceptions, because the binding
   * pool is not part of the unit-tests and the runtime could also be shutdown already.
   */
  virtual ~CalculatorInterfaceProxy() = default;

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'calculatorInterface'.
   * \return Unique proxy ID
   */
  constexpr static ara::com::internal::ProxyId GetProxyIdStatic() { return &proxy_id_; }

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'calculatorInterface'.
   * \return Unique proxy ID
   */
  ara::com::internal::ProxyId GetProxyId() const { return GetProxyIdStatic(); }

  /**
   * \brief Start an asynchronous FindService notification about service updates.
   * \param handler Gets called upon detection of a matching service
   * \param instance If no instance is specified any instance of the service matches.
   * \return As result a FindServiceHandle for this search/find request is returned,
   * which is needed to stop the service availability monitoring and related firing of the given handler.
   */
  static ara::com::FindServiceHandle StartFindService(
      ara::com::FindServiceHandler<ara::com::HandleType> handler,
      ara::com::InstanceIdentifier instance = ara::com::InstanceIdentifier::Any);

  /**
   * \brief To stop receiving further notifications the ServiceProxy class shall provide a StopFindService method.
   * \param handle The FindServiceHandle returned by the FindService method with handler registration has to be provided
   * as input parameter.
   */
  static void StopFindService(ara::com::FindServiceHandle handle);

  /**
   * \brief Opposed to StartFindService(handler, instance) this version is a "one-shot" find request, which is
   * synchronous, i.e. it returns after the find has been done and a result list of matching service instances is
   * available. This will return all available service instances, that match the criteria.
   * \param instance The instance to find - wildcard of finding any instance is possible.
   * \return handles, that match the criteria (instance identifier) and to create a service proxy instance.
   */
  static ara::com::ServiceHandleContainer<ara::com::HandleType> FindService(
      ara::com::InstanceIdentifier instance = ara::com::InstanceIdentifier::Any);

  /**
   * \brief Get the calculatorInterfaceProxyImplInterface of this proxy to get access to the binding-specific
   * implementation
   * for methods, events and fields.
   * \return a reference to the binding-related implementation, which realizes the interface for
   * method/event/field requests.
   */
  CalculatorInterfaceProxyImplInterface& GetServiceProxyImplInterface() { return *service_interface_.get(); }

 private:
  /** Proxy type identifier. Address of this member uniquely identifies the proxy type */
  static ara::com::internal::ProxyIdObject proxy_id_;

  /**
   * \brief Helper method to construct the binding-related calculatorInterfaceProxyImplInterface
   * and pass the pointer to the attribute service_interface_. Called by the ctor of this class.
   * \param handle To trigger the factory to build a backend.
   * \return The interface, that is stored within the constructed proxy object.
   */
  static std::unique_ptr<CalculatorInterfaceProxyImplInterface> ConstructInterface(const ara::com::HandleType& handle);

  /**
   * \brief Manages the incoming service updates (offers and stopped offers) after StartFindService.
   * \details This is a static object, because a StartFindService call is not bound to a concrete ServiceProxy
   * object but static.
   */
  static ara::com::internal::ServiceUpdateObserver findservice_observer_;

  /** ServiceProxyImplInterface of this proxy to access the implementation */
  std::unique_ptr<CalculatorInterfaceProxyImplInterface> service_interface_;

 public:
  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Service method 'subtract' as an object of an instantiated proxy.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  proxy::methods::MethodSubtract subtract;

  /**
   * \brief Service method 'add' as an object of an instantiated proxy.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  proxy::methods::MethodAdd add;

  /**
   * \brief Service method 'divide' as an object of an instantiated proxy.
   * \param divident IN parameter of type uint32
   * \param divisor IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  proxy::methods::MethodDivide divide;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Event 'DivisionByZero' as an object of an instantiated proxy.
   * \param data Event sample of type boolean (boolean has the value space required to support the mathematical concept
   * of binary-valued logic: {true, false}.)
   * \return A proxy event object supporting event operations (samples, subscribe/unsubscribe, ...)
   */
  proxy::events::EventDivisionByZero divisionByZero;

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Field 'divideResult' as an object of an instantiated proxy.
   * \param value Field value of type uint32 (uint32 represents integers with a minimum value of 0 and a maximum value
   * of 4294967295. The order-relation on uint32 is: x < y if y - x is positive. uint32 has a lexical representation
   * consisting of a finite-length sequence of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.)
   * \return A proxy field object supporting field operations (Get, Set, event operations)
   */
  proxy::fields::FieldDivideResult divideResult;
};  // class CalculatorInterfaceProxy

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_H_
