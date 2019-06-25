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
/**        \file  someip_stream_message_reader.h
 *        \brief  SOME/IP stream message reader
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_STREAM_MESSAGE_READER_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_STREAM_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/someip_message.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief SomeIpStreamMessageReader.
 */
class SomeIpStreamMessageReader {
 public:
  /**
   * \brief Constructor of SomeIpStreamMessageReader.
   */
  SomeIpStreamMessageReader();
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
  bool IsMessageAvailable() const;
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
   * \brief A helper function for reading a message's header.
   *
   * \param socket A socket.
   */
  void ReadHeader(osabstraction::io::network::socket::Socket* socket);
  /**
   * \brief A helper function for reading a message's body.
   *
   * \param socket A socket.
   */
  void ReadBody(osabstraction::io::network::socket::Socket* socket);
  /**
   * \brief Number of bytes read so far.
   */
  std::size_t bytes_read_;
  /**
   * \brief Length of SOME/IP message beginning with client identifier.
   */
  LengthField length_;
  /**
   * \brief Indicates whether a complete message has been read successfully.
   */
  bool is_message_available_;
  /**
   * \brief Stores message being read.
   */
  SomeIpMessage::DataBuffer message_;
};

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SOMEIP_STREAM_MESSAGE_READER_H_
