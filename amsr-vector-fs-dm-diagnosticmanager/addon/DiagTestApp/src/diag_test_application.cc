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
/**        \file  diag_test_application.cc
 *        \brief  Main class representing the application.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "diag_test_application.h"

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <vector>

#include "ara/com/runtime.h"
#include "ara/log/logging.hpp"

namespace diag_test_application {

DiagTestApplication::DiagTestApplication()
    : exit_requested_(false),
      generic_uds_service_(ara::com::InstanceIdentifier("1")),
      data_element_4711_(ara::com::InstanceIdentifier("1")),
      data_element_F190_(ara::com::InstanceIdentifier("1")),
      routine_control_3009_(ara::com::InstanceIdentifier("1")) {}

void DiagTestApplication::SignalHandlerThread() {
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
        ara::log::LogInfo() << "DiagTestApplication::" << __func__ << ": SIGINT received";
        exit_requested_ = true;
        break;
      case SIGTERM:
        ara::log::LogInfo() << "DiagTestApplication::" << __func__ << ": SIGTERM received";
        exit_requested_ = true;
        break;
      default: /* ignore other signals*/
        break;
    }
  }

  ara::log::LogDebug() << "Runtime::" << __func__ << ": signal handler thread stopped";
}

void DiagTestApplication::Initialize(CommandLineArguments args) {
  ara::log::LogDebug() << "DiagTestApplication::" << __func__ << " called";

  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);

  /* reserve space in std::vector */
  threads_.reserve(kNumOfThreads);
  /* spawn a new signal handler thread*/
  threads_.emplace_back(&DiagTestApplication::SignalHandlerThread, this);

  /* Initialize ara::com runtime */
  ara::com::Runtime::getInstance().Initialize(args.path_to_aracom_application_file);

  /* Initialization of services */
  routine_control_3009_.Initialize();

  /* Offer ServiceInterfaces */
  generic_uds_service_.OfferService();
  data_element_4711_.OfferService();
  data_element_F190_.OfferService();
  routine_control_3009_.OfferService();
}

void DiagTestApplication::Run() {
  ara::log::LogDebug() << "DiagTestApplication::" << __func__ << " called";
  /* store the current point of time in next_run */
  std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());

  /* define a time interval of 200 milliseconds */
  std::chrono::steady_clock::duration duration(std::chrono::milliseconds(200));
  while (!exit_requested_) {
    /* do some work here */
    /* calculate the next point of time to be rescheduled */
    next_run += duration;
    /* Blocks the execution of the current thread until specified point of time (next_run) has been reached */
    std::this_thread::sleep_until(next_run);
  }

  ara::log::LogDebug() << "DiagTestApplication::" << __func__ << ": execution stopped (exit requested)";
}

void DiagTestApplication::Shutdown() {
  ara::log::LogDebug() << "DiagTestApplication::" << __func__ << " called";

  /* Stop offering ServiceInterfaces */
  generic_uds_service_.StopOfferService();
  data_element_4711_.StopOfferService();
  data_element_F190_.StopOfferService();
  routine_control_3009_.StopOfferService();

  /* Call Shutdown method of each services */
  routine_control_3009_.Shutdown();

  /* do application clean-up in here */
  /* wait till other threads have joined */
  for (std::thread& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

}  // namespace diag_test_application
