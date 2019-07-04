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
/**        \file  GenericUDSService_skeleton.h
 *        \brief  Skeleton for service 'GenericUDSService'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SKELETON_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>
#include "ara/com/application_error_exception.h"
#include "ara/com/future.h"
#include "ara/com/internal/skeleton_base.h"
#include "ara/com/internal/skeleton_event.h"
#include "ara/com/internal/skeleton_field.h"
#include "ara/com/internal/skeleton_interface.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/** Forward declaration for inserting as a type into the template class SkeletonEvent */
class GenericUDSServiceSkeleton;

namespace skeleton {

namespace methods {}  // namespace methods

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace application_errors {

/**
 * \brief ApplicationErrorException class for the application error 'UDSServiceFailed'.
 * \details errorContext of UDSServiceFailed is of Type UDSResponseCodeType.
 */
class UDSServiceFailed : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'UDSServiceFailed' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'UDSServiceFailed'.
   * \param errorContext error context of type UDSResponseCodeType
   */
  explicit UDSServiceFailed(const UDSResponseCodeType& errorContext);

  /**
   * \brief Get explanation of the ApplicationErrorException 'UDSServiceFailed'.
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

}  // namespace skeleton

/**
 * \brief Skeleton interface class for the service 'GenericUDSService'.
 * \remark generated
 */
class GenericUDSServiceSkeleton : public ara::com::internal::SkeletonBase,
                                  public ara::com::internal::SkeletonInterface {
 public:
  /**
   * \brief Pointer of unique ownership, which is instantiated for a skeleton implementation, after an OfferService call
   * from this object.
   */
  using SkeletonImplUniquePtr = std::unique_ptr<GenericUDSServiceSkeletonImplInterface>;

  /** Type-alias for the collection of implementation interfaces, returned from an OfferService call */
  using SkeletonImplInterfaceCollection = std::vector<SkeletonImplUniquePtr>;

  /**
   * \brief Constructor of GenericUDSServiceSkeleton.
   * \param instance The service instance ID provided by this skeleton.
   * \param mode The method processing mode used by this skeleton. Default: Event-driven processing
   */
  explicit GenericUDSServiceSkeleton(
      ara::com::InstanceIdentifier instance,
      ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);

  /**
   * \brief Default destructor
   */
  virtual ~GenericUDSServiceSkeleton() = default;

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'GenericUDSService'.
   * \return Unique skeleton ID
   */
  constexpr static ara::com::internal::SkeletonId GetSkeletonIdStatic() { return &skeleton_id_; }

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'GenericUDSService'.
   * \return Unique skeleton ID
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override { return GetSkeletonIdStatic(); }

  /**
   * \brief Offer the service 'GenericUDSService'.
   */
  void OfferService();

  /**
   * \brief Stop offering the service 'GenericUDSService'.
   */
  void StopOfferService();

  /**
   * \brief Getter for all binding-specific implementations provided after a OfferService call.
   * \return The collection of implementation interfaces to send the event via multiple bindings.
   */
  SkeletonImplInterfaceCollection& GetImplementationInterfaces() { return impl_interfaces_; }

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton service method 'Service'.
   * \param SID IN parameter of type uint8
   * \param RequestData IN parameter of type DataArrayType
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         ResponseData OUT parameter of type DataArrayType
   */
  virtual ara::com::Future<skeleton::methods::Service::Output> Service(const uint8& SID,
                                                                       const DataArrayType& RequestData,
                                                                       const MetaInfoType& MetaInfo) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

 private:
  /** Skeleton type identifier. Address of this member uniquely identifies the proxy type */
  static ara::com::internal::SkeletonIdObject skeleton_id_;

  /**
   * \brief Each skeleton holds a collection of implementation interfaces after an OfferService call
   * from this instance. The binding pool will then return a collection of all implementation interfaces.
   */
  SkeletonImplInterfaceCollection impl_interfaces_;
};  // class GenericUDSServiceSkeleton

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_SKELETON_H_
