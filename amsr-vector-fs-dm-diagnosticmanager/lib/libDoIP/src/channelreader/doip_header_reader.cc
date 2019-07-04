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
/**        \file  doip_header_reader.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "doip_header_reader.h"
#include "tcp/doip_tcp_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

ara::diag::udstransport::ByteVector DoIPHeaderReader::Initialize(Ptr doip_msg) {
  doip_msg_ = std::move(doip_msg);
  return ara::diag::udstransport::ByteVector(buffer_);
}

ContextNeededParams DoIPHeaderReader::Evaluate() {
  ContextNeededParams params_for_reader;
  params_for_reader.action = ActionForReader::kContinueReading;
  params_for_reader.number_of_bytes_to_discard = ContextNeededParams::kNothingToDiscard;
  ReaderType next_reader = ReaderType::kHeaderReader;
  // Fill the DoIP Message with the Header value available in the Header buffer.
  FillMessageHeader();
  // Check the Header validity according to the DoIP ISO 13400-2 standard.
  doip::tcp::DoIPTCPHandler& tcp_handler = context_.GetTcpHandler();
  DoIPNACKCode nack_code = tcp_handler.CheckHeader(*doip_msg_);
  // If header is not valid, so Prepared a NACK (negative acknowledge message) and send it.
  if (nack_code != DoIPNACKCode::kValidHeader) {
    DoIPMessage response_msg;
    response_msg.payload.resize(doip::kGenericNegativeAcknowledgeLength);
    PrepareNACK(nack_code, response_msg);
    context_.SendResponse(response_msg);
    // [DoIP-087]
    if ((nack_code == DoIPNACKCode::kIncorrectPattern) || (nack_code == DoIPNACKCode::kInvalidPayloadLength)) {
      params_for_reader.action = ActionForReader::kCloseSocket;
    } else {
      params_for_reader.action = ActionForReader::kFinishedReading;
    }
    params_for_reader.number_of_bytes_to_discard = doip_msg_->payload_length;
    // Assign the HeaderReader to be the active one.
    next_reader = ReaderType::kHeaderReader;
  } else {
    if (doip_msg_->payload_type == PayloadType::kDiagnosticMessage) {
      // Assign the PreDiagnosticMessageReader to be the active one.
      next_reader = ReaderType::kPreDiagnosticMessageReader;
    } else {
      // Assign the NonDiagMessageReader to be the active one.
      next_reader = ReaderType::kNonDiagnosticMessageReader;
    }
  }
  context_.SetNextReader(next_reader, std::move(doip_msg_));
  return params_for_reader;
}

void DoIPHeaderReader::Reset() {
  if (doip_msg_) {
    context_.SetNextReader(ReaderType::kHeaderReader, std::move(doip_msg_));
  }
}

void DoIPHeaderReader::FillMessageHeader() {
  assert(doip_msg_);
  assert(buffer_.size() == kGenericHeaderNoOfBytes);
  uint16_t payload_type_processor = 0;
  doip_msg_->protocol_version = buffer_[0];
  doip_msg_->inv_protocol_version = buffer_[1];
  amsr::diag::common::SetByte(payload_type_processor, buffer_[2], 1);
  amsr::diag::common::SetByte(payload_type_processor, buffer_[3], 0);
  for (uint8_t index = 4; index < kGenericHeaderNoOfBytes; index++) {
    amsr::diag::common::SetByte(doip_msg_->payload_length, buffer_[index], 7 - index);
  }
  doip_msg_->payload_type = static_cast<PayloadType>(payload_type_processor);
}

void DoIPHeaderReader::PrepareNACK(DoIPNACKCode nack_code, DoIPMessage& response_msg) {
  response_msg.protocol_version = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  response_msg.inv_protocol_version = static_cast<uint8_t>(~response_msg.protocol_version);
  response_msg.payload_type = PayloadType::kGenericNegativeAcknowledge;
  response_msg.payload_length = doip::kGenericNegativeAcknowledgeLength;
  assert(response_msg.payload.size() >= kGenericNegativeAcknowledgeLength);
  response_msg.payload[0] = nack_code;
}

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
