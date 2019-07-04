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
/**        \file  doip_diagnostic_message_reader.cc
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
#include "doip_diagnostic_message_reader.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

ara::diag::udstransport::ByteVector DoIPDiagnosticMessageReader::Initialize(Ptr doip_msg) {
  doip_msg_ = std::move(doip_msg);
  uds_message_ = context_.ReleaseUdsMessage();
  if (uds_message_ == nullptr) {
    throw std::runtime_error("DiagnosticMessageReader's Initialization expects a valid UdsMessage stored in Channel.");
  }
  return ara::diag::udstransport::ByteVector(uds_message_->GetPayload().data(), uds_message_->GetPayload().size());
}

ContextNeededParams DoIPDiagnosticMessageReader::Evaluate() {
  ContextNeededParams params_for_reader;
  params_for_reader.action = ActionForReader::kFinishedReading;
  // The size of UDS payload should be compared to the payload length to see if needed to discard something. This will
  // be implemented while implementing the Discard message reader.
  params_for_reader.number_of_bytes_to_discard = ContextNeededParams::kNothingToDiscard;
  ReaderType next_reader = ReaderType::kHeaderReader;

  context_.SetUdsMessage(std::move(uds_message_));
  context_.SetNextReader(next_reader, std::move(doip_msg_));
  return params_for_reader;
}

void DoIPDiagnosticMessageReader::Reset() {
  if (doip_msg_) {
    context_.SetNextReader(ReaderType::kHeaderReader, std::move(doip_msg_));
  }
}

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
