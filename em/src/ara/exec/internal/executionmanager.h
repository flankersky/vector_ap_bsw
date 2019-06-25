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
/**        \file  executionmanager.h
 *        \brief  Main class representing the executionmanager.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_EXECUTIONMANAGER_H_
#define SRC_ARA_EXEC_INTERNAL_EXECUTIONMANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process_list_builder.h>
#include <osabstraction/messagequeue/queuebuilder.h>
#include <osabstraction/messagequeue/receiverqueue.h>
#include <osabstraction/messagequeue/senderqueue.h>
#include <osabstraction/process/process.h>
#include <vac/container/static_list.h>
#include <vac/testing/test_adapter.h>
#include <ara/log/logging.hpp>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "ara/exec/internal/functiongroup.h"
#include "ara/exec/internal/types.h"
#include "internal/state_client_base.h"

namespace ara {
namespace exec {

/** Forward declaration for unit testing */
class StateClientTestFixture;
/** Forward declaration for unit testing */
class ApplicationClientTestFixture;
/** Forward declaration for unit testing */
class ApplicationClientTestFixture_QueueSetup_Test;
/** Forward declaration for unit testing */
class ApplicationClientTestFixture_ReportApplicationState_Test;

namespace internal {

/**
 * \brief The minimal duration between a shutdown request to an app and the forced kill of an app.
 */
const std::chrono::steady_clock::duration kKill_offset(std::chrono::milliseconds(350));

/**
 * \brief The sleep duration for Executionmanager::Sleep().
 */
constexpr static const std::chrono::steady_clock::duration kSleepInterval(std::chrono::microseconds(10));

/**
 * \brief The states of the executionmanager.
 *
 * \trace SWS_EM_01023
 * \trace SWS_EM_01024
 * \trace SWS_EM_01025
 */
enum class ExecutionManagerStates {
  kError_state, /**< An error has occured. go to safe state. */
  kRunning,     /**< The executionmanager is in its normal operation state. */
};

/**
 * \brief Main class representing the application
 */
class Executionmanager {
 public:
  /**
   * Type definition of function group list
   */
  using FunctionGroupList = vac::container::StaticList<FunctionGroup>;

  /**
   * \brief Constructor
   * \param path_to_machine_manifest_schema Either the absolute path to the application manifest schema or the
   * relative path to the current working directory of the executionmanager.
   * \param path_to_machine_manifest Either the absolute path to the machine manifest or the relative path to the
   * current working directory of the executionmanager.
   * \param path_to_application_manifest_schema Either the absolute path to the application manifest schema or the
   * relative path to the current working directory of the executionmanager.
   * \param path_to_applications Either the absolute path to the applications or the relative path to the
   * current working directory of the executionmanager.
   * \param log_ctx_id The context id used for logging.
   * \param log_ctx_description The context description used for logging.
   * \param queue_builder Builder of message queue.
   * \param list_builder Builder of application list.
   */
  explicit Executionmanager(
      StringView path_to_machine_manifest_schema,      // NOFORMAT
      StringView path_to_machine_manifest,             // NOFORMAT
      StringView path_to_application_manifest_schema,  // NOFORMAT
      StringView path_to_applications,                 // NOFORMAT
      const std::string& log_ctx_id = "00",
      const std::string& log_ctx_description = "ara::exec::internal::Executionmanager",
      osabstraction::messagequeue::QueueBuilder&& queue_builder = osabstraction::messagequeue::QueueBuilder(),
      ProcessListBuilder&& list_builder = ProcessListBuilder());

  /**
   * \brief Destructor
   */
  ~Executionmanager() = default;

  /**
   * \brief Starts the executionmanager state machine.
   */
  void Run();

  /**
   * \brief Check EM state and perform action according to state
   */
  void ProcessEmState();

  /**
   * \brief Get Pointer to requested state
   * \param functionGroup Name of function group which state is searched
   * \param state Name of requested state
   * \return Returns pointer to requested state. If state wasn't found nullptr is returned
   */
  const SafeString* GetFunctionGroupStateRef(StringView functionGroup, StringView state) const {
    const SafeString* ret_str = nullptr;
    if (function_groups_) {
      for (const auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_str = fg.GetStateRef(state);
          break;
        }
      }
    }
    return ret_str;
  }

  /**
   * \brief Get Pointer to name of requested function group
   * \param functionGroup Name of requested function group
   * \return Returns pointer to requested function group name. If state wasn't found nullptr is returned
   */
  const SafeString* GetFunctionGroupNameRef(StringView functionGroup) const {
    const SafeString* ret_str = nullptr;
    if (function_groups_) {
      for (const auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_str = &fg.GetName();
          break;
        }
      }
    }
    return ret_str;
  }

  /**
   * \brief Set requested state of function group active
   * \param functionGroup Name of function group which state is searched
   * \param state Name of requested state
   * \return Returns ReturnType::kOk if state could be set active. Otherwise ReturnType::kNotOk is returned
   */
  ReturnType UpdateActiveFunctionGroupState(StringView functionGroup, StringView state) {
    ReturnType ret_val = ReturnType::kNotOk;
    if (function_groups_) {
      for (auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_val = fg.UpdateActiveState(state);
          break;
        }
      }
    }
    return ret_val;
  }

  /**
   * \brief Get Pointer to requested function group
   * \param functionGroup Name of requested function group
   * \return Returns pointer to requested function group. If state wasn't found nullptr is returned
   */
  const FunctionGroup* GetFunctionGroupRef(StringView functionGroup) const {
    const FunctionGroup* ret_group = nullptr;
    if (function_groups_) {
      for (const auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_group = &fg;
          break;
        }
      }
    }
    return ret_group;
  }

 private:
  /**
   * \brief Recursive shutdown of all Applications depending on the parameter (including the parameter)
   * \param app which should be shutdown
   */
  void ShutDownApp(const ProcessReferenceList::iterator app);

  /**
   * \brief Perform the running
   *
   * \trace DSGN-Exec23129
   * \trace SWS_EM_01060
   */
  void PerformRunning();

  /**
   * \brief Performs all necessary steps to change the machine mode
   *
   * \trace SWS_EM_01026
   */
  void PerformMachineModeChange();

  /**
   * \brief Reads data from machine manifest.
   * \param path_to_machine_manifest Either the absolute path to the machine manifest or the relative path to the
   * current working directory of the executionmanager.
   * \param path_to_machine_manifest_schema Either the absolute path to the application manifest schema or the
   * relative path to the current working directory of the executionmanager.
   * \returns ReturnType::kOk in case of success else ReturnType::kNotOk
   *
   * \trace DSGN-Exec23128
   */
  ReturnType ReadMachineManifest(StringView path_to_machine_manifest,
                                 StringView path_to_machine_manifest_schema) noexcept;

  /**
   * \brief Update all processes according to requested function group
   * \param functionGroup Name of function group
   */
  void UpdateAllProcesses(StringView functionGroup);

  /**
   * \brief Stops all running processes.
   *
   * \trace DSGN-Exec23106
   * \trace SWS_EM_01013
   */
  void StopAllRunningProcesses();

  /**
   * \brief Stops running processes after machine state change.
   *
   * \trace DSGN-Exec23106
   * \trace SWS_EM_01060
   */
  void StopRunningProcesses();

  /**
   * \brief Starts all apps which are referenced in applications_to_execute_.
   * The EM will not actively start depending processes. It is assumed that the dependent processes are configured
   * correctly and will be started during the next main loops.
   *
   * \trace DSGN-Exec23129
   * \trace SWS_EM_01030
   */
  void StartAllRunnableApps();

  /**
   * \brief Kills all non-idle apps.
   *
   * \trace DSGN-Exec23106
   */
  void KillAllApps();

  /**
   * \brief Iterates over all available applications and adds them to the list applications_to_execute_ according when
   * their machine mode matches with current_active_machine_mode_
   *
   * \trace DSGN-Exec23129
   */
  void UpdateListAppsToExecute();

  /**
   * \brief Checks the Status of the terminating applications.
   * If the kKill_offset duration is elapsed the remaining apps will be killed.
   *
   * \trace DSGN-Exec23106
   * \trace SWS_EM_01037
   * \trace SWS_EM_01000
   */
  void CheckStatusOfTerminatingApps();

  /**
   * \brief Sleep until a event or the timeout sleep_timeout_ occurs.
   */
  void Sleep();

  /**
   * \brief Handles the communication with the machine state client
   *
   * \trace SWS_EM_01026
   */
  void HandleMachineStateClientMessages();

  /**
   * \brief Recovery action to restart a process
   * The process should be restarted with the same startup config (resource group, scheduling policy, scheduling
   * priority and startup options) as before
   * \param process_id The identification of the process to be started
   * \return On success kOk, on failure KNotOk
   *
   *  \trace SWS_EM_01016
   *  \trace SWS_EM_01062
   */
  ReturnType RestartProcess(pid_t process_id);

  /**
   * \brief Recovery action to override the state of a process
   *
   * \param function_group
   * \param state
   * \return  On success kOk, on failure KNotOk
   *
   * \trace SWS_EM_01018
   * \trace SWS_EM_01061
   */
  ReturnType OverrideState(SafeString function_group, SafeString state);

  /**
   * \brief Handles the get state messages from machine state client
   * \param received_message The machine mode to be set.
   * \param response_message The message structure to be filled with state error response
   *
   */
  void HandleGetStateManagementMessage(const internal::StateManagementMessage& received_message,
                                       internal::StateManagementMessage& response_message);

  /**
   * \brief Handles the set state messages from machine state client
   * \param received_message The machine mode to be set.
   * \param response_message The message structure to be filled with state error response
   *
   */
  void HandleSetStateManagementMessage(const internal::StateManagementMessage& received_message,
                                       internal::StateManagementMessage& response_message);

  /**
   * \brief Handles the communication with the application client
   */
  void HandleApplicationClients();

  /**
   * \brief Check if function group was defined in machine manifest
   * \param functionGroup Function group to verify
   * \return True if function group was defined
   */
  bool HasFunctionGroup(StringView functionGroup) const {
    bool ret_val = false;
    if (function_groups_) {
      for (const auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_val = true;
          break;
        }
      }
    }
    return ret_val;
  }

  /**
   * \brief Check if function group was defined in machine manifest
   * \param functionGroup Function group to verify
   * \return Return pointer to active function group. When function group doesn't exist, nullptr is returned.
   */
  const SafeString* GetActiveFunctionGroupState(StringView functionGroup) const {
    const SafeString* ret_str = nullptr;
    if (function_groups_) {
      for (const auto& fg : *function_groups_) {
        if (fg.GetName().compare(functionGroup.data()) == 0) {
          ret_str = fg.GetActiveState();
          break;
        }
      }
    }
    return ret_str;
  }

  /**
   * Pointer to list of function groups
   */
  std::unique_ptr<FunctionGroupList> function_groups_;

  /**
   * \brief The internal state of the execution manager.
   */
  ExecutionManagerStates state_;

  /**
   * \brief The list of all adaptive applications in the system.
   */
  ProcessList process_all_;

  /**
   * \brief The list of all applications which shall start in current machine state.
   */
  ProcessReferenceList process_to_execute_;

  /**
   * \brief The list of all running applications.
   */
  ProcessReferenceList process_running_;

  /**
   * \brief The list of all applications which shall terminate.
   */
  ProcessReferenceList process_to_terminate_;

  /**
   * \brief The list of all idle applications.
   */
  ProcessReferenceStaticList process_idle_;

  /**
   * \brief Message containing requested function Group state
   *
   * Message will be unlocked after requested state change was processed
   */
  struct {
    SafeString functionGroup;
    SafeString state;
    bool in_progress;
  } state_change_req_;

  /**
   * \brief The point of time where the last shutdown request was sent to an application.
   */
  std::chrono::steady_clock::time_point shutdown_time_point_;

  /**
   * \brief The IPC file  of the message queue to write to a machine state client.
   */
  osabstraction::messagequeue::SenderQueue em_ipc_tx_;

  /**
   * \brief The IPC file handle of the message queue to read from a machine state client.
   */
  osabstraction::messagequeue::ReceiverQueue em_ipc_rx_;

  /**
   * \brief Instance of MachineStateClient Base to handle communication with MSC
   *
   * \trace SWS_EM_01028
   * \trace SWS_EM_01029
   */
  ara::exec::StateClientBase state_client_;

  /**
   * \brief The logger instance.
   */
  ara::log::Logger& log_;

  FRIEND_TEST(ExecutionManagerTestFixture, ValidMachineManifest);         /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ExecutionManagerTest, NoValidMachineManifestPath);          /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ExecutionManagerTestFixture, ShutdownAll);                  /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ExecutionManagerTestFixture, RecoveryActionOverrideState);  /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ExecutionManagerTestFixture, RecoveryActionRestartProcess); /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ExecutionManagerTestFixture, StopRunningProcesses);         /**< \brief Friend declaration for testing. */
  FRIEND_TEST(StateClientTestFixture, RegisterSuccess);                   /**< \brief Friend declaration for testing. */
  FRIEND_TEST(EmMockTestFixture, StartApplicationMock);                   /**< \brief Friend declaration for testing. */
  friend class ara::exec::StateClientTestFixture;                         /**< \brief Friend declaration for testing. */
  friend class EmMockTestFixture;                                         /**< \brief Friend declaration for testing. */
  friend class ara::exec::ApplicationClientTestFixture;                   /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ara::exec::ApplicationClientTestFixture, QueueSetup);       /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ara::exec::ApplicationClientTestFixture,
              ReportApplicationState); /**< \brief Friend declaration for testing. */
};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_EXECUTIONMANAGER_H_
