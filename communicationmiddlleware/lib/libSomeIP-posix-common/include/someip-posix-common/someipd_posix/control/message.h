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
/**        \file  control/message.h
 *        \brief  Message
 *
 *      \details  This file specifies the communication protocol for control messages exchanged between an application
 *                and the SOME/IP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_CONTROL_MESSAGE_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_CONTROL_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <limits>

namespace someip_posix_common {
namespace someipd_posix {
namespace control {

/**
 * \brief Protocol version.
 */
static const int kVersion = 1;

/**
 * \brief Message types.
 */
enum MessageType {
  /**
   * A request sent by an application to the SOME/IP daemon to allocate a unique client identifier.
   */
  kGetClientIdRequest = 0,
  /**
   * A message sent by the SOME/IP daemon to an application as a response to a GetClientId request.
   */
  kGetClientIdResponse,
  /**
   * A request sent by an application to the SOME/IP daemon to free a previously allocated client identifier.
   */
  kReleaseClientIdRequest,
  /**
   * A request sent by an application to the SOME/IP daemon to get a list of currently
   * available SOME/IP service instances.
   */
  kFindServiceRequest,
  /**
   * A message sent by the SOME/IP daemon to an application as a response to a FindService request.
   */
  kFindServiceResponse,
  /**
   * A request sent by an application to the SOME/IP daemon to activate asynchronous notification
   * about the status of a SOME/IP service instance(s).
   */
  kStartFindService,
  /**
   * A request sent by an application to the SOME/IP daemon to deactivate asynchronous notification
   * about the status of a SOME/IP service instance(s).
   */
  kStopFindService,
  /**
   * A request sent by an application to the SOME/IP daemon informing it about the availability
   * of a SOME/IP service instance offered by this application.
   */
  kOfferService,
  /**
   * A request sent by an application to the SOME/IP daemon informing it that previously offered SOME/IP
   * service instance is no longer available.
   */
  kStopOfferService,
  /**
   * A request sent by an application to the SOME/IP daemon instructing it to subscribe a SOME/IP event
   * on behalf of the application and to forward it to the application.
   */
  kSubscribeEvent,
  /**
   * A request sent by an application to the SOME/IP daemon to cancel a subscription of a SOME/IP event.
   */
  kUnsubscribeEvent,
  /**
   * A request sent by an application to the SOME/IP daemon informing it about the Requested
   *  SOME/IP service instance by this application.
   */
  kRequestService,
  /**
   * A request sent by an application to the SOME/IP daemon informing it about the Requested
   *  SOME/IP service instance by this application.
   */
  kReleaseService
};

/**
 * \brief Message header.
 */
struct MessageHeader {
  std::uint32_t version_;     ///< A protocol version
  std::uint32_t type_;        ///< A message type
  std::uint32_t length_;      ///< The length of a message in bytes excluding the message header
  std::uint32_t channel_id_;  ///< A routing channel identifier
};

static constexpr std::uint32_t kInvalidChannelId = std::numeric_limits<std::uint32_t>::max();

/**
 * \brief GetClientId response message.
 */
struct MessageGetClientIdResponse {
  std::uint16_t client_id_;  ///< A client identifier
};

/**
 * \brief ReleaseClientId request message.
 */
struct MessageReleaseClientIdRequest {
  std::uint16_t client_id_;  ///< A client identifier
};

/**
 * \brief FindService request message.
 */
struct MessageFindServiceRequest {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief Service instance entry.
 */
struct FindServiceEntry {
  /**
   * \brief Service ID of a SOME/IP service
   */
  std::uint16_t service_id_;
  /**
   * \brief Instance ID of a SOME/IP service
   */
  std::uint16_t instance_id_;
};

/**
 * \brief StartFindService request message.
 */
struct MessageStartFindService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief StopFindService request message.
 */
struct MessageStopFindService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief OfferService request message.
 */
struct MessageOfferService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief StopOfferService request message.
 */
struct MessageStopOfferService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief OfferService request message.
 */
struct MessageRequestService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief OfferService request message.
 */
struct MessageReleaseService {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
};

/**
 * \brief SubscribeEvent request message.
 */
struct MessageSubscribeEvent {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
  std::uint16_t event_id_;     ///< A SOME/IP event identifier
};

/**
 * \brief UnsubscribeEvent request message.
 */
struct MessageUnsubscribeEvent {
  std::uint16_t service_id_;   ///< A SOME/IP service identifier
  std::uint16_t instance_id_;  ///< A SOME/IP service instance identifier
  std::uint16_t event_id_;     ///< A SOME/IP event identifier
};

}  // namespace control
}  // namespace someipd_posix
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIPD_POSIX_CONTROL_MESSAGE_H_
