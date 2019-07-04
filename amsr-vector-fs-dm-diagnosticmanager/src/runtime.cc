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
/**        \file  runtime.cc
 *        \brief  Main class representing the application.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <runtime.h>
#include <signal.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <utility>
#include <vector>

#include "ara/com/instance_identifier.h"
#include "ara/com/runtime.h"
#include "ara/log/logging.hpp"
#include "configuration/diagnostic_configuration.h"
#include "server/conversation/conversation.h"
#include "vac/memory/three_phase_allocator.h"

namespace amsr {
namespace diag {

/** alias for AllocationPhaseManager */
using vac::memory::AllocationPhaseManager;

Runtime::Runtime() : exit_requested_(false), timer_manager_(&reactor_) {}

void Runtime::SignalHandlerThread() {
  sigset_t signal_set;
  sigemptyset(&signal_set);          /* empty the set of signals */
  ::sigaddset(&signal_set, SIGTERM); /* add SIGTERM to set */
  ::sigaddset(&signal_set, SIGINT);  /* add SIGINT to set */

  ara::log::LogDebug() << "Runtime::" << __func__ << ": signal handler thread running";

  int signal = -1;
  // run loop until the signals SIGTERM or SIGINT are received.
  while (exit_requested_ == false) {
    sigwait(&signal_set, &signal);
    switch (signal) {
      case SIGINT:
        ara::log::LogInfo() << "Runtime::" << __func__ << ": SIGINT received";
        exit_requested_ = true;
        break;
      case SIGTERM:
        ara::log::LogInfo() << "Runtime::" << __func__ << ": SIGTERM received";
        exit_requested_ = true;
        break;
      default: /* ignore other signals*/
        break;
    }
  }

  reactor_.Unblock();

  ara::log::LogDebug() << "Runtime::" << __func__ << ": signal handler thread stopped";
}

void Runtime::Initialize(CommandLineArguments args) {
  ara::log::LogDebug() << "Initialize";
  AllocationPhaseManager::GetInstance().SetPhase(AllocationPhaseManager::AllocationPhase::allocation);

  /* Block all signals for this thread. Signal mask will be inherited by
   * subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);

  /* reserve space in std::vector */
  threads_.reserve(kNumOfThreads);
  /* spawn a new signal handler thread :
  Called during initialization in order to be able to catch the signal before calling Run method of other classes.*/
  threads_.emplace_back(&Runtime::SignalHandlerThread, this);

  /* Initialize instance for ara::com communication. */
  ara::com::Runtime::getInstance().Initialize(args.path_to_aracom_application_file);

  /* Load configuration */
  configuration::DiagnosticConfiguration diagnostic_configuration;
  diagnostic_configuration.LoadConfigurationFile(args.path_to_meta_configuration_file);

  /* Generic configuration */
  server::conversation::Conversation::SetLogMaxNumberPayloadBytes(
      diagnostic_configuration.generic_configuration_.log_output_configuration.max_payload_bytes);

  /* Setup components */
  // Transport protocol manager.
  uds_transport_protocol_mgr_impl_.Initialize(args.path_to_transport_protocol_configuration_file, &reactor_,
                                              &timer_manager_);

  // Diagnostic managers.
  diagnostic_server_list_.reserve(diagnostic_configuration.dext_configurations.size());
  for (std::vector<amsr::diag::configuration::DextConfiguration>::reference dext_config :
       diagnostic_configuration.dext_configurations) {
    diagnostic_server_list_.emplace_back(uds_transport_protocol_mgr_impl_);
    diagnostic_server_list_.back().Initialize(dext_config, timer_manager_);
  }
}

void Runtime::Run() {
  ara::log::LogDebug() << "Runtime::" << __func__ << ": starting UDS transport & diagnostic servers";
  AllocationPhaseManager::GetInstance().SetPhase(AllocationPhaseManager::AllocationPhase::steady);

  /* Run TransportProtocolManager */
  uds_transport_protocol_mgr_impl_.Run();
  /* Run every Diagnostic managers */
  for (vac::container::StaticList<amsr::diag::server::DiagnosticServer>::reference diagnosticServer :
       diagnostic_server_list_) {
    diagnosticServer.Run();
  }

  /* Listen to Reactor and Timer Manager and handle events */
  while (!exit_requested_) {
    const std::pair<bool, struct timeval> expiry = timer_manager_.GetNextExpiry();
    if (expiry.first) {
      reactor_.HandleEvents(&expiry.second);
    } else {
      reactor_.HandleEvents(nullptr);
    }
    timer_manager_.HandleTimerExpiry();
  }

  ara::log::LogDebug() << "Runtime::" << __func__ << ": execution stopped (exit requested)";
}

void Runtime::Shutdown() {
  ara::log::LogDebug() << "Runtime::" << __func__ << " called";
  AllocationPhaseManager::GetInstance().SetPhase(AllocationPhaseManager::AllocationPhase::deallocation);

  /* Shutdown every Diagnostic managers before transport protocol manager. */
  for (vac::container::StaticList<amsr::diag::server::DiagnosticServer>::reference diagnosticServer :
       diagnostic_server_list_) {
    diagnosticServer.Shutdown();
  }

  /* do application clean-up in here */
  uds_transport_protocol_mgr_impl_.Shutdown();
  reactor_.Unblock();

  /* wait till other threads are finished */
  for (std::vector<std::thread>::reference thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

}  // namespace diag
}  // namespace amsr
