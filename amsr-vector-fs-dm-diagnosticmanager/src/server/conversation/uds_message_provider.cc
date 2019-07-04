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
/**        \file  uds_message_provider.cc
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
#include "server/conversation/uds_message_provider.h"
#include "udstransport/uds_message_helper.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

/**
 * \brief Constructor.
 */
UdsMessageProvider::UdsMessageProvider(size_type max_number_of_uds_messages, size_type max_payload_size) {
  pool_.reserve(max_number_of_uds_messages);
  buffer_provider_.reserve(max_number_of_uds_messages, max_payload_size);
}

ara::diag::udstransport::UdsMessage::Ptr UdsMessageProvider::Acquire(size_type payload_size,
                                                                     const UdsMessageParameter& uds_message_params) {
  // Request buffer of given size
  vac::memory::SmartBufferProvider<uint8_t>::UniqueBufferPtr buffer = buffer_provider_.allocate(payload_size);

  // Check if resources were available for buffer.
  if (buffer == nullptr) {
    ara::log::LogError() << "UdsMessageProvider::" << __func__ << " : Couldn't create UDS message (payload_size='"
                         << payload_size << "') - allocation of buffer failed!";
    return ara::diag::udstransport::UdsMessage::Ptr(nullptr);
  }

  ara::log::LogDebug() << "UdsMessageProvider::" << __func__ << " : Try to create UDS message for "
                       << amsr::diag::udstransport::ToString(uds_message_params);

  // Create UdsMessage
  ara::diag::udstransport::UdsMessage::Ptr uds_msg_ptr = pool_.create(
      uds_message_params.source_address_, uds_message_params.target_address_, uds_message_params.target_address_type_,
      static_cast<uint32_t>(payload_size), std::move(buffer), uds_message_params.global_channel_identifier_);

  if (uds_msg_ptr == nullptr) {
    ara::log::LogError() << "UdsMessageProvider::" << __func__
                         << " : Couldn't create UDS message - pool full or create failed!";
  }

  return uds_msg_ptr;
}

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr
