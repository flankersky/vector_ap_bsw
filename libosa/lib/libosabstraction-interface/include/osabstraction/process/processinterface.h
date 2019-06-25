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
 *        \brief  Abstract interface of process.
 *
 *      \details  This file defines an OS independent interface to work with processes.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sched.h>
#include <cstdint>
#include <functional>
#include <utility>
#include <vector>
#include "osabstraction/process/cpu_core_control_interface.h"

namespace osabstraction {
namespace process {

class Process;

/**
 * \brief Process identifier which is defined, if the process is created.
 */
using ProcessId = std::int32_t;

/**
 * \brief Program image that defines the executed machine code which can be executed in a process.
 */
using ProgramImage = const char*;

/**
 * \brief Command line argument which can be passed to a process.
 */
using Argument = const char*;

/**
 * \brief A list of command line arguments.
 */
using Arguments = std::vector<Argument>;

/**
 * \brief The Scheduling policies available for applications
 */
enum class SchedulingPolicies : uint8_t {
  kOther = SCHED_OTHER,  /**< Other OS specific scheduling policy. */
  kFifo = SCHED_FIFO,    /**< First in-first out (FIFO) scheduling policy. */
  kRoundRobin = SCHED_RR /**< Round robin scheduling policy. */
};

/**
 * \brief Returns the ID of the calling process.
 */
ProcessId GetProcessId();

/**
 * \brief Define the termination signal of a child process to be ignored
 */
void IgnoreChildTerminationSignal();

/**
 * \brief Abstract interface of a process.
 *
 * A process is an instance of a program image that is being executed. It has its own address space and a set of
 * system resources.
 */
class ProcessInterface {
 public:
  /**
   * \brief Send termination request.
   *
   * The process is not forced to terminate itself. Reaction is dependent on the program which is executed by the
   * process.
   *
   */
  virtual void SendTerminationRequest() = 0;

  /**
   * \brief Returns whether the process is running or terminated.
   *
   * \retval true   Process is running.
   * \retval false  Process is not running.
   */
  virtual bool IsRunning() = 0;

  /**
   * \brief Returns the process identifier.
   *
   * Identifiers may be recycled, after process termination.
   *
   * \return Process identifier.
   */
  virtual ProcessId GetId() const = 0;

  /**
   * \brief Allocates all required OS resources and starts process execution.
   */
  ProcessInterface() {}

  /**
   * \brief Terminates the process and releases all process related OS resources.
   */
  virtual ~ProcessInterface() {}
};

/**
 * \brief Abstract interface of a process builder.
 *
 * The ProcessBuilder creates processes and is responsible for management of process related OS resources.
 *
 * Usage:
 *
 *     ProcessBuilder builder;
 *     Process myProcess = builder.SetProgramImage("/bin/app/a.out")
 *                            .AddArgument("--help")
 *                            .AddArgument("--version")
 *                            .Build();
 *
 */
class ProcessBuilderInterface {
 public:
  /**
   * \brief Set the program image for the process to be build.
   *
   * \param[in]   image         Program image that contains the executed machine code.
   *
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& SetProgramImage(ProgramImage image);

  /**
   * \brief Adds an command line argument for the process to be build.
   *
   * \param[in]   arg           Command line argument which shall be passed to the process.
   *
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& AddArgument(Argument arg);

  /**
   * \brief Empty object constructor.
   */
  ProcessBuilderInterface() : image_(0), args_(), scheduling_priority_(), scheduling_policy_(), cpu_core_control_() {}

  /**
   * \brief Default copy constructor.
   */
  ProcessBuilderInterface(const ProcessBuilderInterface& other) = default;

  /**
   * \brief Default move constructor.
   */
  ProcessBuilderInterface(ProcessBuilderInterface&& other) = default;

  /**
   * \brief Copy assignment operator.
   */
  ProcessBuilderInterface& operator=(const ProcessBuilderInterface& other) = default;

  /**
   * \brief Move assignment operator.
   */
  ProcessBuilderInterface& operator=(ProcessBuilderInterface&& other) = default;

  /**
   * \brief Sets the scheduling priority for the process to be build.
   *
   * \param[in]   scheduling_priority for the application under process.
   *
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& SetSchedulingPriority(unsigned int scheduling_priority);

  /**
   * \brief Sets the scheduling policy for the process to be build.
   *
   * \param[in]   scheduling_policy for the application under process.
   *
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& SetSchedulingPolicy(const SchedulingPolicies& scheduling_policy);

  /**
   * \brief Sets the callback function which will be called after the process ID of the new child is known.
   *
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
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& SetProcessCreatedCallout(
      const std::function<void(osabstraction::process::ProcessId)>& process_created_callout = nullptr);

  /**
   * \brief Sets the CPU core control for the process to be build.
   *
   * \param[in]   cpu_core_control for the process.
   *
   * \return Reference to process builder.
   */
  ProcessBuilderInterface& SetCpuCoreControl(CPUCoreControl* cpu_core_control);

  virtual ~ProcessBuilderInterface() {}

  /**
   * \brief Creates the configured process.
   *
   * Creates and starts a process as configured. The current working directory will be the directory of the program
   * image.
   *
   * \return Created process.
   */
  virtual Process Build() = 0;

 protected:
  /**
   * \brief Program image of the process which will be built next.
   */
  ProgramImage image_;

  /**
   * \brief Arguments to be passed to the process which will be built next.
   */
  Arguments args_;

  /**
   * \brief The scheduling priority for the application under process.
   */
  unsigned int scheduling_priority_;

  /**
   * \brief The scheduling policy set for this application.
   */
  SchedulingPolicies scheduling_policy_;

  /**
   * \brief The CPU core control instance for setting the core affinity for the process.
   */
  CPUCoreControl* cpu_core_control_;

  /**
   * \brief If this function was set, it will be called by the parent process. The child process
   * will not be executed before this function was either performed completely or was not set. Afterwards the binary of
   * the child will be loaded and executed.
   * The conditions for an call to this function are:
   *    The fork was performed successfully
   *    The childprocess is ready to be executed
   *    The function was set
   * The execution of the childprocess is blocked until the given function was performed. Therefore, this function can
   * be used to perform some necessary things. For example to create the messagequeues which are used by the child to
   * communicate with the parent process.
   */
  std::function<void(osabstraction::process::ProcessId)> process_created_callout_ = nullptr;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_PROCESSINTERFACE_H_
