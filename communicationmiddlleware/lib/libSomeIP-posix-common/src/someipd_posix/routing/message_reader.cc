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
/**        \file  message_reader.cc
 *        \brief  Message reader
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someipd_posix/routing/message_reader.h"

#include <unistd.h>
#include <cassert>

namespace someip_posix_common {
namespace someipd_posix {
namespace routing {

MessageReader::MessageReader() : bytes_read_{0U}, is_message_available_{false} {}

void MessageReader::Read(osabstraction::io::network::socket::Socket* socket) {
  if (is_message_available_) {
    return;
  }
  if (bytes_read_ < sizeof(MessageHeader)) {
    ReadHeader(socket);
  }
  if (bytes_read_ == sizeof(MessageHeader)) {
    message_.body_.resize(message_.header_.length_);
  }
  if (bytes_read_ >= sizeof(MessageHeader)) {
    if (message_.header_.length_ > 0U) {
      ReadBody(socket);
    }
    if (bytes_read_ == (sizeof(MessageHeader) + message_.header_.length_)) {
      is_message_available_ = true;
      bytes_read_ = 0U;
    }
  }
}

bool MessageReader::IsMessageAvailable() const { return is_message_available_; }

MessageReader::Message MessageReader::GetNextMessage() {
  is_message_available_ = false;
  return std::move(message_);
}

void MessageReader::ReadHeader(osabstraction::io::network::socket::Socket* socket) {
  assert(bytes_read_ < sizeof(MessageHeader));
  const std::size_t n = socket->Receive(reinterpret_cast<std::uint8_t*>(&message_.header_) + bytes_read_,
                                        sizeof(MessageHeader) - bytes_read_);
  bytes_read_ += n;
}

void MessageReader::ReadBody(osabstraction::io::network::socket::Socket* socket) {
  assert(bytes_read_ >= sizeof(MessageHeader) && bytes_read_ < (sizeof(MessageHeader) + message_.body_.size()));
  std::size_t offset = bytes_read_ - sizeof(MessageHeader);
  std::size_t n = socket->Receive(&message_.body_.at(offset), message_.body_.size() - offset);
  bytes_read_ += n;
}

}  // namespace routing
}  // namespace someipd_posix
}  // namespace someip_posix_common
