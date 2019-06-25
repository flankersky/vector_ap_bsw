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
 *        \brief  QNX CPU core control class
 *
 *      \details  This file defines the QNX specific CPU core handling.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_QNX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
#define LIB_LIBOSABSTRACTION_QNX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
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
 * \brief Maximum supported Core.
 */
constexpr const unsigned int kMaxSupportedCores = 1024;

/**
 * \brief Default mask size in bytes.
 */
constexpr const unsigned int kSizeOfBitmask = (kMaxSupportedCores / 8);  // Size of bitmask in bytes

/**
 * \brief Interface to control the CPU core
 */
class CPUCoreControl : public CPUCoreControlInterface {
 public:
  /**
   * \brief Constructor.
   */
  CPUCoreControl() : runmask_(nullptr), inherit_runmask_(nullptr) {
    static_assert(kMaxSupportedCores % 8 == 0, " kMaxSupportedCores value should be a multiple of 8");
    std::memset(&thread_runmask_buffer_, 0, sizeof(thread_runmask_buffer_));
    // Set mask pointers
    SetMask();
    RMSK_SET(kDefaultCpuCore, runmask_);
    RMSK_SET(kDefaultCpuCore, inherit_runmask_);
  }

  /**
   * \brief Destructor.
   */
  virtual ~CPUCoreControl() {}

  /**
   * \brief Sets the CPU core to the mask.
   * \param cpu_core Core to be set
   */
  void SetCore(std::uint8_t cpu_core) override {
    RMSK_SET(cpu_core, runmask_);
    RMSK_SET(cpu_core, inherit_runmask_);
  }

  /**
   * \brief Removes the CPU core from the mask.
   * \param cpu_core Core to be removed
   */
  void ClearCore(std::uint8_t cpu_core) override {
    RMSK_CLR(cpu_core, runmask_);
    RMSK_CLR(cpu_core, inherit_runmask_);
  }

  /**
   * \brief Sets all the CPU cores to the mask.
   */
  void SetAllCores() override {
    for (int i = 0; i < kMaxSupportedCores; i++) {
      RMSK_SET(i, runmask_);
      RMSK_SET(i, inherit_runmask_);
    }
  }

  /**
   * \brief Clears the CPU cores from the mask.
   */
  void ClearAllCores() override {
    for (int i = 0; i < kMaxSupportedCores; i++) {
      RMSK_CLR(i, runmask_);
      RMSK_CLR(i, inherit_runmask_);
    }
  }

  /**
   * \brief Sets the core affinity to the specific process.
   * \param pid Process id for which the core affinity needs to be set.
   * \return kSuccess in case of affinity set operation successful otherwise kFailure.
   */
  CoreAffinitySettingError SetAffinity(std::int32_t pid) override {
    assert(pid == getpid());

    // After the function (ThreadCtlExt) has completed, the contents of tmask_ will be replaced with the previous
    // runmask for the thread. So shadow (temporary) buffer is being used to keep the main buffer intact.
    char shadow_runmask_buffer[sizeof(int) + 2 * kSizeOfBitmask];
    std::memcpy(shadow_runmask_buffer, thread_runmask_buffer_, sizeof(thread_runmask_buffer_));
    void *tmask = reinterpret_cast<void *>(shadow_runmask_buffer);
    CoreAffinitySettingError retval = CoreAffinitySettingError::kFailure;

    // pid must currently be passed as 0 as per QNX documentation.
    if (ThreadCtlExt(0, 0, _NTO_TCTL_RUNMASK_GET_AND_SET_INHERIT, tmask) != -1) {
      retval = CoreAffinitySettingError::kSuccess;
    }

    return retval;
  }

  /**
   * \brief Checks if the CPU core set in the mask.
   * \param cpu_core Core to be checked
   * \return True if CPU is set
   */
  bool IsCpuSet(uint8_t cpu_core) override {
    return ((RMSK_ISSET(cpu_core, runmask_)) && (RMSK_ISSET(cpu_core, inherit_runmask_)));
  }

 private:
  /**
    * \brief Sets the Mask pointers.
    */
  void SetMask() {
    // The number of elements in each mask array
    int *mask_size;

    // Get the number of unsigned integers needed for the masks
    // RMSK_SIZE(): Macro to determine how many unsigned integers you need for the masks
    // _syspage_ptr->num_cpu: The number of CPUs in the system.
    int num_mask = RMSK_SIZE(_syspage_ptr->num_cpu);

    // Size of structure to describe CPU mask
    int structure_size = sizeof(*mask_size);
    structure_size += sizeof(*runmask_) * num_mask;
    structure_size += sizeof(*inherit_runmask_) * num_mask;

    // Abort if structure_size > size of thread_runmask_buffer_
    assert(structure_size <= sizeof(thread_runmask_buffer_));

    mask_size = reinterpret_cast<int *>(thread_runmask_buffer_);
    runmask_ = reinterpret_cast<unsigned *>(mask_size + 1);
    inherit_runmask_ = runmask_ + num_mask;
    *mask_size = num_mask;
  }

  /**
   * \brief Buffer for the CPU mask structure (with space for an integer (the number of elements in each mask array) and
   * the two masks).
   */
  char thread_runmask_buffer_[sizeof(int) + 2 * kSizeOfBitmask];

  /**
   * \brief runmask for structure _thread_runmask. Each set bit in runmask represents a processor that the thread can
   * run on.
   */
  unsigned *runmask_;

  /**
   * \brief calling thread's inheritance mask for structure _thread_runmask. If the calling thread creates any children,
   * the children inherit this mask
   */
  unsigned *inherit_runmask_;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_QNX_INCLUDE_OSABSTRACTION_PROCESS_CPU_CORE_CONTROL_H_
