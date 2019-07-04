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
/**        \file  did_parser.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_DID_PARSER_H_
#define SRC_SERVER_SERVICE_PROCESSOR_DID_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <assert.h>

#include "common/multi_byte_type.h"
#include "server/data/did_info.h"
#include "vac/container/array_view.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

/**
 * \brief This class parses DIDs from an UDS message payload.
 */
class DidParser {
 public:
  /**
   *
   * \param payload_view payload that shall be parsed.
   */
  explicit DidParser(vac::container::const_array_view<uint8_t> payload_view)
      : payload_view_(payload_view), position_(0) {}

  /**
   * \brief Checks if there are more DIDs in the payload.
   * \return true if there are at least two more bytes in the payload, else false.
   */
  bool HasNextDid() const { return (position_ + 1) < payload_view_.size(); }

  /**
    * \brief Reads the current DID and jumps to the next one.
    * \throws std::out_of_range if current DID would exceed array range. Check with HasNextDidFirst!
    * \return current DID
    */
  data::DidInfo::Did PopNextDid() {
    data::DidInfo::Did nextDid = ReadNextDid();
    position_ += 2;
    return nextDid;
  }

 private:
  /**
   * Returns the DID corresponding to the next two bytes.
   * \remark no range checks performed.
   * \throws std::out_of_range if current DID would exceed array range. Check with HasNextDidFirst!
   * \return the read DID
   */
  data::DidInfo::Did ReadNextDid() const {
    const uint8_t high_byte = payload_view_.at(position_);
    const uint8_t low_byte = payload_view_.at(position_ + 1);
    data::DidInfo::Did result = low_byte;
    amsr::diag::common::SetByte(result, high_byte, 1);
    return result;
  }

  vac::container::const_array_view<uint8_t> payload_view_;
  vac::container::const_array_view<uint8_t>::size_type position_;
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_DID_PARSER_H_
