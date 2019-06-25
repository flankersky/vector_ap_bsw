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
/**        \file  someip_datagram_message_reader.cc
 *        \brief  Datagram message reader
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "someip-posix-common/someip/serialize.h"
#include "someip-posix-common/someip/someip_datagram_message_reader.h"

namespace someip_posix_common {
namespace someip {

SomeIpDatagramMessageReader::SomeIpDatagramMessageReader(std::size_t max_datagram_length)
    : logger_(ara::log::CreateLogger("SomeIpDatagramMessageReader", "")),
      max_datagram_length_(max_datagram_length),
      bytes_read_(0U),
      is_message_available_(false),
      bad_datagram_counter_(0) {}

void SomeIpDatagramMessageReader::Read(osabstraction::io::network::socket::Socket* socket) {
  if (is_message_available_) {
    return;
  }
  ReadDatagram(socket);
}

bool SomeIpDatagramMessageReader::IsMessageAvailable() {
  if (is_message_available_) {
    return true;
  } else {
    ReadMessage();
  }
  return is_message_available_;
}

SomeIpMessage SomeIpDatagramMessageReader::GetNextMessage() {
  assert(is_message_available_);
  SomeIpMessage ret{std::move(message_), from_address_};
  is_message_available_ = false;
  return ret;
}

void SomeIpDatagramMessageReader::ReadDatagram(osabstraction::io::network::socket::Socket* socket) {
  datagram_.resize(max_datagram_length_);
  const std::size_t n = static_cast<typename std::make_unsigned<ssize_t>::type>(
      socket->Receive(datagram_.data(), datagram_.size(), &from_address_));
  logger_.LogVerbose() << __func__ << ":" << __LINE__ << ": datagram length " << n;
  datagram_.resize(n);
  bytes_read_ = 0U;
}

void SomeIpDatagramMessageReader::ReadMessage() {
  assert(!is_message_available_);
  assert(bytes_read_ <= datagram_.size());
  // Calculate number of bytes left to consume
  const auto bytes_left{datagram_.size() - bytes_read_};
  logger_.LogVerbose() << __func__ << ":" << __LINE__ << ": bytes read so far " << bytes_read_
                       << ", bytes left to read " << bytes_left;
  if ((datagram_.size() == 0U) || (bytes_read_ == datagram_.size())) {
    return;
  }
  // A SOME/IP message must be at least of length of SOME/IP message header
  if (bytes_left < kHeaderSize) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": invalid SOME/IP message of length " << bytes_left;
    bytes_read_ = 0U;
    datagram_.resize(0U);
    ++bad_datagram_counter_;
    logger_.LogError() << __func__ << ":" << __LINE__ << ": bad datagrams so far " << bad_datagram_counter_;
    return;
  }
  LengthField length_field{0U};
  // Calculate offset to length field from the beginning of SOME/IP message header
  const auto offset{bytes_read_ + kLengthFieldOffset};
  // Deserialize length field inside SOME/IP header
  serialization::write<serialization::SomeIpMessagePolicy>(length_field, datagram_.begin() + offset);
  // Calculate total length of the SOME/IP message with header
  const auto total_length = CalculateSomeIpMessageLengthFromLengthField(length_field);
  if (bytes_left < total_length) {
    logger_.LogError() << __func__ << ":" << __LINE__ << ": invalid SOME/IP message total length, got " << bytes_left
                       << " expected at least " << total_length;
    bytes_read_ = 0U;
    datagram_.resize(0U);
    ++bad_datagram_counter_;
    logger_.LogError() << __func__ << ":" << __LINE__ << ": bad datagrams so far " << bad_datagram_counter_;
    return;
  }
  // From now on we assume having a valid SOME/IP message
  auto start = datagram_.begin() + static_cast<int>(bytes_read_);
  auto end = start + static_cast<int>(total_length);
  message_ = SomeIpMessage::DataBuffer(start, end);
  bytes_read_ += total_length;
  is_message_available_ = true;
}

}  // namespace someip
}  // namespace someip_posix_common
