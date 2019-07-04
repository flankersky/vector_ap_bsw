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
/**        \file  state_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>

#include "server/conversation/conversation_manager.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

StateManager::StateManager(const amsr::diag::configuration::DextConfiguration::SessionsArray& sessions) {
  for (amsr::diag::configuration::DextConfiguration::SessionsArray::const_reference session : sessions) {
    session_times_.emplace(static_cast<SessionId>(session.session_id), p2Times(session.p2_time, session.p2_star_time));
  }
}

std::chrono::milliseconds StateManager::GetP2(SessionId session_id) {
  // return the duration of P2 timer. Throw out of range if session id is not found.
  return session_times_.at(session_id).time_p2;
}

std::chrono::milliseconds StateManager::GetP2Star(SessionId session_id) {
  // return the duration of P2 star timer. Throw out of range if session id is not found.
  return session_times_.at(session_id).time_p2_star;
}
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr
