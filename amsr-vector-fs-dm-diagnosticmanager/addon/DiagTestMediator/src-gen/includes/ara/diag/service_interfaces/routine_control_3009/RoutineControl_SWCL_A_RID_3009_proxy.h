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
/**        \file  RoutineControl_SWCL_A_RID_3009_proxy.h
 *        \brief  Proxy for service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/com/application_error_exception.h"
#include "ara/com/handle_type.h"
#include "ara/com/internal/method.h"
#include "ara/com/internal/proxy_event.h"
#include "ara/com/internal/proxy_field.h"
#include "ara/com/internal/service_update_observer.h"
#include "ara/com/internal/types.h"
#include "ara/com/runtime.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {
namespace proxy {
namespace methods {

/**
 * \brief Type-alias for service method 'Start', that is part of the proxy.
 */
using MethodStart = ara::com::internal::MethodParameters<MetaInfoType&, boolean&, uint32&>::ProxyMethod<
    RoutineControl_SWCL_A_RID_3009ProxyImplInterface, proxy::methods::Start,
    &RoutineControl_SWCL_A_RID_3009ProxyImplInterface::MethodStart>;

/**
 * \brief Type-alias for service method 'Stop', that is part of the proxy.
 */
using MethodStop = ara::com::internal::MethodParameters<MetaInfoType&>::ProxyMethod<
    RoutineControl_SWCL_A_RID_3009ProxyImplInterface, proxy::methods::Stop,
    &RoutineControl_SWCL_A_RID_3009ProxyImplInterface::MethodStop>;

/**
 * \brief Type-alias for service method 'RequestResults', that is part of the proxy.
 */
using MethodRequestResults = ara::com::internal::MethodParameters<MetaInfoType&>::ProxyMethod<
    RoutineControl_SWCL_A_RID_3009ProxyImplInterface, proxy::methods::RequestResults,
    &RoutineControl_SWCL_A_RID_3009ProxyImplInterface::MethodRequestResults>;

}  // namespace methods

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace application_errors {

/**
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailedOnStart'.
 * \details errorContext of UDSServiceFailedOnStart is of Type UDSResponseCodeType.
 */
class UDSServiceFailedOnStart : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailedOnStart' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailedOnStart'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailedOnStart(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailedOnStart'.
   * \see SWS_CM_10356
   * \return Explanatory string
   */
  const char* what() const noexcept override;

  /**
   * \brief Get the error context of 'errorContext'
   * \return Error context of 'errorContext'
   */
  const UDSResponseCodeType& geterrorContext() const;

 private:
  /**
   * \brief Error context value of 'errorContext'
   */
  const UDSResponseCodeType errorContext_;
};

/**
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailedOnStop'.
 * \details errorContext of UDSServiceFailedOnStop is of Type UDSResponseCodeType.
 */
class UDSServiceFailedOnStop : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailedOnStop' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailedOnStop'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailedOnStop(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailedOnStop'.
   * \see SWS_CM_10356
   * \return Explanatory string
   */
  const char* what() const noexcept override;

  /**
   * \brief Get the error context of 'errorContext'
   * \return Error context of 'errorContext'
   */
  const UDSResponseCodeType& geterrorContext() const;

 private:
  /**
   * \brief Error context value of 'errorContext'
   */
  const UDSResponseCodeType errorContext_;
};

/**
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailedOnRequestResults'.
 * \details errorContext of UDSServiceFailedOnRequestResults is of Type UDSResponseCodeType.
 */
class UDSServiceFailedOnRequestResults : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailedOnRequestResults' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailedOnRequestResults'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailedOnRequestResults(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailedOnRequestResults'.
   * \see SWS_CM_10356
   * \return Explanatory string
   */
  const char* what() const noexcept override;

  /**
   * \brief Get the error context of 'errorContext'
   * \return Error context of 'errorContext'
   */
  const UDSResponseCodeType& geterrorContext() const;

 private:
  /**
   * \brief Error context value of 'errorContext'
   */
  const UDSResponseCodeType errorContext_;
};

}  // namespace application_errors

}  // namespace proxy

/**
 * \brief Proxy class for the service 'RoutineControl_SWCL_A_RID_3009'.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009Proxy {
 public:
  /**
   * \brief Constructor of RoutineControl_SWCL_A_RID_3009Proxy. The communication management shall provide a constructor
   * for each
   * specific ServiceProxy class taking a handle returned by any FindService method of the ServiceProxy class to get a
   * valid ServiceProxy based on the handles returned by FindService.
   * \param handle The identification of the service the proxy should represent. Handle are generated by synchronous
   * RoutineControl_SWCL_A_RID_3009Proxy::FindService or asynchronous
   * RoutineControl_SWCL_A_RID_3009Proxy::StartFindService.
   */
  explicit RoutineControl_SWCL_A_RID_3009Proxy(const ara::com::HandleType& handle);

  /**
   * \brief The application developer must ensure, that any existing FindService jobs must be terminated in the
   * bindings for this ServiceProxy and he ensures that all resources are freed!
   * Calling something in the binding-pool within the destructor may lead to runtime exceptions, because the binding
   * pool is not part of the unit-tests and the runtime could also be shutdown already.
   */
  virtual ~RoutineControl_SWCL_A_RID_3009Proxy() = default;

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'RoutineControl_SWCL_A_RID_3009'.
   * \return Unique proxy ID
   */
  constexpr static ara::com::internal::ProxyId GetProxyIdStatic() { return &proxy_id_; }

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'RoutineControl_SWCL_A_RID_3009'.
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
   * \brief Get the RoutineControl_SWCL_A_RID_3009ProxyImplInterface of this proxy to get access to the binding-specific
   * implementation
   * for methods, events and fields.
   * \return a reference to the binding-related implementation, which realizes the interface for
   * method/event/field requests.
   */
  RoutineControl_SWCL_A_RID_3009ProxyImplInterface& GetServiceProxyImplInterface() { return *service_interface_.get(); }

 private:
  /** Proxy type identifier. Address of this member uniquely identifies the proxy type */
  static ara::com::internal::ProxyIdObject proxy_id_;

  /**
   * \brief Helper method to construct the binding-related RoutineControl_SWCL_A_RID_3009ProxyImplInterface
   * and pass the pointer to the attribute service_interface_. Called by the ctor of this class.
   * \param handle To trigger the factory to build a backend.
   * \return The interface, that is stored within the constructed proxy object.
   */
  static std::unique_ptr<RoutineControl_SWCL_A_RID_3009ProxyImplInterface> ConstructInterface(
      const ara::com::HandleType& handle);

  /**
   * \brief Manages the incoming service updates (offers and stopped offers) after StartFindService.
   * \details This is a static object, because a StartFindService call is not bound to a concrete ServiceProxy
   * object but static.
   */
  static ara::com::internal::ServiceUpdateObserver findservice_observer_;

  /** ServiceProxyImplInterface of this proxy to access the implementation */
  std::unique_ptr<RoutineControl_SWCL_A_RID_3009ProxyImplInterface> service_interface_;

 public:
  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Service method 'Start' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param Req_Reset IN parameter of type boolean
   * \param Req_CounterLimit IN parameter of type uint32
   */
  proxy::methods::MethodStart Start;

  /**
   * \brief Service method 'Stop' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  proxy::methods::MethodStop Stop;

  /**
   * \brief Service method 'RequestResults' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  proxy::methods::MethodRequestResults RequestResults;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};  // class RoutineControl_SWCL_A_RID_3009Proxy

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_H_
