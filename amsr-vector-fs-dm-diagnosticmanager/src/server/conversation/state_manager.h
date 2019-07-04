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
/**        \file  state_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_STATE_MANAGER_H_
#define SRC_SERVER_CONVERSATION_STATE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <functional>
#include <map>
#include <utility>
#include <vector>

#include "configuration/dext_configuration.h"
#include "server/session_id.h"

#include "vac/memory/leaky_array_allocator.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

/**
 * \brief Implementation of StateManager.
 */

class StateManager {
 public:
  /**
   * \brief Storing p2 and p2_star times in a structure.
   */
  struct p2Times {
    /**
     * \brief Constructor.
     */
    p2Times() : time_p2(0), time_p2_star(0) {}

    /**
     * \brief Constructor.
     */
    p2Times(std::chrono::milliseconds p2, std::chrono::milliseconds p2_star) : time_p2(p2), time_p2_star(p2_star) {}

    /**
     * \brief Duration of p2 in ms.
     */
    std::chrono::milliseconds time_p2;

    /**
     * \brief Duration of p2_star in ms.
     */
    std::chrono::milliseconds time_p2_star;
  };

  /**
   * \brief Type definition for map containing the sessions Id with p2 and p2 star durations.
   */
  using SessionTimes = std::map<SessionId, p2Times, std::less<std::uint8_t>,
                                vac::memory::LeakyArrayAllocator<std::pair<SessionId, p2Times>>>;
  /**
   * \brief Constructor
   */
  explicit StateManager(const amsr::diag::configuration::DextConfiguration::SessionsArray& sessions);

  /**
   * \brief Return the duration of corresponding p2.
   * \param session_id id of session.
   * \return std::duration of p2.
   */
  std::chrono::milliseconds GetP2(SessionId session_id);

  /**
   * \brief Return the duration of corresponding p2 star.
   * \param session_id id of session.
   * \return std::duration of p2 star.
   */
  std::chrono::milliseconds GetP2Star(SessionId session_id);

 private:
  /**
   * \brief Map associating the session id with duration of p2 and p2star times.
   */
  SessionTimes session_times_;
};

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_STATE_MANAGER_H_
