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
/**        \file  doip_udp_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_udp_handler.h"

#include <chrono>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/address/ip_socket_address.h"
#include "osabstraction/io/network/address/socket_address.h"
#include "vac/timer/timer.h"

#include "common/multi_byte_type.h"
#include "doip_transport_protocol_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace udp {

DoIPUDPHandler::DoIPUDPHandler(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
                               const amsr::diag::configuration::DoIPChannelConfiguration& doip_channel_configuration,
                               DoIPTransportProtocolHandler& transport_protocol_handler)
    : DoIPHandler(doip_channel_configuration),
      udp_socket_(osabstraction::io::network::socket::UDPSocket::CreateSocket(
          osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
              doip_channel_configuration.udp_channel_configuration.udp_ip_address, kUDPDiscoveryPort),
          true)),
      reactor_(reactor),
      timer_manager_(timer_manager),
      transport_protocol_handler_(transport_protocol_handler),
      vehicle_announce_timer_(this, timer_manager_),
      vehicle_ident_timer_(this, timer_manager_),
      msg_buf_{0, 0, PayloadType::kGenericNegativeAcknowledge, 0},
      udp_broadcast_ip_address_(doip_channel_configuration.udp_channel_configuration.udp_broadcast_address),
      node_type_(doip_channel_configuration.node_type) {
  msg_buf_.payload.reserve(kMaxUDPMessageSize);
  // Need to register event Handler for the UDP socket on the reator_
  reactor_->RegisterEventHandler(udp_socket_.GetHandle(), this, osabstraction::io::EventType::kReadEvent, nullptr);
  // Fill the map of supported payload type/length pairs.
  // The Configuration file could have new extended types. for adding them, it should be done here
  supported_payload_type_length_[PayloadType::kGenericNegativeAcknowledge] = doip::kGenericNegativeAcknowledgeLength;
  supported_payload_type_length_[PayloadType::kVehicleIdentificationRequest] =
      doip::kVehicleIdentificationRequestLength;
  supported_payload_type_length_[PayloadType::kVehicleIdentificationRequestWithEID] =
      doip::kVehicleIdentificationRequestWithEIDLength;
  supported_payload_type_length_[PayloadType::kVehicleIdentificationRequestWithVIN] =
      doip::kVehicleIdentificationRequestWithVINLength;
  supported_payload_type_length_[PayloadType::kVehicleAnnouncementMessage] = doip::kVehicleAnnouncementMessageLength;
  supported_payload_type_length_[PayloadType::kDoIPEntityStatusRequest] = doip::kDoIPEntityStatusRequestLength;
  supported_payload_type_length_[PayloadType::kDoIPEntityStatusResponse] = doip::kDoIPEntityStatusResponseLength;
  supported_payload_type_length_[PayloadType::kDiagnosticPowerModeInfoRequest] =
      doip::kDiagnosticPowerModeInfoRequestLength;
  supported_payload_type_length_[PayloadType::kDiagnosticPowerModeInfoResponse] =
      doip::kDiagnosticPowerModeInfoResponseLength;
}

bool DoIPUDPHandler::HandleRead(int handle) {
  ara::log::LogDebug() << "DoIPUDPHandler::HandleRead called";

  try {
    osabstraction::io::network::address::SocketAddress remote_address;
    if (!this->PeekMessageHeader()) {
      this->CreateNACK(DoIPNACKCode::kIncorrectPattern);
      uint8_t buffer;
      udp_socket_.Receive(&buffer, 1, &remote_address);
      this->SendResponse(udp_socket_, msg_buf_, &remote_address);
      return IsValid();
    }
    DoIPNACKCode header_status = this->CheckHeader(msg_buf_);
    if (header_status != DoIPNACKCode::kValidHeader) {
      this->CreateNACK(header_status);
      uint8_t buffer;
      udp_socket_.Receive(&buffer, 1, &remote_address);
      this->SendResponse(udp_socket_, msg_buf_, &remote_address);
      return IsValid();
    }
    std::array<uint8_t, kGenericHeaderNoOfBytes> dummy_buffer;
    std::array<struct iovec, 2> local_iovec = {
        {{dummy_buffer.data(), dummy_buffer.size()}, {msg_buf_.payload.data(), msg_buf_.payload_length}}};
    udp_socket_.Receive(osabstraction::io::network::socket::Socket::IovecContainer(local_iovec), &remote_address);

    // a valid request.
    this->ReactOnUDPRequest(remote_address);
  } catch (...) {
    ara::log::LogDebug() << "Error in reading socket ";
  }
  return IsValid();
}

void DoIPUDPHandler::ReactOnUDPRequest(osabstraction::io::network::address::SocketAddress remote_address) {
  // Discard the request if it is Vehicle Identification response or Vehicle Announcement message.
  if (msg_buf_.payload_type == PayloadType::kVehicleIdentificationRequest) {
    // Supporting the VehicleIdentification Request [DoIP-046]
    // use a timer with randomly generated period up to 500ms [DoIP-051].
    srand(static_cast<uint32_t>(time(NULL)));
    std::chrono::milliseconds period{rand() % kADoIPAnnounceWait};
    vehicle_ident_timer_.SetPeriod(period);
    vehicle_ident_timer_.SetCountDown(1);
    vehicle_ident_timer_.SetRemoteAddress(remote_address);
    vehicle_ident_timer_.SetBroadcast(!kBroadcast);
    vehicle_ident_timer_.Start();
  } else if (msg_buf_.payload_type == PayloadType::kVehicleIdentificationRequestWithEID) {
    // Supporting the VehicleIdentification Request [DoIP-047]
    // The EID value should come from an application or a configuration param.
    // So, this should be forked into a separate thread.
    // check the EID firstly. if match, send the Vehicle Identification message after the timer elapse [DoIP-053].
    SendVehicleIdentMessage(!kBroadcast, remote_address);
  } else if (msg_buf_.payload_type == PayloadType::kVehicleIdentificationRequestWithVIN) {
    // The VIN value should come from an application or a Configuration param.
    // So, this should be forked into a separate thread.
    // check the VIN firstly. if match, send the Vehicle Identification message after the timer elapse [DoIP-052].
    SendVehicleIdentMessage(!kBroadcast, remote_address);
  } else if (msg_buf_.payload_type == PayloadType::kDoIPEntityStatusRequest) {
    // SendEntityResponse [DoIP-121].
    // Max. data size param is optional and not supported yet.
    msg_buf_.payload_type = PayloadType::kDoIPEntityStatusResponse;
    msg_buf_.payload_length = doip::kDoIPEntityStatusResponseLength;
    msg_buf_.payload[0] = node_type_;
    doip::tcp::DoIPTCPHandler& tcp_handler = transport_protocol_handler_.GetTcpHandler();
    msg_buf_.payload[1] = amsr::diag::common::GetByte(tcp_handler.GetMaxTCPConnections(), 0);
    msg_buf_.payload[2] = tcp_handler.GetNoEstablishedSockets();
    this->SendResponse(udp_socket_, msg_buf_, &remote_address);
  } else if (msg_buf_.payload_type == PayloadType::kDiagnosticPowerModeInfoRequest) {
    // SendPowerModeInfoResponse [DoIP-118].
    msg_buf_.payload_type = PayloadType::kDiagnosticPowerModeInfoResponse;
    msg_buf_.payload_length = doip::kDiagnosticPowerModeInfoResponseLength;
    // TODO(PAASR-2018) Add API to support providing the Diagnostic power mode.
    // This value should come from an application to indication the current power mode.
    // So, this should be forked into a separate thread.
    // Assumption: the Power mode info will be sent as ready.
    msg_buf_.payload[0] = DoIPDiagPowerModeInfoCode::kReady;
    this->SendResponse(udp_socket_, msg_buf_, &remote_address);
  }
}

bool DoIPUDPHandler::PeekMessageHeader() {
  std::array<uint8_t, kGenericHeaderNoOfBytes> message_header;
  std::size_t read_size = udp_socket_.Peek(message_header.data(), message_header.size());
  if (read_size < kGenericHeaderNoOfBytes) {
    this->CreateNACK(DoIPNACKCode::kIncorrectPattern);
    return false;
  }
  msg_buf_.protocol_version = message_header[0];
  msg_buf_.inv_protocol_version = message_header[1];
  uint16_t payload_type_processor = 0;
  amsr::diag::common::SetByte(payload_type_processor, message_header[3], 0);
  amsr::diag::common::SetByte(payload_type_processor, message_header[2], 1);
  msg_buf_.payload_type = static_cast<PayloadType>(payload_type_processor);

  amsr::diag::common::SetByte(msg_buf_.payload_length, message_header[7], 0);
  amsr::diag::common::SetByte(msg_buf_.payload_length, message_header[6], 1);
  amsr::diag::common::SetByte(msg_buf_.payload_length, message_header[5], 2);
  amsr::diag::common::SetByte(msg_buf_.payload_length, message_header[4], 3);
  return true;
}

// Implements Requirements: DoIP-049, DoIP-050
void DoIPUDPHandler::VehicleAnnouncementMessage(const std::array<uint8_t, 17>& VIN, const std::array<uint8_t, 6>& EID,
                                                const std::array<uint8_t, 6>& GID, uint8_t FurtherAction) {
  ara::log::LogDebug() << "Handling of Vehicle Announcement Message";
  uint16_t logical_address = transport_protocol_handler_.GetPrimaryTargetAddress();
  // need to add API calls to get the VIN, EID & GID instead of getting them as params

  // Construct DoIP Message
  vehicle_announce_buffer_[0] = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  vehicle_announce_buffer_[1] = ~DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  vehicle_announce_buffer_[2] = amsr::diag::common::GetByte(PayloadType::kVehicleAnnouncementMessage, 1);
  vehicle_announce_buffer_[3] = amsr::diag::common::GetByte(PayloadType::kVehicleAnnouncementMessage, 0);
  vehicle_announce_buffer_[4] = 0;
  vehicle_announce_buffer_[5] = 0;
  vehicle_announce_buffer_[6] = 0;
  vehicle_announce_buffer_[7] = kVehicleAnnounceNoOfBytes - kGenericHeaderNoOfBytes;

  for (uint8_t i = 0; i < 17; i++) {
    vehicle_announce_buffer_[i + 8] = VIN[i];
  }
  vehicle_announce_buffer_[25] = amsr::diag::common::GetByte(logical_address, 1);
  vehicle_announce_buffer_[26] = amsr::diag::common::GetByte(logical_address, 0);
  for (uint8_t i = 0; i < 6; i++) {
    vehicle_announce_buffer_[27 + i] = EID.at(i);
  }
  for (uint8_t i = 0; i < 6; i++) {
    vehicle_announce_buffer_[33 + i] = GID.at(i);
  }
  vehicle_announce_buffer_[39] = FurtherAction;

  // Send first Message
  // This is a broadcasting message
  osabstraction::io::network::address::IPSocketAddress remote_address =
      osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(udp_broadcast_ip_address_,
                                                                            kUDPDiscoveryPort);
  this->SendVehicleIdentMessage(kBroadcast, remote_address);

  // Configure Timer
  vehicle_announce_timer_.SetPeriod(kADoIPAnnounceInterval);
  vehicle_announce_timer_.SetCountDown(kADoIPAnnounceNum - 1);
  vehicle_announce_timer_.SetRemoteAddress(remote_address);
  vehicle_announce_timer_.SetBroadcast(kBroadcast);
  vehicle_announce_timer_.Start();
}

void DoIPUDPHandler::SendVehicleIdentMessage(bool broad_cast,
                                             osabstraction::io::network::address::SocketAddress remote_address) {
  try {
    // according to [DoIP-125], No broadcast for sending the Vehicle Identification Response.
    if (!broad_cast) {
      udp_socket_.Send(vehicle_announce_buffer_.data(), vehicle_announce_buffer_.size(), remote_address);
    } else {
      // Give the socket permission to broadcast.
      udp_socket_.SetBroadcast(kBroadcast);

      udp_socket_.Send(vehicle_announce_buffer_.data(), vehicle_announce_buffer_.size(), remote_address);

      // Reset the socket permission to broadcast.
      udp_socket_.SetBroadcast(!kBroadcast);
    }
  } catch (...) {
    ara::log::LogDebug() << "Error in sending message ";
    ara::log::LogDebug() << "Exit with error : " << errno;
  }
}

bool DoIPUDPHandler::IsSupportedPayloadType(PayloadType payload_type) {
  return (supported_payload_type_length_.find(payload_type) != supported_payload_type_length_.end());
}

bool DoIPUDPHandler::IsValidPayloadLength(PayloadType payload_type, uint32_t payload_length) {
  return (this->IsSupportedPayloadType(payload_type) &&
          (supported_payload_type_length_[payload_type] == payload_length));
}

void DoIPUDPHandler::CreateNACK(DoIPNACKCode nack_code) {
  msg_buf_.protocol_version = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  msg_buf_.inv_protocol_version = static_cast<uint8_t>(~msg_buf_.protocol_version);
  msg_buf_.payload_type = PayloadType::kGenericNegativeAcknowledge;
  msg_buf_.payload_length = doip::kGenericNegativeAcknowledgeLength;
  msg_buf_.payload[0] = nack_code;
}

}  // namespace udp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
