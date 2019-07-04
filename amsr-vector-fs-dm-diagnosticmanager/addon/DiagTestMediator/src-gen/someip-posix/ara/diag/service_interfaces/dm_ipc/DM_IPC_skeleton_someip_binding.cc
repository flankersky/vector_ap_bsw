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
/**        \file  DM_IPC_skeleton_someip_binding.cc
 *        \brief  SOME/IP skeleton binding of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton_someip_binding.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC.h"
#include "someip-posix-common/someip/marshalling.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

DM_IPCSkeletonSomeIpBinding::DM_IPCSkeletonSomeIpBinding(
    ::someip_posix_common::someip::InstanceId instance_id,
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding, DM_IPCSkeleton& skeleton)
    : instance_id_(instance_id),
      someip_binding_(someip_binding),
      skeleton_(skeleton),
      method_manager_RequestData_(*this) {
  static constexpr ::someip_posix_common::someip::ServiceId service_id = DM_IPCSkeletonSomeIpBinding::kServiceId;
  // Register this skeleton binding for method requests and event requests in the AraComSomeIpBinding.
  someip_binding_.RegisterSkeletonBinding(std::make_tuple(service_id, instance_id_), this);
}

DM_IPCSkeletonSomeIpBinding::~DM_IPCSkeletonSomeIpBinding() {
  static constexpr ::someip_posix_common::someip::ServiceId service_id = DM_IPCSkeletonSomeIpBinding::kServiceId;
  someip_binding_.DeRegisterSkeletonBinding(std::make_tuple(service_id, instance_id_));
}

void DM_IPCSkeletonSomeIpBinding::HandleMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                                                      RootDeserializerAlias&& deserializer) {
  // Get method ID from header
  const ::someip_posix_common::someip::MethodId method_id = header.method_id_;

  // Based on the method id -> static dispatching to the method request/response manager
  switch (method_id) {
    case DM_IPCSkeletonSomeIpMethodManagerRequestData::kMethodId: {
      method_manager_RequestData_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    default: {
      // Method implementation is missing.
      SendErrorResponseOnMethodRequest<::someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod>(header);
      break;
    }
  }
}

void DM_IPCSkeletonSomeIpBinding::SendMethodResponse(::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendMethodRequest(instance_id_, std::move(packet));
}

void DM_IPCSkeletonSomeIpBinding::SendEventNotification(::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendEventNotification(instance_id_, std::move(packet));
}

/* ---- Events ----------------------------------------------------------------------------------------------------- */
/* ---- Fields ----------------------------------------------------------------------------------------------------- */

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
