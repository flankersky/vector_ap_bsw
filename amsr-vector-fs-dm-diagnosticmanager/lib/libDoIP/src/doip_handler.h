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
/**        \file  doip_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_DOIP_HANDLER_H_
#define LIB_LIBDOIP_SRC_DOIP_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket.h"

#include "configuration/doip_channel_configuration.h"
#include "doip_message.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

/**
 * \brief kGenericHeaderNoOfBytes number of bytes in the generic header.
 */
constexpr uint8_t kGenericHeaderNoOfBytes = 8;

/**
 * Base class for UDP & TCP Handlers
 */
class DoIPHandler {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPHandler(const amsr::diag::configuration::DoIPChannelConfiguration& doip_channel_configuration)
      : max_message_size_(doip_channel_configuration.max_message_length) {}
  virtual ~DoIPHandler() = default;

  /**
   * \brief Check the validity of DoIP Generic Header.
   *
   * \param msg The DoIP message that contain the header received and to be checked.
   *
   * \returns one of the NACK code (kValidHeader if a valid Header is sent).
   */
  VIRTUALMOCK DoIPNACKCode CheckHeader(DoIPMessage& msg);

  /**
   * \brief Check if Payload Type is valid.
   *
   * \param payload_type The payload type to be checked.
   *
   * \return true if Payload Type is valid, false otherwise.
   */
  virtual bool IsSupportedPayloadType(PayloadType payload_type) = 0;

  /**
   * \brief Check if the Payload Length is valid.
   *
   * \param payload_type The payload type to which the payload length is checked against.
   * \param payload_length The payload length to be checked.
   *
   * \return true if a valid Payload type / Payload Length pair, false otherwise.
   */
  virtual bool IsValidPayloadLength(PayloadType payload_type, uint32_t payload_length) = 0;

  /**
   * \brief Send response Messages.
   */
  VIRTUALMOCK void SendResponse(osabstraction::io::network::socket::Socket& socket, DoIPMessage& msg,
                                osabstraction::io::network::address::SocketAddress* remote_address = nullptr);

  /**
   * \brief Get the maximum message size which comes from the configuration.
   *
   * \return the maximum message size.
   */
  uint32_t GetMaxMessageSize() { return max_message_size_; }

 private:
  /**
   * \brief The Maximum size of a DoIP Message (This should come from configuration file).
   */
  uint32_t max_message_size_;
};

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_DOIP_HANDLER_H_
