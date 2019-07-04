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
/**
 *      \file  diagnostic_server_configuration.h
 *      \brief  DiagnosticServerConfig implementation
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#include <utility>

#include "vac/container/static_vector.h"
#include "vac/memory/three_phase_allocator.h"

#ifndef SRC_SERVER_DIAGNOSTIC_SERVER_CONFIGURATION_H_
#define SRC_SERVER_DIAGNOSTIC_SERVER_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/service/processor/context/session_configuration.h"
#include "server/session_info.h"

namespace amsr {
namespace diag {
namespace server {

/**
 * This interface specifies the methods required for the configuration of the DiagnosticServer.
 */
class DiagnosticServerConfiguration : public service::processor::context::SessionConfiguration {
 public:
  /**
   * The type of the session list.
   */
  using SessionInfoList = vac::container::StaticVector<SessionInfo, vac::memory::ThreePhaseAllocator<SessionInfo>>;

  virtual ~DiagnosticServerConfiguration() = default;  //< Default Destructor

  /**
   * Gets the configured limit of response pending messages.
   * \returns the maximum number of to be sent response pending messages
   */
  virtual std::uint8_t GetMaxNumberOfResponsePending() const = 0;

  /**
   * Gets read-only access to the session information list.
   * \returns a read-only reference to a StaticVector of SessionInfo instances
   */
  virtual const SessionInfoList& GetSessionInfos() const = 0;
};

/**
 * The implementation of DiagnosticServerConfiguration.
 */
class DiagnosticServerConfigurationImpl final : public DiagnosticServerConfiguration {
 public:
  /**
   * Initializes a DiagnosticServerConfiguration instance.
   * \param max_number_of_response_pending_responses The maximum number of to be sent response pending messages.
   * \param max_number_of_session_items The maximum number of SessionInfo items.
   */
  DiagnosticServerConfigurationImpl(std::uint8_t max_number_of_response_pending_responses,
                                    std::size_t max_number_of_session_items)
      : max_number_of_response_pending_responses_(max_number_of_response_pending_responses) {
    sessions_.reserve(max_number_of_session_items);
  }

  virtual ~DiagnosticServerConfigurationImpl() = default;  //< Default Destructor

  std::uint8_t GetMaxNumberOfResponsePending() const override { return max_number_of_response_pending_responses_; }

  bool ContainsSessionInfo(std::uint8_t session_id) const override;

  SessionInfo GetSessionInfo(std::uint8_t session_id) const override;

  const SessionInfoList& GetSessionInfos() const override { return sessions_; }

  /**
   * Adds a new session info item to the session list.
   * \param session_info The to be added SessionInfo
   * \throws std::bad_alloc when the session list (StaticVector) is already full
   */
  void AddSessionInfo(SessionInfo session_info);

 private:
  const SessionInfo* FindFirstMatchingSessionInfo(std::uint8_t session_id) const;

 private:
  std::uint8_t max_number_of_response_pending_responses_{0};
  SessionInfoList sessions_;
};

}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DIAGNOSTIC_SERVER_CONFIGURATION_H_
