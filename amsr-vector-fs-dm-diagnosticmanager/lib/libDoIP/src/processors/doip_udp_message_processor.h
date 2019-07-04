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
/**        \file  doip_udp_message_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_UDP_MESSAGE_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_UDP_MESSAGE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "doip_message.h"
#include "osabstraction/io/network/address/socket_address.h"
#include "processors/doip_payload_type_info.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

class DoIPUDPHandler;  // forward declaration

namespace processors {

/**
 * \brief Base class for MessageProcessor designed for handling messages in context of UDP socket.
 */
class DoIPUdpMessageProcessor : public DoIPPayloadTypeInfo {
 public:
  /**
   * \brief Unique pointer to DoIPUdpMessageProcessor.
   */
  using Ptr = std::unique_ptr<DoIPUdpMessageProcessor>;

  /**
   * \brief Constructor
   */
  DoIPUdpMessageProcessor(PayloadType payload_type, DoIPUDPHandler& udp_handler)
      : DoIPPayloadTypeInfo(payload_type), udp_handler_(udp_handler) {}

  /**
   * \brief Default destructor.
   */
  virtual ~DoIPUdpMessageProcessor() = default;

  /**
   * \brief Handles the given DoIP message.
   *
   * \param msg The DoIP message to be processed.
   * \param remote_address The remote address from which the message was received.
   */
  virtual void HandleMessage(const DoIPMessage& msg,
                             osabstraction::io::network::address::SocketAddress& remote_address) const = 0;

 private:
  /**
   * \brief The UPD Handler which gives the context of message processing.
   */
  DoIPUDPHandler& udp_handler_;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_UDP_MESSAGE_PROCESSOR_H_
