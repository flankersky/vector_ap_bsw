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
/**        \file  doip_tcp_message_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_TCP_MESSAGE_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_TCP_MESSAGE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "doip_message.h"
#include "processors/doip_payload_type_info.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPChannel;  // forward declaration.
namespace processors {

/**
 * \brief Base class for MessageProcessor designed for handling messages in context of TCP socket.
 */
class DoIPTcpMessageProcessor : public DoIPPayloadTypeInfo {
 public:
  /**
   * \brief Unique pointer to DoIPTcpMessageProcessor.
   */
  using Ptr = std::unique_ptr<DoIPTcpMessageProcessor>;

  /**
   * \brief Constructor.
   */
  explicit DoIPTcpMessageProcessor(PayloadType payload_type) : DoIPPayloadTypeInfo(payload_type) {}

  /**
   * \brief Default destructor.
   */
  virtual ~DoIPTcpMessageProcessor() = default;

  /**
   * \brief Handles the given DoIP message.
   *
   * \param msg The DoIP message to be processed.
   * \param channel The channel determines the context for the message processing.
   */
  virtual void HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const = 0;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_TCP_MESSAGE_PROCESSOR_H_
