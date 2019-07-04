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
/**    \file   libosabstraction-gmock/include/osabstraction/process/process.h
 *     \brief  Mock implementation of the process interface.
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
#define LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <vac/language/cpp14_backport.h>
#include <memory>
#include "osabstraction/process/processinterface.h"

namespace osabstraction {
namespace process {
ProcessId GetProcessId() {
  // Implement mock here if required.
  return 42;
}

/**
 * Process Mock
 */
class ProcessMock : public ProcessInterface {
 public:
  /**
   * \brief Mocked Constructor
   * \return
   */
  MOCK_METHOD2(Process, void(ProgramImage image, Arguments args));
  /**
   * \brief Mocked IsRunning
   * \return
   */
  MOCK_METHOD0(IsRunning, bool());
  /**
   * \brief Mocked SendTerminationRequest
   * \return
   */
  MOCK_METHOD0(SendTerminationRequest, void());
  /**
   * \brief Mocked GetId
   * \return
   */
  MOCK_CONST_METHOD0(GetId, ProcessId());
};

/**
 * \brief Stub process class.
 */
class Process final : public ProcessInterface {
 public:
  /**
   * Constructor
   */
  Process() : mock_(vac::language::make_unique<ProcessMock>()) {}

  /**
   * \brief Kills the process.
   */
  ~Process() = default;

  /**
   * \brief Move constructor.
   */
  Process(Process&& other) = default;

  /**
   * \brief Move assignment operator.
   */
  Process& operator=(Process&& other) = default;

  /**
   * \brief Checks whether the process is still running.
   */
  bool IsRunning() override { return mock_->IsRunning(); }

  /**
   * \brief Sends a termination request to the process.
   */
  void SendTerminationRequest() override { mock_->SendTerminationRequest(); }

  /**
   * \brief Returns the process ID.
   */
  ProcessId GetId() const override { return mock_->GetId(); }

  /**
   * \brief pointer to wrapped mock
   */
  std::unique_ptr<ProcessMock> mock_;

 private:
  Process(const Process& other) = delete;
  Process& operator=(const Process& other) = delete;
};

/**
 * \brief Process builder mock
 */
class ProcessBuilderMock : public ProcessBuilderInterface {
 public:
  /**
   * \brief Mocked Build
   * \return
   */
  MOCK_METHOD0(Build, Process());
};

/**
 * \brief Process builder for processes.
 */
class ProcessBuilder : public ProcessBuilderInterface {
 public:
  ProcessBuilder() : mock(vac::language::make_unique<ProcessBuilderMock>()) {}

  /**
   * \brief Creates a process object with given program image and arguments.
   */
  Process Build() { return mock->Build(); }

  /**
   * \brief Wrapped Proces builder mock
   */
  std::unique_ptr<ProcessBuilderMock> mock;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_GMOCK_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
