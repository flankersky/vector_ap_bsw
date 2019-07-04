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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton_someip_binding.cc
 *        \brief  SOME/IP skeleton binding of service 'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_someip_binding.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"
#include "someip-posix-common/someip/marshalling.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding(
    ::someip_posix_common::someip::InstanceId instance_id,
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding,
    RoutineControl_SWCL_A_RID_3009Skeleton& skeleton)
    : instance_id_(instance_id),
      someip_binding_(someip_binding),
      skeleton_(skeleton),
      method_manager_Start_(*this),
      method_manager_Stop_(*this),
      method_manager_RequestResults_(*this) {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kServiceId;
  // Register this skeleton binding for method requests and event requests in the AraComSomeIpBinding.
  someip_binding_.RegisterSkeletonBinding(std::make_tuple(service_id, instance_id_), this);
}

RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::~RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding() {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kServiceId;
  someip_binding_.DeRegisterSkeletonBinding(std::make_tuple(service_id, instance_id_));
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Get method ID from header
  const ::someip_posix_common::someip::MethodId method_id = header.method_id_;

  // Based on the method id -> static dispatching to the method request/response manager
  switch (method_id) {
    case RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::kMethodId: {
      method_manager_Start_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::kMethodId: {
      method_manager_Stop_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::kMethodId: {
      method_manager_RequestResults_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    default: {
      // Method implementation is missing.
      SendErrorResponseOnMethodRequest<::someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod>(header);
      break;
    }
  }
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::SendMethodResponse(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendMethodRequest(instance_id_, std::move(packet));
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::SendEventNotification(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendEventNotification(instance_id_, std::move(packet));
}

/* ---- Events ----------------------------------------------------------------------------------------------------- */
/* ---- Fields ----------------------------------------------------------------------------------------------------- */

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
