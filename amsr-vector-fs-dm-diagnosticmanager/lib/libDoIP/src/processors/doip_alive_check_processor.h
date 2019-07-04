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
/**        \file  doip_alive_check_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ALIVE_CHECK_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ALIVE_CHECK_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_channel.h"
#include "doip_message.h"
#include "doip_tcp_message_processor.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

/**
 * \brief Message processor for DoIP message Alive Check.
 */
class DoIPAliveCheckProcessor : public DoIPTcpMessageProcessor {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPAliveCheckProcessor(doip::PayloadType payload_type) : DoIPTcpMessageProcessor(payload_type) {}

  virtual ~DoIPAliveCheckProcessor() = default;

  bool IsValidPayloadLength(uint32_t length) const override;

  void HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const override;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_ALIVE_CHECK_PROCESSOR_H_
