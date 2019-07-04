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
/**        \file  process_interface.h
 *        \brief  The representation of an adaptive application.
 *
 *      \details  Contains data from application manifest and methods to control the execution of an adaptive
 *applications.
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_PROCESS_INTERFACE_H_
#define SRC_ARA_EXEC_INTERNAL_PROCESS_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/process/process.h>
#include <vac/memory/optional.h>
#include <ara/log/logging.hpp>

#include <array>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ara/exec/internal/types.h"

namespace ara {
namespace exec {
namespace internal {

/**
 * \brief The representation of an application.
 */
class ProcessInterface {
 public:
  ProcessInterface() = default;
  virtual ~ProcessInterface() = default;

  /**
   * \brief Returns the application state.
   * \return The application state.
   */
  virtual ProcessState GetState() = 0;

  /**
   * \brief Returns the name of the application process.
   * \return The application name.
   * \trace SWS_EM_01017
   */
  virtual const Name& GetProcessName() const noexcept = 0;

  /**
   * \brief Returns the dependencies the application requires before it can run.
   * \return The startup dependencies to other applications.
   */
  virtual const Dependencies& GetDependencies() = 0;

  /**
   * \brief Returns if the application is a platform component.
   */
  virtual bool IsPlatformApplication() = 0;

  /**
   * \brief Starts the application.
   */
  virtual void Start() = 0;

  /**
   * \brief Requests the application to shutdown.
   */
  virtual void Shutdown() = 0;

  /**
   * \brief Forces the application to terminate.
   */
  virtual void Kill() = 0;

  /**
   * \brief Checks whether the application is terminated.
   * \return True if application is terminated
   * \trace DSGN-Exec23095
   */
  virtual bool IsProcessTerminated() = 0;

  /**
   * \brief Returns process ID of the application process.
   * \return Process ID of the application process
   */
  virtual osabstraction::process::ProcessId GetPid() = 0;

  /**
   * \brief Returns the startup options of the application
   * \return The startup options of the application.
   */
  virtual const std::vector<std::string>& GetStartUpOptions() = 0;

  /**
   * \brief Gets the reported application state via ipc.
   * \trace DSGN-Exec23098
   */
  virtual void HandleApplicationClientMessages() = 0;

  /**
   * \brief Set process status - on/off - of specified function group.
   *
   * When function group status changed, global active function group counter is modified.
   *
   * \param functionGroup Name of function group
   */
  virtual void UpdateProcessStatus(StringView functionGroup) = 0;

  /**
   * \brief Verify if at least one function group or machine state is active
   * \return True if at least one function group or machine state is active
   */
  virtual bool HasActiveFunctionGroup() const noexcept = 0;
};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_PROCESS_INTERFACE_H_
