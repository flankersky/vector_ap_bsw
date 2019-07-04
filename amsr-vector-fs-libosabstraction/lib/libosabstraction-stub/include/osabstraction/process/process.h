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
 *        \brief  Stub implementation of the process interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
#define LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/processinterface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Stub process class.
 */
class Process : public ProcessInterface {
 public:
  /**
   * \brief Creates and starts a process.
   *
   * \param[in]   image   Program image that contains the executed machine code.
   * \param[in]   args    Command line argument which shall be passed to the process.
   */
  Process(ProgramImage image, Arguments args);

  /**
   * \brief Kills the process.
   */
  ~Process();

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
   */
  bool IsRunning() override;

  /**
   * \brief Sends a termination request to the process.
   */
  void SendTerminationRequest() override;

  /**
   * \brief Returns the process ID.
   */
  ProcessId GetId() const override;

 private:
  Process(const Process& other) = delete;
  Process& operator=(const Process& other) = delete;
};

/**
 * \brief Process builder for processes.
 */
class ProcessBuilder : public ProcessBuilderInterface {
 public:
  /**
   * \brief Creates a process object with given program image and arguments.
   */
  Process Build();
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
