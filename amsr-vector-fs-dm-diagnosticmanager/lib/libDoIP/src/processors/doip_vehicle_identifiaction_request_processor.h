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
/**        \file  doip_vehicle_identifiaction_request_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_VEHICLE_IDENTIFIACTION_REQUEST_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_VEHICLE_IDENTIFIACTION_REQUEST_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "doip_message.h"
#include "processors/doip_udp_message_processor.h"
#include "udp/doip_udp_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

/**
 * \brief Message processor for DoIP message VehicleIdentificationRequest.
 */
class DoIPVehicleIdentifiactionRequestProcessor : public DoIPUdpMessageProcessor {
 public:
  /**
   * \brief Constructor
   */
  explicit DoIPVehicleIdentifiactionRequestProcessor(DoIPUDPHandler& udp_handler)
      : DoIPUdpMessageProcessor(PayloadType::kVehicleIdentificationRequest, udp_handler) {}

  /**
   * \brief Default destructor.
   */
  virtual ~DoIPVehicleIdentifiactionRequestProcessor() = default;

  bool IsValidPayloadLength(uint32_t length) const override;

  void HandleMessage(const DoIPMessage& msg,
                     osabstraction::io::network::address::SocketAddress& remote_address) const override;
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_VEHICLE_IDENTIFIACTION_REQUEST_PROCESSOR_H_
