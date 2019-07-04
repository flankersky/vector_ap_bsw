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
/**        \file  someip_message.h
 *        \brief  This class represents a single complete SOME/IP message.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_MESSAGE_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/uio.h>
#include <array>
#include <utility>
#include <vector>
#include "osabstraction/io/network/socket/socket.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "vac/container/array_view.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief SOME/IP message class.
 */
class SomeIpMessage {
 public:
  /**
   * \brief Data buffer type.
   */
  using DataBuffer = someip_posix_common::someip::PacketBuffer;
  /**
   * \brief Socket address.
   */
  using SocketAddress = osabstraction::io::network::address::SocketAddress;
  /**
   * \brief Container of struct iovec.
   */
  using IovecContainer = vac::container::array_view<struct iovec>;
  /**
   * \brief Constructor of SomeIpMessage.
   *
   * \param data A data buffer containing a complete SOME/IP message.
   */
  explicit SomeIpMessage(DataBuffer&& data);
  /**
   * \brief Constructor of SomeIpMessage.
   *
   * \param data A data buffer containing a complete SOME/IP message.
   * \param from_address A socket address of the sender of the SOME/IP message.
   */
  SomeIpMessage(DataBuffer&& data, const SocketAddress& from_address);
  /**
   * \brief Returns the SOME/IP message header of the contained SOME/IP message.
   *
   * \return SOME/IP message header.
   */
  const SomeIpMessageHeader& GetHeader() const { return deserializer_.GetDeserializedHeader(); }
  /**
   * \brief Returns a pointer to the body of the contained SOME/IP message.
   *
   * \return Pointer to SOME/IP message body.
   */
  const std::uint8_t* GetBodyPtr() const { return data_.data() + kHeaderSize; }
  /**
   * \brief Returns the sender's socket address.
   *
   * \return a socket address.
   */
  const std::pair<SocketAddress, bool>& GetFromAddress() const { return from_address_; }
  /**
   * \brief Returns the total size of the contained SOME/IP message with header.
   *
   * \return total SOME/IP message size.
   */
  std::size_t GetTotalSize() const { return data_.size(); }
  /**
   * \brief Returns the total size of the contained SOME/IP message body.
   *
   * \return size of SOME/IP message body.
   */
  std::size_t GetBodySize() const { return data_.size() - kHeaderSize; }
  /**
   * \brief Returns a container of struct iovec which cover the whole SOME/IP message.
   *
   * \return a container of struct iovec.
   */
  IovecContainer GetIovecContainer() { return IovecContainer(io_vector_array_); }

 private:
  /**
   * \brief A data buffer containing a complete SOME/IP message with header.
   */
  DataBuffer data_;
  /**
   * \brief A socket address of the sender of the SOME/IP message.
   */
  const std::pair<SocketAddress, bool> from_address_;
  /**
   * \brief A SOME/IP header deserializer.
   */
  serialization::SomeIpHeaderDeserializer<serialization::BEPayloadNoLengthFieldPolicy> deserializer_;
  /**
   * \brief An array of struct iovec covering the whole SOME/IP message.
   */
  std::array<struct iovec, 1> io_vector_array_;
};

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_MESSAGE_H_
