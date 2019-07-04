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
/**        \file  session_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_SESSION_CONFIGURATION_H_
#define SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_SESSION_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/session_info.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
namespace context {

/**
 * \brief Context for session configuration in ServiceProcessors
 */
class SessionConfiguration {
 public:
  virtual ~SessionConfiguration() = default;

  /**
   * Checks if the requested SessionInfo instance is available.
   * \param session_id The session identifier
   * \return true when a SessionInfo item with the provided session identifier exists; otherwise, false
   */
  virtual bool ContainsSessionInfo(std::uint8_t session_id) const = 0;

  /**
   * Get a copy of the first SessinInfo instance with a matching session identifier.
   * \param session_id The session identifier
   * \return The corresponding SessionInfo
   * \throws std::out_of_range when the session list contains no item with a matching session identifier
   */
  virtual SessionInfo GetSessionInfo(std::uint8_t session_id) const = 0;
};

}  // namespace context
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_SESSION_CONFIGURATION_H_
