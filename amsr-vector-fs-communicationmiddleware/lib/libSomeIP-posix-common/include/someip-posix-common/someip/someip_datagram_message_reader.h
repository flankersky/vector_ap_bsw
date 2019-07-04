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
/**        \file  someip_datagram_message_reader.h
 *        \brief  SOME/IP datagram message reader
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_DATAGRAM_MESSAGE_READER_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_DATAGRAM_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "someip-posix-common/someip/someip_message.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief SomeIpDatagramMessageReader.
 */
class SomeIpDatagramMessageReader {
 public:
  /**
   * \brief Constructor of SomeIpDatagramMessageReader.
   *
   * \param max_datagram_length Maximum length of a datagram.
   */
  explicit SomeIpDatagramMessageReader(std::size_t max_datagram_length);
  /**
   * \brief Tries to read a single message from a file descriptor.
   *
   * The message reader is able to read and buffer only one message at a time.
   *
   * \param socket A socket from which a single message will be read.
   */
  void Read(osabstraction::io::network::socket::Socket* socket);
  /**
   * \brief Indicates whether a complete message has been successfully read.
   *
   * \return true - a complete message is available,
             false - a message is not yet complete.
   */
  bool IsMessageAvailable();
  /**
   * \brief Transfers the ownership of the read message to the caller.
   *
   * This function should be called to fetch the next message if it is available.
   * The message reader can read the next message after returning.
   *
   * \return the next complete message.
   */
  SomeIpMessage GetNextMessage();

 private:
  /**
   * \brief A helper function for reading a datagram.
   *
   * \param socket A socket.
   */
  void ReadDatagram(osabstraction::io::network::socket::Socket* socket);
  /**
   * \brief A helper function for reading the next message contained in the received datagram.
   */
  void ReadMessage();
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Maximum length of a datagram.
   */
  const std::size_t max_datagram_length_;
  /**
   * \brief Number of bytes read so far.
   */
  std::size_t bytes_read_;
  /**
   * \brief Indicates whether a complete message has been read successfully.
   */
  bool is_message_available_;
  /**
   * \brief Stores a single SOME/IP message.
   */
  SomeIpMessage::DataBuffer message_;
  /**
   * \brief Stores a single datagram.
   */
  SomeIpMessage::DataBuffer datagram_;
  /**
   * \brief A remote socket address from which the message came.
   */
  osabstraction::io::network::address::SocketAddress from_address_;
  /**
   * \brief A counter for bad datagrams.
   */
  std::size_t bad_datagram_counter_;
};

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_DATAGRAM_MESSAGE_READER_H_
