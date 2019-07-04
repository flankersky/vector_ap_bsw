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
/**        \file  DataIdentifier_SWCL_A_DID_F190_skeleton_someip_binding.cc
 *        \brief  SOME/IP skeleton binding of service 'DataIdentifier_SWCL_A_DID_F190'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_skeleton_someip_binding.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190.h"
#include "someip-posix-common/someip/marshalling.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding(
    ::someip_posix_common::someip::InstanceId instance_id,
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding,
    DataIdentifier_SWCL_A_DID_F190Skeleton& skeleton)
    : instance_id_(instance_id),
      someip_binding_(someip_binding),
      skeleton_(skeleton),
      method_manager_Read_(*this),
      method_manager_Write_(*this),
      method_manager_Cancel_(*this) {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::kServiceId;
  // Register this skeleton binding for method requests and event requests in the AraComSomeIpBinding.
  someip_binding_.RegisterSkeletonBinding(std::make_tuple(service_id, instance_id_), this);
}

DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::~DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding() {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::kServiceId;
  someip_binding_.DeRegisterSkeletonBinding(std::make_tuple(service_id, instance_id_));
}

void DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Get method ID from header
  const ::someip_posix_common::someip::MethodId method_id = header.method_id_;

  // Based on the method id -> static dispatching to the method request/response manager
  switch (method_id) {
    case DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerRead::kMethodId: {
      method_manager_Read_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerWrite::kMethodId: {
      method_manager_Write_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerCancel::kMethodId: {
      method_manager_Cancel_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    default: {
      // Method implementation is missing.
      SendErrorResponseOnMethodRequest<::someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod>(header);
      break;
    }
  }
}

void DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::SendMethodResponse(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendMethodRequest(instance_id_, std::move(packet));
}

void DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding::SendEventNotification(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendEventNotification(instance_id_, std::move(packet));
}

/* ---- Events ----------------------------------------------------------------------------------------------------- */
/* ---- Fields ----------------------------------------------------------------------------------------------------- */

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
