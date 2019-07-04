/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  doip_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "doip_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

DoIPNACKCode DoIPHandler::CheckHeader(DoIPMessage& msg) {
  // Implementing [DoIP-037], Figure 7.

  // Check invalid Header format.
  // if (version is not 2010 or version is not 2012) or
  //    (inverted version is not equal to inversion of version) or
  //    (size read is less than a header size)
  if ((msg.protocol_version != DoIPProtocolVersion::kDoIpIsoDis13400_2_2012) ||
      (msg.inv_protocol_version != static_cast<uint8_t>(~msg.protocol_version))) {
    // Construct NACK message with code Incorrect pattern format [DoIP-041]
    return DoIPNACKCode::kIncorrectPattern;
    // Close the channel for TCP
  } else if (!this->IsSupportedPayloadType(msg.payload_type)) {
    // Construct NACK message with code Unknown payload type [DoIP-042].
    return DoIPNACKCode::kUnkownPayloadType;
  } else if (msg.payload_length > max_message_size_) {
    // Construct NACK message with code payload length exceeds the maximum processable size [DoIP-043].
    return DoIPNACKCode::kMessageTooLarge;
  } else if (!this->IsValidPayloadLength(msg.payload_type, msg.payload_length)) {
    // Construct NACK message with code Invalid payload length [DoIP-045].
    return DoIPNACKCode::kInvalidPayloadLength;
  } else {  // Should Out of Memory be checked out also [DoIP-044]?
    // Message should be received
    return DoIPNACKCode::kValidHeader;
  }
}

void DoIPHandler::SendResponse(osabstraction::io::network::socket::Socket& socket, DoIPMessage& msg,
                               osabstraction::io::network::address::SocketAddress* remote_address) {
  std::array<uint8_t, 8> buffer;
  buffer[0] = msg.protocol_version;
  buffer[1] = msg.inv_protocol_version;
  buffer[2] = amsr::diag::common::GetByte(msg.payload_type, 1);
  buffer[3] = amsr::diag::common::GetByte(msg.payload_type, 0);
  buffer[4] = amsr::diag::common::GetByte(msg.payload_length, 3);
  buffer[5] = amsr::diag::common::GetByte(msg.payload_length, 2);
  buffer[6] = amsr::diag::common::GetByte(msg.payload_length, 1);
  buffer[7] = amsr::diag::common::GetByte(msg.payload_length, 0);

  std::array<struct iovec, 2> data_to_send{{{buffer.data(), buffer.size()}, {msg.payload.data(), msg.payload_length}}};

  if (remote_address != nullptr) {
    socket.Send(osabstraction::io::network::socket::Socket::IovecContainer(data_to_send), *remote_address);
  } else {
    socket.Send(osabstraction::io::network::socket::Socket::IovecContainer(data_to_send));
  }
}

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
