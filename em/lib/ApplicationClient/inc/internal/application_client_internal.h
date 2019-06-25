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
/**        \file  application_client_internal.h
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/types.h>
#include <chrono>
#include <cstdint>
#include <iostream>

#include "osabstraction/process/process.h"

#ifndef LIB_APPLICATIONCLIENT_INC_INTERNAL_APPLICATION_CLIENT_INTERNAL_H_
#define LIB_APPLICATIONCLIENT_INC_INTERNAL_APPLICATION_CLIENT_INTERNAL_H_

namespace ara {
namespace exec {

/* forward declarations */
enum class ApplicationState : uint8_t;
enum class ResetCause : uint8_t;

namespace internal {

/**
 * \brief Path to Application Client message queue (write only for Application Client)
 *
 * \details A posix message queue is placed under an extra filesystem (not the root-filesystem),
 * therefore only a "/" is needed in front of the name of the mqueue. More information can be found
 * on the man page of mq_overview (7).
 *
 * The message queues have the following syntax: "/<kIpcTxPath><PID>"
 * e.g. "/application_client_app_tx_23"
 */
const char kIpcTxPath[] = "/application_client_app_tx_";

/**
 * \brief Path to Execution Manager message queue (read only for Application Client)
 *
 * \details A posix message queue is placed under an extra filesystem (not the root-filesystem),
 * therefore only a "/" is needed in front of the name of the mqueue. More information can be found
 * on the man page of mq_overview (7).
 *
 * The message queues have the following syntax: "/<kIpcRxPath><PID>"
 * e.g. "/application_client_app_rx_23"
 */
const char kIpcRxPath[] = "/application_client_app_rx_";

/**
 * \brief Timeout for receiving application client messages.
 */
constexpr std::chrono::steady_clock::duration kTimeoutReceiveApplicationClient(std::chrono::milliseconds(10));

/**
 * \brief Number of bytes to be reserved for the message queue to communicate the application client.
 */
constexpr unsigned int kMaxIpcPathLengthRx = sizeof(kIpcRxPath) + 32;

/**
 * \brief Number of bytes to be reserved for the message queue to communicate the application client.
 */
constexpr unsigned int kMaxIpcPathLengthTx = sizeof(kIpcTxPath) + 32;

/**
 * \brief the Enum implementing the messageId
 */
enum class ApplicationClientMessageId : uint8_t { kState = 0x1, kResetCause = 0x2 };

/**
 * \brief the Enum implementing the last reset cause message operation
 */
enum class ApplicationClientLastResetCauseOperation : uint8_t { kSet = 0x1, kGet = 0x2 };

/**
 * \brief The message with the last reset cause state.
 */
struct ApplicationClientMessageDataResetCause {
  ApplicationClientLastResetCauseOperation operation; /**< The operation */
  ResetCause cause;                                   /**< The actual cause */
};

/**
 * \brief The message with the application state.
 */
struct ApplicationClientMessageDataReportState {
  osabstraction::process::ProcessId pid; /**< The process id of the adaptive application */
  ara::exec::ApplicationState state;     /**< The actual application state */
};

/**
 * \brief general message type for application client
 */
struct ApplicationClientMessage {
  ApplicationClientMessageId messageId; /**< The messageId of the message */
  union {                               /**< The actual content of the message */
    ApplicationClientMessageDataReportState reportState;
    ApplicationClientMessageDataResetCause resetCause;
  };

  ApplicationClientMessage() = default;

  /**
   * \brief Constructor
   * \param reportState_ the state data
   */
  explicit ApplicationClientMessage(ApplicationClientMessageDataReportState reportState_) {
    messageId = ApplicationClientMessageId::kState;
    reportState = reportState_;
  }
  /**
   * \brief Constructor
   * \param resetCause_ the reset cause data
   */
  explicit ApplicationClientMessage(ApplicationClientMessageDataResetCause resetCause_) {
    messageId = ApplicationClientMessageId::kResetCause;
    resetCause = resetCause_;
  }
};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // LIB_APPLICATIONCLIENT_INC_INTERNAL_APPLICATION_CLIENT_INTERNAL_H_
