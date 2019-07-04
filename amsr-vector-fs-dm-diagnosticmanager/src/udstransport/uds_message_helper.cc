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
/**        \file  uds_message_helper.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iomanip>
#include <sstream>

#include "uds_message_helper.h"

namespace amsr {
namespace diag {
namespace udstransport {

static const char tab[] = "   ";

static constexpr std::uint8_t print_size_uint8 = 2;

std::string ToString(const ara::diag::udstransport::UdsMessage::Ptr& uds_message, std::size_t max_payload_bytes) {
  std::stringstream log_stream;

  // Check if UdsMessage::Ptr is not null.
  if (uds_message == nullptr) {
    log_stream << "UDSMessage: nullptr received. \n";
    return log_stream.str();
  }

  ara::diag::udstransport::ByteVector payload = uds_message->GetPayload();

  // Change parameter of the stream to print values in hexadecimal
  log_stream << std::setfill('0') << std::hex;

  // Print TA, SA and TA type
  log_stream << "UDSMessage: \n";
  log_stream << tab << "Source Address: " << ToString(uds_message->GetSa()) << "\n";
  log_stream << tab << "Target Address: " << ToString(uds_message->GetTa()) << "\n";
  log_stream << tab << "Target Address type: " << ToString(uds_message->GetTaType()) << "\n";
  log_stream << tab << "Global Channel Identifier: " << ToString(uds_message->GetGlobalChannelIdentifier()) << "\n";

  // Print Payload
  log_stream << tab << "Payload: 0x";
  ara::diag::udstransport::ByteVector::iterator payload_iterator = payload.begin();
  for (std::size_t printed_bytes = 0; ((payload_iterator != payload.end()) && (printed_bytes < max_payload_bytes));
       ++printed_bytes) {
    log_stream << std::setw(print_size_uint8) << static_cast<std::uint16_t>(*payload_iterator);
    ++payload_iterator;
  }
  log_stream << "\n";
  return log_stream.str();
}

std::string ToString(const amsr::diag::server::conversation::UdsMessageParameter& uds_msg_parameter) {
  std::stringstream log_stream;

  // Change parameter of the stream to print values in hexadecimal
  log_stream << std::setfill('0') << std::hex;

  // Print TA, SA and TA type
  log_stream << "UdsMessageParameter: \n";
  log_stream << tab << "Source Address: " << ToString(uds_msg_parameter.source_address_) << "\n";
  log_stream << tab << "Target Address: " << ToString(uds_msg_parameter.target_address_) << "\n";
  log_stream << tab << "Target Address type: " << ToString(uds_msg_parameter.target_address_type_) << "\n";
  log_stream << tab << "Global Channel Identifier: " << ToString(uds_msg_parameter.global_channel_identifier_) << "\n";
  return log_stream.str();
}

std::string ToString(const ara::diag::udstransport::UdsMessage::GlobalChannelIdentifier& channel_identifier) {
  ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id = std::get<0>(channel_identifier);
  ara::diag::udstransport::ChannelID channel_id = std::get<1>(channel_identifier);
  std::string result{"TPHandlerID='"};
  result.append(std::to_string(static_cast<std::uint16_t>(handler_id)));
  result.append("', ChannelID='");
  result.append(std::to_string(channel_id));
  result.append("'");
  return result;
}

std::string ToString(ara::diag::udstransport::UdsMessage::Address address) {
  std::stringstream stream;
  stream << std::setfill('0') << std::hex;
  constexpr std::uint8_t print_size_uint16 = 4;
  stream << "0x" << std::setw(print_size_uint16) << address;
  return stream.str();
}

std::string ToString(ara::diag::udstransport::UdsMessage::TargetAddressType type) {
  switch (type) {
    case ara::diag::udstransport::UdsMessage::TargetAddressType::FUNCTIONAL:
      return "FUNCTIONAL";
    case ara::diag::udstransport::UdsMessage::TargetAddressType::PHYSICAL:
      return "PHYSICAL";
    default:
      std::stringstream stream;
      stream << std::setfill('0') << std::hex;
      stream << "0x" << std::setw(print_size_uint8) << static_cast<std::uint16_t>(type);
      return stream.str();
  }
}

}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
