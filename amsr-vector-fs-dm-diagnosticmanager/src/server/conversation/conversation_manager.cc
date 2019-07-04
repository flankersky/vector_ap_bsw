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
/**        \file  conversation_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>

#include "ara/log/logging.hpp"

#include "server/conversation/conversation_manager.h"
#include "server/diagnostic_server.h"
#include "server/diagnostic_server_configuration.h"
#include "udstransport/uds_transport_protocol_manager_impl.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

ConversationManager::ConversationManager(
    const configuration::DextConfiguration& dext_config, vac::timer::TimerManager& timer_manager,
    amsr::diag::udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_mgr,
    server::DiagnosticServer& diagnostic_server)
    : target_address_(dext_config.target_address),
      timer_manager_(timer_manager),
      max_uds_message_size_(dext_config.uds_message_length),
      state_manager_(dext_config.sessions),
      uds_transport_protocol_mgr_(uds_transport_protocol_mgr),
      uds_message_provider_(kNumberUDSBuffer, dext_config.uds_message_length),
      diagnostic_server_(diagnostic_server),
      access_notification_manager_(kMaxNumberOfNotificationManagerSubscribers) {
  conversations_list_.reserve(dext_config.number_conversations);
}

ConversationManager::~ConversationManager() { Shutdown(); }

const server::DiagnosticServerConfiguration& ConversationManager::GetDiagnosticServerConfiguration() const {
  return diagnostic_server_.GetConfiguration();
}

Conversation* ConversationManager::GetOrCreateConversation(
    ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier channel_id,
    ara::diag::udstransport::UdsMessage::Address source_address) {
  // try find matching existing conversation, return this if available
  if (!conversations_list_.empty()) {
    vac::container::StaticList<Conversation>::iterator result = std::find_if(
        conversations_list_.begin(), conversations_list_.end(),
        [channel_id, source_address](Conversation& conversation) {
          return (conversation.GetConnectionId() == channel_id) && (conversation.GetSourceAddress() == source_address);
        });

    if (result != conversations_list_.end()) {
      ara::log::LogDebug() << "ConversationManager::GetOrCreateConversation : returning existing conversation";
      return &(*result);
    }
  }

  // if conversation list is full search for conversation in state kFree
  if (conversations_list_.full()) {
    // find Conversation in state kFree
    vac::container::StaticList<Conversation>::iterator free_conversation =
        std::find_if(conversations_list_.begin(), conversations_list_.end(), [](const Conversation& conversation) {
          return (conversation.GetConversationState() == Conversation::ConversationState::kFree);
        });

    // when no conversation can be recycled - no further processing
    if (free_conversation == conversations_list_.end()) {
      ara::log::LogWarn() << "ConversationManager::GetOrCreateConversation : Conversation list is full "
                             "and no free item exists (returning nullptr)";
      return nullptr;  //< no free conversation available
    }

    assert(free_conversation != conversations_list_.end());
    assert(free_conversation->GetConversationState() == Conversation::ConversationState::kFree);

    ara::log::LogDebug() << "ConversationManager::GetOrCreateConversation : Erasing free conversation";

    // recycle conversation
    free_conversation->Shutdown();
    conversations_list_.erase(free_conversation);
    assert(conversations_list_.full() == false);  //< now we should be able to add one Conversation
  }

  ara::log::LogDebug() << "ConversationManager::GetOrCreateConversation : Creating conversation";

  const server::DiagnosticServerConfiguration& diag_server_cfg = GetDiagnosticServerConfiguration();
  std::uint8_t response_pending_limit = diag_server_cfg.GetMaxNumberOfResponsePending();

  // add new conversation to list
  conversations_list_.emplace_back(channel_id, source_address, timer_manager_, *this, response_pending_limit);
  return &conversations_list_.back();
}

void ConversationManager::Shutdown() {
  ara::log::LogDebug() << "ConversationManager::Shutdown called.";
  // Shutdown message handlers.
  for (vac::container::StaticList<Conversation>::reference conversation : conversations_list_) {
    conversation.Shutdown();
  }
}
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr
