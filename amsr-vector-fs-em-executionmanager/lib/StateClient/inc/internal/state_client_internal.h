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
/**        \file  state_client_internal.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_INTERNAL_H_
#define LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_INTERNAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/process/process.h>
#include <sys/types.h>
#include <vac/container/string_view.h>
#include <array>
#include <cstdint>
#include <string>

#include "internal/state_return_type.h"

namespace ara {
namespace exec {

/**
 * \brief Path to Execution Manager message queue (read only for Machine State Client)
 *
 * \details A posix message queue is placed under an extra filesystem (not the
 * root-filesystem), therefore only a "/" is needed in front of the name of the mqueue. More information can be found
 * on the man page of mq_overview (7).
 */
constexpr char kIpcRxPathReceiveStateClientIpc[] = "/em_receive_state_mq";

/**
 * \brief Path to Machine State Client message queue (write only for Machine State Client)
 *
 * \details A posix message queue is placed under an extra filesystem (not the
 * root-filesystem), therefore only a "/" is needed in front of the name of the mqueue. More information can be found
 * on the man page of mq_overview (7).
 */
constexpr char kIpcTxPathSendStateClientIpc[] = "/em_send_state_mq";

/**
 * \brief Definition of machine states for the communication with the execution manager
 */
enum class MachineState : uint8_t { kStartup = 0x0, kShutdown = 0x1, kRestart = 0x2, kChange = 0x3, kDummy = 0xFF };

namespace internal {

/**
 * Type definition of string view for state client
 */
using StringView = vac::container::string_view;

/**
 * \brief Enum Class to communicate about what the MSC wants the EM to do.
 */
enum class StateManagementMessageFunction : uint8_t { kGetState = 0x0, kSetState = 0x1, kResponse = 0x2 };

/**
 * \brief The size of the struct Machine_state_message
 */
static constexpr int32_t max_state_size_ = 255;

/**
 * \brief The message format for state management.
 */
struct StateManagementMessage {
  StateManagementMessageFunction function;         /**< The function called from an adaptive Application */
  std::array<char, max_state_size_> functionGroup; /**< The function group */
  std::array<char, max_state_size_> state;         /**< The function group state */
  StateReturnType response;                        /**< The response from the EM */

  /**
   * \brief Constructor for Machine State Message with default values
   */
  StateManagementMessage()
      : function(StateManagementMessageFunction::kResponse), state({0}), response(StateReturnType::kGeneralError) {}
};

/**
 * \brief The size of the struct StateManagementMessage
 */
constexpr int32_t kStateManagementMessageSize = sizeof(StateManagementMessage) + 1;

}  // namespace internal

}  // namespace exec
}  // namespace ara

#endif  // LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_INTERNAL_H_
