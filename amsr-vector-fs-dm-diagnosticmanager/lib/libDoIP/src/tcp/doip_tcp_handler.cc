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
/**        \file  doip_tcp_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>

#include "doip_tcp_handler.h"
#include "doip_transport_protocol_handler.h"
#include "processors/doip_tcp_nack_processor.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace tcp {

DoIPTCPHandler::DoIPTCPHandler(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
                               const amsr::diag::configuration::DoIPChannelConfiguration& doip_channel_configuration,
                               DoIPTransportProtocolHandler& transport_protocol_handler, const char* port)
    : DoIPHandler(doip_channel_configuration),
      tcp_socket_acceptor_(osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
                               doip_channel_configuration.tcp_channel_configuration.tcp_ip_address, port),
                           *this),
      reactor_(reactor),
      timer_manager_(timer_manager),
      transport_protocol_handler_(transport_protocol_handler),
      state_pool_(),
      routing_activation_processor_(PayloadType::kRoutingActivationRequest),
      alive_check_processor_(PayloadType::kAliveCheckResponse),
      diagnostic_message_processor_(PayloadType::kDiagnosticMessage),
      nack_processor_(PayloadType::kGenericNegativeAcknowledge) {
  max_tcp_channels_ = doip_channel_configuration.tcp_channel_configuration.max_tcp_channels;

  // reserving the max. no. of channels + 1 channel to handle the initialized state that waits for the Alive check
  // to other channels to be processed
  channels_.reserve(max_tcp_channels_ + 1);
  for (uint8_t index = 0; index < (max_tcp_channels_ + 1); index++) {
    channels_.emplace_back(reactor, timer_manager, *this, state_pool_, kDefaultChannelID);
  }

  last_used_id_ = kDefaultChannelID;

  // add the target addresses that the DoIP can communicate with (known to it).
  known_target_addresses_.reserve(doip_channel_configuration.vector_target_address.size());
  for (uint32_t index = 0; index < doip_channel_configuration.vector_target_address.size(); index++) {
    known_target_addresses_.push_back(doip_channel_configuration.vector_target_address[index]);
  }

  reactor_->RegisterEventHandler(tcp_socket_acceptor_.GetHandle(), &tcp_socket_acceptor_,
                                 osabstraction::io::EventType::kReadEvent, nullptr);
}

udstransport::doip::DoIPChannel* DoIPTCPHandler::TryGetListeningDoIPChannel() {
  for (vac::container::StaticList<DoIPChannel>::reference channel : channels_) {
    if (channel.GetState()->GetHandle() == tcp::StateHandle::kListen) {
      return &channel.GetContext();
    }
  }
  return nullptr;
}

uint8_t DoIPTCPHandler::GetNoEstablishedSockets() {
  uint8_t no_established_sockets = 0;
  for (vac::container::StaticList<DoIPChannel>::reference channel : channels_) {
    if (channel.GetState()->GetHandle() != tcp::StateHandle::kListen) {
      no_established_sockets++;
    }
  }
  return no_established_sockets;
}

bool DoIPTCPHandler::IsSourceAddressAlreadyRegistered(uint16_t source_address) {
  vac::container::StaticList<DoIPChannel>::iterator result =
      std::find_if(channels_.begin(), channels_.end(), [source_address](DoIPChannel& channel) {
        return ((channel.GetState()->GetHandle() != tcp::StateHandle::kListen) &&
                (channel.GetSourceAddress() == source_address));
      });
  return (result != channels_.end());
}

void DoIPTCPHandler::CreateDoIPChannel(osabstraction::io::network::socket::TCPSocket&& tcp_socket) {
  ara::log::LogDebug() << "CreateTcpConnection from new socket.";
  DoIPChannel* channel = this->TryGetListeningDoIPChannel();
  if (channel != nullptr) {
    ara::log::LogDebug() << "Attaching to an Initialized channel";
    // change state to be initialized
    bool is_success_to_transit_to_state = channel->TryChangeState(StateHandle::kInitialized);
    assert(is_success_to_transit_to_state);
    channel->RegisterTCPSocket(tcp_socket);
  } else {
    // No Connection is available. Close socket as this can't be reacted on.
    tcp_socket.Close();
  }
}

bool DoIPTCPHandler::IsSupportedPayloadType(PayloadType payload_type) {
  return GetSupportedMessageProcessor(payload_type) != nullptr;
}

bool DoIPTCPHandler::IsValidPayloadLength(PayloadType payload_type, uint32_t payload_length) {
  const doip::processors::DoIPTcpMessageProcessor* processor = GetSupportedMessageProcessor(payload_type);
  if (processor != nullptr) {
    return processor->IsValidPayloadLength(payload_length);
  } else {
    return false;
  }
}

const doip::processors::DoIPTcpMessageProcessor* DoIPTCPHandler::GetSupportedMessageProcessor(
    PayloadType payload_type) {
  switch (payload_type) {
    case PayloadType::kAliveCheckResponse:
      return &alive_check_processor_;
    case PayloadType::kDiagnosticMessage:
      return &diagnostic_message_processor_;
    case PayloadType::kRoutingActivationRequest:
      return &routing_activation_processor_;
    case PayloadType::kGenericNegativeAcknowledge:
      return &nack_processor_;
    default:
      return nullptr;
  }
}

void DoIPTCPHandler::Transmit(ara::diag::udstransport::UdsMessage::Ptr message,
                              ara::diag::udstransport::ChannelID channel_id) {
  if (DoIPChannel* channel = GetChannelById(channel_id)) {
    channel->Transmit(std::move(message));
  } else {
    transport_protocol_handler_.GetUdsTransportProtocolMgr().TransmitConfirmation(
        std::move(message), ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult::kTransmitFailed);
  }
}

DoIPChannel* DoIPTCPHandler::GetChannelById(ara::diag::udstransport::ChannelID channel_id) {
  vac::container::StaticList<DoIPChannel>::iterator result =
      std::find_if(channels_.begin(), channels_.end(), [channel_id](DoIPChannel& channel) {
        return ((channel.GetState()->GetHandle() == tcp::StateHandle::kRegisteredRoutingActive) &&
                (channel.GetChannelId() == channel_id));
      });
  if (result != channels_.end()) {
    return &*result;
  }
  return nullptr;
}

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
