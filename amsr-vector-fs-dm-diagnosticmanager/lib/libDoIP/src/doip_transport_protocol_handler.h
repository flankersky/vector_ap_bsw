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
/**        \file  doip_transport_protocol_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_DOIP_TRANSPORT_PROTOCOL_HANDLER_H_
#define LIB_LIBDOIP_SRC_DOIP_TRANSPORT_PROTOCOL_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/reactor_interface.h"
#include "vac/timer/timer_manager.h"

#include "configuration/doip_channel_configuration.h"
#include "tcp/doip_tcp_handler.h"
#include "udp/doip_udp_handler.h"
#include "udstransport/uds_transport_protocol_handler.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

/**
 * \brief kUDPDiscoveryPort.
 */
constexpr char kUDPDiscoveryPort[] = "13400";

/**
 * \brief kTCPDataPort.
 */
constexpr char kTCPDataPort[] = "13400";

/**
 * DoIP implementation of ChannelProvider
 */
class DoIPTransportProtocolHandler : public ara::diag::udstransport::UdsTransportProtocolHandler {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPTransportProtocolHandler(
      const ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id,
      ara::diag::udstransport::UdsTransportProtocolMgr& transport_protocol_mgr,
      osabstraction::io::ReactorInterface* const reactor, vac::timer::TimerManager* const timer_manager,
      const amsr::diag::configuration::DoIPChannelConfiguration& do_ip_channel_configuration);

  /**
   * \brief Destructor.
   */
  virtual ~DoIPTransportProtocolHandler() = default;

  /**
   * \brief Initializes handler.
   *
   * Must be called before Start(). The idea is to have "initialization" of handler-plugin separated from its ctor.
   *
   * \throws  ara::diag::udstransport::UdsTransportProtocolHandlerException if initialization fails.
   */
  void Initialize() override{};

  /**
   * \brief Start providing channels.
   */
  void Start() override;

  /**
   * \brief Stop providing channels.
   */
  void Stop() override;

  /**
   * \brief Tells the UdsTransportProtocolHandler, that it shall notify the DM core
   * via UdsTransportProtocolMgr::ChannelReestablished())
   *
   * \param channel_id channelID, whose re-establishment shall be notified to UdsTransportProtocolMgr
   *
   * \returns true if notification request is accepted and can be fulfilled.
   */
  bool NotifyReestablishment(ara::diag::udstransport::ChannelID channel_id) override { return true; };

  /**
   * \brief Transmit a Uds message via the underlying Uds Transport Protocol channel.
   *
   * \param message The message to be transmitted as a UdsMessage::Ptr.
   *
   * \param channel_id identification of channel on which to transmit.
   *
   */
  void Transmit(ara::diag::udstransport::UdsMessage::Ptr message,
                ara::diag::udstransport::ChannelID channel_id) override;

  /**
   * \brief Returns a reference to the UdsTransportProtocolMgr.
   */
  ara::diag::udstransport::UdsTransportProtocolMgr& GetUdsTransportProtocolMgr() { return transportprotocol_manager_; }

  /**
   * \brief Returns the target primary address.
   */
  uint16_t GetPrimaryTargetAddress() const { return primary_ta_address_; }

  /**
   * \brief Returns the tcp handler.
   */
  tcp::DoIPTCPHandler& GetTcpHandler() { return tcp_handler_; }

 private:
  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* const reactor_;

  /**
   * \brief A UDP requests/responses handler
   */
  udp::DoIPUDPHandler udp_handler_;

  /**
   * \brief A TCP requests/responses handler
   */
  tcp::DoIPTCPHandler tcp_handler_;

  /**
   * \brief The Primary target address of the DoIP.
   */
  uint16_t primary_ta_address_;
};

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_DOIP_TRANSPORT_PROTOCOL_HANDLER_H_
