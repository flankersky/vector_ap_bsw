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
/**        \file  application_client.hpp
 *        \brief  ApplicationClient to report application state to the executionmanagement.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vac/testing/test_adapter.h>
#include <array>
#include <cstdint>
#include <iostream>

#include "internal/application_client_internal.h"
#include "osabstraction/messagequeue/receiverqueue.h"
#include "osabstraction/messagequeue/senderqueue.h"
#include "osabstraction/process/signalreceiver.h"

#ifndef LIB_APPLICATIONCLIENT_INCLUDE_ARA_EXEC_APPLICATION_CLIENT_HPP_
#define LIB_APPLICATIONCLIENT_INCLUDE_ARA_EXEC_APPLICATION_CLIENT_HPP_

namespace ara {
namespace exec {
// Forward declerations for unit tests
class ApplicationClientTestFixture_QueueSetup_Test;
class ApplicationClientTestFixture_ReportApplicationState_Test;
class ApplicationClientTestFixture_GetLastResetCause_Test;
class ApplicationClientTestFixture_SetLastResetCause_Test;

/**
 * \brief Enum Class to implement the application states.
 *
 * \trace SWS_EM_02000
 */
enum class ApplicationState : uint8_t { kRunning = 0x0, kTerminating = 0x1 };

/**
 * \brief Enum Class to implement the application return type.
 *
 * \trace SWS_EM_02070
 */
enum class ApplicationReturnType : uint8_t { kSuccess = 0x0, kGeneralError = 0x1 };

/**
 * \brief Enum Class to implement the reset cause.
 *
 * \trace SWS_EM_02041
 */
enum class ResetCause : uint8_t { kHardReset = 0x0, kSoftReset = 0x1, kKeyOffOnReset = 0x2, kRapidPowerShutdown = 0x3 };

/**
 * \brief Each Adaptive Application shall create an instance of this class to report its state to the Execution
 * Management.
 *
 * \trace SWS_EM_02001, DSGN-Exec23098
 */
class ApplicationClient {
 public:
  /**
   * \brief Constructor that creates the Application Client.
   * Creates an instance of ApplicationClient which opens the Execution Management's IPC channel.
   *
   * \throws std::runtime_error in case of the IPC path is too long or could not be opened
   *
   * \trace SWS_EM_02030
   */
  ApplicationClient();

  /**
   * \brief Destructor of the Application Client instance.
   *
   * \trace SWS_EM_02002
   */
  ~ApplicationClient();

  /**
   * \brief Reports the current application state to the Execution Management.
   *
   * Applications shall use this interface according to SWS_EM_02031
   *
   * \param state Internal application state which shall be reported.
   * \return whether reporting the state was successful or not
   *
   * \trace SWS_EM_02003
   */
  ApplicationReturnType ReportApplicationState(ApplicationState state);

  /**
   * \brief Reports the last reset cause to the Execution Management.
   *
   * \param cause Last reset cause
   * \return whether reporting the cause was successful or not
   *
   * \trace SWS_EM_02042
   */
  ApplicationReturnType SetLastResetCause(ResetCause cause);

  /**
   * \brief Retrieve the last reset cause from the Execution Management.
   *
   * \param cause Last reset cause
   * \return whether retrieving the cause was successful or not
   *
   * \trace SWS_EM_02043
   */
  ApplicationReturnType GetLastResetCause(ResetCause &cause);

 private:
  /**
   * \brief Opens the Execution Management's IPC channel.
   */
  void OpenQueues();

  /**
   * \brief The IPC handle to transmit messages to the ExecutionManager.
   * \see kIpcTxPath
   */
  osabstraction::messagequeue::SenderQueue ipc_tx_;

  /**
   * \brief The IPC handle to receive messages from the ExecutionManager.
   * \see kIpcRxPath
   */
  osabstraction::messagequeue::ReceiverQueue ipc_rx_;

  /**
   * \brief Path to the Application Client message queue.
   * \see kIpcTxPath
   */
  std::array<char, internal::kMaxIpcPathLengthTx> ipc_tx_path_;

  /**
   * \brief Path to the Execution Management message queue.
   * \see kIpcRxPath
   */
  std::array<char, internal::kMaxIpcPathLengthRx> ipc_rx_path_;

  /**
   * \brief Receiver of POSIX signals.
   */
  osabstraction::process::SignalReceiver signal_receiver_;

  /** \brief Friend declaration for testing. */
  FRIEND_TEST(ApplicationClientTestFixture, QueueSetup);
  /** \brief Friend declaration for testing. */
  FRIEND_TEST(ApplicationClientTestFixture, ReportApplicationState);
  /** \brief Friend declaration for testing. */
  FRIEND_TEST(ApplicationClientTestFixture, SetLastResetCause);
  /** \brief Friend declaration for testing. */
  FRIEND_TEST(ApplicationClientTestFixture, GetLastResetCause);
};

}  // namespace exec
}  // namespace ara

#endif  // LIB_APPLICATIONCLIENT_INCLUDE_ARA_EXEC_APPLICATION_CLIENT_HPP_
