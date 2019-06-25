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
/**        \file  someip_posix_types.h
 *        \brief  General SOME/IP types which can be used across component boundaries.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_POSIX_TYPES_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_POSIX_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace someip_posix_common {
namespace someip {

/**
 * \brief Type definition for the SOME/IP Service ID
 */
using ServiceId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Instance ID
 */
using InstanceId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Client ID
 */
using ClientId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Session ID
 */
using SessionId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Event ID
 */
using EventId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Eventgroup ID
 */
using EventgroupId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP Method ID
 */
using MethodId = std::uint16_t;

/**
 * \brief Type definition for the SOME/IP MessageType
 */
using MessageType = std::uint8_t;

/**
 * \brief Type definition for the SOME/IP ReturnCode
 */
using ReturnCode = std::uint8_t;

/**
 * \brief Type definition for the SOME/IP ProtocolVersion
 */
using ProtocolVersion = std::uint8_t;

/**
 * \brief Type definition for the SOME/IP InterfaceVersion
 */
using InterfaceVersion = std::uint8_t;

/**
 * \brief Major version used for the SOME/IP deployment.
 */
using MajorVersion = InterfaceVersion;

/**
 * \brief Minor version used for the SOME/IP messages.
 */
using MinorVersion = std::uint32_t;

/**
 * \brief Length definition for the SOME/IP header
 */
using LengthField = std::uint32_t;

/**
 * \brief Type-alias for the socket path which can be later exchanged with a
 * specific version of string.
 */
using SocketPath = std::string;

/**
 * \brief Type-alias for an IP address.
 * Can be later exchanged with a specific static / safe string type.
 */
using IpAddress = std::string;

/**
 * \brief Type definition for the SOME/IP Port
 */
using Port = std::uint16_t;

/**
 * \brief Type-alias for the SOME/IP TTL
 */
using Ttl = std::uint32_t;

/**
 * \brief Alias for Socket Channel Id
 */
using ChannelId = int;

/**
 * \brief Boolean flag if Quality of Service option is enabled for a specific socket.
 */
using QoSEnable = bool;

/**
 * \brief Value type for the QoS priority.
 */
using QoSPriority = int;

/**
 * \brief QoSOption pair for the configuration.
 */
using QoSOption = std::pair<QoSEnable, QoSPriority>;

/**
 * \brief KeepAlive parameters for TCP sockets. These parameters can be set
 * via setsocketopt() POSIX API.
 */
struct KeepAliveParameters {
  /**
   * \brief The duration between two keepalive transmissions in idle condition in seconds.
   * By default this is set to 2 hours.
   */
  std::chrono::seconds keep_alive_time_;

  /**
   * \brief The time (in seconds) between individual keepalive probes.
   */
  std::chrono::seconds keep_alive_interval_;

  /**
   * \brief Number of retransmissions to be carried out before declaring that remote end is not available.
   */
  int keep_alive_retry_count_;
};

/**
 * \brief Flag if KeepAlive has user-specific settings.
 */
using KeepAliveEnable = bool;

/**
 * \brief A pair of KeepAlive enable (bool) and parameters.
 */
using TcpKeepAliveOption = std::pair<KeepAliveEnable, KeepAliveParameters>;

/**
 * \brief Options of a socket combined in a structure.
 * Possible options are
 * - QoS
 * - KeepAlive
 */
struct SocketOptions {
  /**
   * \brief Socket QoS option
   */
  QoSOption qos_;
  /**
   * \brief KeepAlive option for TCP sockets.
   */
  TcpKeepAliveOption keep_alive_;
};

/**
 * \brief Type definition for subscription state of an event
 */
enum class SubscriptionState : uint8_t { kSubscribed = 0, kNotSubscribed = 1, kSubscriptionPending = 2 };

/**
 * \brief Service instance.
 */
struct ServiceInstance {
  /**
   * \brief Service ID of a SOME/IP service
   */
  ServiceId service_id_;
  /**
   * \brief Instance ID of a SOME/IP service
   */
  InstanceId instance_id_;
};

/**
 * \brief Container for service instances.
 */
using ServiceInstanceContainer = std::vector<ServiceInstance>;

/**
 * \brief Service discovery event subscription state entry.
 */
struct EventSubscriptionState {
  /**
   * \brief SOME/IP service identifier
   */
  ServiceId service_id_;
  /**
   * \brief SOME/IP service instance identifier
   */
  InstanceId instance_id_;
  /**
   * \brief SOME/IP event identifier
   */
  EventId event_id_;
  /**
   * \brief An event subscription state
   */
  SubscriptionState state_;
};

/**
 * \brief Represents a SOME/IP message.
 */
using PacketBuffer = std::vector<std::uint8_t>;

/**
 * \brief Represents a unique pointer to a SOME/IP message.
 */
using SomeIpPacket = std::unique_ptr<PacketBuffer>;

/**
 * \brief Transmission trigger buffer size in bytes.
 */
using TransmissionTriggerBufferSize = std::uint32_t;

/**
 * \brief Maximum transmission unit.
 */
using MTU = std::uint32_t;

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_POSIX_TYPES_H_
