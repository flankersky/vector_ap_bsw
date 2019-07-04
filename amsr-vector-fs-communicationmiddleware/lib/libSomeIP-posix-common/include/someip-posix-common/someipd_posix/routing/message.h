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
/**        \file  routing/message.h
 *        \brief  Message
 *
 *      \details  This file specifies the communication protocol for routing messages between an application and
 *                the SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_ROUTING_MESSAGE_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_ROUTING_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace someip_posix_common {
namespace someipd_posix {
namespace routing {

/**
 * \brief Protocol version.
 */
static const int kVersion = 1;

/**
 * \brief Message types.
 */
enum MessageType {
  /**
   * A routing communication channel established between an application and the SOME/IP daemon has a unique
   * identifier. The ChannelId message is sent as the first message by the SOME/IP daemon after the routing
   * communication channel is established. This message contains the unique identifier assigned to the established
   * routing communication channel. The channel identifier is required for control messages sent by the application
   * to the SOME/IP daemon in order for the SOME/IP daemon to identify the corresponding routing communication
   * channel to which a received control message applies.
   */
  kChannelId,
  /**
   * A message containing a SOME/IP message. This type of message can be sent by both an application
   * and the SOME/IP daemon.
   */
  kSomeIP,
  /**
   * A message sent by the SOME/IP daemon to an application informing it about the status of SOME/IP
   * service instances.
   */
  kServiceDiscoveryOfferService,
  /**
   * A message sent by the SOME/IP daemon to an application informing it about event subscription state update.
   */
  kServiceDiscoveryEventSubscriptionState
};

/**
 * \brief Message header.
 */
struct MessageHeader {
  std::uint32_t version_;      ///< A protocol version
  std::uint32_t type_;         ///< A message type
  std::uint32_t length_;       ///< The length of a message in bytes excluding the message header
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief ChannelId message.
 */
struct MessageChannelId {
  std::uint32_t channel_id_;  ///< A unique identifier of the routing communication channel
};

/**
 * \brief Service discovery offered service entry.
 */
struct MessageServiceDiscoveryOfferedServiceEntry {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief Service discovery event subscription state entry.
 */
struct MessageServiceDiscoveryEventSubscriptionStateEntry {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
  std::uint16_t event_id_;     ///< A SOME/IP event identifier
  std::uint16_t state_;        ///< An event subscription state (\see ServiceDiscoveryEventSubscriptionState)
};

}  // namespace routing
}  // namespace someipd_posix
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_ROUTING_MESSAGE_H_
