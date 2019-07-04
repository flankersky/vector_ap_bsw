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
/**        \file  doip_routing_activation_request_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "doip_channel.h"
#include "doip_routing_activation_request_processor.h"
#include "tcp/doip_channel_state.h"
#include "tcp/doip_tcp_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

bool DoIPRoutingActivationRequestProcessor::IsValidPayloadLength(uint32_t length) const {
  return length == doip::kRoutingActivationRequestLength;
}

void DoIPRoutingActivationRequestProcessor::HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const {
  // Here implement the Routing Activation Handler and TCP_Data Socket Handler
  ara::log::LogDebug() << "Handling Routing activation request";
  // Extract the SA firstly.
  uint16_t source_address = msg.payload[1];
  amsr::diag::common::SetByte(source_address, msg.payload[0], 1);
  RoutingActivationResponseCode response_code;
  // Extract the routing type.
  uint8_t routing_type = msg.payload[2];
  bool close_channel = false;
  if (!CheckSaIsKnown(source_address)) {
    // [DoIP-059]
    response_code = RoutingActivationResponseCode::kUnknownSA;
    close_channel = true;
  } else if (!CheckRoutingType(routing_type)) {
    // [DoIP-151]
    response_code = RoutingActivationResponseCode::kUnsupportedActivationType;
    close_channel = true;
  } else {
    SocketRegistrationStatus socket_status = PerformSocketRegistrationHandling(channel, source_address);
    if (socket_status.is_socket_registered_) {
      // Check if authentication is required
      // Check if confirmation is required
      ara::log::LogDebug() << "Socket is registered.";
      channel.TryChangeState(tcp::StateHandle::kRegisteredAuthenticationPending);
    } else {
      close_channel = true;
    }
    response_code = socket_status.response_code_;
  }
  std::unique_ptr<DoIPMessage> response_msg = channel.CreateRoutingActivationResponse(source_address, response_code);
  channel.SendResponse(*response_msg);
  if (close_channel) {
    channel.TryChangeState(tcp::StateHandle::kFinalize);
  }
}

SocketRegistrationStatus DoIPRoutingActivationRequestProcessor::PerformSocketRegistrationHandling(
    DoIPChannel& channel, uint16_t source_address) const {
  SocketRegistrationStatus socket_registration_status;
  tcp::DoIPTCPHandler& tcp_handler = channel.GetTcpHandler();
  if (tcp_handler.GetNoEstablishedSockets() == 0) {
    socket_registration_status.is_socket_registered_ = kSocketRegistrationAccepted;
  } else if (CheckIfSocketIsRegistered(channel)) {  // Check if socket is already Registered
    if (source_address == channel.GetSourceAddress()) {
      socket_registration_status.is_socket_registered_ = kSocketRegistrationAccepted;
    } else {
      socket_registration_status.is_socket_registered_ = kSocketRegistrationRejected;
      socket_registration_status.response_code_ = RoutingActivationResponseCode::kDifferentSA;
    }
  } else if (tcp_handler.IsSourceAddressAlreadyRegistered(source_address)) {
    // Alive check should be performed (single SA).
    // can generate this response in case of SA is registered to an active socket.
    // channel.SetRoutingResponseCode(RoutingActivationResponseCode::kRegisteredSA);
    ara::log::LogDebug() << "Alive check to be performed on this specific SA according to socket handler";
    // [DoIP-150]
    socket_registration_status.is_socket_registered_ = kSocketRegistrationAccepted;
  } else if (tcp_handler.GetNoEstablishedSockets() >= tcp_handler.GetMaxTCPConnections()) {
    // Alive check should be performed on all registered sockets.
    // can generate this response in case of all sockets are registered and active.
    // channel.SetRoutingResponseCode(RoutingActivationResponseCode::kNoAvailableSocket);
    ara::log::LogDebug() << "Alive check to be performed on All registered sockets according to socket handler";
    // [DoIP-060]
    socket_registration_status.is_socket_registered_ = kSocketRegistrationAccepted;
  } else {
    socket_registration_status.is_socket_registered_ = kSocketRegistrationAccepted;
  }

  if (socket_registration_status.is_socket_registered_) {
    channel.SetSourceAddress(source_address);
    channel.SetChannelId(source_address);
    socket_registration_status.response_code_ = RoutingActivationResponseCode::kSuccessfullyActivated;
  }
  return socket_registration_status;
}

bool DoIPRoutingActivationRequestProcessor::CheckIfSocketIsRegistered(DoIPChannel& channel) const {
  switch (channel.GetState()->GetHandle()) {
    case tcp::StateHandle::kRegisteredAuthenticationPending:
    case tcp::StateHandle::kRegisteredConfirmationPending:
    case tcp::StateHandle::kRegisteredRoutingActive:
      return true;
    default:
      return false;
  }
}

bool DoIPRoutingActivationRequestProcessor::CheckSaIsKnown(uint16_t source_address) const {
  return ((source_address >= kMinimumKnownSA) && (source_address <= kMaximumKnownSA));
}

bool DoIPRoutingActivationRequestProcessor::CheckRoutingType(uint8_t routing_type) const {
  return routing_type == kDefaultRoutingType;
}

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
