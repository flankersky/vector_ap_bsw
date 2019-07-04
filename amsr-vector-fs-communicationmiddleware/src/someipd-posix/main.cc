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
 *        \brief  SOME/IP daemon for POSIX OS
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <getopt.h>
#include <unistd.h>
#include <atomic>
#include <csignal>
#include <thread>
#include "ara/log/logging.hpp"
#include "osabstraction/io/reactor.h"
#include "someipd-posix/application/application_manager.h"
#include "someipd-posix/configuration/json_configuration.h"
#include "someipd-posix/packet_router/packet_router.h"
#include "someipd-posix/service_discovery/service_discovery.h"
#include "vac/language/cpp14_backport.h"
#include "vac/memory/three_phase_allocator.h"
#include "vac/timer/timer_manager.h"

#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
#include <ara/exec/application_client.hpp>
using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT)

/**
 * \brief Container for command line arguments.
 */
struct CommandLineArguments {
  /**
   * \brief Path to configuration file.
   */
  std::string cfg_path_;
  /**
   * \brief Verbosity level.
   */
  std::size_t verbose_;
};

/**
 * \brief Prints the usage message.
 *
 * \param progname Program name.
 */
static void Usage(const char* progname) {
  ara::log::Logger& logger = ara::log::CreateLogger("Usage SOME/IP daemon", "");
  logger.LogInfo() << "usage: " << progname
                   << " [-h] [-v] -c <config file path>\n"
                      "-h                       Print this message and exit.\n"
                      "-v                       Verbosity level.\n"
                      "-c <config file path>    Specify the location of the configuration file.\n";
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
  ara::log::Logger& logger = ara::log::CreateLogger("Arguments parser SOME/IP daemon", "");
  args.cfg_path_ = "";
  args.verbose_ = 0;
  printf("ParseArguments into\n");
  while ((c = getopt(argc, argv, "hc:v")) != -1) {
    printf("while into\n");
    switch (c) {
      case 'h':
        printf("---h\n");
        Usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'c':
        printf("---c\n");
        args.cfg_path_ = optarg;
        printf("optarg = %s\n",optarg);
        break;
      case 'v':
        printf("---v\n");
        if (args.verbose_ < static_cast<std::size_t>(ara::log::LogLevel::kVerbose)) {
          args.verbose_++;
        }
        break;
      case '?':
        //printf("---?\n");
      default:
        Usage(argv[0]);
        printf("---default\n");
        exit(EXIT_FAILURE);
        break;
    }
  }
  if (args.cfg_path_ == "") {
    logger.LogError() << "No configuration file path provided\n";
    exit(EXIT_FAILURE);
  }

  return args;
}

/**
 * \brief Checks whether the configured UNIX domain sockets already exist or not and throw
 * an exception in case at least one of them does.
 *
 * \param config A configuration.
 */
static void CheckUnixDomainSockets(const someipd_posix::configuration::Configuration* config) {
  int ret;
  ret = access(config->GetControlSocketPath().c_str(), F_OK);
  if (ret == 0) {
    throw std::runtime_error("File '" + config->GetControlSocketPath() + "' already exists");
  }
  ret = access(config->GetRoutingSocketPath().c_str(), F_OK);
  if (ret == 0) {
    throw std::runtime_error("File '" + config->GetRoutingSocketPath() + "' already exists");
  }
}

/**
 * \brief Block this thread from receiving SIGTERM and SIGINT
 */
static void SetupSignalHandling() {
  sigset_t sigset;

  ::sigemptyset(&sigset);
  ::sigaddset(&sigset, SIGTERM);
  ::sigaddset(&sigset, SIGINT);

  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  // Deactivate SIGPIPE and use errno instead
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  int ret = sigaction(SIGPIPE, &sa, NULL);
  if (ret < 0) {
    throw std::runtime_error("Setting up SIGPIPE handler failed");
  }
}

/**
 * \brief Blocks until the signals SIGTERM or SIGINT are received.
 */
static void HandleSignals() {
  sigset_t sigset;
  int signal;
  bool done;

  ara::log::Logger& logger = ara::log::CreateLogger("Handle signals SOME/IP daemon", "");

  ::sigemptyset(&sigset);
  ::sigaddset(&sigset, SIGTERM);
  ::sigaddset(&sigset, SIGINT);

  done = false;

  while (!done) {
    sigwait(&sigset, &signal);
    logger.LogInfo() << "caught signal " << signal;
    switch (signal) {
      case SIGINT:
        done = true;
        break;
      case SIGTERM:
        done = true;
        break;
    }
  }
}

/**
 * \brief Main function of someipd-posix.
 *
 * Sets up signal handling and spawns the reactor thread.
 *
 * \param argc Argument Count
 * \param argv Argument Vector
 *
 * \return 0 in case the application terminated correctly, undefined otherwise.
 */
int main(int argc, char* argv[]) {
#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
  ApplicationClient app_client;
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT)
  printf("someip enter\n");
  CommandLineArguments args = ParseArguments(argc, argv);
  //ara::log::InitLogging("SomeIPd-Posix", "SOME/IP Communication Daemon for Posix",
                        //static_cast<ara::log::LogLevel>(args.verbose_), ara::log::LogMode::kConsole, "");
  //ara::log::Logger& logger = ara::log::CreateLogger("main", "");
  printf("someip-------- zhuang1\n");
  SetupSignalHandling();
  printf("someip--------- zhuang2\n");

  osabstraction::io::Reactor reactor;
  vac::timer::TimerManager timer_manager(&reactor);
  printf("someip---------- zhuang3\n");

  try {
    // Initialize configuration
     printf("someip---------- zhuang3.1\n");
    someipd_posix::configuration::JsonConfiguration config(args.cfg_path_);
     printf("someip---------- zhuang3.2\n");
    CheckUnixDomainSockets(&config);
    printf("someip---------- zhuang4\n");
    // Initialize packet router
    printf("someip---------- zhuang5\n");
    someipd_posix::packet_router::PacketRouter packet_router(&config);
    // Initialize service discovery
    printf("someip---------- zhuang6\n");
    someipd_posix::service_discovery::ServiceDiscovery service_discovery(&config, &reactor, &timer_manager,
                                                                         &packet_router);
    printf("someip---------- zhuang7\n");
    // Initialize application manager
    someipd_posix::application::ApplicationManager application_manager(config, &reactor, &packet_router,
                                                                       &service_discovery);
    printf("someip---------- zhuang8\n");
#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
    app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT)
    // Update allocation phase according to the application state
    printf("someip---------- zhuang9\n");
    vac::memory::AllocationPhaseManager::GetInstance().SetPhase(
        vac::memory::AllocationPhaseManager::AllocationPhase::steady);
    printf("someip---------- zhuang10\n");

    std::atomic<bool> reactor_done(false);
    std::thread reactor_thread = std::thread([&reactor, &timer_manager, &reactor_done]() {
      while (!reactor_done) {
        printf("someip------ zhuang11");
        const std::pair<bool, struct timeval> expiry = timer_manager.GetNextExpiry();
        if (expiry.first) {
          reactor.HandleEvents(&expiry.second);
        } else {
          reactor.HandleEvents(nullptr);
        }
        timer_manager.HandleTimerExpiry();
      }
    });

    HandleSignals();
#if defined(ENABLE_EXEC_MANAGER_SUPPORT)
    app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif  // defined(ENABLE_EXEC_MANAGER_SUPPORT)
    // Update allocation phase according to the application state
    vac::memory::AllocationPhaseManager::GetInstance().SetPhase(
        vac::memory::AllocationPhaseManager::AllocationPhase::deallocation);

    printf("Stopping the reactor thread\n");
    reactor_done = true;
    reactor.Unblock();
    reactor_thread.join();
  } catch (std::system_error& e) {
    //logger.LogError() << e.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
