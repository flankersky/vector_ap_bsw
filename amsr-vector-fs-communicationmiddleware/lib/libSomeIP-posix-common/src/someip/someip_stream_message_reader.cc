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
/**        \file  someip_stream_message_reader.cc
 *        \brief  Stream message reader
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "someip-posix-common/someip/serialize.h"
#include "someip-posix-common/someip/someip_stream_message_reader.h"

namespace someip_posix_common {
namespace someip {

SomeIpStreamMessageReader::SomeIpStreamMessageReader() : bytes_read_(0), length_(0), is_message_available_(false) {}

void SomeIpStreamMessageReader::Read(osabstraction::io::network::socket::Socket* socket) {
  if (is_message_available_) {
    return;
  }
  if (bytes_read_ == 0 && message_.size() == 0) {
    message_.resize(kHeaderLength);
  }
  if (bytes_read_ < kHeaderLength) {
    ReadHeader(socket);
  }
  if (bytes_read_ == kHeaderLength) {
    serialization::write<serialization::SomeIpMessagePolicy>(length_, message_.begin() + kLengthFieldOffset);
    message_.resize(kHeaderLength + length_);
  }
  if (bytes_read_ >= kHeaderLength) {
    ReadBody(socket);
    if (bytes_read_ == message_.size()) {
      is_message_available_ = true;
      bytes_read_ = 0;
    }
  }
}

bool SomeIpStreamMessageReader::IsMessageAvailable() const { return is_message_available_; }

SomeIpMessage SomeIpStreamMessageReader::GetNextMessage() {
  is_message_available_ = false;
  length_ = 0;
  return SomeIpMessage(std::move(message_));
}

void SomeIpStreamMessageReader::ReadHeader(osabstraction::io::network::socket::Socket* socket) {
  assert(bytes_read_ < kHeaderLength);
  std::size_t n = socket->Receive(&message_.at(bytes_read_), kHeaderLength - bytes_read_);
  bytes_read_ += n;
}

void SomeIpStreamMessageReader::ReadBody(osabstraction::io::network::socket::Socket* socket) {
  assert(bytes_read_ >= kHeaderLength && bytes_read_ < message_.size());
  std::size_t n = socket->Receive(&message_.at(bytes_read_), message_.size() - bytes_read_);
  bytes_read_ += n;
}

}  // namespace someip
}  // namespace someip_posix_common
