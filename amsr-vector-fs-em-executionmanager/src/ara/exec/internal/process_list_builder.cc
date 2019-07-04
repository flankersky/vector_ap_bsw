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
/**        \file  process_list_builder.cc
 *        \brief  The representation of all available applications in a list
 *
 *      \details  Traverse the file system for applications and manifests. Instantiate applications according to their
 *                manifests respectively. Holds all applications in a list.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process_list_builder.h>
#include <ara/per/internal/json_parser.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <vac/container/string_view.h>
#include <vac/language/cpp14_backport.h>
#include <ara/log/logging.hpp>
#include <cstring>
#include <exception>
#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "ara/per/internal/exception/configuration_exceptions.h"

#include "ara/exec/internal/executionmanager.h"
#include "ara/exec/internal/types.h"

namespace ara {
namespace exec {
namespace internal {

ProcessListBuilder::ProcessListBuilder()
    : log_(ara::log::CreateLogger("ProcessListBuilder",
                                  "Logging context for the amsr-vector-fs-em-executionmanager ProcessListBuilder")),
      manifest_name_("manifest") {}

SchedulingPolicies ProcessListBuilder::DetermineSchedulingPolicy(const std::string &policy_string) {
  if (policy_string.compare("SCHEDULING-POLICY-OTHER") == 0) {
    return SchedulingPolicies::kOther;
  } else if (policy_string.compare("SCHEDULING-POLICY-FIFO") == 0) {
    return SchedulingPolicies::kFifo;
  } else if (policy_string.compare("SCHEDULING-POLICY-ROUND-ROBIN") == 0) {
    return SchedulingPolicies::kRoundRobin;
  } else {
    log_.LogWarn() << __func__ << " Determination of scheduling policy failed. Using kFifo as default policy.";
    return SchedulingPolicies::kFifo;
  }
}

bool ProcessListBuilder::CheckIfApplicationBinaryExists(const std::string &app_binary_path,
                                                        const std::string &application_name) {
  return (access((app_binary_path + application_name).c_str(), F_OK) != -1);
}

Dependencies ProcessListBuilder::GetExecutionDependency(const std::string &app_manifest_path,
                                                        rapidjson::Value &startup_configs) {
  const char *kManifestProcessExecutionDependency{"executionDependency"};
  Dependencies dependencies;
  if (startup_configs.HasMember(kManifestProcessExecutionDependency)) {
    auto execution_dependencies = startup_configs[kManifestProcessExecutionDependency].GetObject();
    for (const auto &ed : execution_dependencies) {
      Name name = ed.name.GetString();
      ProcessState state = ConvertToProcessState(ed.value.GetUint());
      dependencies.emplace_back(name, state);
    }
  } else {
    log_.LogWarn() << __func__ << " " << app_manifest_path << " does not contain object "
                   << kManifestProcessExecutionDependency;
  }
  return dependencies;
}

Name ProcessListBuilder::GetProcessName(const char *kManifestProcessName, rapidjson::Value &process_entry) {
  Name process_name;
  if (process_entry.HasMember(kManifestProcessName)) {
    if (process_entry[kManifestProcessName].IsString()) {
      process_name = process_entry[kManifestProcessName].GetString();
    } else {
      log_.LogWarn() << __func__ << " "
                     << " object " << kManifestProcessName << " is not a string. Skipping this process.";
    }
  } else {
    log_.LogWarn() << __func__ << " "
                   << " does not contain object " << kManifestProcessName << " Skipping this process.";
  }
  return process_name;
}

SchedulingPolicies ProcessListBuilder::GetSchedulingPolicy(const std::string &app_manifest_path,
                                                           rapidjson::Value &startup_configs) {
  const char *kManifestProcessSchedulingPolicy{"schedulingPolicy"};
  SchedulingPolicies scheduling_policy{SchedulingPolicies::kOther};
  if (startup_configs.HasMember(kManifestProcessSchedulingPolicy)) {
    if (startup_configs[kManifestProcessSchedulingPolicy].IsString()) {
      scheduling_policy = DetermineSchedulingPolicy(startup_configs[kManifestProcessSchedulingPolicy].GetString());
    } else {
      log_.LogWarn() << __func__ << " " << app_manifest_path << " object " << kManifestProcessSchedulingPolicy
                     << " is not a string.";
    }
  } else {
    log_.LogWarn() << __func__ << " " << app_manifest_path << " does not contain object "
                   << kManifestProcessSchedulingPolicy;
  }
  return scheduling_policy;
}

unsigned int ProcessListBuilder::GetSchedulingPrio(const std::string &app_manifest_path,
                                                   rapidjson::Value &startup_configs) {
  unsigned int scheduling_prio{0};
  const char *kManifestProcessSchedulingPriority{"schedulingPriority"};
  if (startup_configs.HasMember(kManifestProcessSchedulingPriority)) {
    if (startup_configs[kManifestProcessSchedulingPriority].IsUint()) {
      scheduling_prio = startup_configs[kManifestProcessSchedulingPriority].GetUint();
    } else {
      log_.LogWarn() << __func__ << " " << app_manifest_path << " object " << kManifestProcessSchedulingPriority
                     << " is not a string.";
    }
  } else {
    log_.LogWarn() << __func__ << " " << app_manifest_path << " does not contain object "
                   << kManifestProcessSchedulingPriority;
  }
  return scheduling_prio;
}

std::vector<std::string> ProcessListBuilder::GetStartupOptions(const std::string &app_manifest_path,
                                                               rapidjson::Value &startup_configs) {
  std::vector<std::string> startup_options;
  const char *kManifestProcessOptions{"options"};
  if (startup_configs.HasMember(kManifestProcessOptions)) {
    if (startup_configs[kManifestProcessOptions].IsArray()) {
      for (const auto &option : startup_configs[kManifestProcessOptions].GetArray()) {
        startup_options.emplace_back(option.GetString());
      }
    } else {
      log_.LogWarn() << __func__ << " " << app_manifest_path << " object " << kManifestProcessOptions
                     << " is not an array.";
    }
  } else {
    log_.LogWarn() << __func__ << " " << app_manifest_path << " does not contain object " << kManifestProcessOptions;
  }
  return startup_options;
}

template <typename T>
void ProcessListBuilder::AppendFunctionGroup(const FunctionGroup &group, T &fg_states,
                                             std::unique_ptr<Process::FunctionGroupList> &fg_list) {
  // Create new function group list element

  fg_list->emplace_back(group.GetName(), group.GetActiveStateRef());

  // Allocate memory for function group states
  if (fg_states.Size() == 0) {
    log_.LogWarn() << __func__ << " Function Group: " << fg_list->back().name.c_str()
                   << ": No function group states are defined.";
    fg_list = nullptr;
  } else {
    fg_list->back().states.reserve(fg_states.Size());
  }
  // Iterate over all states of one function group
  for (const auto &fg_state : fg_states) {
    // Append function group state to list
    const std::string &fg_state_name = fg_state.GetString();
    const SafeString *state_ref = group.GetStateRef(StringView(fg_state_name));
    if (state_ref != nullptr) {
      fg_list->back().states.push_back(std::ref(*state_ref));
    }
  }
}

std::unique_ptr<Process::FunctionGroupList> ProcessListBuilder::GetFunctionGroups(const Executionmanager &em,
                                                                                  rapidjson::Value &startup_configs) {
  // Create function group list
  std::unique_ptr<Process::FunctionGroupList> fg_list = vac::language::make_unique<Process::FunctionGroupList>();

  const char *kManifestProcessesFunctionGroups{"functionGroups"};
  const char *kManifestProcessesMachineStates{"machineStates"};
  std::size_t fgs_size = 0;

  // Check if function groups are defined in manifest file
  if (startup_configs.HasMember(kManifestProcessesFunctionGroups)) {
    fgs_size = startup_configs[kManifestProcessesFunctionGroups].GetArray().Size();
  }

  if (startup_configs.HasMember(kManifestProcessesMachineStates)) {
    fgs_size++;
  }

  // Allocate memory for function groups
  if (fgs_size != 0) {
    fg_list->reserve(fgs_size);
  } else {
    log_.LogInfo() << __func__ << " No function group or machine state is defined.";
    fg_list = nullptr;
  }

  // Append machine states
  if (startup_configs.HasMember(kManifestProcessesMachineStates)) {
    // Append machine state, when existing
    auto machine_states = startup_configs[kManifestProcessesMachineStates].GetArray();
    // Append machine states as function group MachineStates
    const FunctionGroup *ref_group =
        em.GetFunctionGroupRef(StringView(kManifestFormatMachineState, (sizeof(kManifestFormatMachineState) - 1)));
    // Create new function group list element
    if (ref_group != nullptr) {
      AppendFunctionGroup(*ref_group, machine_states, fg_list);
    }
  }

  // Append function groups
  if (startup_configs.HasMember(kManifestProcessesFunctionGroups)) {
    auto fgs = startup_configs[kManifestProcessesFunctionGroups].GetArray();
    // Iterate over all function groups
    for (const auto &function_group : fgs) {
      // Append function group element
      const std::string &fg_name = function_group.GetObject()["name"].GetString();
      auto fg_states = function_group.GetObject()["states"].GetArray();
      const FunctionGroup *ref_group = em.GetFunctionGroupRef(StringView(fg_name));
      // Append function groups
      if (ref_group != nullptr) {
        AppendFunctionGroup(*ref_group, fg_states, fg_list);
      }
    }
  }

  return fg_list;
}

osabstraction::process::CPUCoreControl ProcessListBuilder::GetCpuCoreMask(StringView app_manifest_path,
                                                                          rapidjson::Value &startup_configs) {
  const char *kManifestCores{"cores"};
  const char *kManifestShallNotRunOn{"shallNotRunOn"};
  const char *kManifestShallRunOn{"shallRunOn"};
  osabstraction::process::CPUCoreControl cpu_core;

  // Check for cores
  if (startup_configs.HasMember(kManifestCores)) {
    if (startup_configs[kManifestCores].IsObject()) {
      // Get cores
      auto cores = startup_configs[kManifestCores].GetObject();

      if (cores.HasMember(kManifestShallRunOn)) {
        auto shall_run_on_array = cores[kManifestShallRunOn].GetArray();
        // Set CPU cores to mask
        for (const auto &core_number : shall_run_on_array) {
          if (core_number.IsInt()) {
            cpu_core.SetCore(static_cast<uint8_t>(core_number.GetInt()));

          } else {
            log_.LogWarn() << __func__ << " " << app_manifest_path.data() << " object " << kManifestCores
                           << " does not contain integer values for shallRunOn.";
          }
        }
      } else if (cores.HasMember(kManifestShallNotRunOn)) {
        auto shall_not_run_on_array = cores[kManifestShallNotRunOn].GetArray();
        // Set all the cores to the mask, so that processes are allowed to run on all the cores.
        cpu_core.SetAllCores();
        for (const auto &core_number : shall_not_run_on_array) {
          if (core_number.IsInt()) {
            // Remove the cores from the mask for which the process is not eligible to run on.
            cpu_core.ClearCore(static_cast<uint8_t>(core_number.GetInt()));
          } else {
            log_.LogWarn() << __func__ << " " << app_manifest_path.data() << " object " << kManifestCores
                           << " does not contain integer values for shallNotRunOn.";
          }
        }
      }
    } else {
      log_.LogWarn() << __func__ << " " << app_manifest_path.data() << " object " << kManifestCores
                     << " is not an object.";
    }
  } else {
    log_.LogWarn() << __func__ << " " << app_manifest_path.data() << " does not contain object " << kManifestCores;
  }

  return cpu_core;
}

void ProcessListBuilder::UpdateProcessList(const Executionmanager &em, ara::per::internal::json::JsonDocument &manifest,
                                           const std::string &app_manifest_path, const std::string &executable_name,
                                           const bool category_platform_level, const std::string &app_binary_path,
                                           ProcessList &application_list, const bool is_adaptive_application) {
  ResourceGroup resource_group;
  ProcessCreationError process_creation_state = ProcessCreationError::kSuccess;

  // iterate over all processes
  const char *kManifestProcess{"processes"};
  if (manifest[kManifestProcess].IsArray()) {
    for (auto &process_entry : manifest[kManifestProcess].GetArray()) {
      const char *kManifestProcessName{"name"};
      // Get Process Name from parsed structure
      Name process_name{GetProcessName(kManifestProcessName, process_entry)};
      if (process_name.empty()) {
        log_.LogWarn() << __func__ << " Application Manifest of " << app_binary_path
                       << " does not contain a process name"
                       << " Application will not be instantiated.";
        break;
      }
      // Get startup config object
      const char *kManifestProcessesStartupConfigs{"startupConfigs"};
      if (process_entry.HasMember(kManifestProcessesStartupConfigs)) {
        if (process_entry[kManifestProcessesStartupConfigs].IsArray()) {
          for (auto &startup_configs : process_entry[kManifestProcessesStartupConfigs].GetArray()) {
            // Read startup config data
            SchedulingPolicies scheduling_policy = GetSchedulingPolicy(app_manifest_path, startup_configs);

            // Get Scheduling priority
            unsigned int scheduling_prio = GetSchedulingPrio(app_manifest_path, startup_configs);

            // Read command line arguments
            std::vector<std::string> startup_options = GetStartupOptions(app_manifest_path, startup_configs);

            // Execution dependency
            Dependencies dependencies = GetExecutionDependency(app_manifest_path, startup_configs);

            // Function Groups
            std::unique_ptr<Process::FunctionGroupList> fg_list = GetFunctionGroups(em, startup_configs);

            // Cores
            osabstraction::process::CPUCoreControl cpu_core_control =
                GetCpuCoreMask(StringView{app_manifest_path}, startup_configs);

            // Create and append process to process list
            SafeString bin_path{app_binary_path.c_str()};
            application_list.emplace_back(vac::language::make_unique<Process>(std::move(process_name),       // NOFORMAT
                                                                              std::move(bin_path),           // NOFORMAT
                                                                              std::move(dependencies),       // NOFORMAT
                                                                              category_platform_level,       // NOFORMAT
                                                                              std::move(resource_group),     // NOFORMAT
                                                                              std::move(scheduling_policy),  // NOFORMAT
                                                                              scheduling_prio,               // NOFORMAT
                                                                              std::move(startup_options),    // NOFORMAT
                                                                              cpu_core_control,              // NOFORMAT
                                                                              is_adaptive_application,       // NOFORMAT
                                                                              std::move(fg_list)));          // NOFORMAT

            log_.LogDebug() << " Added valid application. Manifest data: executable_name: " << executable_name
                            << " process_name: " << process_name << " app_binary_path: " << app_binary_path;

            // clear all vectors for next application
            resource_group.clear();
          }

          if (process_creation_state != ProcessCreationError::kSuccess) {
            break;
          }
        } else {
          log_.LogWarn() << __func__ << " " << app_manifest_path << " object " << kManifestProcessName
                         << " is not an array.";
        }
      } else {
        log_.LogWarn() << __func__ << " " << app_manifest_path << " does not contain object "
                       << kManifestProcessesStartupConfigs;
      }
    }
  } else {
    log_.LogWarn() << __func__ << " Application Manifest of " << app_binary_path << " does not contain "
                   << kManifestProcess << " section."
                   << " Application will not be instantiated.";
  }
}

bool ProcessListBuilder::LoadManifest(const std::string &app_manifest_path, const Name &file_name,
                                      const std::string &path_to_application_manifest_schema,
                                      const struct dirent *application_entry, std::string &app_binary_path,
                                      ara::per::internal::json::JsonDocument *manifest) {
  bool success{false};

  // check if binary of app exists
  if (!CheckIfApplicationBinaryExists(app_binary_path, application_entry->d_name)) {
    log_.LogError() << " Directory does not contain a valid named binary: " << app_binary_path
                    << application_entry->d_name
                    << " The directory name must match the binary name. Skipping this application.";
  }
  // create binary path
  app_binary_path.append(application_entry->d_name);

  log_.LogDebug() << __func__ << " Reading manifest " << app_manifest_path << file_name;

  try {
    // Read the json file w/ validation
    *manifest = ara::per::internal::json::LoadFile(app_manifest_path + file_name, path_to_application_manifest_schema);
    success = true;
  } catch (const ara::per::internal::json::exception::FileNotFound &e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (const ara::per::internal::json::exception::ParserError &e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (const ara::per::internal::json::exception::ValidationError &e) {
    log_.LogError() << __func__ << " " << e.what();
  } catch (...) {
    log_.LogError() << __func__ << " Caught unknown exception.";
  }
  return success;
}

void ProcessListBuilder::CreateProcessList(const Executionmanager &em, const std::string &app_manifest_path,
                                           const std::string &path_to_application_manifest_schema,
                                           struct dirent *application_entry, std::string &app_binary_path,
                                           ProcessList &application_list) {
  struct dirent *manifest_directory_content;
  bool application_manifest_found = false;

  DIR *manifest_directory_handler = opendir(app_manifest_path.c_str());
  if (manifest_directory_handler) {
    // Traverse the path /opt/<app>/etc
    while (0 != (manifest_directory_content = readdir(manifest_directory_handler))) {
      // skip "." and ".."
      if (strcmp(manifest_directory_content->d_name, ".") != 0 &&
          strcmp(manifest_directory_content->d_name, "..") != 0) {
        std::string file_name(manifest_directory_content->d_name);
        if (file_name.rfind(manifest_name_) != std::string::npos) {
          // Load and parse manifest file
          ara::per::internal::json::JsonDocument manifest;
          if (!LoadManifest(app_manifest_path, file_name, path_to_application_manifest_schema, application_entry,
                            app_binary_path, &manifest)) {
            // LoadManifest failed
            log_.LogWarn() << __func__ << " Load of applicationmanifest: " << app_manifest_path
                           << " failed. Skipping this applicaiton!";
            break;
          }
          // read application name and application category from manifest
          // \trace SWS_EM_01017
          const bool category_platform_level{ReadCategory(manifest)};
          const std::string executable_name{ReadExecutableName(manifest)};
          const bool is_adaptive_application{ReadIsAdaptiveApplication(manifest)};

          // prepare suffix for following check
          char cstr_suffix[kPathSizeMax] = "/bin/";
          std::strncat(cstr_suffix, executable_name.c_str(), (kPathSizeMax - sizeof("/bin/")));
          StringView suffix = StringView(static_cast<const char *>(cstr_suffix));

          // check if the name of the executable is equal to the name specified in the manifest
          if ((app_binary_path.size() >= suffix.size()) &&
              (app_binary_path.compare((app_binary_path.size() - suffix.size()), suffix.size(), suffix.data()) != 0)) {
            log_.LogWarn() << __func__ << " Load of application with manifest: " << app_manifest_path
                           << " failed. No executable, with expected filename found. Please ensure the executable has "
                              "the same name as specified in the manifest. Skipping this applicaiton!";
            break;
          }
          // iterate over all processes
          UpdateProcessList(em, manifest, app_manifest_path, executable_name, category_platform_level, app_binary_path,
                            application_list, is_adaptive_application);

          // stop loop as we already found a valid application manifest
          application_manifest_found = true;
          break;
        }
      }
    }
    if (!application_manifest_found) {
      log_.LogWarn() << __func__ << " No application manifest (" << manifest_name_ << ") found in "
                     << app_manifest_path;
    }
  }
  closedir(manifest_directory_handler);
}

ProcessList ProcessListBuilder::Create(const Executionmanager &em, StringView app_path, StringView app_schema_path) {
  DIR *application_directory_handler;
  struct dirent *application_entry;
  ProcessList application_list;
  std::string path_to_applications{
      app_path.begin(),
      app_path.end()};  // Temporary solution, remove the std::string usage when ProcessListBuilder is reworked
  std::string path_to_application_manifest_schema{
      app_schema_path.begin(),
      app_schema_path.end()};  // Temporary solution, remove the std::string usage when ProcessListBuilder is reworked

  log_.LogDebug() << __func__ << " Searching for applications in: " << path_to_applications;

  application_directory_handler = opendir(path_to_applications.c_str());
  if (application_directory_handler) {
    while (0 != (application_entry = readdir(application_directory_handler))) {
      // skip "." and ".."
      if (strcmp(application_entry->d_name, ".") != 0 && strcmp(application_entry->d_name, "..") != 0) {
        // check /opt/<app>/etc directory for manifest files
        std::string single_app_path(path_to_applications);

        single_app_path.append("/");
        single_app_path.append(application_entry->d_name);

        log_.LogDebug() << __func__ << " Found possible application location at: " << single_app_path
                        << " Checking for " << manifest_name_ << ".json";

        // create the binary path (e.g. /opt/<app>/bin/)
        std::string app_binary_path = single_app_path;
        app_binary_path.append("/bin/");

        // create the manifest path (e.g. /opt/<app>/etc/)
        std::string app_manifest_path = single_app_path;
        app_manifest_path.append("/etc/");

        // Create list of all possible processes
        CreateProcessList(em, app_manifest_path, path_to_application_manifest_schema, application_entry,
                          app_binary_path, application_list);
      }
    }
    closedir(application_directory_handler);
    log_.LogDebug() << __func__ << " Finished creating application list.";
  } else {
    throw std::runtime_error("Reading application directory failed");
  }

  return std::move(application_list);
}

Name ProcessListBuilder::ReadExecutableName(const ara::per::internal::json::JsonDocument &manifest) {
  const std::string kManifestExecutableName{"name"};
  Name executable_name{""};
  if (manifest.HasMember(kManifestExecutableName.c_str())) {
    if (manifest[kManifestExecutableName.c_str()].IsString()) {
      executable_name = manifest[kManifestExecutableName.c_str()].GetString();
    } else {
      log_.LogError() << __func__ << " Manifest member \" " << kManifestExecutableName << "\"";
    }
  } else {
    log_.LogError() << __func__ << " Manifest has no member \" " << kManifestExecutableName << "\"";
  }
  return executable_name;
}

bool ProcessListBuilder::ReadCategory(const ara::per::internal::json::JsonDocument &manifest) {
  const std::string kManifestCategory{"category"};
  const std::string kPlatformLevel{"PLATFORM_LEVEL"};
  const std::string kApplicationLevel{"APPLICATION_LEVEL"};
  bool category_platform_level{false};
  if (manifest.HasMember(kManifestCategory.c_str())) {
    if (manifest[kManifestCategory.c_str()].IsString()) {
      std::string category{manifest[kManifestCategory.c_str()].GetString()};
      if (category.compare(kPlatformLevel.c_str()) == 0) {
        category_platform_level = true;
      } else if (category.compare(kApplicationLevel.c_str()) == 0) {
        category_platform_level = false;
      } else {
        log_.LogWarn() << __func__ << " Determination of application category failed!";
      }
    } else {
      log_.LogWarn() << __func__ << " Manifest member \"" << kManifestCategory << "\" is not a string";
    }
  } else {
    log_.LogWarn() << __func__ << " Manifest has no member \" " << kManifestCategory << "\"";
  }
  return category_platform_level;
}

bool ProcessListBuilder::ReadIsAdaptiveApplication(const ara::per::internal::json::JsonDocument &manifest) const {
  const char *kKey = "isAdaptiveApplication";
  bool is_adaptive_application{true};
  if (manifest.HasMember(kKey)) {
    if (manifest[kKey].IsInt()) {
      switch (manifest[kKey].GetInt()) {
        case 0:
          is_adaptive_application = false;
          break;
        default:
          is_adaptive_application = true;
      }

    } else {
      log_.LogWarn() << __func__ << " Member \"" << kKey << "\" is not an integer.";
    }
  } else {
    log_.LogWarn() << __func__ << " Manifest does not contain member \"" << kKey << "\"";
  }
  return is_adaptive_application;
}

ProcessState ProcessListBuilder::ConvertToProcessState(unsigned int execution_dependency_state) {
  ProcessState retval{ProcessState::kIdle};
  // Convert to the specified ApplicationStates
  switch (execution_dependency_state) {
    case 0:
      retval = ProcessState::kRunning;
      break;
    case 1:
      retval = ProcessState::kTerminated;
      break;
    default:
      log_.LogWarn() << __func__ << " Conversion of: " << execution_dependency_state << " to ProcessState failed.";
  }
  return retval;
}

}  // namespace internal
}  // namespace exec
}  // namespace ara
