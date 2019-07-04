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
/**        \file  doip_message.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_DOIP_MESSAGE_H_
#define LIB_LIBDOIP_SRC_DOIP_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <vector>

#include "common/multi_byte_type.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

/**
 * \brief Enumerator for DoIp Protocol versions.
 */
enum DoIPProtocolVersion : uint8_t { kDoIpIsoDis13400_2_2010 = 0x01, kDoIpIsoDis13400_2_2012 = 0x02 };

/**
 * \brief Enumerator for DoIp Payload Types.
 */
enum PayloadType : uint16_t {
  /* Node Management PAYLOAD Types (0x0XXX) */
  kGenericNegativeAcknowledge = 0x0000,
  kVehicleIdentificationRequest = 0x0001,
  kVehicleIdentificationRequestWithEID = 0x0002,
  kVehicleIdentificationRequestWithVIN = 0x0003,
  kVehicleAnnouncementMessage = 0x0004,
  kRoutingActivationRequest = 0x0005,
  kRoutingActivationResponse = 0x0006,
  kAliveCheckRequest = 0x0007,
  kAliveCheckResponse = 0x0008,

  /* Vehicle Information PAYLOAD Types (0x4XXX) */
  kDoIPEntityStatusRequest = 0x4001,
  kDoIPEntityStatusResponse = 0x4002,
  kDiagnosticPowerModeInfoRequest = 0x4003,
  kDiagnosticPowerModeInfoResponse = 0x4004,

  /* Diagnostic PAYLOAD Types (0x8XXX) */
  kDiagnosticMessage = 0x8001,
  kDiagnosticMessagePositiveAck = 0x8002,
  kDiagnosticMessageNegativeAck = 0x8003
};

/**
 * \brief Enumerator for DoIp header NACK codes.
 */
enum DoIPNACKCode : uint8_t {
  kIncorrectPattern = 0x00,
  kUnkownPayloadType = 0x01,
  kMessageTooLarge = 0x02,
  kOutOfMemory = 0x03,
  kInvalidPayloadLength = 0x04,
  kValidHeader = 0xff
};

/**
 * \brief Constant for the Generic NACK message length.
 */
constexpr uint32_t kGenericNegativeAcknowledgeLength = 1;

/**
 * \brief Constant for the Vehicle Identification Request message length.
 */
constexpr uint32_t kVehicleIdentificationRequestLength = 0;

/**
 * \brief Constant for the Vehicle Identification Request with EID message length.
 */
constexpr uint32_t kVehicleIdentificationRequestWithEIDLength = 6;

/**
 * \brief Constant for the Vehicle Identification Request with VIN message length.
 */
constexpr uint32_t kVehicleIdentificationRequestWithVINLength = 17;

/**
 * \brief Constant for the Vehicle Announcement message length.
 */
constexpr uint32_t kVehicleAnnouncementMessageLength = 32;

/**
 * \brief Constant for the Entity Status Request message length.
 */
constexpr uint32_t kDoIPEntityStatusRequestLength = 0;

/**
 * \brief Constant for the Entity Status Response message length.
 */
constexpr uint32_t kDoIPEntityStatusResponseLength = 3;  // can be 7 as there is an optional part.

/**
 * \brief Constant for the Power Mode Info Request message length.
 */
constexpr uint32_t kDiagnosticPowerModeInfoRequestLength = 0;

/**
 * \brief Constant for the Power Mode Info Response message length.
 */
constexpr uint32_t kDiagnosticPowerModeInfoResponseLength = 1;

/* Length below here is mostly non-deterministic as the TCP Payload doesn't have a specific length*/

/**
 * \brief Constant for the Routing Activation Request message length.
 */
constexpr uint32_t kRoutingActivationRequestLength = 7;  // can be 11 as there is an optional part.

/**
 * \brief Constant for the Routing Activation Response message length.
 */
constexpr uint32_t kRoutingActivationResponseLength = 9;  // can be 13 as there is an optional part.

/**
 * \brief Constant for the Alive Check Request message length.
 */
constexpr uint32_t kAliveCheckRequestLength = 0;

/**
 * \brief Constant for the Alive Check Response message length.
 */
constexpr uint32_t kAliveCheckResponseLength = 2;

/**
 * \brief Constant for the Diagnostic message length.
 */
constexpr uint32_t kDiagnosticMessageMinimumLength = 5;  // can be up to d bytes of the message.

/**
 * \brief Constant for the Diagnostic message positive acknowledge length.
 */
constexpr uint32_t kDiagnosticMessagePositiveAckMinimumLength = 5;  // can be up to d bytes of the message.

/**
 * \brief Constant for the Diagnostic message negative acknowledge length.
 */
constexpr uint32_t kDiagnosticMessageNegativeAckMinimumLength = 5;  // can be up to d bytes of the message.

/**
 * \brief Enumerator for Diagnostic power mode information codes.
 */
enum DoIPDiagPowerModeInfoCode : uint8_t { kNotReady = 0x00, kReady = 0x01, kNotSupported = 0x02 };

/**
 * \brief Class representing a DoIP message
 */
struct DoIPMessage {
  /**
   * define the ByteVector of the payload.
   */
  using ByteVector = std::vector<uint8_t>;

  /**
   * \brief The protocolVersion DoIp header field.
   */
  uint8_t protocol_version;

  /**
   * \brief The Inversion of protocolVersion DoIp header field.
   */
  uint8_t inv_protocol_version;

  /**
   * \brief The PayloadType DoIp header field.
   */
  PayloadType payload_type;

  /**
   * \brief The Payload length.
   */
  uint32_t payload_length;

  /**
   * \brief The payload
   */
  ByteVector payload;
};

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_DOIP_MESSAGE_H_
