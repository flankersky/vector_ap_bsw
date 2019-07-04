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
/**
 *      \file  uds_message_helper.h
 *      \brief  Helper functions for processing of UdsMessages
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UDSTRANSPORT_UDS_MESSAGE_HELPER_H_
#define SRC_UDSTRANSPORT_UDS_MESSAGE_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "server/conversation/uds_message_parameter.h"
#include "udstransport/uds_message.h"

namespace amsr {
namespace diag {
namespace udstransport {

/**
 * \brief Helper Function to create a string representation of an UDS message
 * \param uds_message The UDS message
 * \param max_payload_bytes The maximum payload size
 * \returns The string representation of the UDS message
 */
std::string ToString(const ara::diag::udstransport::UdsMessage::Ptr& uds_message, std::size_t max_payload_bytes);

/**
 * \brief Helper Function to create a string representation of an UdsMessageParameter POD
 * \param uds_msg_parameter The UDS message parameter POD
 * \returns The string representation of the UdsMessageParameter
 */
std::string ToString(const amsr::diag::server::conversation::UdsMessageParameter& uds_msg_parameter);

/**
 * \brief Helper Function to create a string representation of a GlobalChannelIdentifier
 * \param channel_identifier The global channel identifier of an UDS message
 * \returns The string representation of the GlobalChannelIdentifier
 */
std::string ToString(const ara::diag::udstransport::UdsMessage::GlobalChannelIdentifier& channel_identifier);

/**
 * \brief Helper Function to create a string representation of a UDS Address
 * \param address The address (of an UDS message)
 * \returns The string representation of the provided address
 */
std::string ToString(ara::diag::udstransport::UdsMessage::Address address);

/**
 * \brief Helper Function to create a string representation of a UDS AddressType
 * \param type The address type (of an UDS message)
 * \returns The string representation of the provided address type
 */
std::string ToString(ara::diag::udstransport::UdsMessage::TargetAddressType type);

}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UDSTRANSPORT_UDS_MESSAGE_HELPER_H_
