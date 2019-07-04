/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  amsr-vector-fs-dm-diagnosticmanager/src/main.cc
 *        \brief  TODO
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <runtime.h>

#include <osabstraction/commandlineparser/commandlineparser.h>
#include <thread>
#include "ara/log/logging.hpp"
#include "arguments_parser.h"

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
#include <ara/exec/application_client.hpp>
using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR)
#include <exec/application_state_client.hpp>
using ApplicationClient = ara::exec::applicationstate::ApplicationStateClient;
using ApplicationState = ara::exec::applicationstate::ApplicationState;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR)

/**
 * \brief Entry Point of the process.
 *
 * \param argc Command line argument count.
 * \param argv Array of pointers to command line arguments.
 * \return int termination value.
 */
int main(int argc, char* argv[]) {
  /* Parse the arguments */
  amsr::diag::CommandLineArguments args = amsr::diag::ArgumentsParser::ParseArguments(argc, argv);

  /* Initialize Logger */
  ara::log::InitLogging("DiagnosticManager", "AdaptivePlatformComponent for handling Diagnostics.",
                        args.verbosity_level, ara::log::LogMode::kConsole, "log");

  /* Create the runtime object */
  amsr::diag::Runtime application;
/* Execute the application lifecycle */

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
  // Instantiate the ApplicationClient
  ApplicationClient app_client;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)

  try {
    application.Initialize(args);

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
    // Report application state to execution management
    app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)

    application.Run();
  } catch (const std::system_error& sys_exception) {
    ara::log::LogError() << "main: system_error exception occurred (check if someipd is running): "
                         << sys_exception.what();
  } catch (const std::exception& ex) {
    ara::log::LogError() << "main: exception occurred: " << ex.what();
  }

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)

  application.Shutdown();
  /* Terminate process */
  return 0;
}
