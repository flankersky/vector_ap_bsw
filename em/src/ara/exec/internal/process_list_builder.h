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
/**        \file  process_list_builder.h
 *        \brief  The representation of all available applications in a list
 *
 *      \details  Contains all available applications and their states
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_PROCESS_LIST_BUILDER_H_
#define SRC_ARA_EXEC_INTERNAL_PROCESS_LIST_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process.h>
#include <ara/exec/internal/process_interface.h>
#include <ara/per/internal/json_parser.h>
#include <dirent.h>
#include <vac/container/static_list.h>
#include <ara/log/logging.hpp>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "ara/exec/internal/functiongroup.h"
#include "ara/exec/internal/types.h"

namespace ara {
namespace exec {
namespace internal {

class Executionmanager;

/**
 * \brief Typedef for a list of applications
 */
using ProcessList = std::vector<std::unique_ptr<ProcessInterface>>;

/**
 * \brief Typedef for list of application references.
 */
using ProcessReferenceList = std::vector<std::reference_wrapper<ProcessInterface>>;

/**
 * \brief Typedef for static list of application references.
 */
using ProcessReferenceStaticList = vac::container::StaticList<std::reference_wrapper<ProcessInterface>>;

/**
 * \brief Enum Class to define the error codes for process creation.
 */
enum class ProcessCreationError : uint8_t { kSuccess, kMachineModeError };

/**
 * \brief ProcessList Class to hold list of available applications and their states
 */
class ProcessListBuilder {
 public:
  ProcessListBuilder();
  virtual ~ProcessListBuilder() = default;

  /**
   * \brief Creates the application list
   * \trace DSGN-Exec23128
   *
   * \param em Instance of execution manager
   * \param path_to_applications contains the path where applications are placed (e.g. /opt)
   * \param path_to_application_manifest_schema contains the path to the application manifest schema file used for
   * validation
   * \return A list of adapitve applications found in the system.
   *
   */
  VIRTUALMOCK ProcessList Create(const Executionmanager &em, StringView path_to_applications,
                                 StringView path_to_application_manifest_schema);

 private:
  /**
   * \brief Function to determine the SchedulingPolicies from a string (read from json)
   */
  SchedulingPolicies DetermineSchedulingPolicy(const std::string &policy_string);

  /**
   * \brief Checks if the binary of an application exists
   */
  bool CheckIfApplicationBinaryExists(const std::string &app_binary_path, const std::string &application_name);

  /**
   * \brief Reads application name from Manifest.
   * \param manifest The manifest to read from.
   * \return The executablename
   */
  Name ReadExecutableName(const ara::per::internal::json::JsonDocument &manifest);

  /**
   * \brief Reads from Manifest whether the application is a real adaptive application.
   * \param manifest The manifest to read from.
   * \return true if the application is an adaptive application otherwise false.
   */
  bool ReadIsAdaptiveApplication(const ara::per::internal::json::JsonDocument &manifest) const;

  /**
   * \brief Reads the application category from Manifest.
   * \param manifest The manifest to read from.
   * \return true if platfrom level else false.
   */
  bool ReadCategory(const ara::per::internal::json::JsonDocument &manifest);

  /**
   * \brief Converts the integer from a execution dependency of the Application manifest to an ApplicationState.
   * \param execution_dependency_state The value to convert.
   * \return The converted application state.
   */
  ProcessState ConvertToProcessState(unsigned int execution_dependency_state);

  /**
   * \brief Extracts execution dependencies settings from startup configuration
   * \param app_manifest_path
   * \param startup_configs
   * \return
   */
  Dependencies GetExecutionDependency(const std::string &app_manifest_path, rapidjson::Value &startup_configs);

  /**
   * \brief Extracts process name from startup configuration
   * \param kManifestProcessName
   * \param process_entry
   * \return
   */
  Name GetProcessName(const char *kManifestProcessName, rapidjson::Value &process_entry);

  /**
   * \brief Extracts scheduling policy from startup configuration
   * \param app_manifest_path
   * \param startup_configs
   * \return
   */
  SchedulingPolicies GetSchedulingPolicy(const std::string &app_manifest_path, rapidjson::Value &startup_configs);

  /**
   * \brief Extracts scheduling priority from startup configuration
   * \param app_manifest_path
   * \param startup_configs
   * \return
   */
  unsigned int GetSchedulingPrio(const std::string &app_manifest_path, rapidjson::Value &startup_configs);

  /**
  * \brief Extracts startup options from startup configuration
  * \param app_manifest_path
  * \param startup_configs
  * \return
  */
  std::vector<std::string> GetStartupOptions(const std::string &app_manifest_path, rapidjson::Value &startup_configs);

  /**
   * \brief Append elements to process list
   * \param em Instance of execution manager
   * \param manifest
   * \param app_manifest_path
   * \param executable_name
   * \param category_platform_level
   * \param app_binary_path
   * \param application_list
   * \param is_adaptive_application
   */
  void UpdateProcessList(const Executionmanager &em, ara::per::internal::json::JsonDocument &manifest,
                         const std::string &app_manifest_path, const std::string &executable_name,
                         const bool category_platform_level, const std::string &app_binary_path,
                         ProcessList &application_list, const bool is_adaptive_application);

  /**
   * \brief Load and parse manifest file
   * \param [in] app_manifest_path
   * \param [in] file_name
   * \param [in] path_to_application_manifest_schema
   * \param [in] application_entry
   * \param [in,out] app_binary_path
   * \param [out] manifest
   * \return true in case of success otherwise false
   */
  bool LoadManifest(const std::string &app_manifest_path, const Name &file_name,
                    const std::string &path_to_application_manifest_schema, const struct dirent *application_entry,
                    std::string &app_binary_path, ara::per::internal::json::JsonDocument *manifest);

  /**
   * \brief Create list of processes
   * \details Create list of processes by going through directory recursively searching for manifest files, loading
   * files and creating process objects for each configuration
   * \param em Instance of execution manager
   * \param app_manifest_path
   * \param path_to_application_manifest_schema
   * \param application_entry
   * \param app_binary_path
   * \param application_list
   */
  void CreateProcessList(const Executionmanager &em, const std::string &app_manifest_path,
                         const std::string &path_to_application_manifest_schema, struct dirent *application_entry,
                         std::string &app_binary_path, ProcessList &application_list);

  /**
   * \brief Parse manifest file for function groups and return pointer to function group structure
   * \param em Instance of execution manager
   * \param startup_configs Object containing startup configurations
   * \return Pointer to list of function group structures
   */
  std::unique_ptr<Process::FunctionGroupList> GetFunctionGroups(const Executionmanager &em,
                                                                rapidjson::Value &startup_configs);

  /**
   * \brief Append function group to function group list
   * \param group Reference to function group
   * \param fg_states List of function group states
   * \param fg_list List of function groups
   */
  template <typename T>
  void AppendFunctionGroup(const FunctionGroup &group, T &fg_states,
                           std::unique_ptr<Process::FunctionGroupList> &fg_list);

  /**
   * \brief Parse manifest file for cores and get cpu_core_control from shallNotRunOn and shallRunOn.
   * \param app_manifest_path Manifest file path
   * \param startup_configs Object containing startup configurations
   * \return the CPU core mask
   */
  osabstraction::process::CPUCoreControl GetCpuCoreMask(StringView app_manifest_path,
                                                        rapidjson::Value &startup_configs);

  /**
   * \brief Logging context
   */
  ara::log::Logger &log_;

  /**
   * \brief the manifest file name
   */
  const std::string manifest_name_;
};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_PROCESS_LIST_BUILDER_H_
