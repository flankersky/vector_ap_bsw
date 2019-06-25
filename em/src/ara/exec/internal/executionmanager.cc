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
/**        \file  executionmanager.cc
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/exec/internal/executionmanager.h"
#include <ara/exec/internal/process_list_builder.h>
#include <ara/per/internal/exception/configuration_exceptions.h>
#include <ara/per/internal/json_parser.h>
#include <limits.h>
#include <sys/time.h>

#include <ara/exec/application_client.hpp>
#include <ara/log/logging.hpp>

#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <utility>

namespace ara {
namespace exec {
namespace internal {
Executionmanager::Executionmanager(StringView path_to_machine_manifest_schema, StringView path_to_machine_manifest,
                                   StringView path_to_application_manifest_schema, StringView path_to_applications,
                                   const std::string& log_ctx_id, const std::string& log_ctx_description,
                                   osabstraction::messagequeue::QueueBuilder&& queue_builder,
                                   ProcessListBuilder&& list_builder)
    : state_(ExecutionManagerStates::kRunning),
      em_ipc_tx_(),
      em_ipc_rx_(),
      state_client_(),
      log_(ara::log::CreateLogger(log_ctx_id, log_ctx_description)) {
  /* We assert there is an initial state called "Startup". See [SWS_EM_01023]
   * \trace SWS_EM_01023
   */

  /* SIGCHLD will be explicitly ignored to prevent child processes from being left back as zombies */
  osabstraction::process::IgnoreChildTerminationSignal();

  /* read machine manifest */
  if (ReturnType::kOk != ReadMachineManifest(path_to_machine_manifest, path_to_machine_manifest_schema)) {
    state_ = ExecutionManagerStates::kError_state;
  }

  /* create application list */
  process_all_ = list_builder.Create(*this, path_to_applications, path_to_application_manifest_schema);
  /* reserve space in lists */
  process_to_execute_.reserve(process_all_.size());
  process_running_.reserve(process_all_.size());
  process_to_terminate_.reserve(process_all_.size());
  process_idle_.reserve(process_all_.size());

  // Initially all the applications are idle.
  for (ProcessList::iterator app = process_all_.begin(); app != process_all_.end(); app++) {
    process_idle_.push_back(std::ref(**app));
  }

  // Build message queues
  queue_builder.SetCreate().SetMessageSize(internal::kStateManagementMessageSize).SetQueueLength(10);

  queue_builder.SetId(ara::exec::kIpcRxPathReceiveStateClientIpc);
  em_ipc_tx_ = queue_builder.BuildSenderQueue();

  queue_builder.SetId(ara::exec::kIpcTxPathSendStateClientIpc);
  em_ipc_rx_ = queue_builder.BuildReceiverQueue();

  // Initialize function group state request
  state_change_req_.in_progress = true;
  state_change_req_.functionGroup.reserve(ara::exec::internal::max_state_size_);
  state_change_req_.functionGroup.assign(kManifestFormatMachineState);
  state_change_req_.state.reserve(ara::exec::internal::max_state_size_);
  state_change_req_.state.assign(kManifestFormatMachineModeStartup);

  if (process_all_.empty()) {
    state_ = ExecutionManagerStates::kError_state;
    log_.LogError() << __func__ << " No applications found. Going into error state.";
  }
}

void Executionmanager::Run() {
  log_.LogDebug() << __func__ << " Executionmanager running";
  for (;;) {
    switch (state_) {
      case ExecutionManagerStates::kRunning: {
        PerformRunning();
        break;
      }
      case ExecutionManagerStates::kError_state:
      default: {
        log_.LogFatal() << __func__ << " Executionmanager: Running into error state and will shutdown now.";
        return;
      }
    }
    HandleApplicationClients();
    HandleMachineStateClientMessages();
    Sleep();
  }
}

void Executionmanager::Sleep() {
  /* TODO: wake up on signal reception (signal-handler thread with self-pipe) */
  /* TODO: wake up on machine_state change */
  /* wait until timeout elapsed or a file descriptor contains data */
  struct timespec timeout;
  timeout.tv_sec = 0;
  static_assert(std::chrono::duration_cast<std::chrono::nanoseconds>(kSleepInterval).count() <
                    std::numeric_limits<decltype(timeout.tv_nsec)>::max(),
                "Value exceeds nanoseconds");
  timeout.tv_nsec = static_cast<decltype(timeout.tv_nsec)>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(kSleepInterval).count());
  nanosleep(&timeout, nullptr);
}

void Executionmanager::PerformRunning() {
//log_.LogDebug() << __func__;

  // \trace SWS_EM_01026
  if (state_change_req_.in_progress) {
    PerformMachineModeChange();
    Sleep();
  }

  /* start new applications, after all previous running applications are terminated see [SWS_EM_01060] */
  if (process_to_terminate_.empty()) {
    StartAllRunnableApps();
  } else {
    CheckStatusOfTerminatingApps();
  }
  /* TODO: check status of all non-idle apps here */
}

void Executionmanager::PerformMachineModeChange() {
// for debug
log_.LogDebug() << __func__;

  // Update all processes after active function group state has changed
  UpdateAllProcesses(StringView(state_change_req_.functionGroup));

  /* reset all application which had to be started in the old machine mode */
  process_to_execute_.clear();

  /* stop running processes */
//  StopRunningProcesses();

  /* get all apps which shall run in the new state */
  UpdateListAppsToExecute();
  state_change_req_.in_progress = false;
}

ReturnType Executionmanager::ReadMachineManifest(StringView path_to_machine_manifest,
                                                 StringView path_to_machine_manifest_schema) noexcept {
  ReturnType retval{ReturnType::kNotOk};
  try {
    // TODO(PAASR-2241) Reimplement function with vector json parser
    // Read the manifest file
    ara::per::internal::json::JsonDocument manifest =
        ara::per::internal::json::LoadFile(path_to_machine_manifest.data(), path_to_machine_manifest_schema.data());

    std::unique_ptr<FunctionGroupList> fg_list = vac::language::make_unique<FunctionGroupList>();
    fg_list->reserve(manifest["machine"]["functionGroups"].GetArray().Size() + 1);
    fg_list->emplace_back(StringView("MachineState"));
    fg_list->back().GetStates().reserve(manifest["machine"]["states"].GetArray().Size());
    for (auto& machine_states : manifest["machine"]["states"].GetArray()) {
      fg_list->back().GetStates().push_back(machine_states.GetString());
    }

    for (auto& fg : manifest["machine"]["functionGroups"].GetArray()) {
      const SafeString& name = fg["name"].GetString();
      fg_list->emplace_back(StringView(name));
      fg_list->back().GetStates().reserve(fg["states"].GetArray().Size() + 1);
      // \trace SWS_EM_01110
      fg_list->back().GetStates().push_back(kFunctionGroupStateOff);
      for (auto& state : fg["states"].GetArray()) {
        const SafeString& fg_state = state.GetString();
        fg_list->back().GetStates().push_back(SafeString(fg_state.c_str()));
      }
    }
    function_groups_ = std::move(fg_list);
    // Initial MachineState is set
    retval = UpdateActiveFunctionGroupState(StringView(kManifestFormatMachineState),
                                            StringView(kManifestFormatMachineModeStartup));
  } catch (const ara::per::internal::json::exception::FileNotFound& e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (const ara::per::internal::json::exception::ParserError& e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (const ara::per::internal::json::exception::ValidationError& e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (...) {
    log_.LogError() << __func__ << " Caught unknown exception.";
  }

  return retval;
}

void Executionmanager::ShutDownApp(const ProcessReferenceList::iterator app_to_terminate) {
  // for debug
  log_.LogDebug() << "stopping: " << app_to_terminate->get().GetProcessName();

  const Name app_to_terminate_name = app_to_terminate->get().GetProcessName();
  // loop through all applications
  for (ProcessReferenceList::iterator app_of_loop = process_running_.begin(); app_of_loop != process_running_.end();
       app_of_loop++) {
    // just perform if the found app_to_terminate is not the same as the app_to_terminate
    if (app_of_loop->get().GetProcessName().compare(app_to_terminate_name) != 0) {
      // loop through all dependencies of the app
      for (Dependencies::const_iterator depandencie_it = app_of_loop->get().GetDependencies().begin();
           depandencie_it != app_of_loop->get().GetDependencies().end(); depandencie_it++) {
        // if the app has an dependency to the app_to_terminate do recursive call
        if (depandencie_it->first.compare(app_to_terminate_name) == 0) {
          // recursive call of function with the app has an dependency to the app_to_terminate
          ShutDownApp(app_of_loop);
        }
      }
    }
  }
  log_.LogDebug() << "stopping: " << app_to_terminate_name;
  // ask the application to shutdown
  app_to_terminate->get().Shutdown();
  /* add applications to new list for later check if application has actually performed the shutdown */
  process_to_terminate_.push_back(std::ref(*app_to_terminate));
}

void Executionmanager::UpdateAllProcesses(StringView functionGroup) {
  // Update all processes after active function group state has changed
  for (auto& proc : process_all_) {
    proc.get()->UpdateProcessStatus(functionGroup);
  }
}

void Executionmanager::StopAllRunningProcesses() {
  log_.LogDebug() << __func__ << " Execution manager: Stop all the running apps";

  /* terminate running applications */
  for (ProcessReferenceList::iterator app = process_running_.begin(); app != process_running_.end(); app++) {
    /* TODO: do we need a defined shutdown order?
     * first stop all business logic apps and then the platform components? */
    if (app->get().GetState() == ProcessState::kRunning) {
      ShutDownApp(app);
    }
  }
  /* store the current point in time to determine the moment when to kill the remaining applications. */
  shutdown_time_point_ = std::chrono::steady_clock::now();
  /*clear list */
  process_running_.clear();
}

void Executionmanager::StopRunningProcesses() {
  log_.LogDebug() << __func__ << " Execution manager: Stop the running apps";

  // \trace SWS_EM_01060
  // terminate running process as per changed machine state.
  for (ProcessReferenceList::iterator app = process_running_.begin(); app != process_running_.end();) {
    // Check if the process has previous machine state and process does not have
    // requested machine state.
    if (!(app->get().HasActiveFunctionGroup())) {
      ShutDownApp(app);

      // Remove from process running list
      app = process_running_.erase(app);
    } else {
      app++;
    }
  }

  /* store the current point in time to determine the moment when to kill the remaining applications. */
  shutdown_time_point_ = std::chrono::steady_clock::now();
}

void Executionmanager::KillAllApps() {
  log_.LogDebug() << __func__ << " Execution manager: Kill all the apps";
  for (ProcessList::iterator app = process_all_.begin(); app != process_all_.end(); app++) {
    /* terminate all non-idle applications */
    if (ProcessState::kIdle != (*app)->GetState() && ProcessState::kTerminated != (*app)->GetState()) {
      (*app)->Kill();
    }
  }
}

void Executionmanager::StartAllRunnableApps() {
  for (ProcessReferenceList::iterator app = process_to_execute_.begin(); app != process_to_execute_.end();) {
    if (app->get().GetDependencies().empty()) {
      /* Start application which has no dependencies */
      app->get().Start();
      log_.LogDebug() << __func__ << " Started application process: " << app->get().GetProcessName();
      /* Move application to applications_running_ list */
      process_running_.push_back(std::ref(*app));
      app = process_to_execute_.erase(app);
    } else {
      /* Application has dependencies. Check Status of the dependencies. */
      size_t dependencies_open = app->get().GetDependencies().size();
      /* Iterate over dependencies. */
      for (Dependencies::const_iterator dependency = app->get().GetDependencies().begin();
           dependency != app->get().GetDependencies().end(); dependency++) {
        /* Iterate over all applications to link dependency with application */
        for (ProcessList::iterator supplier_app = process_all_.begin(); supplier_app != process_all_.end();
             supplier_app++) {
          /* Check whether the process names match */
          if ((*supplier_app)->GetProcessName().compare(dependency->first) == 0) {
            /* Check whether the process state matches the dependency state */
            if ((*supplier_app)->GetState() == dependency->second) {
              /* The dependency is fulfilled */
              dependencies_open--;
            }
          }
        }
      }
      // \trace SWS_EM_01050
      if (0 == dependencies_open) {
        log_.LogDebug() << __func__ << " All dependencies from application process " << app->get().GetProcessName();
        /* All dependencies are fulfilled */
        app->get().Start();
        /* Move application to next list */
        app = process_to_execute_.erase(app);
        process_running_.push_back(std::ref(*app));

        break;
      } else {
        log_.LogDebug() << __func__ << " Dependencies open from application process " << app->get().GetProcessName()
                        << ": " << dependencies_open;
        ++app;
      }
    }
  }
}

void Executionmanager::UpdateListAppsToExecute() {
  for (ProcessReferenceStaticList::iterator app = process_idle_.begin(); app != process_idle_.end();) {
    /* check if application shall run */
    if (app->get().HasActiveFunctionGroup()) {
      // Only PLATFORM_LEVEL application shall startup in the initial machine mode "Startup".
      // APPLICATION_LEVEL applications shall be ignored.
      if (!(!(app->get().IsPlatformApplication()) && state_change_req_.state == kManifestFormatMachineModeStartup)) {
        // add it to the execute list
        process_to_execute_.push_back(std::ref(*app));
        log_.LogDebug() << __func__ << " Added app to execution list: " << app->get().GetProcessName();
        // remove from idle list
        app = process_idle_.erase(app);
      } else {
        ++app;
      }
    } else {
      ++app;
    }
  }
}

void Executionmanager::CheckStatusOfTerminatingApps() {
  /* check if point of time to kill applications has come */
  bool kill_apps = (kKill_offset <= (std::chrono::steady_clock::now() - shutdown_time_point_));

  for (ProcessReferenceList::iterator app = process_to_terminate_.begin(); app != process_to_terminate_.end();) {
    if (app->get().GetState() == ProcessState::kTerminated) {
      /* app is terminated, remove it from list */
      app = process_to_terminate_.erase(app);

      // add it to idle list
      process_idle_.push_back(*app);
    } else {
      if (kill_apps) {
        app->get().Kill();
        log_.LogDebug() << __func__ << " Killed app: " << app->get().GetProcessName();
      }
      ++app;
    }
  }
}

void Executionmanager::HandleApplicationClients() {
  for (ProcessReferenceList::iterator app = process_running_.begin(); app != process_running_.end(); app++) {
    app->get().HandleApplicationClientMessages();
  }
}

void Executionmanager::HandleMachineStateClientMessages() {
  // placeholder variable for message
  internal::StateManagementMessage received_message;

  // if one or more messages are in the mqueue
  bool message_available{false};
  try {
    message_available = em_ipc_rx_.IsMessageAvailable();
  } catch (...) {
    log_.LogError() << __func__ << " Caught unknown exception from em_ipc_rx_.IsMessageAvailable()";
  }

  if (message_available) {
    try {
      // Read message
      state_client_.ReceiveFromMessageQueue(em_ipc_rx_, &received_message);
      // Print message
      state_client_.PrintStateManagementMessage(received_message);
    } catch (const std::system_error& e) {
      log_.LogError() << __func__
                      << " Caught exception from state_client_.ReceiveFromMessageQueue(em_ipc_rx_, &received_message); "
                      << e.what();
      return;
    } catch (...) {
      log_.LogError() << __func__ << " Caught unknown exception from "
                                     "state_client_.ReceiveFromMessageQueue(em_ipc_rx_, &received_message)";

      return;
    }

    // create a default message to be filled in the switch-cases
    // TODO(PAASR-2233) One message for receive and response would be sufficient
    internal::StateManagementMessage response_message;
    // determine what to do
    switch (received_message.function) {
      /* Case GETSTATE */
      case internal::StateManagementMessageFunction::kGetState:
        HandleGetStateManagementMessage(received_message, response_message);
        break;

      /* Case SETSTATE
       * \trace SWS_EM_01026
       */
      case internal::StateManagementMessageFunction::kSetState: {
        HandleSetStateManagementMessage(received_message, response_message);
      } break;

      /* Case INVALIDREQUEST */
      default:
        // create invalid request response
        response_message.function = internal::StateManagementMessageFunction::kResponse;
        response_message.response = StateReturnType::kGeneralError;
        break;
    }
    try {
      state_client_.SendToMessageQueue(em_ipc_tx_, &response_message);
      state_client_.PrintStateManagementMessage(response_message);
    } catch (const std::system_error& e) {
      log_.LogError() << __func__
                      << " Caught exception from state_client_.SendToMessageQueue(em_ipc_tx_, &response_message): "
                      << e.what();
    } catch (...) {
      log_.LogError()
          << __func__
          << " Caught unknown exception from state_client_.SendToMessageQueue(em_ipc_tx_, &response_message)";
    }
  }
}

ReturnType Executionmanager::RestartProcess(pid_t process_id) {
  ReturnType result = ReturnType::kNotOk;
  bool found_app = false;
  /* search for the application */
  for (auto& app : process_all_) {
    if (app->GetPid() == process_id) {
      if (app->GetState() == ProcessState::kRunning) {
        /* kill the app */
        app->Kill();
        /* start app again */
        app->Start();
        result = ReturnType::kOk;
      } else {
        log_.LogWarn() << __func__ << " Requested process " << process_id
                       << " to restart, but process is not running. \n";
      }
      found_app = true;
    }
  }

  if (!found_app) {
    log_.LogWarn() << __func__ << " Requested process " << process_id << " to restart, but process is non existing. \n";
  }
  return result;
}

ReturnType Executionmanager::OverrideState(SafeString function_group, SafeString state) {
  // for debug
  log_.LogError() << __func__ ;

  // Stop "ongoing" state change
  // \trace SWS_EM_01061
  state_change_req_.in_progress = false;

  // Change active function group state
  // \trace SWS_EM_01018
  ReturnType ret_val = UpdateActiveFunctionGroupState(StringView(function_group), StringView(state));

  if (ret_val == ReturnType::kOk) {
    // Override requested function group state to change
    state_change_req_.state.assign(state.begin(), state.end());
    state_change_req_.functionGroup.assign(function_group.begin(), function_group.end());

    // Perform the function group state change immediately
    PerformMachineModeChange();
  } else {
    log_.LogWarn() << __func__ << " Function group state couldn't be updated.";
  }

  return ret_val;
}

/**
 * \brief Helper function for setting function group name or state
 * \param in Input Array
 * \param out Output converted from array to string
 * \param mq_element_out According element of output string
 */
template <typename T>
static inline void SetFunctionGroupElement(const T& in, SafeString& out, T& mq_element_out) {
  // Convert from char array to string
  out.assign(in.begin(), std::find(in.begin(), in.end(), '\0'));

  // create positive response message with the new machine mode
  auto iter = std::copy(out.begin(), out.end(), mq_element_out.begin());
  *iter = '\0';
}

void Executionmanager::HandleGetStateManagementMessage(const internal::StateManagementMessage& received_message,
                                                       internal::StateManagementMessage& response_message) {
  if (!state_change_req_.in_progress) {
    SetFunctionGroupElement(received_message.functionGroup, state_change_req_.functionGroup,
                            response_message.functionGroup);

    const SafeString* active_state = GetActiveFunctionGroupState(StringView(state_change_req_.functionGroup));
    if (active_state != nullptr) {
      // response with the actual machine state
      // copy current active state into the char array of the message
      std::copy(active_state->begin(), active_state->end(), state_change_req_.state.begin());
      std::copy(active_state->begin(), active_state->end(), response_message.state.begin());
      response_message.response = StateReturnType::kSuccess;
    } else {
      // requested machine mode is invalid
      // \trace SWS_EM_01034]
      response_message.response = StateReturnType::kGeneralError;
    }
  } else {
    log_.LogDebug() << __func__ << " There is currently an machine mode change ongoing";
    // create negative response
    // \trace SWS_EM_01034
    response_message.response = StateReturnType::kBusy;
  }
}

void Executionmanager::HandleSetStateManagementMessage(const internal::StateManagementMessage& received_message,
                                                       internal::StateManagementMessage& response_message) {
  if (!state_change_req_.in_progress) {
    // Copy group and state to data member and prepare response message
    SetFunctionGroupElement(received_message.functionGroup, state_change_req_.functionGroup,
                            response_message.functionGroup);
    SetFunctionGroupElement(received_message.state, state_change_req_.state, response_message.state);

    StringView functionGroup = StringView(state_change_req_.functionGroup);
    // Change active function group state
    if (UpdateActiveFunctionGroupState(functionGroup, StringView(state_change_req_.state)) == ReturnType::kOk) {
      // requested machine mode is valid
      state_change_req_.in_progress = true;

      response_message.response = StateReturnType::kSuccess;
    } else {
      // requested machine mode is invalid
      // \trace SWS_EM_01034
      response_message.response = StateReturnType::kGeneralError;
    }
  } else {
    log_.LogDebug() << __func__ << " There is currently an machine mode change ongoing";
    // create negative response
    // \trace SWS_EM_01034
    response_message.response = StateReturnType::kBusy;
  }
}

}  // namespace internal
}  // namespace exec
}  // namespace ara
