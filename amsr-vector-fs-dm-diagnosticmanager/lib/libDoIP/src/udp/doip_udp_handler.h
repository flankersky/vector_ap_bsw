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
/**        \file  doip_udp_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_UDP_DOIP_UDP_HANDLER_H_
#define LIB_LIBDOIP_SRC_UDP_DOIP_UDP_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/event_handler.h>
#include <osabstraction/io/network/socket/udp_socket.h>
#include <osabstraction/io/reactor.h>
#include <sys/time.h>
#include <vac/testing/test_adapter.h>
#include <vac/timer/timer.h>
#include <vac/timer/timer_manager.h>

#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "doip_handler.h"
#include "doip_message.h"
#include "udp/doip_udp_timer.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPTransportProtocolHandler;  // forward declaration.
namespace udp {

/**
 * \brief kADoIPAnnounceNum number of Vehicle Announcement Message to be sent.
 */
constexpr uint8_t kADoIPAnnounceNum = 3;

/**
 * \brief kADoIPAnnounceInterval time between Vehicle Announcement Messages in ms.
 */
constexpr std::chrono::milliseconds kADoIPAnnounceInterval{500};

/**
 * \brief kVehicleAnnounceNoOfBytes number of Vehicle Announcement Message to be sent.
 */
constexpr uint8_t kVehicleAnnounceNoOfBytes = 40;

/**
 * \brief kADoIPAnnounceWait max. of the random interval of the timer used to send the Vehicle identification
 * response.
 */
constexpr uint16_t kADoIPAnnounceWait = 500;

/**
 * \brief kMaxUDPMessageSize the maximum size if UDP message received/sent according to the standard (excluding the 8
 * bytes of the Generic Header).
 */
constexpr uint8_t kMaxUDPMessageSize = 33;

/**
 * \brief kBroadcast flag for broadcasting
 */
constexpr bool kBroadcast = true;

/**
 * DoIP UDP Handler.
 */
class DoIPUDPHandler : public DoIPHandler, public osabstraction::io::EventHandler {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPUDPHandler(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
                          const amsr::diag::configuration::DoIPChannelConfiguration& doip_channel_configuration,
                          DoIPTransportProtocolHandler& transport_protocol_handler);

  /**
   * \brief Destructor.
   */
  virtual ~DoIPUDPHandler() = default;

  /**
   * \brief Construct and send Vehicle Announcement message.
   *
   * \param VIN The Vehicle Identification Number.
   * \param EID The Entity Identification.
   * \param GID The Group Identification.
   * \param FurtherAction The indication to the external tester that a centralized security approach should be used.
   */
  void VehicleAnnouncementMessage(const std::array<uint8_t, 17>& VIN, const std::array<uint8_t, 6>& EID,
                                  const std::array<uint8_t, 6>& GID, uint8_t FurtherAction);

  /**
   * \brief send Vehicle Identification Message.
   *
   * \param remote_address The remote address to which DoIP should sent the message.
   * \param broad_cast Flag to distinguish if it is a broadcasting message or not.
   */
  VIRTUALMOCK void SendVehicleIdentMessage(bool broad_cast,
                                           osabstraction::io::network::address::SocketAddress remote_address);

  /**
   * \brief Handler that is called by reactor whenever a new message is to be received.
   *
   * \param handle The accepting socket handle.
   * \return returns true if the event handler can be called.
   */
  bool HandleRead(int handle) override;

  /**
   * \brief Check if the valid Payload Type.
   *
   * \param payload_type payload type to be checked.
   *
   * \return true of a valid Payload Type, false otherwise.
   */
  VIRTUALMOCK bool IsSupportedPayloadType(PayloadType payload_type) override;

  /**
   * \brief Check if the valid Payload Length.
   *
   * \param payload_length payload length to be checked.
   * \param payload_type The payload type to which the payload length is checked against.
   *
   * \return true of a valid Payload Length, false otherwise.
   */
  bool IsValidPayloadLength(PayloadType payload_type, uint32_t payload_length) override;

  /**
   * \brief Peek a message and check valid DoIP header.
   *
   * \return true if size of Header = 8, false otherwise.
   */
  VIRTUALMOCK bool PeekMessageHeader();

  /**
   * \brief Create NACK Messages.
   *
   * \param nack_code The NACK code to be used.
   */
  VIRTUALMOCK void CreateNACK(DoIPNACKCode nack_code);

 private:
  /**
   * \brief Reacting on the Received UDP Request.
   */
  VIRTUALMOCK void ReactOnUDPRequest(osabstraction::io::network::address::SocketAddress remote_address);
  /**
   * \brief a VehicleAnnouncementMessage payload (stream of bytes).
   */
  std::array<uint8_t, kVehicleAnnounceNoOfBytes> vehicle_announce_buffer_;

  /**
   * \brief the UDP socket opened and to be used
   */
  osabstraction::io::network::socket::UDPSocket udp_socket_;

  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* const reactor_;

  /**
   * \brief Timer manager.
   */
  vac::timer::TimerManager* const timer_manager_;

  /**
   * \brief Connection provider.
   */
  amsr::diag::udstransport::doip::DoIPTransportProtocolHandler& transport_protocol_handler_;

  /**
   * \brief Timer for Vehicle Announcement Message.
   */
  DoIPUDPTimer vehicle_announce_timer_;

  /**
   * \brief Timer for Vehicle Identification Response.
   */
  DoIPUDPTimer vehicle_ident_timer_;

  /**
   * \brief Map for Payload Type/Length available pairs.
   */
  std::map<uint16_t, uint32_t> supported_payload_type_length_;

  /**
   * \brief DoIPMessage to carry the received DoIP Message the NACK or the normal response.
   */
  DoIPMessage msg_buf_;

  /**
   * \brief udp_broadcast_ip_address to hold the Broadcasting UDP IP address.
   */
  std::string udp_broadcast_ip_address_;

  /**
   * \brief The DoIP node type.
   */
  uint8_t node_type_;

  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReceiveUDPAnnouncementMessage);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, SendVehicleIdentMessageTimerStart);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReceiveUDPAnnouncementMessage);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, HandleRead1);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, HandleRead2);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, HandleRead3);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReactOnUDPRequest1);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReactOnUDPRequest2);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReactOnUDPRequest3);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReactOnUDPRequest4);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, ReactOnUDPRequest5);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, PeekMessageHeader1);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTest, PeekMessageHeader2);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPHandlerTestNackCodes, CreateNACK);
};

}  // namespace udp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_UDP_DOIP_UDP_HANDLER_H_
