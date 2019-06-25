/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**       \file  calculator_main.cc
 *        \brief  Main entry point of the calculator server example application
 *
 *        \details  -
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/com/runtime.h>
#include <ara/com/types.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <ara/exec/application_client.hpp>
#include <ara/log/logging.hpp>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "calculator_server.h"

using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;

using namespace ara::log;  // NOLINT 'using' for ara::log should be OK.

/* Forward declarations */
void Initialize(void);
void SignalHandler(void);

/**
 * \brief Flag to identify whether the application was requested to terminate, i.e., has received a SIGTERM
 */
std::atomic_bool exit_requested_(false);

/**
 * \brief Vector to store all threads spawned by this application.
 */
std::vector<std::thread> threads_;

/**
 * \brief Prints the usage message.
 *
 * \param argc argument count
 * \param argv command line arguments
 */
static void Usage(const int argc, char* argv[]) {
  if (argc > 0) {
    std::cerr << "usage: " << argv[0] <<
        R"([-h] -c <config file path>
           -h                            Print this message and exit.
           -c <config file path>         Specify the location of the configuration file.
        )";
  } else {
    std::cerr << "usage: calculatorClient "
              <<
        R"([-h] -c <config file path>
           -h                            Print this message and exit.
           -c <config file path>         Specify the location of the configuration file.
        )";
  }
}

/**
 * \brief Parses command line arguments.
 *
 * \param argc Command line argument count.
 * \param argv Array of pointers to command line arguments.
 * \return Parsed arguments.
 */
static std::string ParseArguments(int argc, char* argv[]) {
  int c;
  while ((c = getopt(argc, argv, "hec:")) != -1) {
    switch (c) {
      case 'h':
        Usage(argc, argv);
        exit(EXIT_SUCCESS);
        break;
      case 'c':
        return std::string(optarg);
      default:
        Usage(argc, argv);
        exit(EXIT_FAILURE);
        break;
    }
  }
  Usage(argc, argv);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  InitLogging("CSRV", "Adaptive Calculator Server", LogLevel::kVerbose, LogMode::kConsole, "");
  LogInfo() << "Starting calculatorServer";

  // Instantiate the ApplicationClient
  ApplicationClient app_client;

  LogInfo() << "Initializing...";
  Initialize();
  // Loading config file and initialize runtime
  LogInfo() << "Initializing runtime...";
  ara::com::Runtime::getInstance().Initialize(ParseArguments(argc, argv));
  LogInfo() << "Instanciating...";
  // Create service with corresponding instance id
  vector::calculator::calculatorServer myService(ara::com::InstanceIdentifier{"0x86c7"});
  LogInfo() << "Reporting application state...";

  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);

  LogInfo() << "Finished initialization sequence.";
  // Loop until the Execution Manager requests the application to exit
  while (!exit_requested_) {
    std::this_thread::sleep_for(std::chrono::seconds(100));
  }

  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
  return 0;
}

/**
  * \brief  Function to initialize the calculator server
  */
void Initialize(void) {
  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);
  /* spawn a new signal handler thread*/
  threads_.emplace_back(SignalHandler);
}

/**
  * \brief Signal handler function for SIGTERM
  */
void SignalHandler(void) {
  sigset_t signal_set;
  int sig = -1;
  sigemptyset(&signal_set);        /* Empty the set of signals */
  sigaddset(&signal_set, SIGTERM); /* Add only SIGTERM to set */
  LogInfo() << "SignalHandler started";
  while (sig != SIGTERM) {
    sigwait(&signal_set, &sig);
    LogInfo() << "Received signal number:" << sig;
  }
  LogInfo() << "Received signal SIGTERM";
  exit_requested_ = true;
}
