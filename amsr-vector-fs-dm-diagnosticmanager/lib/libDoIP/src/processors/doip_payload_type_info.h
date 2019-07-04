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
/**        \file  doip_payload_type_info.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_PAYLOAD_TYPE_INFO_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_PAYLOAD_TYPE_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_message.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

/**
 * \brief Base class for Message Processors.
 *
 * A message processor can handle a particular type of DoIP message. The type is determined
 * by the payload type of the DoIP message.
 */
class DoIPPayloadTypeInfo {
 public:
  /**
   * \brief Constructor.
   *
   * \param payload_type The payload type of the DoIP messages that can be handled by given implementation.
   */
  explicit DoIPPayloadTypeInfo(PayloadType payload_type) : payload_type_(payload_type) {}

  /**
   * \brief Default destructor.
   */
  virtual ~DoIPPayloadTypeInfo() = default;

  /**
   * \brief Returns the PayloadType of messages that can be processed by this Processor.
   */
  inline PayloadType GetPayloadType() const { return payload_type_; }

  /**
   * \brief Checks if given length is a valid payloadLength.
   *
   * \param length The payload length to be checked.
   *
   * \returns true, if length is a valid payload length, false otherwise.
   */
  virtual bool IsValidPayloadLength(uint32_t length) const = 0;

 private:
  const PayloadType payload_type_;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_PAYLOAD_TYPE_INFO_H_
