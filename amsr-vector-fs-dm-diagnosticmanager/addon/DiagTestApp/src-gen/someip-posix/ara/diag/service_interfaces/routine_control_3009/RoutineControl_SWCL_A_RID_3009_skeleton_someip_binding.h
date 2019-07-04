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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton_someip_binding.h
 *        \brief  SOME/IP binding of service skeleton for service 'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_BINDING_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_interface.h"
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_impl_interface.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_someip_event_manager.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_someip_method_manager.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/**
 * \brief SOME/IP skeleton binding of Service 'RoutineControl_SWCL_A_RID_3009'.
 * \details Handles serialization and deserialization of all method calls, events, etc.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding
    : public RoutineControl_SWCL_A_RID_3009SkeletonImplInterface,
      public ara::com::someip_posix::AraComSomeIpSkeletonInterface {
 public:
  /**
   * \brief Generated SOME/IP related service ID.
   */
  static constexpr ::someip_posix_common::someip::ServiceId kServiceId{0x842U};

  /**
   * \brief SOME/IP major version of this service interface deployment.
   */
  static constexpr ::someip_posix_common::someip::MajorVersion kMajorVersion{1U};

  /**
   * \brief Constructor
   * \param instance_id SOME/IP service instance ID offered by this skeleton
   * \param someip_binding Reference to the related SOME/IP transport binding
   * \param skeleton Reference to the related frontend skeleton
   */
  explicit RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding(
      ::someip_posix_common::someip::InstanceId instance_id,
      ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding,
      RoutineControl_SWCL_A_RID_3009Skeleton& skeleton);

  /**
   * \brief Default destructor must de-register the implementation from the AraComSomeIpBinding.
   */
  ~RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding();

  /**
   * \brief Get the SkeletonId this SkeletonImplInterface can handle.
   * \returns The SkeletonId this SkeletonImplInterface can handle.
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override {
    return RoutineControl_SWCL_A_RID_3009Skeleton::GetSkeletonIdStatic();
  }

  /**
   * \brief Getter for accessing the related frontend skeleton.
   */
  RoutineControl_SWCL_A_RID_3009Skeleton& GetSkeleton() const { return skeleton_; }

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Receive handler for method requests.
   *
   * \param header Deserialized SOME/IP header.
   * \param deserializer Root deserializer holding the packet buffer.
   */
  void HandleMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                           RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Forward a prepared packet to the AraComSomeIpBinding.
   * \param packet the serialized response packet.
   */
  void SendMethodResponse(::someip_posix_common::someip::SomeIpPacket&& packet);

  /**
   * \brief Is needed for infrastructural faults such as:
   * Deserialization fails or the service/method isn't implemented at all.
   * \tparam ReturnCode The error return code as template parameter to make a
   * compile-time check.
   * \param request_header The SOME/IP header from the request, which is used
   * to build the error response.
   */
  template <typename ::someip_posix_common::someip::ReturnCode ReturnCode>
  void SendErrorResponseOnMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& request_header) {
    static_assert((ReturnCode >= ::someip_posix_common::someip::SomeIpReturnCode::kNotOk) &&
                      (ReturnCode <= ::someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd),
                  "Return code for errors must be in the range of 0x01 .. 0x5E.");
    someip_binding_.SendErrorResponseOnMethodRequest(ReturnCode, instance_id_, request_header);
  }

  /**
   * \brief Send an error response if the SOME/IP request is malformed. This method is invoked
   * when the return code of the error response is only known during runtime.
   * \param return_code The return code to send.
   * \param request_header The header taken from the request.
   */
  void SendErrorResponseOnMethodRequest(const ::someip_posix_common::someip::ReturnCode return_code,
                                        const ::someip_posix_common::someip::SomeIpMessageHeader& request_header) {
    someip_binding_.SendErrorResponseOnMethodRequest(return_code, instance_id_, request_header);
  }

  /**
   * \brief Send an event notification
   * \param packet SOME/IP packet with header and payload.
   */
  void SendEventNotification(::someip_posix_common::someip::SomeIpPacket&& packet);

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

 private:
  /** SOME/IP instance ID offered by this skeleton */
  ::someip_posix_common::someip::InstanceId instance_id_;

  /** Related SOME/IP binding */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;

  /** Reference to the related frontend skeleton */
  RoutineControl_SWCL_A_RID_3009Skeleton& skeleton_;

  /* ---- Methods manager ------------------------------------------------------------------------------------------ */
  /** Method request/response manager for skeleton method 'Start' */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart method_manager_Start_;

  /** Method request/response manager for skeleton method 'Stop' */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop method_manager_Stop_;

  /** Method request/response manager for skeleton method 'RequestResults' */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults method_manager_RequestResults_;

  /* ---- Event manager -------------------------------------------------------------------------------------------- */
  /* ---- Field manager -------------------------------------------------------------------------------------------- */
};

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_BINDING_H_
