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
/**        \file  diagnostic_server.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DIAGNOSTIC_SERVER_H_
#define SRC_SERVER_DIAGNOSTIC_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <udstransport/protocol_manager_with_conversation_manager_handling.h>
#include "vac/memory/optional.h"

#include "ara/log/logging.hpp"

#include "configuration/dext_configuration.h"
#include "osabstraction/io/reactor.h"
#include "server/conversation/conversation_manager.h"
#include "server/data/did_manager.h"
#include "server/diagnostic_server_configuration.h"
#include "server/proxys/proxys_pool.h"
#include "server/service/service_dispatcher.h"

namespace amsr {
namespace diag {
namespace server {

/**
 * \brief Implementation of diagnostic manager.
 */
class DiagnosticServer {
 public:
  /**
   * \brief Constructor.
   */
  explicit DiagnosticServer(
      amsr::diag::udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_mgr)
      : uds_transport_protocol_mgr_(uds_transport_protocol_mgr) {}

  /**
   * \brief Destructor.
   */
  VIRTUALMOCK ~DiagnosticServer() = default;

  /**
   * \brief Life-cycle Method for Initialization.
   */
  VIRTUALMOCK void Initialize(const amsr::diag::configuration::DextConfiguration& dext_config,
                              vac::timer::TimerManager& timer_manager);

  /**
   * \brief Life-cycle Methods for Run Mode.
   */
  VIRTUALMOCK void Run();

  /**
   * \brief Life-cycle Methods for Shutdown.
   */
  VIRTUALMOCK void Shutdown();

  /**
   * \brief Return the service dispatcher.
   */
  VIRTUALMOCK amsr::diag::server::service::ServiceDispatcher& GetServiceDispatcher() {
    return service_dispatcher_.value();
  }

  /**
   * \brief Return the Did Manager.
   */
  VIRTUALMOCK data::DidManager& GetDidManager();

  /**
     * \brief Return the proxys pool.
     */
  proxys::ProxysPool& GetProxysPool() { return proxys_pool_; }

  /**
   * \brief Gets the diagnostic server configuration
   */
  VIRTUALMOCK const DiagnosticServerConfiguration& GetConfiguration() const;

 private:
  /**
   * \brief Conversation manager.
   */
  vac::memory::optional<amsr::diag::server::conversation::ConversationManager> conversation_manager_;

  /**
   * \brief Pointer to the transport protocol manager in which ConversationsManagers are registered.
   */
  amsr::diag::udstransport::ProtocolManagerWithConversationManagerHandling& uds_transport_protocol_mgr_;

  /**
   * \brief Service dispatcher.
   */
  vac::memory::optional<service::ServiceDispatcher> service_dispatcher_;

  /**
   * \brief Did Manager.
   */
  vac::memory::optional<data::DidManager> did_manager_;

  /**
   * The DiagnosticServerConfiguration, stored by Initialize
   */
  vac::memory::optional<DiagnosticServerConfigurationImpl> configuration_;

  /**
   * \brief Pool of used proxys.
   */
  proxys::ProxysPool proxys_pool_;

  FRIEND_TEST(DiagnosticServerTestFixture, CreateInitializeRunShutdownTest);
  FRIEND_TEST(DiagnosticServerTestFixture, CreateInitializeShutdownTest);
};

}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DIAGNOSTIC_SERVER_H_
