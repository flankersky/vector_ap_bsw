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
/**        \file  someip/message.h
 *        \brief  Message
 *
 *      \details  This file provides the SOME/IP protocol type and constant definitions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief SOME/IP message types.
 *
 * \uptrace PRS_SOMEIP_00055
 */
enum SomeIpMessageType {
  kRequest = 0x00U,          ///< A request expecting a response
  kRequestNoReturn = 0x01U,  ///< A 'fire & forget' request
  kNotification = 0x02U,     ///< An event expecting no response
  kResponse = 0x80U,         ///< A response message
  kError = 0x81U,            ///< A response containing an error
};

/**
 * \brief SOME/IP return codes.
 *
 * \uptrace PRS_SOMEIP_00191
 */
enum SomeIpReturnCode {
  kOk = 0x00U,                     ///< No error occurred
  kNotOk = 0x01U,                  ///< An unspecified error occurred, or client id is not 0 for event notification
  kUnknownService = 0x02U,         ///< The requested service id is unknown
  kUnknownMethod = 0x03U,          ///< The requested method id is unknown
  kNotReady = 0x04U,               ///< Service id and method id are known. Application not running
  kWrongProtocolVersion = 0x07U,   ///< Version of SOME/IP protocol not supported
  kWrongInterfaceVersion = 0x08U,  ///< Wrong interface version
  kMalformedMessage = 0x09U,       ///< Deserialization error, so that payload cannot be deserialized.
  kWrongMessageType =
      0x0AU,  ///< An unexpected message type was received (e.g. REQUEST_NO_RETURN for a method defined as REQUEST.)
  kRangeServiceErrorsStart = 0x20U,  ///< Reserved range start for service and method specific errors
  kRangeServiceErrorsEnd = 0x5EU,    ///< Reserved range end for service and method specific errors
};

/**
 * \brief SOME/IP message header.
 */
struct SomeIpMessageHeader {
  ServiceId service_id_;  ///< A SOME/IP service identifier
  MethodId method_id_;    ///< A SOME/IP method or event identifier
  LengthField length_;    ///< Contains the length of a SOME/IP message in bytes starting from Client ID
  ClientId client_id_;    ///< A unique identifier which allows to differentiate multiple clients of the same ECU
  SessionId session_id_;  ///< A unique identifier which allows to differentiate multiple calls to the same method
  ProtocolVersion protocol_version_;    ///< A SOME/IP protocol version
  InterfaceVersion interface_version_;  ///< The major version of a SOME/IP service interface
  MessageType message_type_;            ///< A SOME/IP message type
  ReturnCode return_code_;              ///< A return code which signal whether a request was successful
};

/**
 * \brief SOME/IP header size - represents the full size including the service ID,
 * method ID and the length field.
 */
static constexpr std::size_t kHeaderSize = sizeof(SomeIpMessageHeader);

/**
 * \brief Slightly pedantic, but to prevent from unwanted alignment -
 * even if the SOME/IP header is made exactly in a size no alignment takes place,
 * we never know what a specific compiler does.
 */
static_assert(kHeaderSize == 16U, "sizeof(SomeIpMessageHeader) does not meet specification.");

/**
 * \brief Default SOME/IP protocol version.
 */
static const ProtocolVersion kProtocolVersion = 0x01U;

/**
 * \brief Minimum SOME/IP length value from the SOME/IP header.
 */
static constexpr LengthField kMinimumPayloadLength = 8U;

/**
 * \brief SOME/IP SD service identifier.
 */
static const ServiceId kSdServiceId = 0xFFFFU;

/**
 * \brief SOME/IP SD method identifier.
 */
static const MethodId kSdMethodId = 0x8100U;

/**
 * \brief SOME/IP SD client identifier.
 */
static const ClientId kSdClientId = 0x0000U;
/**
 * \brief Minimum length of an (empty) SOME/IP-SD message.
 */
static constexpr LengthField kSdMinimumMessageLength = 12U;

/**
 * \brief SOME/IP SD protocol version.
 */
static const ProtocolVersion kSdProtocolVersion = 0x01U;

/**
 * \brief SOME/IP SD interface version.
 */
static const InterfaceVersion kSdInterfaceVersion = 0x01U;

/**
 * \brief Wildcard for service instance identifier.
 */
static const InstanceId kInstanceIdAny = 0xFFFFU;

/**
 * \brief Total length of SOME/IP header fields message identifier and length.
 */
static const std::size_t kHeaderLength = 8U;
/**
 * \brief The offset of the length field in the SOME/IP header.
 */
static const std::size_t kLengthFieldOffset = 4U;

/**
 * \brief Wildcard for the major version of a SOME/IP service.
 */
static const std::uint8_t kMajorVersionAny = 0xFFU;
/**
 * \brief Wildcard for the minor version of a SOME/IP service.
 */
static const std::uint32_t kMinorVersionAny = 0xFFFFFFFFU;

/**
 * \brief SOME/IP SD flags.
 */
enum SomeIpSdFlag {
  kReboot = 0x80,  ///< Reboot flag
  kUnicast = 0x40  ///< Unicast flag
};

/**
 * \brief SOME/IP SD entry types.
 */
enum SomeIpSdEntryType {
  kFindService = 0x00,             ///< FindServiceEntry
  kOfferService = 0x01,            ///< OfferServiceEntry or StopOfferServiceEntry
  kSubscribeEventgroup = 0x06,     ///< SubscriveEventgroupEntry or StopSubscribeEventgroupEntry
  kSubscribeEventgroupAck = 0x07,  ///< SubscriveEventgroupAckEntry or StopSubscribeEventgroupNackEntry
};

/**
 * \brief Shift value for number of 1st options in SOME/IP SD entry.
 */
static const std::size_t kSomeIpSdEntryNumberFirstOptsShift = 4;
/**
 * \brief Mask value for number of options in SOME/IP SD entry.
 */
static const std::size_t kSomeIpSdEntryNumberOptsMask = 0xf;
/**
 * \brief Shift value for major version in SOME/IP SD entry.
 */
static const std::size_t kSomeIpSdEntryMajorVersionShift = 24;
/**
 * \brief Mask value for major version in SOME/IP SD entry.
 */
static const std::size_t kSomeIpSdEntryMajorVersionMask = 0xff;
/**
 * \brief Mask value for TTL in SOME/IP SD entry.
 */
static const std::size_t kSomeIpSdEntryTtlMask = 0xffffff;

/**
 * \brief SOME/IP SD FindService entry.
 */
struct SomeIpSdFindServiceEntry {
  std::uint8_t type_;                ///< Type of entry
  std::uint8_t index_1st_opts_;      ///< Index of the first option run
  std::uint8_t index_2nd_opts_;      ///< Index of the second option run
  std::uint8_t number_of_opts_;      ///< Number of options in first and in second option runs
  std::uint16_t service_id_;         ///< SOME/IP service identifier
  std::uint16_t instance_id_;        ///< SOME/IP instance identifier
  std::uint32_t major_version_ttl_;  ///< Major version and TTL
  std::uint32_t minor_version_;      ///< Minor version of SOME/IP service
};

/**
 * \brief SOME/IP SD OfferService/StopOfferService entry.
 */
struct SomeIpSdOfferServiceEntry {
  std::uint8_t type_;                ///< Type of entry
  std::uint8_t index_1st_opts_;      ///< Index of the first option run
  std::uint8_t index_2nd_opts_;      ///< Index of the second option run
  std::uint8_t number_of_opts_;      ///< Number of options in first and in second option runs
  std::uint16_t service_id_;         ///< SOME/IP service identifier
  std::uint16_t instance_id_;        ///< SOME/IP instance identifier
  std::uint32_t major_version_ttl_;  ///< Major version and TTL
  std::uint32_t minor_version_;      ///< Minor version of SOME/IP service
};

/**
 * \brief SOME/IP SD SubscribeEventgroup/StopSubscribeEventgroup entry.
 */
struct SomeIpSdSubscribeEventgroupEntry {
  std::uint8_t type_;                ///< Type of entry
  std::uint8_t index_1st_opts_;      ///< Index of the first option run
  std::uint8_t index_2nd_opts_;      ///< Index of the second option run
  std::uint8_t number_of_opts_;      ///< Number of options in first and in second option runs
  std::uint16_t service_id_;         ///< SOME/IP service identifier
  std::uint16_t instance_id_;        ///< SOME/IP instance identifier
  std::uint32_t major_version_ttl_;  ///< Major version and TTL
  std::uint8_t reserved_;            ///< Reserved
  std::uint8_t counter_;             ///< Enables differentiating of simultaneous subscriptions of the same eventgroup
  std::uint16_t eventgroup_id_;      ///< Eventgroup identifier
};

/**
 * \brief SOME/IP SD SubscribeEventgroupAck/StopSubscribeEventgroupNack entry.
 */
struct SomeIpSdSubscribeEventgroupAckEntry {
  std::uint8_t type_;                ///< Type of entry
  std::uint8_t index_1st_opts_;      ///< Index of the first option run
  std::uint8_t index_2nd_opts_;      ///< Index of the second option run
  std::uint8_t number_of_opts_;      ///< Number of options in first and in second option runs
  std::uint16_t service_id_;         ///< SOME/IP service identifier
  std::uint16_t instance_id_;        ///< SOME/IP instance identifier
  std::uint32_t major_version_ttl_;  ///< Major version and TTL
  std::uint8_t reserved_;            ///< Reserved
  std::uint8_t counter_;             ///< Enables differentiating of simultaneous subscriptions of the same eventgroup
  std::uint16_t eventgroup_id_;      ///< Eventgroup identifier
};

/**
 * \brief SOME/IP SD endpoint option types.
 */
enum SomeIpSdEndpointOptionType {
  kIPv4UnicastEndpointOption = 0x04,    ///< SOME/IP SD IPv4 unicast endpoint option
  kIPv6UnicastEndpointOption = 0x06,    ///< SOME/IP SD IPv6 unicast endpoint option
  kIPv4MulticastEndpointOption = 0x14,  ///< SOME/IP SD IPv4 multicast endpoint option
  kIPv6MulticastEndpointOption = 0x16,  ///< SOME/IP SD IPv6 multicast endpoint option
  kIPv4SdEndpointOption = 0x24,         ///< SOME/IP SD IPv4 SD endpoint option
  kIPv6SdEndpointOption = 0x26,         ///< SOME/IP SD IPv6 SD endpoint option
};

/**
 * \brief SOME/IP SD endpoint option protocol.
 */
enum SomeIpSdEndpointOptionProto {
  kTCP = 0x06,  ///< TCP protocol
  kUDP = 0x11   ///< UDP protocol
};

/**
 * \brief Value of length field in a SOME/IP SD IPv4 unicast/multicast endpoint option.
 */
static const std::size_t kSomeIpSdIPv4EndpointOptionLength = 0x0009;

/**
 * \brief SOME/IP SD IPv4 unicast/multicast endpoint option.
 */
struct SomeIpSdIPv4EndpointOption {
  std::uint16_t length_;     ///< Length of endpoint option in bytes
  std::uint8_t type_;        ///< Type of endpoint option
  std::uint8_t reserved1_;   ///< Reserved
  std::uint8_t address_[4];  ///< IPv4 endpoint address
  std::uint8_t reserved2_;   ///< Reserved
  std::uint8_t proto_;       ///< L4 protocol
  std::uint16_t port_;       ///< L4 port number
};

/**
 * \brief Value of length field in a SOME/IP SD IPv6 unicast/multicast endpoint option.
 */
static const std::size_t kSomeIpSdIPv6EndpointOptionLength = 0x0015;

/**
 * \brief SOME/IP SD IPv6 unicast/multicast endpoint option.
 */
struct SomeIpSdIPv6EndpointOption {
  std::uint16_t length_;      ///< Length of endpoint option in bytes
  std::uint8_t type_;         ///< Type of endpoint option
  std::uint8_t reserved1_;    ///< Reserved
  std::uint8_t address_[16];  ///< IPv6 endpoint address
  std::uint8_t reserved2_;    ///< Reserved
  std::uint8_t proto_;        ///< L4 protocol
  std::uint16_t port_;        ///< L4 port number
};

/**
 * \brief Calculates the value of the length field in the SOME/IP message header.
 *
 * \param body_length Length of body.
 * \return The value of the length field.
 */
inline std::size_t CalculateSomeIpMessageHeaderLengthField(std::size_t body_length) {
  return (body_length + sizeof(SomeIpMessageHeader) - kHeaderLength);
}

/**
 * \brief Calculates the total length of a SOME/IP message from the value of length field in header.
 *
 * \param length Value of length field in SOME/IP header.
 * \return The total length of the SOME/IP message.
 */
inline std::size_t CalculateSomeIpMessageLengthFromLengthField(std::size_t length) { return (length + kHeaderLength); }

/**
 * \brief Maximum value of SOME/IP SD session ID.After reaching this value, the session ID must set back to
 * kSomeIpSdMinimumSessionId.
 */
static constexpr SessionId kSomeIpSdMaximumSessionId = 0xFFFFU;

/**
 * \brief Starting (Minimum) value of SOME/IP SD session identifier.
 */
static constexpr SessionId kSomeIpSdMinimumSessionId = 0x0001U;

/**
 * \brief Invalid value of SOME/IP SD session identifier.
 */
static constexpr SessionId kSomeIpSdInvalidSessionId = 0U;

/**
 * \brief The offset reboot flag in the SOME/IP SD flags.
 */
static const std::size_t kSomeIpSdRebootFlagOffset = 7U;

/**
 * \brief Mask value to check flag bit in the SOME/IP SD flags.
 */
static const std::size_t kSomeIpSdFlagMask = 0x1U;

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_H_
