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
/**        \file  conversation_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_CONVERSATION_MANAGER_H_
#define SRC_SERVER_CONVERSATION_CONVERSATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <vac/container/intrusive_list.h>
#include <vac/container/static_list.h>
#include <vac/testing/test_adapter.h>

#include "configuration/dext_configuration.h"
#include "server/conversation/access/access_state_notification_manager.h"
#include "server/conversation/conversation.h"
#include "server/conversation/state_manager.h"
#include "server/conversation/uds_message_provider.h"
#include "udstransport/protocol_manager_with_conversation_manager_handling.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_handler.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace amsr {
namespace diag {
namespace server {
class DiagnosticServer;               //< forward declaration
class DiagnosticServerConfiguration;  //< forward declaration

namespace conversation {

/**
 * \brief Number of available Buffer.
 *
 * TODO(PAASR-2458) : move it to config file ?
 */
constexpr std::size_t kNumberUDSBuffer = 5;

/** Maximum number of subscribers for the notification manager*/
constexpr std::size_t kMaxNumberOfNotificationManagerSubscribers = 10;

/**
 * \brief Implementation of conversation.
 */
class ConversationManager {
 public:
  /**
   * \brief Constructor.
   */
  ConversationManager(const configuration::DextConfiguration& dext_config, vac::timer::TimerManager& timer_manager,
                      udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_mgr,
                      server::DiagnosticServer& diagnostic_server);

  virtual ~ConversationManager();

  ConversationManager(const ConversationManager&) = delete;
  ConversationManager& operator=(const ConversationManager&) = delete;

  /**
   * \brief Return the target address.
   */
  VIRTUALMOCK const ara::diag::udstransport::UdsMessage::Address GetTA() const { return target_address_; }

  /**
   * \brief Get or create a conversation if it doesn't exist..
   * \param channel_id channel id.
   * \param source_address source address of tester.
   * \return reference to the conversation.
   */
  VIRTUALMOCK Conversation* GetOrCreateConversation(
      ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier channel_id,
      ara::diag::udstransport::UdsMessage::Address source_address);

  /**
   * \brief Return Uds Message Provider.
   */
  UdsMessageProvider& GetUdsMessageProvider() { return uds_message_provider_; }

  /**
   * \brief Return the transport protocol manager.
   */
  amsr::diag::udstransport::ProtocolManagerWithConversationManagerHandling& GetUdsTransportProtocolManager() {
    return uds_transport_protocol_mgr_;
  }

  /**
   * \brief Return the diagnostic manager.
   */
  VIRTUALMOCK server::DiagnosticServer& GetDiagnosticServer() { return diagnostic_server_; }

  /**
   * \brief Gets the DiagnosticServerConfiguration
   */
  VIRTUALMOCK const server::DiagnosticServerConfiguration& GetDiagnosticServerConfiguration() const;

  /**
   * \brief Returns the access notification manager.
   */
  utility::notificationmanagement::NotificationManager<access::AccessStateChangeInfo, access::AccessCategoryMask>&
  GetAccessNotificationManager() {
    return access_notification_manager_;
  }

  /**
   * \brief Method to be called when application is terminating.
   * Currently calling shutdown method of every message handlers.
   */
  void Shutdown();

 private:
  /**
   * \brief List of conversations.
   */
  vac::container::StaticList<Conversation> conversations_list_;

  /**
   * \brief target address.
   * The target_address from the DextConfiguration provided to the constructor is used to initialize this member.
   */
  ara::diag::udstransport::UdsMessage::Address target_address_{0};

  /**
   * \brief Pointer to the timer manager of the diagnostic.
   */
  vac::timer::TimerManager& timer_manager_;

  /**
   * \brief maximum size of UDS message.
   * The uds_message_length from the DextConfiguration provided to the constructor is used to initialize this member.
   */
  std::uint32_t max_uds_message_size_{0};

  /**
   * \brief State manager
   */
  server::conversation::StateManager state_manager_;

  /**
   * \brief Pointer to the transport protocol manager in which ConversationsManagers are registered.
   */
  udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_mgr_;

  /**
   * Buffer provider of uint8_t.
   */
  UdsMessageProvider uds_message_provider_;

  /**
   * \brief Reference to diagnostic server owning this class.
   */
  server::DiagnosticServer& diagnostic_server_;

  access::AccessStateNotificationManager access_notification_manager_;

  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(ConversationManagerTestFixture, CheckInitialStateOfConversationManager);
};
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_CONVERSATION_MANAGER_H_
