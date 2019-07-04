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
/**        \file  diagnostic_server_configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/diagnostic_server_configuration.h"

#include <algorithm>
#include <string>

namespace amsr {
namespace diag {
namespace server {

bool DiagnosticServerConfigurationImpl::ContainsSessionInfo(std::uint8_t session_id) const {
  return (FindFirstMatchingSessionInfo(session_id) != nullptr);
}

SessionInfo DiagnosticServerConfigurationImpl::GetSessionInfo(std::uint8_t session_id) const {
  const SessionInfo* session_info = FindFirstMatchingSessionInfo(session_id);

  if (session_info == nullptr) {
    throw std::out_of_range("No SessionInfo item with the requested identifier " + std::to_string(session_id) +
                            " available!");
  }

  return SessionInfo{*session_info};
}

const SessionInfo* DiagnosticServerConfigurationImpl::FindFirstMatchingSessionInfo(std::uint8_t session_id) const {
  SessionInfoList::const_iterator iter =
      std::find_if(sessions_.cbegin(), sessions_.cend(),
                   [&session_id](const SessionInfo& item) { return item.session_id == session_id; });
  if (iter == sessions_.cend()) {
    return nullptr;  //< no matching session identifier found
  }

  return &(*iter);
}

void DiagnosticServerConfigurationImpl::AddSessionInfo(SessionInfo session_info) { sessions_.push_back(session_info); }

}  // namespace server
}  // namespace diag
}  // namespace amsr
