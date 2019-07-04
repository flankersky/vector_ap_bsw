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
/**        \file  state_client.hpp
 *        \brief  The StateClient class provides the functionality for an Application to request
 *        a Machine State switch or to retrieve the current Machine State to/from Execution Manager.
 *********************************************************************************************************************/

#ifndef LIB_STATECLIENT_INCLUDE_ARA_EXEC_STATE_CLIENT_HPP_
#define LIB_STATECLIENT_INCLUDE_ARA_EXEC_STATE_CLIENT_HPP_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/messagequeue/receiverqueue.h>
#include <osabstraction/messagequeue/senderqueue.h>
#include <osabstraction/process/process.h>
#include <vac/testing/test_adapter.h>
#include <chrono>
#include <string>
#include "internal/state_client_base.h"
#include "internal/state_client_internal.h"
#include "internal/state_return_type.h" /* Required per SWS_EM_02005 */

namespace ara {
namespace exec {

/**
 * \brief The StateClient class provides the functionality for an Application to request
 * a Machine State switch or to retrieve the current Machine State to/from Execution Manager.
 *
 * \trace SWS_EM_02006
 */
class StateClient : public StateClientBase {
 public:
  /**
   * \brief Creates an instance of StateClient which opens the Execution
   * Management’s communication channel for retrieving or requesting Machine
   * States from/to Execution Management.
   *
   * \trace SWS_EM_02007
   */
  StateClient();

  /**
   * \brief Destructor of the Machine State Client instance.
   * Deletes the StateClient instance.
   *
   * \trace SWS_EM_02008
   */
  virtual ~StateClient() = default;

  /**
   * \brief Retrieve the current Machine State from the Execution Manager. It sends a request over the corresponding
   * IPC channel
   *
   * \param[out] state String containing the current Machine State.
   * \param[in] functionGroup Function Group or the string "MachineState" to retrieve the current Machine State.
   *
   * \return kSuccess if retrieval operation succeeded. kBusy if Execution Management is busy and cannot respond.
   *  kGeneralError  if the Execution Management successfully processed the request and is able to return the current
   * Function Group State or Machine State
   *
   * \trace SWS_EM_02047
   */
  StateReturnType GetState(std::string functionGroup, std::string &state);

  /**
   * \brief Requests a new Machine State at the Execution Manager via the corresponding
   * IPC channel of Execution Management for changing the current Machine State.
   * The method returns after Machine State change is confirmed, or on timeout.
   *
   * \param functionGroup Function Group or the string "MachineState" to retrieve the current Machine State.
   * \param state String containing the requested Machine State.
   *
   * \return kSuccess if retrieval operation succeeded. kBusy if Execution Management is busy and cannot respond.
   *  kGeneralError  if the Execution Management successfully processed the request and is able to return the current
   * Function Group State or Machine State
   *
   *
   * \trace SWS_EM_02054
   */
  StateReturnType SetState(std::string functionGroup, std::string state);

 private:
  /**
   * \brief IPC handle to receive from the state client.
   */
  osabstraction::messagequeue::ReceiverQueue ipc_rx_;

  /**
   * \brief IPC handle to send to the state client.
   */
  osabstraction::messagequeue::SenderQueue ipc_tx_;

  /**
   * \brief Friend declaration for testing.
   */
  FRIEND_TEST(StateClientTestFixture, ValidInstantiationOfMSC);
};

}  // namespace exec
}  // namespace ara

#endif  // LIB_STATECLIENT_INCLUDE_ARA_EXEC_STATE_CLIENT_HPP_
