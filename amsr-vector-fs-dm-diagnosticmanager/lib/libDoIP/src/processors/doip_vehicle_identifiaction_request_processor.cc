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
/**        \file  doip_vehicle_identifiaction_request_processor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_vehicle_identifiaction_request_processor.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

bool DoIPVehicleIdentifiactionRequestProcessor::IsValidPayloadLength(uint32_t length) const { return length == 0; }

void DoIPVehicleIdentifiactionRequestProcessor::HandleMessage(
    const DoIPMessage& msg, osabstraction::io::network::address::SocketAddress& remote_address) const {
  // TODO(viswhn): Implement message handling.
}

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
