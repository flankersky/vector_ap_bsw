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
/**        \file  uds_message_provider.h
 *        \brief  TODO documentation is MISSING
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_UDS_MESSAGE_PROVIDER_H_
#define SRC_SERVER_CONVERSATION_UDS_MESSAGE_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/conversation/uds_message_parameter.h"
#include "udstransport/uds_message.h"
#include "vac/memory/buffer_provider.h"
#include "vac/memory/object_pool.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

/**
 * \brief UdsMessageProvider provides resources for UdsMessages.
 *
 */
class UdsMessageProvider {
 public:
  /// Typedef for the size type used in this implementation
  using size_type = std::size_t;

  /**
   * \brief Initializes UdsMessageProvider with given number and size of UDS messages.
   *
   * \param max_number_of_uds_messages maximimum number of uds messages
   * \param max_payload_size maximum payload size of one single message
   */
  UdsMessageProvider(size_type max_number_of_uds_messages, size_type max_payload_size);

  /**
   * \brief Method to create a UdsMessage with given source and target address and payload size.
   * \param payload_size the size of the payload of the created UdsMessages, which has to be less or equal the
   * max_payload_size defined in the constructor
   * \param uds_message_params UDS message parameter
   *
   * \return pointer to created UdsMessage. Returns nullptr if UdsMessage cannot be created due to lack of resources.
   */
  ara::diag::udstransport::UdsMessage::Ptr Acquire(size_type payload_size,
                                                   const UdsMessageParameter& uds_message_params);

 private:
  /**
   * \brief Buffer provider of uint8_t.
   */
  vac::memory::SmartBufferProvider<uint8_t> buffer_provider_;

  /**
   * \brief Object pool for UdsMessages.
   */
  vac::memory::SmartObjectPool<ara::diag::udstransport::UdsMessage> pool_;
};

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_UDS_MESSAGE_PROVIDER_H_
