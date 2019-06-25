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
 *        \brief  POSIX process interface.
 *
 *      \details  This file defines the POSIX implementation variant of the process interface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <unistd.h>  // getpid()
#include <functional>
#include "osabstraction/process/cpu_core_control.h"
#include "osabstraction/process/processinterface.h"

namespace ara {
namespace exec {
// Forward declaration for unit tests
class ApplicationClientTestFixture;
}  // namespace exec
}  // namesapce ara

namespace osabstraction {
namespace process {

/**
 * \brief Settings to create a process.
 */
struct ProcessSettings {
  /**
   * \brief Program image that contains the executed machine code.
   */
  ProgramImage image;

  /**
   * \brief Command line argument which shall be passed to the process.
   */
  Arguments args;

  /**
   * \brief  The scheduling priority for the application under process.
   */
  unsigned int scheduling_priority;

  /**
   * \brief The scheduling policy set for the process.
   */
  SchedulingPolicies scheduling_policy;

  /**
   * \brief The CPU core control for core mask setting of the process.
   */
  CPUCoreControl* cpu_core_control;
};

/**
 * \brief Process class for POSIX OSs.
 */
class Process : public ProcessInterface {
 public:
  /**
   * \brief Creates and starts a process.
   *
   * The constructor forks a new process. In the child process it changes the working directory to the directory of
   * the given program image. Afterwards the program image is executed with the given arguments.
   *
   * \param settings The settings to create a process.
   * \param process_created_callout If this function was set, it will be called by the parent process. The child process
   * will not be executed before this function was either performed completely or was not set. Afterwards the binary of
   * the child will be loaded and executed.
   * The conditions for an call to this function are:
   *    The fork was performed successfully
   *    The childprocess is ready to be executed
   *    The function was set
   * The execution of the childprocess is blocked until the given function was performed. Therefore, this function can
   * be used to perform some necessary things. For example to create the messagequeues which are used by the child to
   * communicate with the parent process.
   *
   * \throws std::runtime_error   If fork() fails
   * \throws std::runtime_error   If program image path is too long
   * \throws std::runtime_error   If changing directory fails
   * \throws std::runtime_error   If executing program image fails
   * \throws std::runtime_error   If setting of scheduling parameters fails
   * \throws std::runtime_error   If setting of core affinity fails
   */
  explicit Process(const ProcessSettings& settings,
                   const std::function<void(osabstraction::process::ProcessId)>& process_created_callout = nullptr);
  /**
   * \brief Kills the process.
   *
   * If creation has been successful, the destructor kills the process by sending a SIGKILL signal.
   */
  virtual ~Process();

  /**
   * \brief Move constructor.
   */
  Process(Process&& other);

  /**
   * \brief Move assignment operator.
   */
  Process& operator=(Process&& other);

  /**
   * \brief Checks whether the process is still running.
   *
   * \return bool True if process is running, false if not.
   *
   * \throws std::system_error   Should basically never throw, because waitpid() options are not
   * variable here. If it throws, it is because of wrong options or because WNOHANG is not set as option in the
   * waitpid() function used in the definition.
   */
  bool IsRunning() override;

  /**
   * \brief Sends a SIGTERM signal to the process.
   */
  void SendTerminationRequest() override;

  /**
   * \brief Returns the process ID which has been given by the kernel.
   */
  ProcessId GetId() const override;

 protected:
  Process(const Process& other) = delete;
  Process& operator=(const Process& other) = delete;

  /**
   * \brief Process ID.
   */
  ProcessId pid_;

  /**
   * \brief Currently assumed running state.
   */
  bool is_running_;

 private:
  friend class ara::exec::ApplicationClientTestFixture;
};

/**
 * \brief Process builder.
 *
 * Trivial implementation no pool or other OS resource handling.
 */
class ProcessBuilder : public ProcessBuilderInterface {
 public:
  virtual ~ProcessBuilder() {}
  /**
   * \brief Creates the configured process.
   *
   * Creates and starts a process as configured. The current working directory will be the directory of the program
   * image.
   *
   * \return Created process.
   */
  Process Build() override {
    ProcessSettings settings = {image_, args_, scheduling_priority_, scheduling_policy_, cpu_core_control_};
    return Process(settings, process_created_callout_);
  }
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
