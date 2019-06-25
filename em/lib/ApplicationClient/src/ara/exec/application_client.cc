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
/**        \file  application_client.cc
 *        \brief  ApplicationClient to report application state to the execution management.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/exec/application_client.hpp"

#include <time.h>
#include <ara/log/logging.hpp>
#include <cstdio>
#include <system_error>

#include <exception>
#include <stdexcept>

#include "internal/application_client_internal.h"
#include "osabstraction/messagequeue/queuebuilder.h"
#include "osabstraction/process/process.h"

namespace ara {
namespace exec {

ApplicationClient::ApplicationClient() : ipc_tx_(), ipc_rx_(), signal_receiver_() {
  /* #10 Fetch current PID and append it to the ipc path. */
  osabstraction::process::ProcessId pid = osabstraction::process::GetProcessId();

  /* #10 Fetch current PID and append it to the ipc path. */
  int bytes_written_app = std::snprintf(ipc_tx_path_.data(), ipc_tx_path_.size(), "%s%d", internal::kIpcTxPath, pid);
  if (bytes_written_app < 0 || bytes_written_app >= static_cast<int>(ipc_rx_path_.size())) {
    ara::log::LogError() << __func__ << " Path to message queue is too long.";
    throw std::runtime_error("Path to tx message queue is too long.");
  }

  int bytes_written_em = std::snprintf(ipc_rx_path_.data(), ipc_rx_path_.size(), "%s%d", internal::kIpcRxPath, pid);
  if (bytes_written_em < 0 || bytes_written_em >= static_cast<int>(ipc_tx_path_.size())) {
    ara::log::LogError() << __func__ << " Path to message queue is too long.";
    throw std::runtime_error("Path to rx message queue is too long.");
  }

  OpenQueues();
}

void ApplicationClient::OpenQueues() {
  osabstraction::messagequeue::QueueBuilder builder;
  ara::log::LogDebug() << __func__ << " Creating queues";

  /* #20 Wait until EM created application client queue. */
  try {
    ipc_rx_ = builder.SetId(ipc_rx_path_.data()).BuildReceiverQueue();
    ara::log::LogDebug() << __func__ << " opened: " << ipc_rx_path_.data();
  } catch (...) {
    ara::log::LogError() << __func__ << " Rx message queue for application client communication could not be opened: "
                         << ipc_rx_path_.data();
    throw std::runtime_error("Rx message queue for application client communication could not be opened.");
  }

  try {
    ipc_tx_ = builder.SetId(ipc_tx_path_.data()).BuildSenderQueue();
    ara::log::LogDebug() << __func__ << " opened: " << ipc_tx_path_.data();
  } catch (...) {
    ara::log::LogError() << __func__ << " Tx Message queue for application client communication could not be opened: "
                         << ipc_tx_path_.data();
    throw std::runtime_error("Tx Message queue for application client communication could not be opened.");
  }
}

ApplicationClient::~ApplicationClient() {}

ApplicationReturnType ApplicationClient::ReportApplicationState(ApplicationState state) {
  osabstraction::process::ProcessId pid = osabstraction::process::GetProcessId();
  internal::ApplicationClientMessage message{internal::ApplicationClientMessageDataReportState{pid, state}};

  ara::log::LogDebug() << __func__ << " sending appstate from pid:" << message.reportState.pid
                       << " and state: " << static_cast<int>(message.reportState.state);

  try {
    ipc_tx_.Send(&message, sizeof(message));
  } catch (const std::system_error &e) {
    ara::log::LogError() << "Caught exception: " << e.what() << '\n';
    return ApplicationReturnType::kGeneralError;
  }

  return ApplicationReturnType::kSuccess;
}

ApplicationReturnType ApplicationClient::SetLastResetCause(ResetCause cause) {
  internal::ApplicationClientMessage message{internal::ApplicationClientMessageDataResetCause{
      internal::ApplicationClientLastResetCauseOperation::kSet, cause}};

  ara::log::LogDebug() << __func__ << " Sending set_last_reset_cause with cause " << static_cast<int>(cause);

  try {
    ipc_tx_.Send(&message, sizeof(message));
  } catch (const std::system_error &e) {
    ara::log::LogError() << "Caught exception: " << e.what() << '\n';
    return ApplicationReturnType::kGeneralError;
  }

  return ApplicationReturnType::kSuccess;
}

ApplicationReturnType ApplicationClient::GetLastResetCause(ResetCause &cause) {
  internal::ApplicationClientMessage message{internal::ApplicationClientMessageDataResetCause{
      internal::ApplicationClientLastResetCauseOperation::kGet, cause}};

  ara::log::LogDebug() << __func__ << " Sending get_last_reset_cause with cause " << static_cast<int>(cause);

  try {
    ipc_tx_.Send(&message, sizeof(message));
  } catch (std::runtime_error &e) {
    return ApplicationReturnType::kGeneralError;
  }

  /* wait and receive answer */

  /* #1031 Read data via ipc */
  osabstraction::messagequeue::ReceiveResult receive_result =
      ipc_rx_.Receive(&message, sizeof(message), internal::kTimeoutReceiveApplicationClient);

  if (receive_result.timeout) {
    /* no data available */
    ara::log::LogDebug() << __func__ << " No reset cause available";
  } else {
    /* #1032 If this was the last message in the queue: */
    if (!ipc_rx_.IsMessageAvailable()) {
      /* check message id */
      switch (message.messageId) {
        case internal::ApplicationClientMessageId::kResetCause: {
          switch (message.resetCause.operation) {
            case internal::ApplicationClientLastResetCauseOperation::kGet: {
              cause = message.resetCause.cause;
              return ApplicationReturnType::kSuccess;
            } break;
            default:
              ara::log::LogWarn() << __func__ << " Wrong operation for reset cause";
          }
        } break;

        default:
          ara::log::LogWarn() << __func__ << " Unknown message received from application";
      }
    }
  }

  return ApplicationReturnType::kGeneralError;
}

}  // namespace exec
}  // namespace ara
