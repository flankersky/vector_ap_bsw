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
 *        \brief  Linux CPU core control class
 *
 *      \details  This file defines the Linux specific CPU core handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
#define LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sched.h>
#include <cstring>
#include <iostream>
#include "osabstraction/process/cpu_core_control_interface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Default CPU core.
 */
constexpr const int kDefaultCpuCore = 0;
/**
 * \brief Interface to control the CPU core
 */
class CPUCoreControl : public CPUCoreControlInterface {
 public:
  /**
   * \brief Constructor.
   */
  CPUCoreControl() : mask_() { CPU_SET(kDefaultCpuCore, &mask_); }

  /**
   * \brief Destructor.
   */
  virtual ~CPUCoreControl() {}

  /**
   * \brief Sets the CPU core to the mask.
   * \param cpu_core Core to be set
   */
  void SetCore(uint8_t cpu_core) override { CPU_SET(cpu_core, &mask_); }

  /**
   * \brief Removes the CPU core from the mask.
   * \param cpu_core Core to be removed
   */
  void ClearCore(uint8_t cpu_core) override { CPU_CLR(cpu_core, &mask_); }

  /**
   * \brief Sets all the CPU cores to the mask.
   */
  void SetAllCores() override {
    for (int i = 0; i < CPU_SETSIZE; i++) {
      CPU_SET(i, &mask_);
    }
  }

  /**
   * \brief Sets the CPU cores to the mask.
   */
  void ClearAllCores() override { CPU_ZERO(&mask_); }

  /**
   * \brief Sets the core affinity to the specific process.
   * \param pid Process id for which the core affinity needs to be set.
   * \return kSuccess in case of affinity set operation successful otherwise kFailure.
   */
  CoreAffinitySettingError SetAffinity(std::int32_t pid) override {
    CoreAffinitySettingError retval = CoreAffinitySettingError::kFailure;
    if (sched_setaffinity(pid, sizeof(mask_), &mask_) == 0) {
      retval = CoreAffinitySettingError::kSuccess;
    }

    return retval;
  }

  /**
   * \brief Checks if the CPU core set in the mask.
   * \param cpu_core Core to be checked
   * \return True if CPU is set
   */
  bool IsCpuSet(uint8_t cpu_core) override { return CPU_ISSET(cpu_core, &mask_); }

 private:
  /**
   * \brief Structure to describe CPU mask.
   */
  cpu_set_t mask_;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
