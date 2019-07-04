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
/**        \file  main.cc
 *        \brief  main function of executionmanager
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/exec/internal/process_list_builder.h>
#include <osabstraction/commandlineparser/commandlineparser.h>
#include <vac/memory/three_phase_allocator.h>
#include <ara/log/logging.hpp>

#include <string>

#include "ara/exec/internal/executionmanager.h"
#include "ara/exec/internal/types.h"

/**
 * \brief The default path where the machine manifest is located.
 */
const ara::exec::internal::SafeString kPathToMachineManifest("/etc/machine_manifest.json");  // NOLINT

/**
 * \brief The default path where the application manifest schema is located.
 */
const ara::exec::internal::SafeString kPathToApplicationManifestSchema(
    "/etc/application_manifest_schema.json");  // NOLINT

/**
 * \brief The default path where the application manifest schema is located.
 */
const ara::exec::internal::SafeString kPathToMachineManifestSchema("/etc/machine_manifest_schema.json");  // NOLINT

/**
 * \brief The default path where the applications are located.
 */
const ara::exec::internal::SafeString kPathToApplications("/opt");  // NOLINT

/**
 * \brief Container for command line arguments.
 */
struct CommandLineArguments {
  /**
   * \brief The path where the machine manifest is located.
   */
  ara::exec::internal::SafeString path_to_machine_manifest;
  /**
   * \brief The path where the application manifest schema is located.
   */
  ara::exec::internal::SafeString path_to_application_manifest_schema;
  /**
   * \brief The path where the applications are located.
   */
  ara::exec::internal::SafeString path_to_applications;
  /**
   * \brief The path where the machine manifest schema is located.
   */
  ara::exec::internal::SafeString path_to_machine_manifest_schema;
};

/**
 * \brief Prints the usage message.
 *
 * \param progname Program name.
 */
static void Usage(const char* progname) {
  ara::log::Logger& logger = ara::log::CreateLogger("Usage", "");
  logger.LogInfo() << "usage: " << progname <<
      R"( [-h] [-a <application base path>] [-m <machine manifest path>] [-s <application manifest schema path>]
         -h                                     Print this message and exit.
         -a <application base path>             Specify the base location of the applications.
         -m <machine manifest path>             Specify the location of the machine manifest file.
         -s <application manifest schema path>  Specify the location of the application manifest schema file.
         -t <machine manifest schema path>      Specify the location of the machine manifest schema file.
)";
}

/**
 * \brief Parses command line arguments.
 *
 * \param argc Command line argument count.
 * \param argv Array of pointers to command line arguments.
 * \return Parsed arguments.
 */
static CommandLineArguments ParseArguments(int argc, char* argv[]) {
  CommandLineArguments args;
  int c;
  ara::log::Logger& logger = ara::log::CreateLogger("Arguments parser", "");

  while ((c = osabstraction::commandlineparser::CommandLineParser(argc, argv, "ha:m:s:t:")) != -1) {
    switch (c) {
      case 'h':
        Usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'a':
        args.path_to_applications = optarg;
        break;
      case 'm':
        args.path_to_machine_manifest = optarg;
        break;
      case 's':
        args.path_to_application_manifest_schema = optarg;
        break;
      case 't':
        args.path_to_machine_manifest_schema = optarg;
        break;
      case '?':
      default:
        Usage(argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }

  if (args.path_to_application_manifest_schema == "") {
    logger.LogWarn() << "No path_to_application_manifest_schema provided. Using default: "
                     << kPathToApplicationManifestSchema.c_str();
    args.path_to_application_manifest_schema = kPathToApplicationManifestSchema;
  }
  if (args.path_to_machine_manifest_schema == "") {
    logger.LogWarn() << "No path_to_machine_manifest_schema provided. Using default: " << kPathToMachineManifestSchema.c_str();
    args.path_to_machine_manifest_schema = kPathToMachineManifestSchema;
  }
  if (args.path_to_applications == "") {
    logger.LogWarn() << "No path_to_applications provided. Using default: " << kPathToApplications.c_str();
    args.path_to_applications = kPathToApplications;
  }
  if (args.path_to_machine_manifest == "") {
    logger.LogWarn() << "No path_to_application_manifest_schema provided. Using default: " << kPathToMachineManifest.c_str();
    args.path_to_machine_manifest = kPathToMachineManifest;
  }

  return args;
}

/**
 * \brief Entry Point of the process.
 */
int main(int argc, char* argv[]) {
  ara::log::InitLogging("amsr-vector-fs-em-executionmanager", "The Adaptive AUTOSAR ExecutionManager",
                        ara::log::LogLevel::kVerbose, ara::log::LogMode::kConsole, "");

  const CommandLineArguments args(ParseArguments(argc, argv));

  ara::exec::internal::Executionmanager em{ara::exec::internal::StringView{args.path_to_machine_manifest_schema},
                                           ara::exec::internal::StringView{args.path_to_machine_manifest},
                                           ara::exec::internal::StringView{args.path_to_application_manifest_schema},
                                           ara::exec::internal::StringView{args.path_to_applications}};
  vac::memory::AllocationPhaseManager::GetInstance().SetPhase(
      vac::memory::AllocationPhaseManager::AllocationPhase::steady);
  em.Run();
  /* Terminate process */
  vac::memory::AllocationPhaseManager::GetInstance().SetPhase(
      vac::memory::AllocationPhaseManager::AllocationPhase::deallocation);
  return 0;
}
