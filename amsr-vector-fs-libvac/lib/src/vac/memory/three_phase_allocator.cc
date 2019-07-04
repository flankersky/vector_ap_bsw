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
/**        \file  three_phase_allocator.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#include "vac/memory/three_phase_allocator.h"

#include <mutex>

#include "vac/language/cpp14_backport.h"

namespace vac {
namespace memory {

std::once_flag AllocationPhaseManager::flag_;
std::unique_ptr<AllocationPhaseManager> AllocationPhaseManager::instance_;

AllocationPhaseManager& AllocationPhaseManager::GetInstance() {
  std::call_once(flag_, []() {
    // Note: make_unique is not available, as the constructor of AllocationPhaseManager is private
    instance_.reset(new AllocationPhaseManager());
  });
  return *instance_.get();
}

}  // namespace memory
}  // namespace vac
