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
/**        \file  conversation_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_CONVERSATION_STATE_H_
#define SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_CONVERSATION_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/conversation/access/access_state.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
namespace context {

/**
 * \brief Context for accessing session, security etc. of a conversation
 */
class ConversationState {
 public:
  virtual ~ConversationState() = default;
  /**
   * Returns the observable access state for the given category.
   * \param category
   * \return
   */
  virtual conversation::access::ObservableAccessState& GetAccessState(
      conversation::access::AccessCategory category) = 0;

  /**
   * Returns the observable access state for the given category (Read only).
   * \param category
   * \return
   */
  virtual const conversation::access::ObservableAccessState& GetAccessState(
      conversation::access::AccessCategory category) const = 0;
};

}  // namespace context
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_CONVERSATION_STATE_H_
