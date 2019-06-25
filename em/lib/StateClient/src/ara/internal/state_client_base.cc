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
/**        \file  state_client_base.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "internal/state_client_base.h"
#include <algorithm>  // copy()
#include <iostream>
#include "ara/log/logging.hpp"

namespace ara {
namespace exec {

StateClientBase::StateClientBase()
    : log_(ara::log::CreateLogger("MSCB", "Logging context for the StateClientBase")), prio_() {}

void StateClientBase::SendToMessageQueue(osabstraction::messagequeue::SenderQueue& mq_fd,
                                         internal::StateManagementMessage* message) {
  mq_fd.Send(message, internal::kStateManagementMessageSize);
}

std::size_t StateClientBase::ReceiveFromMessageQueue(osabstraction::messagequeue::ReceiverQueue& mq_fd,
                                                     internal::StateManagementMessage* message) {
  return mq_fd.Receive(message, internal::kStateManagementMessageSize);
}

std::string StateClientBase::MachineStateToString(MachineState state) {
  switch (state) {
    case MachineState::kChange:
      return "kChange";
      break;
    case MachineState::kRestart:
      return "kRestart";
      break;
    case MachineState::kShutdown:
      return "kShutdown";
      break;
    case MachineState::kStartup:
      return "kStartup";
      break;
    case MachineState::kDummy:
      return "kDummy";
      break;
    default:
      throw std::runtime_error("Can't convert this MachineState to string. No such MachineState available!");
      break;
  }
}

std::string StateClientBase::StateReturnTypeToString(StateReturnType state) {
  switch (state) {
    case StateReturnType::kSuccess:
      return "kSuccess";
      break;
    case StateReturnType::kGeneralError:
      return "kGeneralError";
      break;
    case StateReturnType::kBusy:
      return "kBusy";
      break;
    default:
      throw std::runtime_error("Can't convert this StateReturnType to string. No such MachineState available!");
      break;
  }
}

std::string StateClientBase::StateManagementMessageFunctionToString(internal::StateManagementMessageFunction function) {
  switch (function) {
    case internal::StateManagementMessageFunction::kGetState:
      return "kGetState";
      break;
    case internal::StateManagementMessageFunction::kResponse:
      return "kResponse";
      break;
    case internal::StateManagementMessageFunction::kSetState:
      return "kSetState";
      break;
    default:
      throw std::runtime_error(
          "Can't convert this StateManagementMessageFunction to string. No such StateManagementMessageFunction "
          "available!");
      break;
  }
}

internal::StateManagementMessage StateClientBase::PrepareStateManagementMessage(
    internal::StateManagementMessageFunction function, internal::StringView functionGroup, StateReturnType response) {
  internal::StateManagementMessage created_message;

  created_message.function = function;

  // Copy function group name
  auto iter_fg = std::copy(functionGroup.begin(), functionGroup.end(), created_message.functionGroup.begin());
  if (iter_fg != created_message.functionGroup.end()) {
    *iter_fg = '\0';
  } else {
    log_.LogWarn() << "Function group string couldn't be explicitly null terminated";
  }

  created_message.response = response;

  return created_message;
}

internal::StateManagementMessage StateClientBase::CreateStateManagementMessage(
    internal::StateManagementMessageFunction function, internal::StringView functionGroup, MachineState machine_state,
    StateReturnType response) {
  (void)machine_state;
  // Create message
  return PrepareStateManagementMessage(function, functionGroup, response);
}

internal::StateManagementMessage StateClientBase::CreateStateManagementMessage(
    internal::StateManagementMessageFunction function, internal::StringView functionGroup, std::string state,
    StateReturnType response) {
  // Prepare message without state
  internal::StateManagementMessage created_message = PrepareStateManagementMessage(function, functionGroup, response);
  // Copy function group state name
  auto iter_state = std::copy(state.begin(), state.end(), created_message.state.begin());
  if (iter_state != created_message.state.end()) {
    *iter_state = '\0';
  } else {
    log_.LogWarn() << "Function group state string couldn't be explicitly null terminated";
  }

  return created_message;
}

void StateClientBase::PrintStateManagementMessage(internal::StateManagementMessage message) {
  std::string tmp_machine_state(message.state.begin(), message.state.end());
  std::string tmp_function_group(message.functionGroup.begin(), message.functionGroup.end());

  log_.LogDebug() << " Message content: \n____________________________________________________ \n"
                  << "|--> StateManagementMessageFunction: " << StateManagementMessageFunctionToString(message.function)
                  << "\n"
                  << "|--> FunctionGroupToString: " << tmp_function_group.c_str() << "\n"
                  << "|--> MachineStateToString: " << tmp_machine_state.c_str() << "\n"
                  << "|--> Return value: " << StateReturnTypeToString(message.response) << "\n"
                  << "|___________________________________________________";
}

}  // namespace exec
}  // namespace ara
