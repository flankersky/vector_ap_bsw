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
/**        \file   process.h
 *        \brief  The representation of an adaptive application.
 *
 *      \details  Contains data from application manifest and methods to control the execution of an adaptive
 *applications.
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_PROCESS_H_
#define SRC_ARA_EXEC_INTERNAL_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process_interface.h>
#include <osabstraction/messagequeue/receiverqueue.h>
#include <osabstraction/messagequeue/senderqueue.h>
#include <osabstraction/process/cpu_core_control.h>
#include <osabstraction/process/process.h>
#include <vac/container/static_list.h>
#include <vac/container/static_vector.h>
#include <vac/container/string_view.h>
#include <vac/memory/optional.h>
#include <vac/testing/test_adapter.h>
#include <ara/log/logging.hpp>
#include <array>
#include <chrono>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ara/exec/internal/types.h"
#include "internal/application_client_internal.h"

namespace ara {
namespace exec {

/** Forward declaration for unit testing */
class ApplicationClientTestFixture;
/** Forward declaration for unit testing */
class ApplicationClientTestFixture_QueueSetup_Test;
/** Forward declaration for unit testing */
class ApplicationClientTestFixture_ReportApplicationState_Test;

namespace internal {

/**
 * \brief Timeout for receiving application client messages.
 */
const std::chrono::steady_clock::duration kTimeoutReceiveApplicationState(std::chrono::microseconds(500));

/**
 * \brief Path for the storage of the last reset cause
 */
const char kLastResetCausePath[] = "./last_reset_cause";

/**
 * \brief The representation of an application.
 */
class Process final : public ProcessInterface {
 public:
  /**
   * \brief List of Function Group Elements
   */
  using FunctionGroupStates = vac::container::StaticList<SafeString>;

  /**
   *  \brief Function group structure
   *  \trace SWS_EM_01109
   */
  class FunctionGroup {
   public:
    /**
     * \brief Constructor
     * \param fg_name Name of function group
     * \param active_state_ref pointer to active state of function group
     */
    explicit FunctionGroup(const SafeString& fg_name, const SafeString* const* active_state_ref)
        : name(fg_name), active_state(active_state_ref), is_active(false) {}

    /**
     * \brief Function group name
     */
    const SafeString& name;

    /**
     * \brief Reference to pointer to active state of function group
     */
    const SafeString* const* active_state;

    /**
     * \brief Status if function group is enabled.
     */
    bool is_active;

    /**
     * \brief States of function group
     */
    vac::container::StaticList<std::reference_wrapper<const SafeString>> states;
  };

  /**
   * \brief Typedef for list of function groups
   */
  using FunctionGroupList = vac::container::StaticList<Process::FunctionGroup>;
  /**
   * \brief Constructor to construct a complete application.
   * \param process_name The name of the process as defined in the application manifest.
   * \param binary_path The path to the application binary.
   * \param dependencies Startup dependencies to other applications.
   * \param is_platform_application Application is platform application and not buisness logic application
   * \param resource_group The resource group of the application.
   * \param scheduling_policy The scheduling policy which shall be applied for the application.
   * \param scheduling_priority The scheduling priority which shall be applied for the application.
   * \param startup_option The arguments passed to the application on startup.
   * \param cpu_core_control The CPU core control instance to set the core affinity for processes.
   * \param is_adaptive_application Is this an adaptive process or not.
   * \param log_ctx_id The context id used for logging.
   * \param log_ctx_description The context description used for logging.
   * \param function_groups List of process function groups
   */
  Process(Name&& process_name,                                             // NOFORMAT
          SafeString&& binary_path,                                        // NOFORMAT
          Dependencies&& dependencies,                                     // NOFORMAT
          const bool is_platform_application,                              // NOFORMAT
          ResourceGroup&& resource_group,                                  // NOFORMAT
          SchedulingPolicies&& scheduling_policy,                          // NOFORMAT
          const unsigned int& scheduling_priority,                         // NOFORMAT
          std::vector<std::string>&& startup_option,                       // NOFORMAT
          const osabstraction::process::CPUCoreControl& cpu_core_control,  // NOFORMAT
          const bool is_adaptive_application = true,                       // NOFORMAT
          std::unique_ptr<FunctionGroupList> function_groups = nullptr,    // NOFORMAT
          const std::string& log_ctx_id = "00",                            // NOFORMAT
          const std::string& log_ctx_description = "ara::exec::internal::Application");

  virtual ~Process();

  /**
   * \brief Returns the application state.
   * \return The application state.
   */
  ProcessState GetState();

  /**
   * \brief Returns the name of the application process.
   * \return The application name.
   * \trace SWS_EM_01017
   */
  const Name& GetProcessName() const noexcept;

  /**
   * \brief Returns the dependencies the application requires before it can run.
   * \return The startup dependencies to other applications.
   */
  const Dependencies& GetDependencies();

  /**
   * \brief Returns if the application is a platform component.
   */
  bool IsPlatformApplication();

  /**
   * \brief Returns the startup options of the application
   * \return The startup options of the application.
   */
  const std::vector<std::string>& GetStartUpOptions();

  /**
   * \brief Starts the application.
   */
  void Start();

  /**
   * \brief Requests the application to shutdown.
   */
  void Shutdown();

  /**
   * \brief Forces the application to terminate.
   */
  void Kill();

  /**
   * \brief Set process status - on/off - of specified function group.
   *
   * When function group status changed, global active function group counter is modified.
   *
   * \param functionGroup Name of function group
   */
  void UpdateProcessStatus(StringView functionGroup);

  /**
   * \brief Verify if at least one function group or machine state is active
   * \return True if at least one function group or machine state is active
   */
  bool HasActiveFunctionGroup() const noexcept;

  /**
   * \brief Checks whether the process is terminated.
   * \return True if process is terminated
   * \trace DSGN-Exec23095
   */
  bool IsProcessTerminated();

  /**
   * \brief Returns process ID of the process.
   * \return Process ID of the process
   */
  osabstraction::process::ProcessId GetPid();

  /**
   * Handle application client messages
   */
  void HandleApplicationClientMessages();

 private:
  /**
   * \brief Deleted default constructor.
   */
  Process() = delete;
  /**
   * \brief Deleted default constructor.
   */
  Process(Process&& other) = delete;
  /**
   * \brief Gets the reported application state via ipc.
   * \trace DSGN-Exec23098
   */
  void UpdateState();

  /**
   * \brief Sets the external application state to the internal state.
   * \param state The external application state to be transformed to internal state
   */
  void SetState(ara::exec::ApplicationState& state);

  /**
   * \brief Search for requested state in group and adjust active status of group when necessary
   * \param group Function group instance
   */
  void UpdateFunctionGroupStatus(FunctionGroup& group);

  /**
   * \brief Increment number of active function groups
   * \throws std::overflow_error When number of activated group is greater than data type size
   */
  void ActivateGroup();

  /**
   * \brief Decrement number of active function groups
   * \throws std::underflow_error When more function groups were deactivated than activated
   */
  void DeactivateGroup();

  /**
   * \brief Open application client queues
   *
   * \param id The process ID of the process, for which the queues should be opened.
   */
  void OpenApplicationClientQueues(osabstraction::process::ProcessId id);

  /**
   * \brief Handle already received IPC message
   */
  void ProcessIpcMessage(internal::ApplicationClientMessage& message);

  /**
   * \brief Store the reset cause to disk
   */
  void StoreResetCause(ResetCause& cause);

  /**
   * \brief Read the reset cause from disk
   */
  bool ReadResetCause(ResetCause& cause);

  /**
   * \brief The application state.
   */
  ProcessState state_;

  /**
   * \brief The process of the application.
   */
  vac::memory::optional<osabstraction::process::Process> process_;

  /**
   * \brief The name of the process as defined in the application manifest.
   */
  const Name process_name_;

  /**
   * \brief The path to the executable.
   */
  const SafeString binary_path_;

  /**
   * \brief Dependencies to other applications and their state.
   * According to TPS_ManifestSpecification [constr_1484] the explicit launch dependency is reserved for platform
   * modules.
   */
  const Dependencies dependencies_;

  /**
   * \brief Is This application a platform component?
   */
  const bool is_platform_application_;

  /**
   * \brief The applicable resource groups.
   */
  const ResourceGroup resource_group_;

  /**
   * \brief The scheduling policy set for this application.
   */
  const SchedulingPolicies scheduling_policy_;

  /**
   * \brief The scheduling priority set for this application.
   */
  const unsigned int scheduling_priority_;

  /**
   * \brief The message queue to send to the application client.
   */
  osabstraction::messagequeue::SenderQueue ipc_handle_em_;

  /**
   * \brief The message queue to get messages from the application client.
   */
  osabstraction::messagequeue::ReceiverQueue ipc_handle_app_;

  /**
   * \brief Path to the Application Client message queue.
   */
  std::array<char, kMaxIpcPathLengthTx> ipc_tx_path_;

  /**
   * \brief Path to the Execution Management message queue.
   */
  std::array<char, kMaxIpcPathLengthRx> ipc_rx_path_;

  /**
   * \brief The arguments passed to the application when started.
   * Realized as array of pointers to character strings.
   *
   * \trace SWS_EM_01012
   */
  std::vector<std::string> startup_option_;

  /**
   * \brief The CPU core control instance to set the core affinity for processes.
   */
  osabstraction::process::CPUCoreControl cpu_core_control_;

  /**
   * \brief True if this is an adaptive application process which reports its state and reacts on shutdown
   * requests. This is a Vector extension to support legacy applications.
   */
  const bool is_adaptive_application_;

  /**
   * \brief List of function groups related to process
   * \trace SWS_EM_01109
   */
  std::unique_ptr<FunctionGroupList> function_groups_;

  /**
  * \brief The logger instance.
  */
  ara::log::Logger& log_;

  /**
   * \brief track if process has been started
   */
  bool process_started_;

  /**
   * \brief Count active function groups
   */
  std::uint16_t active_function_group_cnt_;

  FRIEND_TEST(ProcessTestFixture, defaultCtor);                  /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, nonDefaultCtor);               /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, startupShutdown);              /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, startupKill);                  /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, startupRun);                   /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, detectApplicationTermination); /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessTestFixture, UpdateProcessStatus);          /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessListBuilderTestFixture,
              adaptiveApplicationManifest);                         /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessListBuilderTestFixture, functionGroups);       /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ProcessListBuilderTestFixture, cores);                /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ApplicationListBuilder, adaptiveApplicationManifest); /**< \brief Friend declaration for testing. */
  friend class ara::exec::ApplicationClientTestFixture;             /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ara::exec::ApplicationClientTestFixture, QueueSetup); /**< \brief Friend declaration for testing. */
  FRIEND_TEST(ara::exec::ApplicationClientTestFixture,
              ReportApplicationState); /**< \brief Friend declaration for testing. */
};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_PROCESS_H_
