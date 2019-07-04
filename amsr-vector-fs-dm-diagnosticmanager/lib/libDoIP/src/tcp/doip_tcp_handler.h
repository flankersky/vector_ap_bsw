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
/**        \file  doip_tcp_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_TCP_DOIP_TCP_HANDLER_H_
#define LIB_LIBDOIP_SRC_TCP_DOIP_TCP_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/event_handler.h>
#include <osabstraction/io/network/socket/socket_acceptor.h>
#include <osabstraction/io/network/socket/tcp_server_socket.h>
#include <osabstraction/io/network/socket/tcp_socket.h>
#include <osabstraction/io/reactor.h>
#include <vac/container/static_list.h>
#include <vac/memory/leaky_array_allocator.h>
#include <vac/testing/test_adapter.h>
#include <vac/timer/timer_manager.h>
#include <functional>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "doip_channel.h"
#include "doip_handler.h"
#include "processors/doip_alive_check_processor.h"
#include "processors/doip_diagnostic_message_processor.h"
#include "processors/doip_routing_activation_request_processor.h"
#include "processors/doip_tcp_message_processor.h"
#include "processors/doip_tcp_nack_processor.h"
#include "tcp/doip_channel_state_pool.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPTransportProtocolHandler;  // forward declaration.
namespace tcp {

/**
 * \brief The default Channel ID for a channel in a Listen state.
 */
constexpr ara::diag::udstransport::ChannelID kDefaultChannelID = 0xFFFFFFFF;
/**
 * DoIP TCP Handler.
 */
class DoIPTCPHandler : public udstransport::doip::DoIPHandler {
 public:
  /**
   * \brief Constructor.
   */
  explicit DoIPTCPHandler(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
                          const amsr::diag::configuration::DoIPChannelConfiguration& doip_channel_configuration,
                          DoIPTransportProtocolHandler& transport_protocol_handler, const char* port);

  DoIPTCPHandler(const DoIPTCPHandler&) = delete;
  DoIPTCPHandler& operator=(const DoIPTCPHandler&) = delete;

  virtual ~DoIPTCPHandler() = default;

  /**
   * \brief Creates a channel from an accepted Tcp Socket.
   *
   * \param tcp_socket The socket of the Connection to be created.
   */
  void CreateDoIPChannel(osabstraction::io::network::socket::TCPSocket&& tcp_socket);

  /**
   * \brief Returns the DoIP Connection State Pool
   */
  DoIPChannelStatePool& GetStatePool() { return state_pool_; }

  /**
   * \brief Returns the number of in-use channels.
   */
  uint8_t GetNoEstablishedSockets();

  /**
   * \brief Returns the max. number of TCP Connections available.
   */
  uint32_t GetMaxTCPConnections() { return max_tcp_channels_; }

  /**
   * \brief Check the payload type and return the corresponding message processor.
   *
   * \param payload_type payload type to be checked.
   *
   * \return a pointer to the message processor corresponding to the payload type.
   */
  const doip::processors::DoIPTcpMessageProcessor* GetSupportedMessageProcessor(PayloadType payload_type);

  /**
   * \brief Check if the Source address is registered with other channel.
   *
   * \param source_address The source address to be checked.
   *
   * \return True if source address is registered in other channel, false otherwise
   */
  bool IsSourceAddressAlreadyRegistered(uint16_t source_address);

  /**
   * \brief Returns the Connection provider.
   */
  amsr::diag::udstransport::doip::DoIPTransportProtocolHandler& GetConnectionProvider() {
    return transport_protocol_handler_;
  }

  /**
   * \brief dispatch the transmission action to the concerned channel using the channel id.
   *
   * \param message The UDS message to transmitted.
   * \param channel_id The channel id of the channel to used for transmission.
   */
  void Transmit(ara::diag::udstransport::UdsMessage::Ptr message, ara::diag::udstransport::ChannelID channel_id);

 private:
  /**
   * \brief Check if the valid Payload Type.
   *
   * \param payload_type payload type to be checked.
   *
   * \return true of a valid Payload Type, false otherwise.
   */
  bool IsSupportedPayloadType(PayloadType payload_type) override;

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
   * \brief Searching for a channel with a Listen state to be used.
   *
   * \return the DoIPChannel to be used or a null pointer if no one is available.
   */
  VIRTUALMOCK udstransport::doip::DoIPChannel* TryGetListeningDoIPChannel();

  /**
   * \brief Searching for a channel with an kRegisteredRoutingActive state and with the given channel_id.
   *
   * \param channel_id The channel Id.
   *
   * \return the found channel with the given channel_id and in the Registered state, nullptr otherwise.
   */
  VIRTUALMOCK DoIPChannel* GetChannelById(ara::diag::udstransport::ChannelID channel_id);

  /**
   * \brief A Tcp-SocketAcceptor which handles TCP-channel requests.
   */
  osabstraction::io::network::socket::SocketAcceptor<osabstraction::io::network::socket::TCPServerSocket,
                                                     DoIPTCPHandler, &DoIPTCPHandler::CreateDoIPChannel>
      tcp_socket_acceptor_;

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
   * \brief The state pool for DoIP channel states
   */
  DoIPChannelStatePool state_pool_;

  /**
   * \brief The channel pool for DoIP channels.
   */
  vac::container::StaticList<DoIPChannel> channels_;

  /**
   * \brief The last used Channel Id.
   */
  ara::diag::udstransport::ChannelID last_used_id_;

  /**
   * \brief The max number of channels available. The maximum number of channels should occupy only one byte. This comes
   * from the fact that the Entity Status response message has only one byte for that parameter.
   */
  uint8_t max_tcp_channels_;

  /**
   * \brief The Routing activation request processor.
   */
  const doip::processors::DoIPRoutingActivationRequestProcessor routing_activation_processor_;

  /**
   * \brief The Alive check response processor.
   */
  const doip::processors::DoIPAliveCheckProcessor alive_check_processor_;

  /**
   * \brief The Diagnostic Message processor.
   */
  const doip::processors::DoIPDiagnosticMessageProcessor diagnostic_message_processor_;

  /**
   * \brief The generic header NACK processor.
   */
  const doip::processors::DoIPTcpNackProcessor nack_processor_;

  /**
   * \brief The known target addresses that the DoIP can communicate with.
   */
  vac::container::StaticList<uint16_t> known_target_addresses_;

  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, TryGetListeningDoIPChannelOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, TryGetListeningDoIPChannelKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, AssignChannelIdOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, AssignChannelIdKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetSupportedAliveCheckProcessorOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetSupportedDiagnosticMessageProcessorOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetSupportedRoutingActivationRequestProcessorOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetSupportedTcpNackProcessorOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetSupportedMessageProcessorKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTestFixture, IsSourceAddressAlreadyRegisteredReturnFalseNotSameAddress);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTestFixture, IsSourceAddressAlreadyRegisteredReturnFalseAllInListenWithSameAddress);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetNoEstablishedSocketsReturn2);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTest, GetNoEstablishedSocketsReturnAll);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTestFixture, TransmitCallTransmit);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTestFixture, GetChannelByIdReturnFalseAllInListen);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPTCPHandlerTestFixture, GetChannelByIdReturnFalseNotSameChannelId);
};

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_TCP_DOIP_TCP_HANDLER_H_
