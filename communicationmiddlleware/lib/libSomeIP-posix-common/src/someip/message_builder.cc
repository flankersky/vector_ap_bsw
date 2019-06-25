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
/**        \file  message_builder.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#include "someip-posix-common/someip/message_builder.h"

namespace someip_posix_common {
namespace someip {

::someip_posix_common::someip::SomeIpPacket CreateSomeIpErrorHeader(
    const ::someip_posix_common::someip::ReturnCode return_code,
    const ::someip_posix_common::someip::SomeIpMessageHeader& request_header,
    ::someip_posix_common::someip::SomeIpPacket packet) {
  // Header from the failed request is used to get the same service, instance, client and session ID.
  ::someip_posix_common::someip::SomeIpMessageHeader header_response = request_header;

  // The message type of the SOME/IP header must be set to kError
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = return_code;

  // Root serializer
  someip_posix_common::someip::serialization::Serializer<
      someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>
      serializer{std::move(packet)};

  // SOME/IP header serializer
  someip_posix_common::someip::serialization::SomeIpHeaderSerializer<
      someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy, decltype(serializer)>
      header_serializer{&serializer, header_response};

  header_serializer.Close();
  packet = serializer.Close();

  return packet;
}

}  // namespace someip
}  // namespace someip_posix_common
