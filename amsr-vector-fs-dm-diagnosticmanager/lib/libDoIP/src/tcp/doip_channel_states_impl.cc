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
/**        \file  doip_channel_states_impl.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"

#include "doip_channel.h"
#include "doip_channel_states_impl.h"
#include "processors/doip_tcp_message_processor.h"
#include "tcp/doip_tcp_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace tcp {

bool DoIPChannelStateListen::IsValidChange(const StateHandle handle) const {
  // [DoIP-127]
  if (handle == StateHandle::kInitialized) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateInitialized::OnEnter(DoIPChannel& channel) {
  // [DoIP-0127] start the Initial Inactivity Timer
  channel.StartInactivityTimer(kTCPInitialInactivity);
}

void DoIPChannelStateInitialized::OnLeave(DoIPChannel& channel) {
  // [DoIP-0128] stop the Initial Inactivity Timer
  channel.StopInactivityTimer();
}

bool DoIPChannelStateInitialized::IsValidChange(const StateHandle handle) const {
  // [DoIP-128]
  if ((handle == StateHandle::kFinalize) || (handle == StateHandle::kRegisteredAuthenticationPending)) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateInitialized::HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const {
  if (message.payload_type == PayloadType::kRoutingActivationRequest) {
    // Handle RoutingActivation Request by using the Message processor.
    // Change to state RegisteredAuthenticationPending if the Socket Handler is OK with that.
    ara::log::LogDebug() << "Routing activation Request Received. Call RoutingActivation processor now.";
    const doip::processors::DoIPTcpMessageProcessor* processor =
        channel.GetTcpHandler().GetSupportedMessageProcessor(message.payload_type);
    if (processor != nullptr) {
      channel.StopInactivityTimer();
      processor->HandleMessage(message, channel);
    }
  }
  // if not a RoutingActivationRequest, do nothing.
}

void DoIPChannelStateRegisteredAuthenticationPending::OnEnter(DoIPChannel& channel) {
  channel.StartInactivityTimer(kTCPGeneralInactivity);
  // Authentication registration should be done here.
  channel.TryChangeState(tcp::StateHandle::kRegisteredConfirmationPending);
}

bool DoIPChannelStateRegisteredAuthenticationPending::IsValidChange(const StateHandle handle) const {
  // [DoIP-129]
  if ((handle == StateHandle::kFinalize) || (handle == StateHandle::kRegisteredConfirmationPending)) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateRegisteredAuthenticationPending::HandleMessage(const DoIPMessage& message,
                                                                    DoIPChannel& channel) const {
  ara::log::LogDebug() << "RegisteredAuthenticationPending::HandleMessage is called";
}

void DoIPChannelStateRegisteredConfirmationPending::OnEnter(DoIPChannel& channel) {
  // Confirmation should be done here.
  channel.TryChangeState(tcp::StateHandle::kRegisteredRoutingActive);
}

bool DoIPChannelStateRegisteredConfirmationPending::IsValidChange(const StateHandle handle) const {
  // [DoIP-130]
  if ((handle == StateHandle::kFinalize) || (handle == StateHandle::kRegisteredRoutingActive)) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateRegisteredConfirmationPending::HandleMessage(const DoIPMessage& message,
                                                                  DoIPChannel& channel) const {
  ara::log::LogDebug() << "RegisteredConfirmationPending::HandleMessage is called";
  switch (message.payload_type) {
    case PayloadType::kDiagnosticMessage:
    case PayloadType::kAliveCheckResponse:
      channel.StartInactivityTimer(kTCPGeneralInactivity);
      break;
    case PayloadType::kRoutingActivationRequest:
      // If the SA is different than the one associated, send Routing Activation response with code 0x02 & close the
      // socket. If it is the same, reset the timer.
      channel.StartInactivityTimer(kTCPGeneralInactivity);
      ara::log::LogDebug() << "Received Routing Activation Request";
      break;
    // other message Payload Types will be responded by a Generic header NACK and will not reach here.
    default:
      break;
  }
}

void DoIPChannelStateRegisteredRoutingActive::OnEnter(DoIPChannel& channel) {
  channel.StartInactivityTimer(kTCPGeneralInactivity);
}

bool DoIPChannelStateRegisteredRoutingActive::IsValidChange(const StateHandle handle) const {
  // [DoIP-132]
  if (handle == StateHandle::kFinalize) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateRegisteredRoutingActive::HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const {
  ara::log::LogDebug() << "RegisteredRoutingActive::HandleMessage is called";
  channel.StartInactivityTimer(kTCPGeneralInactivity);
  // The message processor will handle the response to be done regardless of the type of the message.
  const doip::processors::DoIPTcpMessageProcessor* processor =
      channel.GetTcpHandler().GetSupportedMessageProcessor(message.payload_type);
  if (processor != nullptr) {
    processor->HandleMessage(message, channel);
  }
}

bool DoIPChannelStateFinalize::IsValidChange(const StateHandle handle) const {
  // [DoIP-133]
  if (handle == StateHandle::kListen) {
    return true;
  } else {
    return false;
  }
}

void DoIPChannelStateFinalize::OnEnter(DoIPChannel& channel) {
  channel.StopInactivityTimer();
  // [DoIP-133]
  channel.TryChangeState(tcp::StateHandle::kListen);
}

void DoIPChannelStateFinalize::OnLeave(DoIPChannel& channel) {
  // [DoIP-133]
  channel.ResetDoIPChannel();
}

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
