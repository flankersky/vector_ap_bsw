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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton.h
 *        \brief  Skeleton for service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_H_

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
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/** Forward declaration for inserting as a type into the template class SkeletonEvent */
class RoutineControl_SWCL_A_RID_3009Skeleton;

namespace skeleton {

namespace methods {}  // namespace methods

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

}  // namespace skeleton

/**
 * \brief Skeleton interface class for the service 'RoutineControl_SWCL_A_RID_3009'.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009Skeleton : public ara::com::internal::SkeletonBase,
                                               public ara::com::internal::SkeletonInterface {
 public:
  /**
   * \brief Pointer of unique ownership, which is instantiated for a skeleton implementation, after an OfferService call
   * from this object.
   */
  using SkeletonImplUniquePtr = std::unique_ptr<RoutineControl_SWCL_A_RID_3009SkeletonImplInterface>;

  /** Type-alias for the collection of implementation interfaces, returned from an OfferService call */
  using SkeletonImplInterfaceCollection = std::vector<SkeletonImplUniquePtr>;

  /**
   * \brief Constructor of RoutineControl_SWCL_A_RID_3009Skeleton.
   * \param instance The service instance ID provided by this skeleton.
   * \param mode The method processing mode used by this skeleton. Default: Event-driven processing
   */
  explicit RoutineControl_SWCL_A_RID_3009Skeleton(
      ara::com::InstanceIdentifier instance,
      ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);

  /**
   * \brief Default destructor
   */
  virtual ~RoutineControl_SWCL_A_RID_3009Skeleton() = default;

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'RoutineControl_SWCL_A_RID_3009'.
   * \return Unique skeleton ID
   */
  constexpr static ara::com::internal::SkeletonId GetSkeletonIdStatic() { return &skeleton_id_; }

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'RoutineControl_SWCL_A_RID_3009'.
   * \return Unique skeleton ID
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override { return GetSkeletonIdStatic(); }

  /**
   * \brief Offer the service 'RoutineControl_SWCL_A_RID_3009'.
   */
  void OfferService();

  /**
   * \brief Stop offering the service 'RoutineControl_SWCL_A_RID_3009'.
   */
  void StopOfferService();

  /**
   * \brief Getter for all binding-specific implementations provided after a OfferService call.
   * \return The collection of implementation interfaces to send the event via multiple bindings.
   */
  SkeletonImplInterfaceCollection& GetImplementationInterfaces() { return impl_interfaces_; }

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton service method 'Start'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param Req_Reset IN parameter of type boolean
   * \param Req_CounterLimit IN parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::Start::Output> Start(const MetaInfoType& MetaInfo,
                                                                   const boolean& Req_Reset,
                                                                   const uint32& Req_CounterLimit) = 0;

  /**
   * \brief Prototype of skeleton service method 'Stop'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::Stop::Output> Stop(const MetaInfoType& MetaInfo) = 0;

  /**
   * \brief Prototype of skeleton service method 'RequestResults'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::RequestResults::Output> RequestResults(const MetaInfoType& MetaInfo) = 0;

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
};  // class RoutineControl_SWCL_A_RID_3009Skeleton

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_H_
