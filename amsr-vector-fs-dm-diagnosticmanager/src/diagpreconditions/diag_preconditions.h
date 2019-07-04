/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  diag_preconditions.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_DIAGPRECONDITIONS_DIAG_PRECONDITIONS_H_
#define SRC_DIAGPRECONDITIONS_DIAG_PRECONDITIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "diag_state_precondition.h"
#include "server/conversation/access/access_state.h"
#include "server/service/service_processing_context.h"

namespace amsr {
namespace diag {
namespace diagpreconditions {
/**
 * \brief Implementation of DiagPreconditions class.
 * This class is used to test if preconditions on Sessions and security access levels are fulfilled.
 * For that purpose this class is containing two lists:
 * - one with the list of ids of allowed sessions
 * - one with the list of ids of allowed security access levels
 */
class DiagPreconditions {
 public:
  /**
   * \brief Constructor.
   * \param preconditions_config contains the preconditions parsed in the configuration file.
   */
  explicit DiagPreconditions(const configuration::Preconditions& preconditions_config)
      : sessions_precondition_(server::conversation::access::AccessCategory::kSession,
                               preconditions_config.sessions_ids),
        security_access_levels_precondition_(server::conversation::access::AccessCategory::kSecurityLevel,
                                             preconditions_config.security_access_levels_ids) {}

  /**
   * \brief Returns the precondition for category given as parameter.
   */
  const DiagStatePrecondition& GetPreconditionsOf(server::conversation::access::AccessCategory access_category) const;

  /**
   * \brief Returns true if the current context verifies the preconditions for the selected category (Session or
   * SecurityAccessLevel).
   */
  bool IsAllowedIn(const server::service::ServiceProcessingContext& context,
                   server::conversation::access::AccessCategory access_category) const;

  // Remove copy/move ctor/assignment.
  DiagPreconditions(const DiagPreconditions&) = delete;
  DiagPreconditions(const DiagPreconditions&&) = delete;
  DiagPreconditions& operator=(const DiagPreconditions&) = delete;
  DiagPreconditions& operator=(const DiagPreconditions&&) = delete;

 private:
  /**
   * \brief Preconditions on Sessions.
   */
  DiagStatePrecondition sessions_precondition_;

  /**
   * \brief Preconditions on SecurityAccessLevels.
   */
  DiagStatePrecondition security_access_levels_precondition_;
};

}  // namespace diagpreconditions
}  // namespace diag
}  // namespace amsr
#endif  // SRC_DIAGPRECONDITIONS_DIAG_PRECONDITIONS_H_
