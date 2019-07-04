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
/**        \file  doip_non_diagnostic_message_reader.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <utility>

#include "ara/log/logging.hpp"

#include "doip_non_diagnostic_message_reader.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {
ara::diag::udstransport::ByteVector DoIPNonDiagnosticMessageReader::Initialize(Ptr doip_msg) {
  doip_msg_ = std::move(doip_msg);
  if (doip_msg_ != nullptr) {
    return ara::diag::udstransport::ByteVector(doip_msg_->payload.data(), doip_msg_->payload_length);
  }
  throw std::runtime_error("DoIPNonDiagnosticMessageReader::Initialize - Invalid DoIP message (nullptr)");
}

ContextNeededParams DoIPNonDiagnosticMessageReader::Evaluate() {
  assert(doip_msg_);

  ContextNeededParams params_for_reader;
  params_for_reader.number_of_bytes_to_discard = ContextNeededParams::kNothingToDiscard;
  params_for_reader.action = ActionForReader::kFinishedReading;
  // Assign the HeaderReader to be the active one.
  ReaderType next_reader = ReaderType::kHeaderReader;
  context_.SetNextReader(next_reader, std::move(doip_msg_));
  return params_for_reader;
}

void DoIPNonDiagnosticMessageReader::Reset() {
  if (doip_msg_) {
    context_.SetNextReader(ReaderType::kHeaderReader, std::move(doip_msg_));
  }
}

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
