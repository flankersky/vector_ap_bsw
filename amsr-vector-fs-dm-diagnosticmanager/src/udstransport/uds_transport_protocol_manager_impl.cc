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
/**        \file  uds_transport_protocol_manager_impl.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include <thread>
#include <utility>

#include "ara/log/logging.hpp"
#include "server/conversation/conversation_manager.h"
#include "uds_message_helper.h"
#include "uds_transport_protocol_manager_impl.h"

namespace amsr {
namespace diag {
namespace udstransport {

UdsTransportProtocolMgrImpl::UdsTransportProtocolMgrImpl() : reactor_(nullptr), timer_manager_(nullptr) {}

void UdsTransportProtocolMgrImpl::Initialize(const std::string& path_tp_config, osabstraction::io::Reactor* reactor,
                                             vac::timer::TimerManager* timer_manager) {
  ara::log::LogDebug() << "UdsTransportProtocolMgr: Initialize";
  /* Load ConnectionProvider, ConnectionProvider are supposed to register connection handling to reactor */
  uds_transport_protocol_handlers_.reserve(kNumOfConnectionProvider);

  // TODO(PAASR-2712): Make maximum number of ConversationManager configurable in UdsTransportProtocolMgrImpl
  constexpr std::size_t kMaxNumberOfConversationManager = 1;
  map_conversation_manager_.reserve(kMaxNumberOfConversationManager);

  reactor_ = reactor;
  timer_manager_ = timer_manager;
  this->InitializeTransportProtocol(0, path_tp_config);
}

void UdsTransportProtocolMgrImpl::Run() {
  ara::log::LogDebug() << "UdsTransportProtocolMgr: Run";
  /* Start all ConnectionProvider */
  for (UdsTransportProtocolHandlers::reference transport_protocol_handler : uds_transport_protocol_handlers_) {
    transport_protocol_handler->Start();
  }
}

void UdsTransportProtocolMgrImpl::Shutdown() {
  ara::log::LogDebug() << "UdsTransportProtocolMgr: Shutdown";
  /* Stop all ConnectionProvider */
  for (UdsTransportProtocolHandlers::reference transport_protocol_handler : uds_transport_protocol_handlers_) {
    transport_protocol_handler->Stop();
  }
}

void UdsTransportProtocolMgrImpl::Register(server::conversation::ConversationManager& conversation_manager) {
  map_conversation_manager_.insert(
      MapConversationManagers::value_type(conversation_manager.GetTA(), &conversation_manager));
}

void UdsTransportProtocolMgrImpl::Unregister(server::conversation::ConversationManager& conversation_manager) {
  map_conversation_manager_.erase(conversation_manager.GetTA());
}

server::conversation::ConversationManager* UdsTransportProtocolMgrImpl::GetConversationManager(
    ara::diag::udstransport::UdsMessage::Address target_address) {
  MapConversationManagers::iterator conversation_manager = map_conversation_manager_.find(target_address);
  // Return nullptr, if conversation manager is not found.
  if (conversation_manager == map_conversation_manager_.end()) {
    return nullptr;
  }
  return conversation_manager->second;
}

UdsTransportProtocolMgrImpl::IndicationPair UdsTransportProtocolMgrImpl::IndicateMessage(
    ara::diag::udstransport::UdsMessage::Address source_addr, ara::diag::udstransport::UdsMessage::Address target_addr,
    ara::diag::udstransport::UdsMessage::TargetAddressType type,
    ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier global_channel_id, std::size_t size,
    std::shared_ptr<ara::diag::udstransport::UdsMessage::MetaInfoMap> /* meta_info */) {
  // Preparing a pair of IndicationResult of kIndicationOverview (DM has no resources) and Null UdsMessage pointer.
  IndicationPair return_no_resources(
      ara::diag::udstransport::UdsTransportProtocolMgr::IndicationResult::kIndicationOverflow, nullptr);

  if (map_conversation_manager_.empty()) {
    ara::log::LogError() << "UdsTransportProtocolMgrImpl::" << __func__ << " : No conversation manager available!";
    return return_no_resources;
  }

  amsr::diag::server::conversation::ConversationManager* conversation_manager_pointer = nullptr;
  if (type == ara::diag::udstransport::UdsMessage::TargetAddressType::FUNCTIONAL) {
    // Currently, the design supports FUNCTIONAL UDS messages only for a single ConversationManager
    assert(!this->map_conversation_manager_.empty());
    // TODO(PAASR-1904): Implement  handling of Functional UDS messages for multiple instances of ConversationManager.
    conversation_manager_pointer = this->map_conversation_manager_.begin()->second;
  } else {
    conversation_manager_pointer = GetConversationManager(target_addr);
  }

  // If conversation manager with corresponding target address exists, get a conversation.
  if (conversation_manager_pointer == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgrImpl::" << __func__ << " : No conversation manager available!";
    return return_no_resources;
  }

  amsr::diag::server::conversation::Conversation* conversation =
      conversation_manager_pointer->GetOrCreateConversation(global_channel_id, source_addr);

  // If conversation exists, call indicate message.
  if (conversation == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgrImpl::" << __func__ << " : No conversation available!";
    return return_no_resources;
  }

  return conversation->IndicateMessage(size, type);
}

void UdsTransportProtocolMgrImpl::HandleMessage(ara::diag::udstransport::UdsMessage::Ptr message) {
  // If message is not nullptr, get conversation manager.
  if (message == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgr::" << __func__ << " : Provided message == nullptr!";
    return;
  }

  if (map_conversation_manager_.empty()) {
    ara::log::LogError() << "UdsTransportProtocolMgr::" << __func__ << " : No conversation manager available!";
    return;
  }

  // log the UDS message (only relevant parts, no PDU)
  ara::log::LogDebug() << "UdsTransportProtocolMgr::" << __func__ << " : " << ToString(message, 0);

  amsr::diag::server::conversation::ConversationManager* conversation_manager_pointer = nullptr;
  if (message->GetTaType() == ara::diag::udstransport::UdsMessage::TargetAddressType::FUNCTIONAL) {
    // Currently, the design supports FUNCTIONAL UDS messages only for a single ConversationManager
    assert(!map_conversation_manager_.empty());
    // TODO(PAASR-1904): Implement  handling of Functional UDS messages for multiple instances of ConversationManager.
    conversation_manager_pointer = map_conversation_manager_.begin()->second;
  } else {
    conversation_manager_pointer = GetConversationManager(message->GetTa());
  }

  // If conversation manager is valid, get the conversation
  if (conversation_manager_pointer == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgr::" << __func__
                         << " : No conversation manager available for current message!";
    return;
  }

  ara::diag::udstransport::UdsMessage::Address source_address = message->GetSa();
  amsr::diag::server::conversation::Conversation* conversation =
      conversation_manager_pointer->GetOrCreateConversation(message->GetGlobalChannelIdentifier(), source_address);

  // If conversation is valid, handle the message
  if (conversation == nullptr) {
    ara::log::LogWarn() << "UdsTransportProtocolMgr::" << __func__ << " : No conversation manager available for SA='"
                        << ToString(source_address) << "'!";
    return;
  }

  conversation->HandleMessage(std::move(message));
}

ara::diag::udstransport::UdsTransportProtocolHandler* UdsTransportProtocolMgrImpl::GetUdsTransportProtocolHandler(
    ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id) {
  UdsTransportProtocolHandlers::const_iterator handler =
      std::find_if(uds_transport_protocol_handlers_.begin(), uds_transport_protocol_handlers_.end(),
                   [handler_id](const ara::diag::udstransport::UdsTransportProtocolHandler::Ptr& handler_ptr) {
                     return handler_ptr->GetHandlerID() == handler_id;
                   });
  if (handler == uds_transport_protocol_handlers_.end()) {
    return nullptr;
  }
  return handler->get();
}

void UdsTransportProtocolMgrImpl::TransmitConfirmation(
    ara::diag::udstransport::UdsMessage::ConstPtr message,
    ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult result) {
  ara::log::LogDebug() << "UdsTransportProtocolMgr::TransmitConfirmation called";

  // If message is valid, get conversation manager.
  if (message == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgr::TransmitConfirmation : UDS message == nullptr";
    return;
  }

  // here we process the response send to the tester so we have to use the source address
  uint16_t source_address = message->GetSa();
  amsr::diag::server::conversation::ConversationManager* conversation_manager_pointer =
      GetConversationManager(source_address);

  // If conversation manager exists, get conversation.
  if (conversation_manager_pointer == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgr::TransmitConfirmation : no ConversationManager available for "
                            "source address of transmitted response";
    return;
  }

  uint16_t target_address = message->GetTa();
  // Get the associated conversation and transmit confirmation
  amsr::diag::server::conversation::Conversation* conversation =
      conversation_manager_pointer->GetOrCreateConversation(message->GetGlobalChannelIdentifier(), target_address);

  // If conversation is valid, call TransmitConfirmation
  if (conversation == nullptr) {
    ara::log::LogError() << "UdsTransportProtocolMgr::TransmitConfirmation : no valid Conversation available";
    return;
  }

  ara::log::LogDebug() << "UdsTransportProtocolMgr::TransmitConfirmation : signal TransmitConfirmation to Conversation";
  conversation->TransmitConfirmation(result);
}

}  // namespace udstransport
}  // namespace diag
}  // namespace amsr
