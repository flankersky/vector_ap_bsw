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
/**        \file  state_client_base.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_BASE_H_
#define LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/messagequeue/receiverqueue.h>
#include <osabstraction/messagequeue/senderqueue.h>
#include <osabstraction/process/process.h>
#include <ara/log/logging.hpp>
#include <string>

#include "internal/state_client_internal.h"

namespace ara {
namespace exec {

/**
 * \brief The StateClientBase class provides the functionality to handle the message queue communication between
 * Execution Management and Machine State Client
 */
class StateClientBase {
 public:
  /**
   * \brief Constructor
   */
  StateClientBase();

  /**
   * \brief Destructor
   */
  virtual ~StateClientBase() = default;

  /**
   * \brief Sends a Message to the message queue
   *
   * \param mq_fd The message queue the message should be send to
   * \param message The message that should be send
   *
   * \throws std::system_error   Sending was not successful.
   */
  void SendToMessageQueue(osabstraction::messagequeue::SenderQueue& mq_fd, internal::StateManagementMessage* message);

  /**
   * \brief Receives a Message from the message queue
   *
   * \param mq_fd The message queue the message should be received from
   * \param message A message struct to save the message
   * \return Returns the number of bytes which have been received.
   *
   * \throws std::system_error   Receiving failed.
   */
  std::size_t ReceiveFromMessageQueue(osabstraction::messagequeue::ReceiverQueue& mq_fd,
                                      internal::StateManagementMessage* message);

  /**
   * \brief Converts a given MachineState into a string
   *
   * \param state A MachineState
   *
   * \return A string containing the MachineState as text
   */
  std::string MachineStateToString(MachineState state);

  /**
   * \brief Converts a given StateReturnType into a string
   *
   * \param state A StateReturnType
   *
   * \return A string containing the StateReturnType as text
   */
  std::string StateReturnTypeToString(StateReturnType state);

  /**
   * \brief Converts a given StateManagementMessageFunction into a string
   *
   * \param function A StateManagementMessageFunction
   *
   * \return A string containing the StateManagementMessageFunction as text
   */
  std::string StateManagementMessageFunctionToString(internal::StateManagementMessageFunction function);

  /**
   * \brief Creates a StateManagementMessage struct from the given parameters
   *
   * \param function The StateManagementMessageFunction
   * \param functionGroup Name of function group
   * \param state The Machine State
   * \param response A StateReturnType
   *
   * \return The Machine State Message filled with given parameters
   */
  internal::StateManagementMessage CreateStateManagementMessage(internal::StateManagementMessageFunction function,
                                                                internal::StringView functionGroup, MachineState state,
                                                                StateReturnType response);

  /**
   * \brief Creates a StateManagementMessage struct from the given parameters
   *
   * \param function The StateManagementMessageFunction
   * \param functionGroup Name of function group
   * \param state A string which contains the machine state
   * \param response A StateReturnType
   *
   * \return The Machine State Message filled with given parameters
   */
  internal::StateManagementMessage CreateStateManagementMessage(internal::StateManagementMessageFunction function,
                                                                internal::StringView functionGroup, std::string state,
                                                                StateReturnType response);
  /**
   * \brief Prints the given message.
   *
   * \param message The Machine State Message
   */
  void PrintStateManagementMessage(internal::StateManagementMessage message);

 protected:
  /**
   * \brief Logging context
   */
  ara::log::Logger& log_;

 private:
  /**
   * \brief The priority of the message queue messages
   */
  unsigned int prio_ = 0;

  /**
   * \brief Prepare a StateManagementMessage struct from the given parameters without function group state
   *
   * \param function The StateManagementMessageFunction
   * \param functionGroup Name of function group
   * \param response A StateReturnType
   *
   * \return The Machine State Message filled with given parameters
   */
  internal::StateManagementMessage PrepareStateManagementMessage(internal::StateManagementMessageFunction function,
                                                                 internal::StringView functionGroup,
                                                                 StateReturnType response);
};

}  // namespace exec
}  // namespace ara

#endif  // LIB_STATECLIENT_INC_INTERNAL_STATE_CLIENT_BASE_H_
