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
/**        \file  doip_transport_protocol_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "doip_message.h"
#include "doip_transport_protocol_handler.h"
#include "osabstraction/io/network/address/ipv4_socket_address.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

/**
 * \brief kInvalidValue used for VIN, GID & EID when they are not yet configured.
 */
constexpr uint8_t kInvalidValue = 0xFF;

/**
 * \brief kNoFurtherAction. Needed for the Vehicle Announcement to indicate that no other actions are needed to initiate
 * central security.
 */
constexpr uint8_t kNoFurtherAction = 0x00;

DoIPTransportProtocolHandler::DoIPTransportProtocolHandler(
    const ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id,
    ara::diag::udstransport::UdsTransportProtocolMgr& transport_protocol_mgr,
    osabstraction::io::ReactorInterface* const reactor, vac::timer::TimerManager* const timer_manager,
    const amsr::diag::configuration::DoIPChannelConfiguration& do_ip_channel_configuration)
    : ara::diag::udstransport::UdsTransportProtocolHandler(handler_id, transport_protocol_mgr),
      reactor_(reactor),
      udp_handler_(reactor, timer_manager, do_ip_channel_configuration, *this),
      tcp_handler_(reactor, timer_manager, do_ip_channel_configuration, *this, kTCPDataPort) {
  primary_ta_address_ = do_ip_channel_configuration.primary_target_address;
}

void DoIPTransportProtocolHandler::Start() {
  ara::log::LogDebug() << "DoIPTransportProtocolHandler: Start";
  /* Setup the ChannelProvider */

  // Sending the Vehicle Announcement.
  // [DoIP-050] Fill the VIN, EID & GID with an Invalid values according to Table 19 & 40 in the DoIP standard.
  std::array<uint8_t, 6> EID;
  EID.fill(kInvalidValue);
  std::array<uint8_t, 6> GID;
  GID.fill(kInvalidValue);
  std::array<uint8_t, 17> VIN;
  VIN.fill(kInvalidValue);
  udp_handler_.VehicleAnnouncementMessage(VIN, EID, GID, kNoFurtherAction);
}

void DoIPTransportProtocolHandler::Stop() {
  ara::log::LogDebug() << "DoIPTransportProtocolHandler: Stop";
  /* Stop the ChannelProvider */
}

void DoIPTransportProtocolHandler::Transmit(ara::diag::udstransport::UdsMessage::Ptr message,
                                            ara::diag::udstransport::ChannelID channel_id) {
  tcp_handler_.Transmit(std::move(message), channel_id);
}

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
