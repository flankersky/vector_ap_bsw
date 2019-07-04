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
/**        \file  types.h
 *        \brief  Contains type definitions used in the executionmanager environment.
 *
 *********************************************************************************************************************/

#ifndef SRC_ARA_EXEC_INTERNAL_TYPES_H_
#define SRC_ARA_EXEC_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vac/container/basic_string.h>
#include <vac/container/string_view.h>

#include <string>
#include <utility>
#include <vector>

#include "ara/exec/application_client.hpp"

namespace ara {
namespace exec {
namespace internal {

/**
 * \brief Type definition for string using leaky allocator
 */
using SafeString = vac::container::SafeString;

/**
 * \brief Type definition of string view
 */
using StringView = vac::container::string_view;

/**
 * \brief The Executuion Management internal application states.
 */
enum class ProcessState : uint8_t {
  kIdle = 0x0,    /**< Application is idle and has never run before. */
  kStarted = 0x1, /**< Application is started but the execution manager has not yet received an state report. */
  kRunning = 0x2, /**< Application reported it is running. */
  kTerminatingRequested = 0x3, /**< Application is asked to terminate. */
  kTerminating = 0x4,          /**< Application reported it is terminating. */
  kTerminated = 0x5            /**< Application has at least run once and is terminated. */
};

/**
 * \brief The Scheduling policies available for applications
 */
using SchedulingPolicies = osabstraction::process::SchedulingPolicies;

/**
 * \brief Return types for EM internal functions
 */
enum class ReturnType : uint8_t {
  kNotOk = 0x0, /**< non-success*/
  kOk = 0x1,    /**< success */
};

/**
 * \brief The application state Initializing as represented in the application manifest
 */
const char kManifestFormatAppStateInitializing[] = "Initializing";

/**
 * \brief The application state Running as represented in the application manifest
 */
const char kManifestFormatAppStateRunning[] = "Running";

/**
 * \brief The application state Terminating as represented in the application manifest
 */
const char kManifestFormatAppStateTerminating[] = "Terminating";

/**
 * \brief The application startup machine mode in the application manifest
 */
const char kManifestFormatMachineModeStartup[] = "Startup";

/**
 * \brief Key for required function group MachineState
 */
const char kManifestFormatMachineState[] = "MachineState";

/**
 * \brief Maximum path size for files on file system
 */
const uint16_t kPathSizeMax = 4096u;

/**
 * \brief The type for application names.
 */
typedef std::string Name;

/**
 * \brief Type definition for a dependency.
 */
typedef std::pair<Name, ProcessState> Dependency;

/**
 * \brief Type definition for a set of dependencies.
 */
typedef std::vector<Dependency> Dependencies;

/**
 * \brief Type definition for a resource group.
 */
typedef std::vector<std::string> ResourceGroup;

/**
 * \brief Default state of function group when disabled
 * \trace SWS_EM_01110
 */
const SafeString kFunctionGroupStateOff{"Off"};

}  // namespace internal
}  // namespace exec
}  // namespace ara

#endif  // SRC_ARA_EXEC_INTERNAL_TYPES_H_
