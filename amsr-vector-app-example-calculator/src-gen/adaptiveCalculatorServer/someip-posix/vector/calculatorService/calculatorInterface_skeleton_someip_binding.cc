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
/**        \file  calculatorInterface_skeleton_someip_binding.cc
 *        \brief  SOME/IP skeleton binding of service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_skeleton_someip_binding.h"
#include "someip-posix-common/someip/marshalling.h"
#include "vector/calculatorService/calculatorInterface.h"

namespace vector {
namespace calculatorService {

CalculatorInterfaceSkeletonSomeIpBinding::CalculatorInterfaceSkeletonSomeIpBinding(
    ::someip_posix_common::someip::InstanceId instance_id,
    ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding, CalculatorInterfaceSkeleton& skeleton)
    : instance_id_(instance_id),
      someip_binding_(someip_binding),
      skeleton_(skeleton),
      method_manager_subtract_(*this),
      method_manager_add_(*this),
      method_manager_divide_(*this),
      event_manager_divisionByZero_(*this),
      field_notifier_divideResult_(*this),
      field_manager_divideResult_get_(*this),
      field_manager_divideResult_set_(*this) {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId;
  // Register this skeleton binding for method requests and event requests in the AraComSomeIpBinding.
  someip_binding_.RegisterSkeletonBinding(std::make_tuple(service_id, instance_id_), this);
}

CalculatorInterfaceSkeletonSomeIpBinding::~CalculatorInterfaceSkeletonSomeIpBinding() {
  static constexpr ::someip_posix_common::someip::ServiceId service_id =
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId;
  someip_binding_.DeRegisterSkeletonBinding(std::make_tuple(service_id, instance_id_));
}

void CalculatorInterfaceSkeletonSomeIpBinding::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Get method ID from header
  const ::someip_posix_common::someip::MethodId method_id = header.method_id_;

  // Based on the method id -> static dispatching to the method request/response manager
  switch (method_id) {
    case CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract::kMethodId: {
      method_manager_subtract_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case CalculatorInterfaceSkeletonSomeIpMethodManagerAdd::kMethodId: {
      method_manager_add_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case CalculatorInterfaceSkeletonSomeIpMethodManagerDivide::kMethodId: {
      method_manager_divide_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet::kMethodId: {
      field_manager_divideResult_get_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    case CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet::kMethodId: {
      field_manager_divideResult_set_.HandleMethodRequest(header, std::move(deserializer));
      break;
    }
    default: {
      // Method implementation is missing.
      SendErrorResponseOnMethodRequest<::someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod>(header);
      break;
    }
  }
}

void CalculatorInterfaceSkeletonSomeIpBinding::SendMethodResponse(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendMethodRequest(instance_id_, std::move(packet));
}

void CalculatorInterfaceSkeletonSomeIpBinding::SendEventNotification(
    ::someip_posix_common::someip::SomeIpPacket&& packet) {
  someip_binding_.SendEventNotification(instance_id_, std::move(packet));
}

/* ---- Events ----------------------------------------------------------------------------------------------------- */

/* Event 'DivisionByZero' */
void CalculatorInterfaceSkeletonSomeIpBinding::SendEventDivisionByZero(const vector::calculatorService::boolean& data) {
  event_manager_divisionByZero_.Send(data);
}
/* ---- Fields ----------------------------------------------------------------------------------------------------- */
/* Field 'SomeipDivideResult' */
void CalculatorInterfaceSkeletonSomeIpBinding::UpdateFieldDivideResult(const vector::calculatorService::uint32& data) {
  field_notifier_divideResult_.Send(data);
}

}  // namespace calculatorService
}  // namespace vector
