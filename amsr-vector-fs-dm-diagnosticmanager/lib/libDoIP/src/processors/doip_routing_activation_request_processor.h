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
/**        \file  doip_routing_activation_request_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ROUTING_ACTIVATION_REQUEST_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ROUTING_ACTIVATION_REQUEST_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_message.h"
#include "doip_tcp_message_processor.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPChannel;  // forward declaration.
namespace processors {

/**
 * \brief Minimum known Source Address (Table 39).
 */
constexpr uint16_t kMinimumKnownSA = 0x0000;

/**
 * \brief Maximum known Source Address (Table 39).
 */
constexpr uint16_t kMaximumKnownSA = 0xFFFF;

/**
 * \brief Default routing activation type.
 */
constexpr uint8_t kDefaultRoutingType = 0;

/**
 * \brief Socket Handler rejected registering a socket.
 */
constexpr bool kSocketRegistrationRejected = false;

/**
 * \brief Socket Handler rejected registering a socket.
 */
constexpr bool kSocketRegistrationAccepted = true;

/**
 * \brief Enumerator for Routing activation response code.
 */
enum class RoutingActivationResponseCode : uint8_t {
  kUnknownSA = 0x00,
  kNoAvailableSocket = 0x01,
  kDifferentSA = 0x02,
  kRegisteredSA = 0x03,
  kMissingAuthetication = 0x04,
  kRejectedConfirmation = 0x05,
  kUnsupportedActivationType = 0x06,
  kSuccessfullyActivated = 0x10,
  kConfirmationRequired = 0x11
};

/**
 * \brief Struct for PerformSocketRegistrationHandling output.
 */
struct SocketRegistrationStatus {
  /**
   * \brief The response code of the Routing activation request output from Socket Handling.
   */
  RoutingActivationResponseCode response_code_;

  /**
   * \brief A flag to indicate if the socket is registered according to Socket Handling.
   */
  bool is_socket_registered_;
};

/**
 * \brief Message processor for DoIP message RoutingActivationRequest.
 */
class DoIPRoutingActivationRequestProcessor : public DoIPTcpMessageProcessor {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPRoutingActivationRequestProcessor(PayloadType payload_type) : DoIPTcpMessageProcessor(payload_type) {}

  virtual ~DoIPRoutingActivationRequestProcessor() = default;

  bool IsValidPayloadLength(uint32_t length) const override;

  void HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const override;

 private:
  /**
   * \brief Perform Socket registration processing on Figure 13 (TCP_DATA socket handler).
   *
   * \param tcp_handler The TCP Handler.
   * \param source_address The source address.
   *
   * \return kSocketRegistrationAccepted if routing is not accepted, kSocketRegistrationRejected otherwise.
   */
  SocketRegistrationStatus PerformSocketRegistrationHandling(DoIPChannel& channel, uint16_t source_address) const;

  /**
   * \brief Check if the SA is known.
   *
   * \param source_address The request source address.
   *
   * \return true if SA is in the known range.
   */
  bool CheckSaIsKnown(uint16_t source_address) const;

  /**
   * \brief Check if the Routing activation type is supported.
   *
   * \param routing_type The Routing Type.
   *
   * \return true if routing type is supported.
   */
  bool CheckRoutingType(uint8_t routing_type) const;

  /**
   * \brief Check the current socket if it already in a registered state.
   *
   * \param channel The channel on which the check will be done.
   *
   * \return true if it is registered, false otherwise.
   */
  bool CheckIfSocketIsRegistered(DoIPChannel& channel) const;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ROUTING_ACTIVATION_REQUEST_PROCESSOR_H_
