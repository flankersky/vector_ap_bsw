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
 *        \brief  Class which allows to change the core affinity of a process.
 *
 *      \details  The implementation for PikeOS is currently a stub and has no impact on CPU affinity, because PikeOS
 *                does not provide dynamic modification of the core affinity of a partition.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/cpu_core_control_interface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Class to control the CPU affinity.
 * Empty implementation.
 */
class CPUCoreControl : public CPUCoreControlInterface {
 public:
  /**
   * \brief Constructor.
   * Empty implementation.
   */
  CPUCoreControl() {}

  /**
   * \brief Destructor.
   * Empty implementation.
   */
  virtual ~CPUCoreControl() {}

  /**
   * \brief Sets the CPU core to the mask.
   * Empty implementation.
   */
  void SetCore(uint8_t) override {}

  /**
   * \brief Removes the CPU core from the mask.
   * Empty implementation.
   */
  void ClearCore(uint8_t) override {}

  /**
   * \brief Sets all the CPU cores to the mask.
   * Empty implementation.
   */
  void SetAllCores() override {}

  /**
   * \brief Clears all the CPU cores in the mask.
   * Empty implementation.
   */
  void ClearAllCores() override {}

  /**
   * \brief Sets the core affinity to the specific process.
   * Empty implementation. Always returns success.
   * \return kSuccess in case of affinity set operation successful otherwise kFailure.
   */
  CoreAffinitySettingError SetAffinity(std::int32_t) override { return CoreAffinitySettingError::kSuccess; }

  /**
   * \brief Checks if the CPU core set in the mask.
   * Empty implementation.
   * \return True
   */
  bool IsCpuSet(uint8_t) override { return true; }
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
