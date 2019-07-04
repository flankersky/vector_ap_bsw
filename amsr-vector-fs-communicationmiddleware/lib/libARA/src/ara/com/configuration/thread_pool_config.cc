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
/**        \file  thread_pool_config.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/com/configuration/thread_pool_config.h>
#include "ara/log/logging.hpp"

namespace ara {
namespace com {
namespace configuration {

ThreadPoolConfig::ThreadPoolConfig() : valid_{false} {}

ThreadPoolConfig::ThreadPoolConfig(const PoolId pool_id, const ThreadCount num_threads, const TaskCount max_tasks)
    : id_{pool_id},
      num_threads_{num_threads},
      max_tasks_{max_tasks},
      valid_{HasValidNumberOfWorkerThreads(num_threads)} {}

ThreadPoolConfig::ThreadPoolConfig(const PoolId pool_id, const ThreadCount num_threads, const TaskCount max_tasks,
                                   const ara::com::internal::RtSchedPolicy scheduling_policy,
                                   const ara::com::internal::SchedPriority scheduling_priority)
    : id_{pool_id},
      scheduling_policy_(scheduling_policy),
      policy_set_{true},
      scheduling_priority_(scheduling_priority),
      num_threads_{num_threads},
      max_tasks_{max_tasks},
      valid_{HasValidNumberOfWorkerThreads(num_threads) && HasValidSchedulingSettings()} {}

ThreadPoolConfig::PoolId ThreadPoolConfig::GetPoolId() const noexcept { return id_; }

ThreadPoolConfig::RtSchedPolicy ThreadPoolConfig::GetSchedulingPolicy() const noexcept { return scheduling_policy_; }

ThreadPoolConfig::SchedPriority ThreadPoolConfig::GetSchedulingPriority() const noexcept {
  return scheduling_priority_;
}

ThreadPoolConfig::ThreadCount ThreadPoolConfig::GetNumberOfWorkerThreads() const noexcept { return num_threads_; }

ThreadPoolConfig::TaskCount ThreadPoolConfig::GetMaxTasks() const noexcept { return max_tasks_; }

bool ThreadPoolConfig::IsValid() const noexcept { return valid_; }

bool ThreadPoolConfig::HasUserDefinedSchedulerSettings() const noexcept { return policy_set_; }

const ThreadPoolConfig::ThreadPoolAssignmentConfigContainer& ThreadPoolConfig::GetAssignmentConfigs() const noexcept {
  return assignment_configs_;
}

ThreadPoolConfig& ThreadPoolConfig::operator<<(const ThreadPoolAssignmentConfig& assignment_config) {
  // Store the thread pool assignment config
  assignment_configs_.emplace_back(assignment_config);

  return *this;
}

bool ThreadPoolConfig::HasValidSchedulingSettings() const noexcept {
  return ((scheduling_priority_ >= ThreadPoolConfig::kMinPriority) &&
          (scheduling_priority_ <= ThreadPoolConfig::kMaxPriority));
}

bool ThreadPoolConfig::HasValidNumberOfWorkerThreads(const ThreadCount num_threads) const noexcept {
  // Maximum number of supported threads by the HW in use.
  const auto supported_threads_hint = std::thread::hardware_concurrency();

  if ((supported_threads_hint > 0U) && (num_threads > supported_threads_hint)) {
    // Print warning and limit the configured value to the supported number of possible threads for this hardware..
    ara::log::LogDebug() << "Number of worker threads defined for this thread-pool exceeds the recommended number of "
                            "threads. Specified: "
                         << static_cast<std::uint32_t>(num_threads) << ", recommended: " << supported_threads_hint;
  }

  return ((num_threads != 0U) && (num_threads >= kMinThreadsPerThreadPool));
}

}  // namespace configuration
}  // namespace com
}  // namespace ara
