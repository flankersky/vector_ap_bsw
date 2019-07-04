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
/**        \file  protocol_manager_with_conversation_manager_handling.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UDSTRANSPORT_PROTOCOL_MANAGER_WITH_CONVERSATION_MANAGER_HANDLING_H_
#define SRC_UDSTRANSPORT_PROTOCOL_MANAGER_WITH_CONVERSATION_MANAGER_HANDLING_H_

#include "udstransport/uds_transport_protocol_manager.h"

namespace amsr {
namespace diag {

namespace server {
namespace conversation {

/// forward declaration for ConversationManager
class ConversationManager;
}
}

namespace udstransport {

/**
 * Extends the interface UdsTransportProtocolMgr with methods for registering/Unregistering of ConversationManagers
 */
class ProtocolManagerWithConversationManagerHandling : public ara::diag::udstransport::UdsTransportProtocolMgr {
 public:
  virtual ~ProtocolManagerWithConversationManagerHandling() = default;

  /**
   * \brief Register a conversation manager.
   *
   * \param conversation_manager ConversationManager to register.
   */
  virtual void Register(server::conversation::ConversationManager& conversation_manager) = 0;

  /**
   * \brief Unregister a conversation manager.
   *
   * \param conversation_manager ConversationManager to unregister.
   */
  virtual void Unregister(server::conversation::ConversationManager& conversation_manager) = 0;
};

} /* namespace udstransport */
} /* namespace diag */
} /* namespace amsr */

#endif  // SRC_UDSTRANSPORT_PROTOCOL_MANAGER_WITH_CONVERSATION_MANAGER_HANDLING_H_
