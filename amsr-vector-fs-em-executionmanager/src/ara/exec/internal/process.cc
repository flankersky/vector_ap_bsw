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
/**        \file  process.cc
 *        \brief  The representation of an adaptive application.
 *
 *      \details  Contains data from application manifest and methods to control the execution of an adaptive
 *applications.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ara/log/logging.hpp>
#include <limits>

#include <array>
#include <chrono>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "internal/application_client_internal.h"
#include "osabstraction/io/file/file.h"
#include "osabstraction/messagequeue/queuebuilder.h"

namespace ara {
namespace exec {
namespace internal {

/**
 * \trace SWS_EM_01002
 */
Process::Process(Name&& process_name,                                             // NOFORMAT
                 SafeString&& binary_path,                                        // NOFORMAT
                 Dependencies&& dependencies,                                     // NOFORMAT
                 const bool is_platform_application,                              // NOFORMAT
                 ResourceGroup&& resource_group,                                  // NOFORMAT
                 SchedulingPolicies&& scheduling_policy,                          // NOFORMAT
                 const unsigned int& scheduling_priority,                         // NOFORMAT
                 std::vector<std::string>&& startup_option,                       // NOFORMAT
                 const osabstraction::process::CPUCoreControl& cpu_core_control,  // NOFORMAT
                 const bool is_adaptive_application,                              // NOFORMAT
                 std::unique_ptr<FunctionGroupList> function_groups,              // NOFORMAT
                 const std::string& log_ctx_id,                                   // NOFORMAT
                 const std::string& log_ctx_description                           // NOFORMAT
                 )

    /* #10 Initialize member variables */
    : state_(ProcessState::kIdle),
      process_(),
      process_name_(std::move(process_name)),
      binary_path_(std::move(binary_path)),
      dependencies_(std::move(dependencies)),
      is_platform_application_(is_platform_application),
      resource_group_(std::move(resource_group)),
      scheduling_policy_(std::move(scheduling_policy)),
      scheduling_priority_(scheduling_priority),
      ipc_handle_em_(),
      ipc_handle_app_(),
      ipc_tx_path_({0}),
      ipc_rx_path_({0}),
      startup_option_(std::move(startup_option)),
      cpu_core_control_(cpu_core_control),
      is_adaptive_application_(is_adaptive_application),
      function_groups_(std::move(function_groups)),
      log_(ara::log::CreateLogger(log_ctx_id, log_ctx_description)),
      process_started_(false),
      active_function_group_cnt_(0) {
  log_.LogDebug() << __func__ << " Constructed Process.";
}

Process::~Process() { log_.LogDebug() << __func__ << " Destructed Process."; }

/**********************************************************************************************************************
 * Application::Start()
 *********************************************************************************************************************/
/*!
 * \internal
 * - #10 Check if application was at least started once
 *  - #101 Set program image
 *  - #102 Construct the arguments for the application
 *  - #103 Create and execute the new process
 *  - #104 Create message queues for application client exchange
 *  - #105 Update application state
 * \endinternal
 */
void Process::Start() {
  /* #10 Check if application was at least started once */
  if ((state_ == ProcessState::kIdle || state_ != ProcessState::kTerminated) && (!process_.has_value())) {
    osabstraction::process::ProcessBuilder process_builder;

    /* #101 Set process parameters */
    process_builder.SetProgramImage(binary_path_.c_str())
        .SetSchedulingPriority(scheduling_priority_)
        .SetSchedulingPolicy(scheduling_policy_)
        .SetCpuCoreControl(&cpu_core_control_)
        .SetProcessCreatedCallout(std::bind(&Process::OpenApplicationClientQueues, this, std::placeholders::_1));

    /* #102 Construct the arguments for the application */
    for (auto option_it = startup_option_.begin(); option_it != startup_option_.end(); option_it++) {
      process_builder.AddArgument((*option_it).c_str());
    }

    // \trace SWS_EM_01012
    /* #103 Create and execute the new process */
    process_.emplace(process_builder.Build());
    process_started_ = true;

    /* #105 Update application state */
    // \trace SWS_EM_01003
    state_ = ProcessState::kStarted;
    log_.LogDebug() << __func__ << " Successfully started process with pid " << GetPid()
                    << " and scheduling parameters are set successfully.";
  }
}

void Process::OpenApplicationClientQueues(osabstraction::process::ProcessId id) {
  int bytes_written = std::snprintf(ipc_tx_path_.data(), ipc_tx_path_.size(), "%s%d", kIpcTxPath, id);
  if (bytes_written < 0 || bytes_written >= static_cast<int>(ipc_tx_path_.size())) {
    ara::log::LogError() << __func__ << " Path to tx message queue is too long.";
    return;
  }

  bytes_written = std::snprintf(ipc_rx_path_.data(), ipc_rx_path_.size(), "%s%d", kIpcRxPath, id);
  if (bytes_written < 0 || bytes_written >= static_cast<int>(ipc_rx_path_.size())) {
    ara::log::LogError() << __func__ << " Path to rx message queue is too long.";
    return;
  }

  /* #104 Create message queue for application state exchange */

  osabstraction::messagequeue::QueueBuilder queue_builder;
  try {
    ipc_handle_app_ = queue_builder.SetCreate()
                          .SetId(ipc_tx_path_.data())
                          .SetMessageSize(sizeof(internal::ApplicationClientMessage))
                          .SetQueueLength(10)
                          .BuildReceiverQueue();
    ara::log::LogDebug() << __func__ << " opened: " << ipc_tx_path_.data();
  } catch (...) {
    log_.LogError() << __func__
                    << " Could not open tx message queue for application client exchange. Affected application: "
                    << this->process_name_;
  }
  try {
    ipc_handle_em_ = queue_builder.SetCreate()
                         .SetId(ipc_rx_path_.data())
                         .SetMessageSize(sizeof(internal::ApplicationClientMessage))
                         .SetQueueLength(10)
                         .BuildSenderQueue();
    ara::log::LogDebug() << __func__ << " opened: " << ipc_rx_path_.data();
  } catch (...) {
    log_.LogError() << __func__
                    << " Could not open rx message queue for application client exchange. Affected application: "
                    << this->process_name_;
  }
}

/**
 * \internal
 * - #10 Send SIGTERM to the process if running
 * - #20 Log message in case process was not running
 * \endinternal
 *
 * \trace SWS_EM_01005
 */
void Process::Shutdown() {
  log_.LogDebug() << __func__ << " SIGTERM to process with pid " << GetPid();
  /* #10 Send SIGTERM to the process if running */
  if (state_ != ProcessState::kIdle || state_ != ProcessState::kTerminated) {
    if (is_adaptive_application_) {
      process_->SendTerminationRequest();
      state_ = ProcessState::kTerminatingRequested;
    } else {
      Kill();
    }
  } else {
    /* #20 Log message in case process was not running */
    log_.LogError() << __func__
                    << " Shutdown request to an not running application is not possible. Affected application: "
                    << this->process_name_;
  }
}

/**
 * \internal
 * - #10 If application is running:
 *  - #101 Release process
 *  - #102 Set application state to Terminated
 * - #20 Log message in case process was not running
 * \endinternal
 */
void Process::Kill() {
  log_.LogDebug() << __func__ << " SIGKILL to process with pid " << GetPid();

  /* #10 If application is running: */
  if (state_ != ProcessState::kIdle && state_ != ProcessState::kTerminated) {
    /* #101 Release process */
    process_.reset();
  } else {
    /* #20 Log message in case process was not running */
    log_.LogError() << __func__ << " Kill of an not running application is not possible. Affected application: "
                    << this->process_name_;
  }
}

bool Process::IsProcessTerminated() { return process_started_ && !(process_.has_value() && process_->IsRunning()); }

/**********************************************************************************************************************
 * Application::HandleApplicationClient()
 *********************************************************************************************************************/
/*!
 * \internal
 * - #10 Check if process exists
 *  - #101 Ask OS for application state
 *   - #1011 Update application state
 *  - #102 Else: Check if ipc channel is open
 *   - #1031 Read data via ipc
 *   - #1032 If this was the last message in the queue:
 *   - #1033 Update application state
 * - #20 If process does not exist
 *  - #201 Set application state to terminated
 * \endinternal
 *
 * \trace SWS_EM_01006
 * \trace SWS_EM_01052
 * \trace SWS_EM_01053
 * \trace SWS_EM_01055
 */
void Process::HandleApplicationClientMessages() {
  /* #102 Else: Check if ipc channel is open */
  if (!ipc_handle_app_.IsOpen() || !ipc_handle_em_.IsOpen()) {
    log_.LogError() << __func__
                    << " Message queue for application client ipc is not initialized. Affected application: "
                    << this->process_name_;
  } else {
    try {
      if (ipc_handle_app_.IsMessageAvailable()) {
        internal::ApplicationClientMessage message;

        /* #1031 Read data via ipc */
        osabstraction::messagequeue::ReceiveResult receive_result =
            ipc_handle_app_.Receive(&message, sizeof(message), kTimeoutReceiveApplicationState);

        if (receive_result.timeout) {
          /* no data available */
          log_.LogDebug() << __func__ << " No application client data available for pid: " << process_->GetId();
        } else {
          /* #1032 If this was the last message in the queue: */
          if (!ipc_handle_app_.IsMessageAvailable()) {
            ProcessIpcMessage(message);
          }
        }
        /* read data until no more state messages are available */
      }
    } catch (...) {
      log_.LogError() << __func__
                      << " Could not receive application client message. Affected application: " << this->process_name_;
    }
  }
}

bool Process::ReadResetCause(ResetCause& cause) {
  /* Read the last reset cause */
  int fd = open(kLastResetCausePath, O_RDWR);

  osabstraction::io::file::File lock_file(fd);

  try {
    lock_file.Lock();
  } catch (const std::system_error& error) {
    log_.LogError() << __func__ << " Error locking file: " << error.code().value() << " - " << error.what();
  }

  read(fd, reinterpret_cast<void*>(&cause), sizeof(cause));

  /* Release lock */
  lock_file.Unlock();
  close(fd);

  /* check enum value */
  if (cause < ResetCause::kHardReset || cause > ResetCause::kRapidPowerShutdown) {
    log_.LogError() << "Read invalid reset cause: " << static_cast<int>(cause);
    return false;
  }

  return true;
}

void Process::StoreResetCause(ResetCause& cause) {
  /* Store the last reset cause */
  int fd = open(kLastResetCausePath, O_RDWR | O_CREAT);
  osabstraction::io::file::File lock_file(fd);
  try {
    lock_file.Lock();
  } catch (const std::system_error& error) {
    log_.LogError() << __func__ << " Error locking file: " << error.code().value() << " - " << error.what();
  }
  write(fd, &cause, sizeof(cause));
  /* Release lock */
  lock_file.Unlock();
  close(fd);
}

void Process::ProcessIpcMessage(internal::ApplicationClientMessage& message) {
  /* check message id */
  switch (message.messageId) {
    case ApplicationClientMessageId::kState: {
      if (message.reportState.pid == process_->GetId()) {
        /* #1033 Update application state */
        SetState(message.reportState.state);
        log_.LogDebug() << __func__
                        << " Received state from application: " << static_cast<int>(message.reportState.state);
      } else {
        log_.LogError() << __func__ << " Received corrupted pid from application. Expected: " << process_->GetId()
                        << ", but received: " << message.reportState.pid;
      }
    } break;
    case ApplicationClientMessageId::kResetCause: {
      switch (message.resetCause.operation) {
        case ApplicationClientLastResetCauseOperation::kSet: {
          /* Store the last reset cause */
          StoreResetCause(message.resetCause.cause);
        } break;

        case ApplicationClientLastResetCauseOperation::kGet: {
          ResetCause cause;

          if (ReadResetCause(cause)) {
            message.resetCause.cause = cause;

            /* send message */
            log_.LogDebug() << "Read reset cause: " << static_cast<int>(message.resetCause.cause);

            ipc_handle_em_.Send(&message, sizeof(message));
          }
        } break;
      }
    } break;
    default:
      log_.LogWarn() << __func__ << " Unknown message received from application.";
  }
}

void Process::SetState(ara::exec::ApplicationState& state) {
  /* #1033 Transform the external application state to the internal state
   * \trace SWS_EM_01053
   * \trace SWS_EM_01055
   */
  switch (state) {
    case ara::exec::ApplicationState::kRunning:
      /* #1034 Update application state */
      state_ = internal::ProcessState::kRunning;
      break;
    case ara::exec::ApplicationState::kTerminating:
      /* #1034 Update application state */
      state_ = internal::ProcessState::kTerminating;
      break;
    default:
      log_.LogError() << __func__ << " Received corrupted state from application: " << static_cast<int>(state);
  }
}
/**
 * \internal
 * - Check if application already terminated
 * - Return the current application state
 * \endinternal
 */
ProcessState Process::GetState() {
  if (ProcessState::kIdle != state_ && ProcessState::kTerminated != state_) {
    /* #10 Check if process exists */
    if (IsProcessTerminated()) {
      /* #1011 Update application state */
      state_ = ProcessState::kTerminated;
    }
  }

  return state_;
}

/**
 * \internal
 * - #10 If process created, return process ID. Return -1 otherwise.
 * \endinternal
 */
osabstraction::process::ProcessId Process::GetPid() {
  /* #10 If process created, return process ID. Return -1 otherwise. */
  return process_.has_value() ? process_->GetId() : -1;
}

const Dependencies& Process::GetDependencies() { return dependencies_; }

const Name& Process::GetProcessName() const noexcept { return process_name_; }

bool Process::IsPlatformApplication() { return is_platform_application_; }

const std::vector<std::string>& Process::GetStartUpOptions() { return startup_option_; }

void Process::UpdateFunctionGroupStatus(FunctionGroup& group) {
  if ((group.active_state != nullptr) && (*group.active_state != nullptr)) {
    bool fg_active = false;
    // Verify if process contains requested function group state
    for (const auto& group_state : group.states) {
      if (group_state.get().compare(**(group.active_state)) == 0) {
        if (!group.is_active) {
          ActivateGroup();
        }
        fg_active = true;
        break;
      }
    }
    if (group.is_active && (!fg_active)) {
      DeactivateGroup();
    }
    group.is_active = fg_active;
  }
}

void Process::UpdateProcessStatus(StringView functionGroup) {
  // Check if containers are valid
  if (function_groups_) {
    // Iterate over all function groups of process
    for (auto& group : *function_groups_) {
      if (group.name.compare(functionGroup.data()) == 0) {
        UpdateFunctionGroupStatus(group);
        break;
      }
    }
  }
}

void Process::ActivateGroup() {
  if (active_function_group_cnt_ == std::numeric_limits<decltype(active_function_group_cnt_)>::max()) {
    throw std::overflow_error("Overflow of number of active groups");
  }
  active_function_group_cnt_++;
}

void Process::DeactivateGroup() {
  if (active_function_group_cnt_ == 0) {
    throw std::underflow_error("Underflow of number of active groups");
  }
  active_function_group_cnt_--;
}

bool Process::HasActiveFunctionGroup() const noexcept { return ((active_function_group_cnt_ == 0) ? false : true); }

}  // namespace internal
}  // namespace exec
}  // namespace ara
