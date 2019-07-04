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
/**        \file  DataIdentifier_SWCL_A_DID_F190_proxy.h
 *        \brief  Proxy for service 'DataIdentifier_SWCL_A_DID_F190'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_H_

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
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {
namespace proxy {
namespace methods {

/**
 * \brief Type-alias for service method 'Read', that is part of the proxy.
 */
using MethodRead = ara::com::internal::MethodParameters<MetaInfoType&>::ProxyMethod<
    DataIdentifier_SWCL_A_DID_F190ProxyImplInterface, proxy::methods::Read,
    &DataIdentifier_SWCL_A_DID_F190ProxyImplInterface::MethodRead>;

/**
 * \brief Type-alias for service method 'Write', that is part of the proxy.
 */
using MethodWrite =
    ara::com::internal::MethodParameters<MetaInfoType&, fixed_size_array_with_17_uint8_items&>::ProxyMethod<
        DataIdentifier_SWCL_A_DID_F190ProxyImplInterface, proxy::methods::Write,
        &DataIdentifier_SWCL_A_DID_F190ProxyImplInterface::MethodWrite>;

/**
 * \brief Type-alias for service method 'Cancel', that is part of the proxy.
 */
using MethodCancel = ara::com::internal::MethodParameters<MetaInfoType&>::ProxyMethod<
    DataIdentifier_SWCL_A_DID_F190ProxyImplInterface, proxy::methods::Cancel,
    &DataIdentifier_SWCL_A_DID_F190ProxyImplInterface::MethodCancel>;

}  // namespace methods

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace application_errors {

/**
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailedOnRead'.
 * \details errorContext of UDSServiceFailedOnRead is of Type UDSResponseCodeType.
 */
class UDSServiceFailedOnRead : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailedOnRead' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailedOnRead'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailedOnRead(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailedOnRead'.
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
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailedOnWrite'.
 * \details errorContext of UDSServiceFailedOnWrite is of Type UDSResponseCodeType.
 */
class UDSServiceFailedOnWrite : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailedOnWrite' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailedOnWrite'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailedOnWrite(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailedOnWrite'.
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
 * \brief Proxy class for the service 'DataIdentifier_SWCL_A_DID_F190'.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190Proxy {
 public:
  /**
   * \brief Constructor of DataIdentifier_SWCL_A_DID_F190Proxy. The communication management shall provide a constructor
   * for each
   * specific ServiceProxy class taking a handle returned by any FindService method of the ServiceProxy class to get a
   * valid ServiceProxy based on the handles returned by FindService.
   * \param handle The identification of the service the proxy should represent. Handle are generated by synchronous
   * DataIdentifier_SWCL_A_DID_F190Proxy::FindService or asynchronous
   * DataIdentifier_SWCL_A_DID_F190Proxy::StartFindService.
   */
  explicit DataIdentifier_SWCL_A_DID_F190Proxy(const ara::com::HandleType& handle);

  /**
   * \brief The application developer must ensure, that any existing FindService jobs must be terminated in the
   * bindings for this ServiceProxy and he ensures that all resources are freed!
   * Calling something in the binding-pool within the destructor may lead to runtime exceptions, because the binding
   * pool is not part of the unit-tests and the runtime could also be shutdown already.
   */
  virtual ~DataIdentifier_SWCL_A_DID_F190Proxy() = default;

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'DataIdentifier_SWCL_A_DID_F190'.
   * \return Unique proxy ID
   */
  constexpr static ara::com::internal::ProxyId GetProxyIdStatic() { return &proxy_id_; }

  /**
   * \brief Get the unique proxy type identifier. Unique for the service 'DataIdentifier_SWCL_A_DID_F190'.
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
   * \brief Get the DataIdentifier_SWCL_A_DID_F190ProxyImplInterface of this proxy to get access to the binding-specific
   * implementation
   * for methods, events and fields.
   * \return a reference to the binding-related implementation, which realizes the interface for
   * method/event/field requests.
   */
  DataIdentifier_SWCL_A_DID_F190ProxyImplInterface& GetServiceProxyImplInterface() { return *service_interface_.get(); }

 private:
  /** Proxy type identifier. Address of this member uniquely identifies the proxy type */
  static ara::com::internal::ProxyIdObject proxy_id_;

  /**
   * \brief Helper method to construct the binding-related DataIdentifier_SWCL_A_DID_F190ProxyImplInterface
   * and pass the pointer to the attribute service_interface_. Called by the ctor of this class.
   * \param handle To trigger the factory to build a backend.
   * \return The interface, that is stored within the constructed proxy object.
   */
  static std::unique_ptr<DataIdentifier_SWCL_A_DID_F190ProxyImplInterface> ConstructInterface(
      const ara::com::HandleType& handle);

  /**
   * \brief Manages the incoming service updates (offers and stopped offers) after StartFindService.
   * \details This is a static object, because a StartFindService call is not bound to a concrete ServiceProxy
   * object but static.
   */
  static ara::com::internal::ServiceUpdateObserver findservice_observer_;

  /** ServiceProxyImplInterface of this proxy to access the implementation */
  std::unique_ptr<DataIdentifier_SWCL_A_DID_F190ProxyImplInterface> service_interface_;

 public:
  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Service method 'Read' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         dataRecord_vin OUT parameter of type fixed_size_array_with_17_uint8_items
   */
  proxy::methods::MethodRead Read;

  /**
   * \brief Service method 'Write' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param dataRecord_vin IN parameter of type fixed_size_array_with_17_uint8_items
   */
  proxy::methods::MethodWrite Write;

  /**
   * \brief Service method 'Cancel' as an object of an instantiated proxy.
   * \param MetaInfo IN parameter of type MetaInfoType
   */
  proxy::methods::MethodCancel Cancel;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};  // class DataIdentifier_SWCL_A_DID_F190Proxy

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_H_
