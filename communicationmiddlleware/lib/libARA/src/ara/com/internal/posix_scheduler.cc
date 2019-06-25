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
/**        \file  posix_scheduler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/posix_scheduler.h"
#include <thread>
#include <type_traits>
#include "ara/log/logging.hpp"

namespace ara {
namespace com {
namespace internal {

bool PosixThreadScheduler::ApplySettings(const pthread_t posix_handle) {
  bool applied{false};
  struct sched_param param;
  const int policy = static_cast<typename std::underlying_type<RtSchedPolicy>::type>(policy_);
  param.sched_priority = priority_;

  if (pthread_setschedparam(posix_handle, policy, &param) == -1) {
    ara::log::LogError() << "Setting the priority and policy failed.";
  } else {
    ara::log::LogDebug() << "RT scheduler parameters applied.";
    applied = true;
  }

  return applied;
}

}  // namespace internal
}  // namespace com
}  // namespace ara
