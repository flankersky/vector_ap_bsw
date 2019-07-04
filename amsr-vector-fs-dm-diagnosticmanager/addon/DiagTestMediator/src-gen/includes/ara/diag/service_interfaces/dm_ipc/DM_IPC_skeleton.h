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
/**        \file  DM_IPC_skeleton.h
 *        \brief  Skeleton for service 'DM_IPC'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_H_

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
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton_impl_interface.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/** Forward declaration for inserting as a type into the template class SkeletonEvent */
class DM_IPCSkeleton;

namespace skeleton {

namespace methods {}  // namespace methods

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace application_errors {

/**
 * \brief ApplicationErrorException class for the application error 'Failed'.
 * \details Generic communication failure
 */
class Failed : public ara::com::ApplicationErrorException {
 public:
  /**
   * \brief Error code of 'Failed' is 1.
   */
  static constexpr ara::com::internal::ApplicationErrorCode kErrorCode{0x1U};

  /**
   * \brief Constructor for ApplicationError 'Failed'.
   */
  Failed();

  /**
   * \brief Get explanation of the ApplicationErrorException 'Failed'.
   * \see SWS_CM_10356
   * \return Explanatory string
   */
  const char* what() const noexcept override;
};

}  // namespace application_errors

}  // namespace skeleton

/**
 * \brief Skeleton interface class for the service 'DM_IPC'.
 * \remark generated
 */
class DM_IPCSkeleton : public ara::com::internal::SkeletonBase, public ara::com::internal::SkeletonInterface {
 public:
  /**
   * \brief Pointer of unique ownership, which is instantiated for a skeleton implementation, after an OfferService call
   * from this object.
   */
  using SkeletonImplUniquePtr = std::unique_ptr<DM_IPCSkeletonImplInterface>;

  /** Type-alias for the collection of implementation interfaces, returned from an OfferService call */
  using SkeletonImplInterfaceCollection = std::vector<SkeletonImplUniquePtr>;

  /**
   * \brief Constructor of DM_IPCSkeleton.
   * \param instance The service instance ID provided by this skeleton.
   * \param mode The method processing mode used by this skeleton. Default: Event-driven processing
   */
  explicit DM_IPCSkeleton(ara::com::InstanceIdentifier instance,
                          ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);

  /**
   * \brief Default destructor
   */
  virtual ~DM_IPCSkeleton() = default;

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'DM_IPC'.
   * \return Unique skeleton ID
   */
  constexpr static ara::com::internal::SkeletonId GetSkeletonIdStatic() { return &skeleton_id_; }

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'DM_IPC'.
   * \return Unique skeleton ID
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override { return GetSkeletonIdStatic(); }

  /**
   * \brief Offer the service 'DM_IPC'.
   */
  void OfferService();

  /**
   * \brief Stop offering the service 'DM_IPC'.
   */
  void StopOfferService();

  /**
   * \brief Getter for all binding-specific implementations provided after a OfferService call.
   * \return The collection of implementation interfaces to send the event via multiple bindings.
   */
  SkeletonImplInterfaceCollection& GetImplementationInterfaces() { return impl_interfaces_; }

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton service method 'RequestData'.
   * \param handle IN parameter of type uint32
   * \param meta_info IN parameter of type MetaInfoType
   * \param request_data IN parameter of type DataArrayType
   * \return ara::com::Future with output struct arguments
   *         response_data OUT parameter of type DataArrayType
   */
  virtual ara::com::Future<skeleton::methods::RequestData::Output> RequestData(const uint32& handle,
                                                                               const MetaInfoType& meta_info,
                                                                               const DataArrayType& request_data) = 0;

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
};  // class DM_IPCSkeleton

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_H_
