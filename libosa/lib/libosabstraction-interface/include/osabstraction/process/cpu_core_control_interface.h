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
/**        \file
 *        \brief  Abstract interface to change the core affinity of a process.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace osabstraction {
namespace process {

class CPUCoreControl;

/**
 * \brief Enum class to define the error codes for core affinity setting operation.
 */
enum class CoreAffinitySettingError : uint8_t { kSuccess, kFailure };

/**
 * \brief Interface to control the CPU core
 */
class CPUCoreControlInterface {
 public:
  /**
   * \brief Constructor.
   */
  CPUCoreControlInterface() {}

  /**
   * \brief Destructor
   */
  virtual ~CPUCoreControlInterface() {}

  /**
   * \brief Sets the CPU core to the mask.
   * \param[in] cpu_core Core to be set
   */
  virtual void SetCore(std::uint8_t cpu_core) = 0;

  /**
   * \brief Removes the CPU core from the mask.
   * \param[in] cpu_core Core to be removed
   */
  virtual void ClearCore(std::uint8_t cpu_core) = 0;

  /**
   * \brief Sets all the CPU cores to the mask.
   */
  virtual void SetAllCores() = 0;

  /**
   * \brief Clears all the CPU cores in the mask.
   */
  virtual void ClearAllCores() = 0;

  /**
   * \brief Sets the core affinity to the specific process.
   * \param[in] pid Process id for which the core affinity needs to be set.
   * \return kSuccess in case of affinity set operation successful otherwise kFailure.
   *
   * \attention TODO(visasl): QNX implementation currently only allows modification of the calling process!
   */
  virtual CoreAffinitySettingError SetAffinity(std::int32_t pid) = 0;

  /**
   * \brief Checks if the CPU core set in the mask.
   * \param[in] cpu_core Core to be checked
   * \return True if CPU is set
   */
  virtual bool IsCpuSet(uint8_t cpu_core) = 0;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_INTERFACE_H_
