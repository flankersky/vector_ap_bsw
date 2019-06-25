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
/**        \file  application_demo.cc
 *        \brief  A demo adaptive application to the execution management.
 *
 *      \details  Handles signals, reports its application states.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "application_demo.h"

#include <signal.h>
#include <unistd.h>

#include <ara/exec/application_client.hpp>
#include <ara/exec/state_client.hpp>
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace application_demo {

/**
 * \brief Timeout for machine state client communication to execution manager.
 */
constexpr std::chrono::milliseconds kMscTimeout(5000);

AdaptiveApplication::AdaptiveApplication()
    : application_client_(), state_client_(), exit_requested_(false), threads_(), is_state_client_(false) {
  ara::log::InitLogging("DEMO-APP", "Logging context of the demo application", ara::log::LogLevel::kVerbose,
                        ara::log::LogMode::kConsole, "");
}

AdaptiveApplication::AdaptiveApplication(bool is_state_client)
    : application_client_(), state_client_(), exit_requested_(false), threads_(), is_state_client_(is_state_client) {
  if (is_state_client) {
    ara::log::InitLogging("MSC-DEMO-APP", "Logging context of the machine_state_client application",
                          ara::log::LogLevel::kVerbose, ara::log::LogMode::kConsole, "");
  } else {
    ara::log::InitLogging("DEMO-APP", "Logging context of the demo application", ara::log::LogLevel::kVerbose,
                          ara::log::LogMode::kConsole, "");
  }
}

void AdaptiveApplication::SignalHandlerThread() {
  sigset_t signal_set;
  int sig = -1;
  sigemptyset(&signal_set);        /* empty the set of signals */
  sigaddset(&signal_set, SIGTERM); /* Add only SIGTERM to set */
  while (sig != SIGTERM) {
    sigwait(&signal_set, &sig);
    ara::log::LogDebug() << __func__ << " Caught Signal: " << sig;
  }
  exit_requested_ = true;
  ara::log::LogDebug() << __func__ << "application with pid " << getpid() << " received signal SIGTERM";
}

void AdaptiveApplication::PeriodicWork(PeriodicWorkStateType& state) {
  /* Do the actual work. */
  state.my_data = 0;
}

void AdaptiveApplication::PeriodicThread() {
  /* store the current point of time in next_run */
  std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());
  /* define a time interval of 100 milliseconds */
  std::chrono::steady_clock::duration duration(std::chrono::milliseconds(100));
  /* define a state variable to store information across runs of PeriodicWork() */
  PeriodicWorkStateType cross_iteration_state;

  while (!exit_requested_) {
    /* do some work here */
    PeriodicWork(cross_iteration_state);
    /* calculate the next point of time to be rescheduled */
    next_run += duration;
    /* Blocks the execution of the current thread until specified point of time (next_run) has been reached */
    std::this_thread::sleep_until(next_run);
  }
}

void AdaptiveApplication::Start() { Initialize(); }

void AdaptiveApplication::Initialize() {
  ara::log::LogDebug() << __func__ << " ADDON APP: Initialize";
  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);
  /* reserve space in std::vector */
  threads_.reserve(kNumOfThreads);
  /* spawn a new signal handler thread*/
  threads_.emplace_back(&AdaptiveApplication::SignalHandlerThread, this);
  /* May spawn some more threads here */
  threads_.emplace_back(&AdaptiveApplication::PeriodicThread, this);

  Run();
}

void AdaptiveApplication::Run() {
  ara::exec::ApplicationReturnType ret =
      application_client_.ReportApplicationState(ara::exec::ApplicationState::kRunning);
  if (ret == ara::exec::ApplicationReturnType::kGeneralError) {
    ara::log::LogWarn() << __func__ << " ADDON APP: Error reporting application state";
  } else {
    ara::log::LogDebug() << __func__ << " ADDON APP: Run";
  }
  /* store the current point of time in next_run */
  std::chrono::steady_clock::time_point next_run(std::chrono::steady_clock::now());
  /* define a time interval of 200 milliseconds */
  std::chrono::steady_clock::duration duration(std::chrono::milliseconds(200));

  std::string function_group = "MachineState";
  std::string machine_state;

  // GetMachineState
  if (is_state_client_) {
    ara::exec::StateReturnType returned_state_error = state_client_.GetState(function_group, machine_state);
    if (returned_state_error == ara::exec::StateReturnType::kSuccess) {
      ara::log::LogDebug() << __func__ << " ADDON APP: GetMachineState success. Machine State is " << machine_state;
    } else if (returned_state_error == ara::exec::StateReturnType::kBusy) {
      ara::log::LogDebug() << __func__ << "ADDON APP: There is currently a machine mode change ongoing";
    } else {
      ara::log::LogDebug() << __func__ << " ADDON APP: Unkown Error while GetMachineState";
    }
  }

  while (!exit_requested_) {
    // SetMachineState
    // \trace SWS_EM_01057
    if (is_state_client_) {
      ara::exec::StateReturnType returned_state_error = state_client_.SetState(function_group, "Running");
      if (returned_state_error == ara::exec::StateReturnType::kSuccess) {
        ara::log::LogDebug() << __func__ << " ADDON APP: SetMachineState success.";
      } else if (returned_state_error == ara::exec::StateReturnType::kBusy) {
        ara::log::LogDebug() << __func__ << "ADDON APP: There is currently an machine mode change ongoing";
      } else {
        ara::log::LogDebug() << __func__ << " ADDON APP: Unkown Error while SetMachineState";
      }
    }
    /* do some work here */
    /* calculate the next point of time to be rescheduled */
    next_run += duration;
    /* Blocks the execution of the current thread until specified point of time (next_run) has been reached */
    std::this_thread::sleep_until(next_run);

    // GetMachineState
    std::string new_machine_state;
    if (is_state_client_) {
      ara::exec::StateReturnType returned_state_error = state_client_.GetState(function_group, new_machine_state);
      if (returned_state_error == ara::exec::StateReturnType::kSuccess) {
        ara::log::LogDebug() << __func__ << " ADDON APP: GetMachineState success. Machine State is "
                             << new_machine_state;
      } else if (returned_state_error == ara::exec::StateReturnType::kBusy) {
        ara::log::LogDebug() << __func__ << "ADDON APP: There is currently an machine mode change ongoing";
      } else {
        ara::log::LogDebug() << __func__ << " ADDON APP: Unkown Error while GetMachineState";
      }
    } else {
      sleep(1);
    }
  }

  Shutdown();
}

void AdaptiveApplication::Shutdown() {
  ara::exec::ApplicationReturnType ret =
      application_client_.ReportApplicationState(ara::exec::ApplicationState::kTerminating);
  if (ret == ara::exec::ApplicationReturnType::kGeneralError) {
    ara::log::LogWarn() << __func__ << " ADDON APP: Error reporting application state";
  } else {
    ara::log::LogDebug() << __func__ << " ADDON APP: Shutdown reported";
  }

  /* do application clean-up in here */
  /* wait till other threads have joined */
  for (std::vector<std::thread>::iterator it = threads_.begin(); it != threads_.end(); ++it) {
    it->join();
  }
}

}  // namespace application_demo
