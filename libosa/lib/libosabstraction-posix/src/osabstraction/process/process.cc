/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  Implementation of POSIX process.
 *
 *      \details  This file implements the process abstraction using POSIX API.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/process.h"

#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>  // getpid()

#include <sched.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace osabstraction {
namespace process {

/**
 * \brief Invalid process ID.
 */
constexpr const int kInvalidProcessId = -1;

ProcessId GetProcessId() { return getpid(); }

void IgnoreChildTerminationSignal() { signal(SIGCHLD, SIG_IGN); }

/**
 * \brief Maximum path length which is accepted for program images.
 * According to POSIX IEEE Std 1003.1 2016 Edition, PATH_MAX includes the terminating null character.
 */
constexpr unsigned int kMaxPathLength = PATH_MAX;

Process::Process(const ProcessSettings& settings,
                 const std::function<void(osabstraction::process::ProcessId)>& process_created_callout)
    : pid_(kInvalidProcessId), is_running_(false) {
  /* #00 Check that program image is not nullptr. */
  if (settings.image == nullptr) {
    throw std::runtime_error("No program image given.");
  }
  /* #00 If program image path to long, throw exception. */
  if ((std::strlen(settings.image) + 1) > kMaxPathLength) {
    throw std::range_error("Given program image path is too long.");
  }

  /* #00 Check that given program image is a regular file. */
  struct stat file_status;
  if (stat(settings.image, &file_status) != 0) {
    throw std::runtime_error(std::string("File status could not be retrieved using stat(). File path is: ") +
                             settings.image);
  }
  if (!S_ISREG(file_status.st_mode)) {
    throw std::runtime_error(std::string("Given program image is not a regular file. File path is: ") + settings.image);
  }
  /* #00 Check that given program image is executable. */
  if (!(file_status.st_mode & S_IXUSR)) {
    throw std::runtime_error(std::string("Given program image is not executable. File path is: ") + settings.image);
  }

  sigset_t signal_set;
  sigfillset(&signal_set);                    // fill the set with all signals
  sigprocmask(SIG_BLOCK, &signal_set, NULL);  // Block all all signals

  /* #00 Create new child process using fork(). */
  pid_ = fork();
  /* #00 If creating child process failed, throw exception. */
  if (pid_ == kInvalidProcessId) {
    throw std::runtime_error("Creating child process using fork() failed.");
    /* #00 If in child process: */
  } else if (pid_ == 0) {
    const char* end_of_dir = std::strrchr(settings.image, '/');

    /* #00 If program image contains a path: */
    if (end_of_dir != 0) {
      /* #00 Change current directory to extracted directory. */
      char working_directory[kMaxPathLength];
      std::memcpy(working_directory, settings.image, end_of_dir - settings.image);
      working_directory[end_of_dir - settings.image] = 0;

      /* #00 If changing directory failed, throw exception. */
      if (-1 == chdir(working_directory)) {
        throw std::runtime_error(std::string("Changing working directory using chdir() failed. Directory: ") +
                                 working_directory);
      }
    }

    // Set core affinity
    if (settings.cpu_core_control != nullptr) {
      osabstraction::process::CoreAffinitySettingError retval = settings.cpu_core_control->SetAffinity(getpid());

      if (retval != osabstraction::process::CoreAffinitySettingError::kSuccess) {
        throw std::runtime_error("Setting core affinity failed.");
      }
    }

    /* #00 Prepare argument list for execv(). */
    std::vector<const char*> argv;

    /* #00 Extract program name from program image path. */
    const char* program_name = settings.image + (end_of_dir + 1 - settings.image);

    /* #00 Add the program name as the first argument. */
    argv.push_back(program_name);

    /* #00 Add regular command line arguments. */
    for (auto iter = settings.args.begin(); iter != settings.args.end(); ++iter) {
      argv.push_back(*iter);
    }

    /* #00 Add terminating null pointer. */
    argv.push_back(0);

    if (process_created_callout != nullptr) {
      int sig = -1;
      sigemptyset(&signal_set);  // empty the set of signals
      sigaddset(&signal_set, SIGUSR1);
      sigwait(&signal_set, &sig);  // wait for reception of the signal
    } else {
      // if there is no process_created_callout function set just ignore the USR1 signal
      signal(SIGUSR1, SIG_IGN);
    }
    /* #00 Load given program image using execv(). */
    /* #00 If loading failed: */
    if (-1 == execv(program_name, const_cast<char* const*>(&argv[0]))) {
      /* #00 Throw exception. */
      throw std::runtime_error(std::string("Loading program image using execv() failed. Program image: ") +
                               program_name);
    }
  } else {
    if (process_created_callout != nullptr) {
      process_created_callout(pid_);  // execute the process_created_callout-function
      kill(pid_, SIGUSR1);            // send SIGUSR1 signal to the child process to trigger execv
    }
  }

  // Set scheduling policy and priority.
  struct sched_param param;
  param.sched_priority = settings.scheduling_priority;
  int posix_scheduling_policy = static_cast<int>(settings.scheduling_policy);

  if (sched_setscheduler(pid_, posix_scheduling_policy, &param) == -1) {
    throw std::runtime_error("Setting scheduling policy failed.");
  }

  /* #00 Set process state to running. */
  is_running_ = true;
}

Process::Process(Process&& other) : pid_(std::move(other.pid_)), is_running_(std::move(other.is_running_)) {
  other.pid_ = kInvalidProcessId;
}

Process& Process::operator=(Process&& other) {
  std::swap(other, *this);
  return *this;
}

Process::~Process() {
  /* #00 If valid process assigned: */
  if (pid_ != kInvalidProcessId) {
    /* #00 Send SIGKILL to process. */
    (void)kill(pid_, SIGKILL);
  }
}

bool Process::IsRunning() {
  pid_t waitpid_result;

  /* #00 If process is assumed to be running: */
  if (is_running_) {
    /* #00 Check whether process is still running using waitpid() without blocking. */
    waitpid_result = waitpid(pid_, 0, WNOHANG);

    /* #00 If something went wrong: */
    if (waitpid_result == -1) {
      /* #00 If error is ECHILD (which means child process is already completely terminated) process is terminated. */
      if (errno == ECHILD) {
        is_running_ = false;
      } else {
        /* #00 Throw exception. */
        throw std::system_error(errno, std::generic_category());
      }
      /* #00 If state has changed: */
    } else if (waitpid_result > 0) {
      /* #00 Set process state to not running. */
      is_running_ = false;
    }
  }

  /* #00 Return process state. */
  return is_running_;
}

void Process::SendTerminationRequest() {
  /* #00 If process is running. */
  if (is_running_) {
    /* #00 Send SIGTERM using kill(). */
    (void)kill(pid_, SIGTERM);
  }
}

ProcessId Process::GetId() const { return pid_; }

}  // namespace process
}  // namespace osabstraction
