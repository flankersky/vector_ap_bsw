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
/**        \file  allocation_phase_manager_test_adapter.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_TESTING_ALLOCATION_PHASE_MANAGER_TEST_ADAPTER_H_
#define LIB_INCLUDE_VAC_TESTING_ALLOCATION_PHASE_MANAGER_TEST_ADAPTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vac/memory/three_phase_allocator.h"

namespace vac {
namespace testing {

/**
 * \brief This class resets the AllocationPhaseManager to a new instance.
 *
 * THIS CLASS MUST NOT BE USED IN PRODUCTION CODE
 */
class AllocationPhaseManagerTestAdapter final {
 public:
  /**
   * \brief Constructor that ensures that a new instance of the AllocationPhaseManager is created
   */
  AllocationPhaseManagerTestAdapter() {
    // Ensure that the run_once flag is set
    vac::memory::AllocationPhaseManager::GetInstance();
    // Destroy the old instance and create a new one
    vac::memory::AllocationPhaseManager::instance_.reset(new vac::memory::AllocationPhaseManager());
  }

  /**
   * \brief Copy Constructor
   */
  AllocationPhaseManagerTestAdapter(const AllocationPhaseManagerTestAdapter&) = default;

  /**
   * \brief Copy Assignment
   */
  AllocationPhaseManagerTestAdapter& operator=(const AllocationPhaseManagerTestAdapter&) = default;

  /**
   * \brief Move Constructor
   */
  AllocationPhaseManagerTestAdapter(AllocationPhaseManagerTestAdapter&&) = default;

  /**
   * \brief Move Assignment
   */
  AllocationPhaseManagerTestAdapter& operator=(AllocationPhaseManagerTestAdapter&&) = default;

  ~AllocationPhaseManagerTestAdapter() = default;
};

}  // namespace testing
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_TESTING_ALLOCATION_PHASE_MANAGER_TEST_ADAPTER_H_
