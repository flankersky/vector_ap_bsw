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
/**        \file  someip/message_verification.h
 *        \brief  Message
 *
 *      \details  This file provides the SOME/IP protocol type and constant definitions.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_VERIFICATION_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_VERIFICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/message.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief General message verification class for SOME/IP method requests / responses, events and errors.
 * \tparam MessageType Contains the data to verify against according to the protocol specification.
 * E.g.: The expected return code and the expected message type.
 * If there are specific measures for a concrete message type, the derived
 * class shall override the behavior from the base.
 */
template <typename MessageType>
class MessageVerification {
 public:
  /**
   * \brief Executes all the checks specified according the AUTOSAR COM_SWS specification.
   *
   * \param header_expected Expected SOME/IP header from the deployment.
   * \param header_request SOME/IP header of the method request.
   * \return E_OK if all checks were successful - specific error return code if any of the
   * verification steps failed.
   */
  static someip_posix_common::someip::SomeIpReturnCode DoAllDeploymentChecks(
      const someip_posix_common::someip::SomeIpMessageHeader& header_expected,
      const someip_posix_common::someip::SomeIpMessageHeader& header_request) noexcept {
    someip_posix_common::someip::SomeIpReturnCode return_code{someip_posix_common::someip::SomeIpReturnCode::kOk};

    const bool valid_interface_v{
        VerifyInterfaceVersion(header_expected.interface_version_, header_request.interface_version_)};

    if (!valid_interface_v) {
      return_code = someip_posix_common::someip::SomeIpReturnCode::kWrongInterfaceVersion;
    }

    return return_code;
  }

  /**
   * \brief Verify that the interface version matches the major version of the
   * SOME/IP service interface deployment.
   *
   * \param expected_major_version Major version from the SOME/IP deployment.
   * \param interface_version The SOME/IP interface version to check against.
   * \return true if the interface version from the request SOME/IP header
   * matches the expected one, false if not.
   */
  static bool VerifyInterfaceVersion(someip_posix_common::someip::MajorVersion expected_major_version,
                                     someip_posix_common::someip::InterfaceVersion interface_version) noexcept {
    return (expected_major_version == interface_version);
  }

  /**
   * \brief Do infrastructural checks that do not have any dependencies to the deployment configuration.
   *
   * \param header_request SOME/IP header from the request.
   * \return return code E_OK if all infrastructural checks were successful - specific error return code
   * if one of the following checks could not be verified successfully.
   */
  static someip_posix_common::someip::SomeIpReturnCode DoInfrastructuralChecks(
      const someip_posix_common::someip::SomeIpMessageHeader& header_request) noexcept {
    someip_posix_common::someip::SomeIpReturnCode return_code{someip_posix_common::someip::SomeIpReturnCode::kOk};
    const bool valid_protocol_v{VerifyProtocolVersion(header_request.protocol_version_)};

    if (!valid_protocol_v) {
      return_code = someip_posix_common::someip::SomeIpReturnCode::kWrongProtocolVersion;
    } else {
      const bool valid_length{VerifyPayloadLength(header_request.length_)};
      const bool valid_return_code{MessageType::VerifyReturnCode(header_request.return_code_)};

      if (!valid_length || !valid_return_code) {
        return_code = someip_posix_common::someip::SomeIpReturnCode::kMalformedMessage;
      } else {
        const bool valid_msg_type{VerifyMessageType(header_request.message_type_)};

        if (!valid_msg_type) {
          return_code = someip_posix_common::someip::SomeIpReturnCode::kWrongMessageType;
        } else {
          const bool valid_client_id{MessageType::VerifyClientId(header_request.client_id_)};
          if (!valid_client_id) {
            return_code = someip_posix_common::someip::SomeIpReturnCode::kNotOk;
          }
        }
      }
    }

    return return_code;
  }

  /**
   * \brief Verify the protocol version of the request / response / notification.
   * \remark Override in derived class if necessary.
   * \remark Same for all verification steps (methods and events).
   * \param protocol_version Protocol version from the request.
   * \return true if the protocol version matches with the value from the specification, false if not.
   */
  static bool VerifyProtocolVersion(someip_posix_common::someip::ProtocolVersion protocol_version) noexcept {
    return (protocol_version == kProtocolVersion);
  }

  /**
   * \brief Verify that the length is effectively larger than 7.
   * \remark Override in derived class if necessary.
   * \remark Same for all verification steps (method requests & responses, events and errors).
   * \param length SOME/IP length value from the incoming response / request / notification / error.
   * \return true if the length is greater than the value from the specification, false if not.
   */
  static bool VerifyPayloadLength(someip_posix_common::someip::LengthField length) noexcept {
    return (length >= kMinimumPayloadLength);
  }

  /**
   * \brief Verify that the return code is set to the one specified for this
   * message type.
   * \uptrace PRS_SOMEIP_00058
   * \param return_code The return_code from the SOME/IP response / request / notification / error.
   * \return true if the return code is set to E_OK, false if it's the wrong return code for a request.
   */
  static bool VerifyReturnCode(someip_posix_common::someip::ReturnCode return_code) noexcept {
    return (return_code == MessageType::kReturnCodeExpected);
  }

  /**
   * \brief Verify the message type of this SOME/IP message.
   * \param message_type The message_type of the SOME/IP message.
   * \return true if the message type is the the same as expected, false if it's the wrong message type for a request.
   */
  static bool VerifyMessageType(someip_posix_common::someip::MessageType message_type) noexcept {
    return (message_type == MessageType::kMessageTypeExpected);
  }

  /**
   * \brief Verify the client id of this SOME/IP message.
   * \uptrace PRS_SOMEIP_00702
   * \param client_id The client_id of the SOME/IP message.
   * \return true. All client ids are normally valid.
   */
  static bool VerifyClientId(someip_posix_common::someip::ClientId client_id) noexcept {
    (void)client_id;
    return true;
  }
};

/**
 * \brief Upon reception of a SOME/IP request message certain check shall be conducted.
 * \uptrace SWS_CM_10302
 */
class RequestMessageVerification : public MessageVerification<RequestMessageVerification> {
 public:
  /**
   * \brief Expected return code for a method request. Verify that the return code is set to E_OK (0x00).
   * \uptrace PRS_SOMEIP_00058
   */
  static constexpr someip_posix_common::someip::SomeIpReturnCode kReturnCodeExpected{
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  /**
   * \brief Expected message type for method requests.
   */
  static constexpr someip_posix_common::someip::SomeIpMessageType kMessageTypeExpected{
      someip_posix_common::someip::SomeIpMessageType::kRequest};
};

/**
 * \brief Upon reception of a SOME/IP request & no return message certain check shall be conducted.
 */
class RequestNoReturnMessageVerification : public MessageVerification<RequestNoReturnMessageVerification> {
 public:
  /**
   * \brief Expected return code for a fire & forget request. Verify that the return code is set to E_OK (0x00).
   * \uptrace PRS_SOMEIP_00058
   */
  static constexpr someip_posix_common::someip::SomeIpReturnCode kReturnCodeExpected{
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  /**
   * \brief Expected message type for fire & forget methods.
   */
  static constexpr someip_posix_common::someip::SomeIpMessageType kMessageTypeExpected{
      someip_posix_common::someip::SomeIpMessageType::kRequestNoReturn};
};

/**
 * \brief Class for verification of event messages.
 */
class EventMessageVerification : public MessageVerification<EventMessageVerification> {
 public:
  /**
   * \brief Expected return code for an event notification message.
   * Verify that the return code is set to E_OK (0x00).
   */
  static constexpr someip_posix_common::someip::SomeIpReturnCode kReturnCodeExpected{
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  /**
   * \brief Expected message type for events and field notifications.
   */
  static constexpr someip_posix_common::someip::SomeIpMessageType kMessageTypeExpected{
      someip_posix_common::someip::SomeIpMessageType::kNotification};

  /**
   * \brief Verify the client id of this SOME/IP message.
   * \param client_id The client_id of the SOME/IP events and field notification messages.
   * \return true if client_id is zero, false otherwise.
   */
  static bool VerifyClientId(someip_posix_common::someip::ClientId client_id) noexcept { return (client_id == 0U); }
};

/**
 * \brief Class for verification of response messages.
 */
class ResponseMessageVerification : public MessageVerification<ResponseMessageVerification> {
 public:
  /**
   * \brief Expected return code for a response message. Verify that the return code is set to E_OK (0x00).
   */
  static constexpr someip_posix_common::someip::SomeIpReturnCode kReturnCodeExpected{
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  /**
   * \brief Expected message type for a response message.
   */
  static constexpr someip_posix_common::someip::SomeIpMessageType kMessageTypeExpected{
      someip_posix_common::someip::SomeIpMessageType::kResponse};
};

/**
 * \brief Class for verification of error messages.
 */
class ErrorMessageVerification : public MessageVerification<ErrorMessageVerification> {
 public:
  /**
   * \brief Expected return code for an error response. Verify that the return code is set to E_NOT_OK (0x01).
   * \remark Shall not be 0x00 (E_OK).
   * \uptrace PRS_SOMEIP_00191
   */
  static constexpr someip_posix_common::someip::SomeIpReturnCode kReturnCodeExpected{
      someip_posix_common::someip::SomeIpReturnCode::kNotOk};

  /**
   * \brief Expected message type for error messages.
   */
  static constexpr someip_posix_common::someip::SomeIpMessageType kMessageTypeExpected{
      someip_posix_common::someip::SomeIpMessageType::kError};

  /**
   * \brief For error messages the return code could be within a range of 0x01 to 0x5E.
   */
  static bool VerifyReturnCode(someip_posix_common::someip::ReturnCode return_code) noexcept {
    return ((return_code >= someip_posix_common::someip::SomeIpReturnCode::kNotOk) &&
            (return_code <= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd));
  }
};

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_VERIFICATION_H_
