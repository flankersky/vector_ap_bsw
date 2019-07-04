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
/**        \file  runtime.h
 *        \brief  Main class representing the application.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef SRC_RUNTIME_H_
#define SRC_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <signal.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <string>
#include <thread>
#include <vector>

#include "arguments_parser.h"
#include "osabstraction/io/reactor.h"
#include "server/diagnostic_server.h"
#include "udstransport/uds_transport_protocol_manager_impl.h"
#include "vac/container/static_list.h"
#include "vac/timer/timer_manager.h"

namespace amsr {
namespace diag {

constexpr uint_least8_t kNumOfThreads = 1; /**< Number of Threads used in Runtime. */

/**
 * \brief Main class representing the application
 */
class Runtime {
 public:
  Runtime();
  virtual ~Runtime() = default;
  /**
   * \brief Lifecycle Method for Initialization.
   * \param args command line arguments.
   */
  void Initialize(CommandLineArguments args);

  /**
   * \brief Lifecycle Methods for Run Mode.
   */
  void Run();

  /**
   * \brief Lifecycle Methods for Shutdown.
   */
  void Shutdown();

 private:
  /**
   * \brief Entry point of the thread receiving signals from the execution
   * manager
   */
  void SignalHandlerThread();

  /**
   * \brief Flag to identify whether the application was requested to terminate,
   * i.e., has received a SIGTERM
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief Vector to store all threads spawned by this application.
   */
  std::vector<std::thread> threads_;

  /**
   * \brief Transport protocol Manager
   */
  amsr::diag::udstransport::UdsTransportProtocolMgrImpl uds_transport_protocol_mgr_impl_;

  /**
   * \brief List of diagnostic Managers
   */
  vac::container::StaticList<server::DiagnosticServer> diagnostic_server_list_;

  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::Reactor reactor_;

  /**
   * \brief A timer manager for managing timers used by the DoIP.
   */
  vac::timer::TimerManager timer_manager_;

  FRIEND_TEST(RuntimeTests, CheckInitializationAndDestruction);
  FRIEND_TEST(RuntimeTests, CheckInitialize);
};

}  // namespace diag
}  // namespace amsr

#endif  // SRC_RUNTIME_H_
