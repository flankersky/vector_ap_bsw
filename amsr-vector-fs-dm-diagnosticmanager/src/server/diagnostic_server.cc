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
/**        \file  diagnostic_server.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <thread>

#include "server/diagnostic_server.h"

namespace amsr {
namespace diag {
namespace server {

void DiagnosticServer::Initialize(const amsr::diag::configuration::DextConfiguration& dext_config,
                                  vac::timer::TimerManager& timer_manager) {
  ara::log::LogDebug() << "DiagnosticServer::" << __func__ << " >> " << this;

  // Instantiate the did manager.
  did_manager_.emplace(dext_config.dids_table);

  // Instantiate the conversation manager.
  conversation_manager_.emplace(dext_config, timer_manager, uds_transport_protocol_mgr_, *this);

  std::uint8_t max_number_of_response_pending_responses = dext_config.max_number_of_response_pending_responses;
  std::size_t max_number_of_session_items = dext_config.sessions.size();

  configuration_.emplace(max_number_of_response_pending_responses, max_number_of_session_items);

  for (amsr::diag::configuration::DextConfiguration::SessionsArray::const_reference session : dext_config.sessions) {
    configuration_.value().AddSessionInfo(SessionInfo(session.session_id, session.p2_time, session.p2_star_time));
  }
  service_dispatcher_.emplace(GetConfiguration(), dext_config);
}

data::DidManager& DiagnosticServer::GetDidManager() {
  if (!did_manager_.has_value()) {
    throw std::runtime_error("DiagnosticServer::GetDidManager did_manager has not been instantiated.");
  }
  return did_manager_.value();
}

const DiagnosticServerConfiguration& DiagnosticServer::GetConfiguration() const {
  if (!configuration_.has_value()) {
    throw std::logic_error(
        "DiagnosticServerConfiguration::GetConfiguration() called but no configuration available!"
        "Maybe Initialize not called before.");
  }
  return configuration_.value();
}

void DiagnosticServer::Run() {
  ara::log::LogDebug() << "DiagnosticServer::" << __func__ << " >> " << this;
  // Register conversation manager.
  uds_transport_protocol_mgr_.Register(conversation_manager_.value());
}

void DiagnosticServer::Shutdown() {
  ara::log::LogDebug() << "DiagnosticServer::" << __func__ << " >> " << this;
  // Unregister the services.
  service_dispatcher_->Shutdown();
  // Unregister conversation manager and shutdown conversation manager.
  uds_transport_protocol_mgr_.Unregister(conversation_manager_.value());
  this->conversation_manager_.value().Shutdown();
  // Destruct the did_manager to clear the map during deallocation phase.
  did_manager_.reset();
}

}  // namespace server
}  // namespace diag
}  // namespace amsr
