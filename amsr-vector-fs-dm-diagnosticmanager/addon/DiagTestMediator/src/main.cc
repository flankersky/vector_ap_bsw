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
/**        \file  amsr-vector-fs-dm-diagnosticmanager/addon/DiagTestMediator/src/main.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <csignal>

#include "ara/log/logging.hpp"
#include "arguments_parser.h"
#include "diag_mediator_application.h"

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
#include <ara/exec/application_client.hpp>
using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;
#endif
#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR)
#include <exec/application_state_client.hpp>
using ApplicationClient = ara::exec::applicationstate::ApplicationStateClient;
using ApplicationState = ara::exec::applicationstate::ApplicationState;
#endif

/**
 * \brief Entry Point of the process.
 */
int main(int argc, char* argv[]) {
  /* Parse arguments */
  diag_test_mediator::CommandLineArguments args = diag_test_mediator::ArgumentsParser::ParseArguments(argc, argv);

  /* Initialize Logger */
  ara::log::InitLogging("DiagTestMediator", "DiagnosticManager Mediator Process.", args.verbosity_level,
                        ara::log::LogMode::kConsole, "log");
  /* Create the application object */
  diag_test_mediator::DiagMediatorApplication mediator;

/* Execute the application lifecycle */
#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
  // Instantiate the ApplicationClient
  ApplicationClient app_client;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)

  try {
    /* Execute the application lifecycle */
    mediator.Initialize(args);

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
    // Report application state to execution management
    app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)

    mediator.Run();
  } catch (const std::system_error& sys_exception) {
    ara::log::LogError() << "main: system_error exception occurred (check if someipd is running): "
                         << sys_exception.what();
  } catch (const std::exception& ex) {
    ara::log::LogError() << "main: exception occurred: " << ex.what();
  }

#if defined(ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR) || defined(ENABLE_EXEC_MANAGER_SUPPORT_VECTOR)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif

  mediator.Shutdown();
  /* Terminate process */
  return 0;
}
