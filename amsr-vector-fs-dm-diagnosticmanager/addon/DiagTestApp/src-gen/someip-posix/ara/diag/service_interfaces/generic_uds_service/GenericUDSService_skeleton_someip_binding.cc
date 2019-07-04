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
/**        \file  GenericUDSService_skeleton_someip_binding.cc
 *        \brief  SOME/IP skeleton binding of service 'GenericUDSService'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton_someip_binding.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService.h"
#include "someip-posix-common/someip/marshalling.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

GenericUDSServiceSkeletonSomeIpBinding::GenericUDSServiceSkeletonSomeIpBinding(
    ::someip_posix_common::someip::InstanceId instance_id,
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding, GenericUDSServiceSkeleton& skeleton)
    : instance_id_(instance_id), someip_binding_(someip_binding), skeleton_(skeleton), method_manager_Service_(*this) {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      GenericUDSServiceSkeletonSomeIpBinding::kServiceId;
  // Register this skeleton binding for method requests and event requests in the AraComSomeIpBinding.
  someip_binding_.RegisterSkeletonBinding(std::make_tuple(service_id, instance_id_), this);
}

GenericUDSServiceSkeletonSomeIpBinding::~GenericUDSServiceSkeletonSomeIpBinding() {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      GenericUDSServiceSkeletonSomeIpBinding::kServiceId;
  someip_binding_.DeRegisterSkeletonBinding(std::make_tuple(service_id, instance_id_));
}

void GenericUDSServiceSkeletonSomeIpBinding::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Get method ID from header
  const ::someip_posix_common::someip::MethodId method_id = header.method_id_;

  // Based on the method id -> static dispatching to the method request/response manager
  switch (method_id) {
    case GenericUDSServiceSkeletonSomeIpMethodManagerService::kMethodId: {
      method_manager_Service_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    default: {
      // Method implementation is missing.
      SendErrorResponseOnMethodRequest<::someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod>(header);
      break;
    }
  }
}

void GenericUDSServiceSkeletonSomeIpBinding::SendMethodResponse(::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendMethodRequest(instance_id_, std::move(packet));
}

void GenericUDSServiceSkeletonSomeIpBinding::SendEventNotification(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendEventNotification(instance_id_, std::move(packet));
}

/* ---- Events ----------------------------------------------------------------------------------------------------- */
/* ---- Fields ----------------------------------------------------------------------------------------------------- */

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
