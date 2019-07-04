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
/**        \file  uds_transport_protocol_manager_impl.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_IMPL_H_
#define SRC_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "configuration/diagnostic_configuration.h"
#include "osabstraction/io/reactor.h"
#include "udstransport/protocol_manager_with_conversation_manager_handling.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_handler.h"
#include "vac/container/static_map.h"
#include "vac/container/static_vector.h"
#include "vac/memory/three_phase_allocator.h"
#include "vac/timer/timer_manager.h"

namespace amsr {
namespace diag {
namespace udstransport {

constexpr uint_least8_t kNumOfConnectionProvider = 1; /**< Number of ConnectionProvider used in ConnectionManager. */

/**
 * \brief Class for managing channels.
 */
class UdsTransportProtocolMgrImpl : public ProtocolManagerWithConversationManagerHandling {
 public:
  /**
   * \brief Constructor.
   */
  UdsTransportProtocolMgrImpl();

  /**
   * \brief Destructor.
   */
  virtual ~UdsTransportProtocolMgrImpl() = default;

  /**
   * \brief Lifecycle method for Initialization.
   * \param path_tp_config path to transport protocol configuration file.
   * \param reactor reactor
   * \param timer_manager timer manager
   */
  VIRTUALMOCK void Initialize(const std::string& path_tp_config, osabstraction::io::Reactor* reactor,
                              vac::timer::TimerManager* timer_manager);

  /**
   * \brief Lifecycle method for Run.
   */
  void Run();

  /**
   * \brief Lifecycle method for Shutdown.
   */
  void Shutdown();

  void Register(server::conversation::ConversationManager& conversation_manager) override;

  void Unregister(server::conversation::ConversationManager& conversation_manager) override;

  /**
   * \brief Indicates a message start.
   *
   * \param source_addr UDS source address of message
   * \param target_addr UDS target address of message
   * \param type indication whether its is phys/func request
   * \param global_channel_id transport protocol channel on which message start happened
   * \param size size in bytes of the UdsMessage starting from SID.
   * \param meta_info meta information for the UDS message.
   *
   * \return A pair of IndicationResult to be filled with Indication result and a pointer to UdsMessage owned/created by
   * DM core and returned to the handler to get filled.
   */
  IndicationPair IndicateMessage(
      ara::diag::udstransport::UdsMessage::Address source_addr,
      ara::diag::udstransport::UdsMessage::Address target_addr,
      ara::diag::udstransport::UdsMessage::TargetAddressType type,
      ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier global_channel_id, std::size_t size,
      std::shared_ptr<ara::diag::udstransport::UdsMessage::MetaInfoMap> meta_info) override;

  /**
   * \brief Indicates, that the message indicated via IndicateMessage() has failure and will not lead to a final
   * HandleMessage() call.
   *
   * \param message unique pointer to the current UDS message
   */
  void NotifyMessageFailure(ara::diag::udstransport::UdsMessage::ConstPtr message) override {}

  /**
   * \brief Hands over a valid received Uds message (currently this is only a request type) from transport layer to
   * session layer.
   * It corresponds to T_Data.ind of Figure 2 from ISO 14229-2. The behavior is asynchronously. I.e. the UdsMessage is
   * handed over to Session Layer and it is expected, that it "instantly" returns, which means, that real processing
   * of the message shall be done asynchronously!
   *
   * \param message The Uds message ptr (unique_ptr semantics) with the request. Ownership of the UdsMessage is given
   * back to the generic DM core here.
   *
   */
  void HandleMessage(ara::diag::udstransport::UdsMessage::Ptr message) override;

  /**
   * \brief notification about the outcome of a transmit request called by core DM at the handler via
   * UdsTransportProtocolHandler::Transmit
   *
   * This transmit API covers T_Data.con of ISO 14229-2 Figure 2.
   *
   * \param message for which message (created in IndicateMessage()) this is the confirmation.
   * \param result Result of transmission. In case UDS message could be transmitted on network layer: kTransmitOk),
   *        kTransmitFailed else.
   */
  VIRTUALMOCK void TransmitConfirmation(
      ara::diag::udstransport::UdsMessage::ConstPtr message,
      ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult result) override;

  /**
   * \brief notification call from the given transport channel, that it has been reestablished since the last (Re)Start
   * from
   * the UdsTransportProtocolHandler to which this channel belongs.
   * To activate this notification a previous call to UdsTransportProtocolHandler::NotifyReestablishment() has to be
   * done.
   * See further documentation at UdsTransportProtocolHandler::NotifyReestablishment().
   *
   * \param global_channel_id transport protocol channel, which is available again.
   */
  void ChannelReestablished(
      ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier global_channel_id) override {}

  /**
   * \brief notification from handler, that it has stopped now (e.g. closed down network connections, freed resources,
   * etc...)
   *
   * This callback is expected as a reaction from handler to a call to UdsTransportProtocolHandler::Stop.
   *
   * \param handler_id indication, which plugin stopped.
   */
  void HandlerStopped(ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id) override {}

  /**
   * \brief return the uds transport protocol handler with the corresponding ID
   *
   * \param  handler_id of the handle.
   * \return reference to the uds transport protocol handler.
   */
  ara::diag::udstransport::UdsTransportProtocolHandler* GetUdsTransportProtocolHandler(
      ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id) override;

 private:
  using UdsTransportProtocolHandler = ara::diag::udstransport::UdsTransportProtocolHandler;

  /**
   * \brief type definition of map of references of registered ConversationManagers.
   */
  using MapConversationManagers = vac::container::StaticMap<ara::diag::udstransport::UdsMessage::Address,
                                                            server::conversation::ConversationManager*>;

  /**
   * \brief type definition of List of transport protocol handlers.
   */
  using UdsTransportProtocolHandlers =
      vac::container::StaticVector<UdsTransportProtocolHandler::Ptr,
                                   vac::memory::ThreePhaseAllocator<UdsTransportProtocolHandler::Ptr>>;

  /**
   * \brief List of transport protocol handler.
   */

  UdsTransportProtocolHandlers uds_transport_protocol_handlers_;

  /**
   * \brief map of pointers of registered ConversationManagers.
   */
  MapConversationManagers map_conversation_manager_;

  /**
     * \brief Return the conversation manager with the corresponding target address.
     *
     * \param target_address target address.
     */
  server::conversation::ConversationManager* GetConversationManager(
      ara::diag::udstransport::UdsMessage::Address target_address);

  /**
   * \brief Initialize the transport protocol.
   * \param handler_id handler identifier
   * \param do_ip_config configuration for DoIP
   */
  void InitializeTransportProtocol(const ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id,
                                   const std::string& path_tp_config);

  /**
   * \brief Reactor used by DoIP for sockets and timers handling.
   */
  osabstraction::io::Reactor* reactor_;

  /**
   * \brief Timer manager used by DoIP.
   */
  vac::timer::TimerManager* timer_manager_;

  FRIEND_TEST(UdsTransportProtocolManagerImplTestFixture, GetConversationMgrReturnsRegisteredManager);
  FRIEND_TEST(UdsTransportProtocolManagerImplTestFixture, GetConversationMgrReturnsNullptrAfterUnRegisterIsCalled);
};

}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_IMPL_H_
