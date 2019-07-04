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
/**        \file  uds_message_parameter.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_UDS_MESSAGE_PARAMETER_H_
#define SRC_SERVER_CONVERSATION_UDS_MESSAGE_PARAMETER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "udstransport/uds_message.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

/**
 * \brief DTO for UDS message parameters
 */
struct UdsMessageParameter {
  /** the source address of the created UdsMessage */
  ara::diag::udstransport::UdsMessage::Address source_address_;

  /** the source address of the created UdsMessage */
  ara::diag::udstransport::UdsMessage::Address target_address_;

  /** the target address type */
  ara::diag::udstransport::UdsMessage::TargetAddressType target_address_type_;

  /** the identifier for the channel */
  ara::diag::udstransport::UdsMessage::GlobalChannelIdentifier global_channel_identifier_;
};

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_UDS_MESSAGE_PARAMETER_H_
