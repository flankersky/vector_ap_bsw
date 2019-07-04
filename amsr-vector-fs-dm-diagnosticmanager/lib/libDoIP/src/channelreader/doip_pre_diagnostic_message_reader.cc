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
/**        \file  doip_pre_diagnostic_message_reader.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "ara/log/logging.hpp"
#include "doip_pre_diagnostic_message_reader.h"
#include "processors/doip_diagnostic_message_processor.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

ara::diag::udstransport::ByteVector DoIPPreDiagnosticMessageReader::Initialize(Ptr doip_msg) {
  doip_msg_ = std::move(doip_msg);
  return ara::diag::udstransport::ByteVector(buffer_);
}

ContextNeededParams DoIPPreDiagnosticMessageReader::Evaluate() {
  assert(doip_msg_ != nullptr);

  ReaderType next_reader = ReaderType::kHeaderReader;
  ContextNeededParams params_for_reader;
  params_for_reader.action = ActionForReader::kContinueReading;
  params_for_reader.number_of_bytes_to_discard = doip_msg_->payload_length - kSaTaBytes;

  assert(doip_msg_->payload.size() >= kSaTaBytes);
  assert(buffer_.size() == kSaTaBytes);

  // Copy the SA & TA (source & target addresses) into the DoIP message's payload.
  for (uint32_t index = 0; index < kSaTaBytes; index++) {
    doip_msg_->payload[index] = buffer_[index];
  }

  // need to call the Diagnostic message processor to ask for buffer with the supplied SA & TA.
  ActionUponDiagnosticMessageHandler action = context_.InvokeDiagnosticMessagePreprocessor(*doip_msg_);
  switch (action) {
    case ActionUponDiagnosticMessageHandler::kProceedWithMessage:
      params_for_reader.number_of_bytes_to_discard = ContextNeededParams::kNothingToDiscard;
      next_reader = ReaderType::kDiagnosticMessageReader;
      break;
    case ActionUponDiagnosticMessageHandler::kDiscardMessage:
      break;
    case ActionUponDiagnosticMessageHandler::kCloseSocket:
      params_for_reader.action = ActionForReader::kCloseSocket;
      break;
    default:
      throw std::logic_error("DoIPPreDiagnosticMessageReader : Error in action from the Diagnostic PreProcessor.");
  }
  context_.SetNextReader(next_reader, std::move(doip_msg_));
  return params_for_reader;
}

void DoIPPreDiagnosticMessageReader::Reset() {
  if (doip_msg_) {
    context_.SetNextReader(ReaderType::kHeaderReader, std::move(doip_msg_));
  }
}

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
