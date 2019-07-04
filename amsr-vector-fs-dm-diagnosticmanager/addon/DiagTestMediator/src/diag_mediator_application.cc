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
/**        \file  diag_test_mediator.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "diag_mediator_application.h"

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <utility>

#include "ara/com/runtime.h"
#include "ara/log/logging.hpp"

namespace diag_test_mediator {

DiagMediatorApplication::DiagMediatorApplication()
    : exit_requested_(false),
      mediator_interface_impl_(ara::com::InstanceIdentifier("1"), did_op_factory_, rid_op_factory_) {}

DiagMediatorApplication::~DiagMediatorApplication() { JoinAllThreads(); }

void DiagMediatorApplication::SignalHandlerThread() {
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
        ara::log::LogInfo() << "DiagTestMediator::" << __func__ << ": SIGINT received";
        exit_requested_ = true;
        break;
      case SIGTERM:
        ara::log::LogInfo() << "DiagTestMediator::" << __func__ << ": SIGTERM received";
        exit_requested_ = true;
        break;
      default: /* ignore other signals*/
        break;
    }
  }

  ara::log::LogDebug() << "Runtime::" << __func__ << ": signal handler thread stopped";
}

void DiagMediatorApplication::Initialize(CommandLineArguments args) {
  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << " called";

  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);

  /* spawn a new signal handler thread*/
  threads_[0] = std::move(std::thread{&DiagMediatorApplication::SignalHandlerThread, this});

  /* Initialize ara::com runtime */
  ara::com::Runtime::getInstance().Initialize(args.path_to_aracom_application_file);

  /* Initialization of services */

  /* Offer ServiceInterfaces */
  mediator_interface_impl_.OfferService();
  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << " >> Mediator service offered.";
}

void DiagMediatorApplication::Run() {
  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << " called";
  /* store the current point of time in next_run */
  std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());

  /* define a time interval of 20 milliseconds */
  std::chrono::steady_clock::duration duration(std::chrono::milliseconds(20));
  while (!exit_requested_) {
    /* do some work here */
    /* calculate the next point of time to be rescheduled */
    next_run += duration;

    ara::com::Future<bool> result = mediator_interface_impl_.ProcessNextMethodCall();
    if (result.get()) {
      ara::log::LogInfo() << "DiagTestMediator::" << __func__ << ": Processing ara::com method call.";
    } else {
      /* Blocks the execution of the current thread until specified point of time (next_run) has been reached */
      std::this_thread::sleep_until(next_run);
    }
  }

  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << ": execution stopped (exit requested)";
}

void DiagMediatorApplication::Shutdown() {
  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << " called";

  /* Stop offering ServiceInterfaces */
  mediator_interface_impl_.StopOfferService();
  ara::log::LogDebug() << "DiagTestMediator::" << __func__ << " >> Mediator service now inactive.";

  /* Call Shutdown method of each services */

  /* do application clean-up in here */
  JoinAllThreads();
}

void DiagMediatorApplication::JoinAllThreads() {
  /* wait till other threads have joined if they are joinable */
  for (std::thread& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

}  // namespace diag_test_mediator
