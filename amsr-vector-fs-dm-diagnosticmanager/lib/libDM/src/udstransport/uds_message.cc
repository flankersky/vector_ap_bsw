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
/**        \file  uds_message.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "udstransport/uds_message.h"
#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace udstransport {

UdsMessage::UdsMessage(Address sa, Address ta, TargetAddressType ta_type, uint32_t bytevector_length,
                       vac::memory::SmartBufferProvider<uint8_t>::UniqueBufferPtr buffer_ptr,
                       GlobalChannelIdentifier global_channel_identifier)
    : target_type_(ta_type),
      source_address_(sa),
      target_address_(ta),
      data_length_(bytevector_length),
      buffer_unique_ptr_(std::move(buffer_ptr)),
      data_(buffer_unique_ptr_.get(), data_length_),
      global_channel_identifier_(global_channel_identifier) {}

UdsMessage::Address UdsMessage::GetSa() const { return this->source_address_; }

UdsMessage::Address UdsMessage::GetTa() const { return this->target_address_; }

UdsMessage::TargetAddressType UdsMessage::GetTaType() const { return this->target_type_; }

const ByteVector& UdsMessage::GetPayload() const { return this->data_; }

ByteVector& UdsMessage::GetPayload() { return this->data_; }

void UdsMessage::AddMetaInfo(std::shared_ptr<MetaInfoMap> meta_info) {
  if (meta_info_.NumberOfRegisteredContainers() < meta_info_.capacity()) {
    meta_info_.push_back(meta_info);
  } else {
    ara::log::LogError() << "UdsMessage::" << __func__ << " : MetaInfo storage is full. AddMetaInfo is ignored.";
  }
}

} /* namespace udstransport */
} /* namespace diag */
} /* namespace ara */
