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
/**        \file  doip_channel.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <utility>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket_eof_exception.h"

#include "channelreader/doip_header_reader.h"
#include "common/multi_byte_type.h"
#include "doip_channel.h"
#include "doip_message.h"
#include "doip_transport_protocol_handler.h"
#include "processors/doip_diagnostic_message_processor.h"
#include "tcp/doip_channel_state_pool.h"
#include "tcp/doip_tcp_handler.h"
#include "udstransport/uds_transport_protocol_manager.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

DoIPChannel::DoIPChannel(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
                         tcp::DoIPTCPHandler& tcp_handler, tcp::DoIPChannelStatePool& state_pool,
                         ara::diag::udstransport::ChannelID channel_id)
    : vac::statemachine::StateOwner<tcp::DoIPChannelState>::StateOwner(state_pool, tcp::StateHandle::kListen),
      reactor_(reactor),
      timer_manager_(timer_manager),
      tcp_handler_(tcp_handler),
      buffer_(nullptr, 0),
      inactivity_timer_(*this, timer_manager_),
      channel_id_(channel_id),
      uds_message_(nullptr),
      header_reader_(*this),
      non_diag_reader_(*this),
      pre_diag_reader_(*this),
      diag_reader_(*this) {
  doip_msg_.payload.resize(kGenericHeaderNoOfBytes + kMaxNonDiagnosticTCPMessageSize);
  doip_msg_ptr_ = vac::language::make_unique<DoIPMessage>(doip_msg_);
  params_from_readers_.number_of_bytes_to_discard = channelreader::ContextNeededParams::kNothingToDiscard;
  params_from_readers_.action = channelreader::ActionForReader::kFinishedReading;
}

DoIPChannel::~DoIPChannel() {
  if (tcp_socket_.has_value()) {
    CloseTCPSocket();
  }
}

void DoIPChannel::RegisterTCPSocket(osabstraction::io::network::socket::TCPSocket& tcp_socket) {
  tcp_socket_.emplace(std::move(tcp_socket));
  reactor_->RegisterEventHandler(GetTCPSocket().GetHandle(), this, osabstraction::io::EventType::kReadEvent, nullptr);
}

void DoIPChannel::ResetDoIPChannel() {
  StopInactivityTimer();                    // Stop the currently running Inactivity timer (if still running).
  CloseTCPSocket();                         // Close the TCP socket.
  NotifyTransportProtocolMgrWithFailure();  // Notify the Transport Protocol Manager with UDS message reception failure,
                                            // if any.
  ResetReader();                            // Read the socket reader.
  SetSourceAddress(kSourceAddressInit);     // Set the source address to the default one that no tester can use.
  SetChannelId(tcp::kDefaultChannelID);     // Set the channel ID to the default value.
  diagnostic_response_code_ = processors::DiagnosticMessageResponseCode::kReserved;
}

void DoIPChannel::CloseTCPSocket() {
  if (tcp_socket_.has_value()) {
    int socket_handle = GetTCPSocket().GetHandle();
    GetTCPSocket().Close();
    reactor_->UnregisterEventHandler(socket_handle, osabstraction::io::EventType::kReadEvent);
    tcp_socket_.reset();
  }
}

void DoIPChannel::NotifyTransportProtocolMgrWithFailure() {
  if ((active_reader_ == &diag_reader_) || (active_reader_ == &pre_diag_reader_)) {
    if (uds_message_) {
      ara::diag::udstransport::UdsTransportProtocolMgr& transport_protcol_mgr =
          tcp_handler_.GetConnectionProvider().GetUdsTransportProtocolMgr();
      transport_protcol_mgr.NotifyMessageFailure(std::move(uds_message_));
    }
  }
}

void DoIPChannel::SetNextReader(ReaderType reader_type, Ptr doip_msg) {
  assert(doip_msg);
  doip_msg_ptr_ = std::move(doip_msg);
  switch (reader_type) {
    case ReaderType::kHeaderReader:
      active_reader_ = &header_reader_;
      break;
    case ReaderType::kNonDiagnosticMessageReader:
      active_reader_ = &non_diag_reader_;
      break;
    case ReaderType::kPreDiagnosticMessageReader:
      active_reader_ = &pre_diag_reader_;
      break;
    case ReaderType::kDiagnosticMessageReader:
      active_reader_ = &diag_reader_;
      break;
    default:
      throw std::runtime_error("Invalid DoIP Channel active reader");
  }
}

bool DoIPChannel::HandleRead(int handle) {
  ara::log::LogDebug() << "DoIPChannel::HandleRead called (handle='" << handle << "')";

  // The active reader should always be assigned to a valid reader.
  if (active_reader_ == nullptr) {
    throw std::runtime_error("DoIPChannel::HandleRead : active reader is nullptr");
  }

  if (AllObsoleteBytesAreDiscardedFromSocket()) {
    if (NewReaderSetUponFirstCallOrDoIPMessageWasJustFinished()) {
      InitializeReader();
    }
    if (!TryToFillBufferRequestedFromReaderWithBytesFromSocket()) {
      TryChangeState(tcp::StateHandle::kFinalize);
      return IsValid();
    }
    if (BufferFromReaderIsFull()) {
      params_from_readers_ = active_reader_->Evaluate();
      if (NewReaderIsSetWhileReadingTheMessage()) {
        InitializeReader();
      }
      if (AllObsoleteBytesAreDiscardedFromSocket()) {
        TryToFinalizeDoIPMessageProcessing(params_from_readers_.action);
      }
    }
  } else {
    TryToDiscardAllObsoleteBytesFromTheSocket();
    if (AllObsoleteBytesAreDiscardedFromSocket()) {
      TryToFinalizeDoIPMessageProcessing(params_from_readers_.action);
    }
  }

  return IsValid();
}

void DoIPChannel::InitializeReader() {
  assert(doip_msg_ptr_);
  buffer_ = active_reader_->Initialize(std::move(doip_msg_ptr_));
  bytes_read_ = 0;
  params_from_readers_.action = channelreader::ActionForReader::kContinueReading;
}

ActionUponDiagnosticMessageHandler DoIPChannel::InvokeDiagnosticMessagePreprocessor(const DoIPMessage& msg) {
  return processors::DoIPDiagnosticMessageProcessor::DiagnosticMessagePreprocessor(msg, *this);
}

void DoIPChannel::SetUdsMessage(ara::diag::udstransport::UdsMessage::Ptr uds_message) {
  uds_message_ = std::move(uds_message);
}

ara::diag::udstransport::UdsMessage::Ptr DoIPChannel::ReleaseUdsMessage() { return std::move(uds_message_); }

bool DoIPChannel::ReadSpecificLengthFromConnection(std::size_t size_to_read, std::size_t position_to_write) {
  assert(buffer_.data());
  assert(buffer_.size() >= size_to_read);
  bool connection_status = true;
  try {
    std::size_t n = GetTCPSocket().Receive(&buffer_.at(position_to_write), size_to_read);
    bytes_read_ += n;
  } catch (const std::system_error& e) {
    ara::log::LogDebug() << "DoIPChannel::" << __func__ << e.code().value() << " - " << e.what();
    ara::log::LogDebug() << "Connection suddenly closed while reading message Payload.";
    connection_status = false;
  } catch (const osabstraction::io::network::socket::SocketEOFException& e) {
    ara::log::LogDebug() << "DoIPChannel::" << __func__ << " channel closed by Tester.";
    connection_status = false;
  }
  return connection_status;
}

void DoIPChannel::ResetReader() {
  active_reader_->Reset();
  bytes_read_ = 0;
  buffer_ = ara::diag::udstransport::ByteVector(nullptr, 0);
  params_from_readers_.action = channelreader::ActionForReader::kFinishedReading;
  params_from_readers_.number_of_bytes_to_discard = channelreader::ContextNeededParams::kNothingToDiscard;
}

void DoIPChannel::CreateNACK(DoIPNACKCode nack_code) {
  doip_msg_.protocol_version = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  doip_msg_.inv_protocol_version = static_cast<uint8_t>(~doip_msg_.protocol_version);
  doip_msg_.payload_type = PayloadType::kGenericNegativeAcknowledge;
  doip_msg_.payload_length = doip::kGenericNegativeAcknowledgeLength;
  doip_msg_.payload[0] = nack_code;
}

void DoIPChannel::StartInactivityTimer(Clock::duration timeout) {
  StopInactivityTimer();
  inactivity_timer_.SetOneShot(timeout);
  inactivity_timer_.Start();
}

void DoIPChannel::StopInactivityTimer() { inactivity_timer_.Stop(); }

tcp::DoIPTCPHandler& DoIPChannel::GetTcpHandler() { return tcp_handler_; }

void DoIPChannel::SetSourceAddress(uint16_t source_address) { source_address_ = source_address; }

uint16_t DoIPChannel::GetSourceAddress() { return source_address_; }

DoIPChannelReaderContext::Ptr DoIPChannel::CreateRoutingActivationResponse(
    uint16_t source_address, processors::RoutingActivationResponseCode response_code) {
  ara::log::LogDebug() << __func__;
  Ptr response_msg = vac::language::make_unique<DoIPMessage>();
  response_msg->payload.resize(kRoutingResponseMessagePayloadSize);

  response_msg->protocol_version = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  response_msg->inv_protocol_version = static_cast<uint8_t>(~response_msg->protocol_version);
  response_msg->payload_type = PayloadType::kRoutingActivationResponse;
  response_msg->payload_length = doip::kRoutingActivationResponseLength;
  // SA to be at 0&1, TA to be at 2&3 (Waiting for configuration to implement it)
  response_msg->payload[0] = amsr::diag::common::GetByte(source_address, 1);  // SA first byte
  response_msg->payload[1] = amsr::diag::common::GetByte(source_address, 0);  // SA second byte
  DoIPTransportProtocolHandler& connection_provider = tcp_handler_.GetConnectionProvider();
  uint16_t primary_target_address = connection_provider.GetPrimaryTargetAddress();
  response_msg->payload[2] = amsr::diag::common::GetByte(primary_target_address, 1);  // TA first byte
  response_msg->payload[3] = amsr::diag::common::GetByte(primary_target_address, 0);  // TA second byte
  response_msg->payload[4] = static_cast<uint8_t>(response_code);
  response_msg->payload[5] = kReservedValue;
  response_msg->payload[6] = kReservedValue;
  response_msg->payload[7] = kReservedValue;
  response_msg->payload[8] = kReservedValue;
  return std::move(response_msg);
}

DoIPChannelReaderContext::Ptr DoIPChannel::CreateDiagnosticMessageResponse(
    uint16_t source_address, uint16_t target_address, processors::DiagnosticMessageResponseCode response_code) {
  ara::log::LogDebug() << __func__;
  Ptr response_msg = vac::language::make_unique<DoIPMessage>();
  response_msg->payload.resize(kDiagResponseMessagePayloadSize);

  response_msg->protocol_version = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  response_msg->inv_protocol_version = static_cast<uint8_t>(~response_msg->protocol_version);
  if (response_code == processors::DiagnosticMessageResponseCode::kConfirmationAck) {
    response_msg->payload_type = PayloadType::kDiagnosticMessagePositiveAck;
    response_msg->payload_length = doip::kDiagnosticMessagePositiveAckMinimumLength;
  } else {
    response_msg->payload_type = PayloadType::kDiagnosticMessageNegativeAck;
    response_msg->payload_length = doip::kDiagnosticMessageNegativeAckMinimumLength;
  }

  // TA to be at 0&1, SA to be at 2&3.
  response_msg->payload[0] = amsr::diag::common::GetByte(target_address, 1);  // TA first byte
  response_msg->payload[1] = amsr::diag::common::GetByte(target_address, 0);  // TA second byte
  response_msg->payload[2] = amsr::diag::common::GetByte(source_address, 1);  // SA first byte
  response_msg->payload[3] = amsr::diag::common::GetByte(source_address, 0);  // SA second byte
  response_msg->payload[4] = static_cast<uint8_t>(response_code);

  // Append the configured number of bytes of the received message. This is currently not supported.
  // Diagnostic Message positive/negative acknowledge structures (tables 28 & 30 in the standard).

  return std::move(response_msg);
}

void DoIPChannel::SendResponse() {
  // lock transmission to avoid parallel transmits on this channel
  ara::log::LogDebug() << __func__;
  std::unique_lock<std::mutex> lk(transmit_mutex_);
  try {
    tcp_handler_.SendResponse(GetTCPSocket(), doip_msg_);
  } catch (const std::system_error& e) {
    ara::log::LogError() << "Error: " << e.code().value() << " - " << e.what();
    ara::log::LogError() << "Connection suddenly closed while Sending Response.";
  }
}

void DoIPChannel::SendResponse(DoIPMessage& response_msg) {
  // lock transmission to avoid parallel transmits on this channel
  std::unique_lock<std::mutex> lk(transmit_mutex_);
  try {
    tcp_handler_.SendResponse(GetTCPSocket(), response_msg);
  } catch (const std::system_error& e) {
    ara::log::LogError() << "Error: " << e.code().value() << " - " << e.what();
    ara::log::LogError() << "Connection suddenly closed while Sending Response.";
  }
}

void DoIPChannel::Transmit(ara::diag::udstransport::UdsMessage::Ptr message) {
  // lock transmission to avoid parallel transmits on this channel
  ara::log::LogDebug() << "DoIPChannel::" << __func__;
  std::unique_lock<std::mutex> lk(transmit_mutex_);
  std::array<uint8_t, 12> buffer;
  buffer[0] = DoIPProtocolVersion::kDoIpIsoDis13400_2_2012;
  buffer[1] = static_cast<uint8_t>(~DoIPProtocolVersion::kDoIpIsoDis13400_2_2012);
  buffer[2] = amsr::diag::common::GetByte(PayloadType::kDiagnosticMessage, 1);
  buffer[3] = amsr::diag::common::GetByte(PayloadType::kDiagnosticMessage, 0);

  assert(message);

  // Payload size = UDS message size + SA & TA sizes.
  uint32_t payload_size = static_cast<uint32_t>(message->GetPayload().size()) + kSaTaSize;
  buffer[4] = amsr::diag::common::GetByte(payload_size, 3);
  buffer[5] = amsr::diag::common::GetByte(payload_size, 2);
  buffer[6] = amsr::diag::common::GetByte(payload_size, 1);
  buffer[7] = amsr::diag::common::GetByte(payload_size, 0);

  buffer[8] = amsr::diag::common::GetByte(message->GetSa(), 1);
  buffer[9] = amsr::diag::common::GetByte(message->GetSa(), 0);

  buffer[10] = amsr::diag::common::GetByte(message->GetTa(), 1);
  buffer[11] = amsr::diag::common::GetByte(message->GetTa(), 0);

  std::size_t bytes_transmitted;
  std::array<iovec, 2> data_to_transmit{
      {{buffer.data(), buffer.size()}, {message->GetPayload().data(), message->GetPayload().size()}}};

  try {
    bytes_transmitted =
        GetTCPSocket().Send(osabstraction::io::network::socket::Socket::IovecContainer(data_to_transmit));
  } catch (const std::system_error& e) {
    ara::log::LogError() << "Error: " << e.code().value() << " - " << e.what();
    ara::log::LogError() << "Connection suddenly closed while Sending a Diagnostic Message.";
    bytes_transmitted = -1;
  }

  // Call to TransmitConfirmation in the UdsTransportProtocolManager for status.
  ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult result;
  if (bytes_transmitted == (message->GetPayload().size() + buffer.size())) {
    result = ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult::kTransmitOk;
  } else {
    result = ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult::kTransmitFailed;
  }

  tcp_handler_.GetConnectionProvider().GetUdsTransportProtocolMgr().TransmitConfirmation(std::move(message), result);
}

std::size_t DoIPChannel::FlushSocket(std::size_t bytes_to_flush) {
  std::array<std::uint8_t, kBufferSizeForDiscardingMessages> buffer;
  if (bytes_to_flush > kBufferSizeForDiscardingMessages) {
    bytes_to_flush = kBufferSizeForDiscardingMessages;
  }
  std::size_t discarded_bytes = 0;
  try {
    discarded_bytes = GetTCPSocket().Receive(buffer.begin(), bytes_to_flush);
    assert(discarded_bytes <= bytes_to_flush);
  } catch (const std::system_error& e) {
    ara::log::LogDebug() << "DoIPChannel::" << __func__ << e.code().value() << " - " << e.what();
    ara::log::LogDebug() << "Connection suddenly closed while reading message Payload.";
  } catch (const osabstraction::io::network::socket::SocketEOFException& e) {
    ara::log::LogDebug() << "DoIPChannel::" << __func__ << " channel closed by Tester.";
  }
  return discarded_bytes;
}

bool DoIPChannel::NewReaderSetUponFirstCallOrDoIPMessageWasJustFinished() const {
  return params_from_readers_.action == channelreader::ActionForReader::kFinishedReading;
}

bool DoIPChannel::NewReaderIsSetWhileReadingTheMessage() const {
  bool ret = (params_from_readers_.action == channelreader::ActionForReader::kContinueReading) &&
             AllObsoleteBytesAreDiscardedFromSocket();
  return ret;
}

void DoIPChannel::TryToDiscardAllObsoleteBytesFromTheSocket() {
  params_from_readers_.number_of_bytes_to_discard -= FlushSocket(params_from_readers_.number_of_bytes_to_discard);
}

bool DoIPChannel::TryToFillBufferRequestedFromReaderWithBytesFromSocket() {
  const std::size_t bytes_to_fill_within_the_buffer = buffer_.size() - bytes_read_;
  return ReadSpecificLengthFromConnection(bytes_to_fill_within_the_buffer, bytes_read_);
}

bool DoIPChannel::TryToFinalizeDoIPMessageProcessing(channelreader::ActionForReader action) {
  switch (action) {
    case channelreader::ActionForReader::kCloseSocket:
      TryChangeState(tcp::StateHandle::kFinalize);
      ResetReader();
      return true;
    case channelreader::ActionForReader::kFinishedReading:
      GetState()->HandleMessage(*doip_msg_ptr_, GetContext());
      ResetReader();
      return true;
    default:
      return false;
  }
}

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
