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
/**        \file  calculatorInterface_skeleton_someip_event_manager.cc
 *        \brief  SOME/IP skeleton event handling for events and field notifications of service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_skeleton_someip_event_manager.h"
#include <utility>
#include "someip-posix/someip_posix.h"
#include "vector/calculatorService/calculatorInterface_skeleton_someip_binding.h"

namespace vector {
namespace calculatorService {

/* ---- Event 'divisionByZero' ------------------------------------------- */

CalculatorInterfaceSkeletonSomeIpEventManagerDivisionByZero::
    CalculatorInterfaceSkeletonSomeIpEventManagerDivisionByZero(
        CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

void CalculatorInterfaceSkeletonSomeIpEventManagerDivisionByZero::Send(const vector::calculatorService::boolean& data) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceSkeletonSomeIpBinding::kServiceId;
  header.method_id_ = 0xB26DU;
  header.client_id_ = 0U;
  header.session_id_ = 0U;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kNotification;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'data' (/AUTOSAR_Platform/ImplementationDataTypes/boolean) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_data{&serializer};
    serializer_data.PushBack(data);
    serializer_data.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance id to AraComSomeIpBinding.
  skeleton_binding_.SendEventNotification(std::move(buffer));
}
/* ---- Field notifier 'divideResult' ------------------------------------------- */

CalculatorInterfaceSkeletonSomeIpFieldNotifierDivideResult::CalculatorInterfaceSkeletonSomeIpFieldNotifierDivideResult(
    CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

void CalculatorInterfaceSkeletonSomeIpFieldNotifierDivideResult::Send(const vector::calculatorService::uint32& data) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceSkeletonSomeIpBinding::kServiceId;
  header.method_id_ = 0x80F2U;
  header.client_id_ = 0U;
  header.session_id_ = 0U;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kNotification;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'data' (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_data{&serializer};
    serializer_data.PushBack(data);
    serializer_data.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance id to AraComSomeIpBinding.
  skeleton_binding_.SendEventNotification(std::move(buffer));
}

}  // namespace calculatorService
}  // namespace vector
