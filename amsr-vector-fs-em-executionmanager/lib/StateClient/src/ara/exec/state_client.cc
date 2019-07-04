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
/**        \file  state_client.cc
 *        \brief  The StateClient class provides the implementation for an Application
 *         to request a Machine State switch or to retrieve the current Machine State to/from Execution Management.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/exec/state_client.hpp"
#include "internal/state_client_base.h"
#include "internal/state_client_internal.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ara/log/logging.hpp>

#include <iostream>
#include <stdexcept>

#include "osabstraction/messagequeue/queuebuilder.h"

namespace ara {
namespace exec {

StateClient::StateClient() : ipc_rx_(), ipc_tx_() {
  ara::log::InitLogging("SC", "Logging context of the state_client library", ara::log::LogLevel::kVerbose,
                        ara::log::LogMode::kConsole, "");

  osabstraction::messagequeue::QueueBuilder builder;
  ipc_rx_ = builder.SetId(kIpcRxPathReceiveStateClientIpc).BuildReceiverQueue();
  ipc_tx_ = builder.SetId(kIpcTxPathSendStateClientIpc).BuildSenderQueue();
}

StateReturnType StateClient::GetState(std::string functionGroup, std::string &state) {
  log_.LogDebug() << __func__ << " Get current" << functionGroup << "ms";
  StateReturnType return_value{StateReturnType::kGeneralError};

  // MachineState and StateError are not required for GetState Request, but the message format is fixed therefore we
  // use dummy values.
  internal::StateManagementMessage get_state_massage = CreateStateManagementMessage(
      internal::StateManagementMessageFunction::kGetState, internal::StringView(functionGroup), MachineState::kDummy,
      StateReturnType::kSuccess);
  get_state_massage.state[0] = '\0';
  // send message
  log_.LogDebug() << "StateClient sends GetState message to ExecutionManager state-client message queue.";
  SendToMessageQueue(ipc_tx_, &get_state_massage);
  PrintStateManagementMessage(get_state_massage);

  // receive response
  internal::StateManagementMessage response_message;
  if (ReceiveFromMessageQueue(ipc_rx_, &response_message) <= 0) {
    return_value = StateReturnType::kGeneralError;
  } else {
    std::string tmp_state(response_message.state.begin(), response_message.state.end());
    state = tmp_state;
    return_value = response_message.response;
  }

  return return_value;
}

StateReturnType StateClient::SetState(std::string functionGroup, std::string state) {
  log_.LogDebug() << __func__ << " Set current" << functionGroup << "ms";
  StateReturnType return_value{StateReturnType::kGeneralError};

  // MachineState and StateReturnType are not being used  for SetState Request.
  internal::StateManagementMessage set_state_message =
      CreateStateManagementMessage(internal::StateManagementMessageFunction::kSetState,
                                   internal::StringView(functionGroup), state, StateReturnType::kSuccess);

  // send message
  log_.LogDebug() << __func__ << " SC sends message to SC Message Queue";
  SendToMessageQueue(ipc_tx_, &set_state_message);
  PrintStateManagementMessage(set_state_message);

  // receive response
  internal::StateManagementMessage response_message;
  if (ReceiveFromMessageQueue(ipc_rx_, &response_message) <= 0) {
    return_value = StateReturnType::kGeneralError;
  } else {
    return_value = response_message.response;
  }

  return return_value;
}

}  // namespace exec
}  // namespace ara
