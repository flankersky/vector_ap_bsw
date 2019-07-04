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
/**        \file  thread_pool_config.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_CONFIG_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>
#include "ara/com/configuration/thread_pool_assignment_config.h"
#include "ara/com/internal/posix_scheduler.h"

namespace ara {
namespace com {
namespace configuration {

/**
 * \brief Thread-Pool configuration
 */
class ThreadPoolConfig {
 public:
  /**
   * \brief Alias for readability.
   */
  using PoolId = std::uint8_t;

  /**
   * \brief Type-alias for the number of threads.
   */
  using ThreadCount = std::uint8_t;

  /**
   * \brief Type for the number of tasks.
   */
  using TaskCount = std::uint32_t;

  /**
   * \brief Type of scheduling policy
   */
  using RtSchedPolicy = ara::com::internal::RtSchedPolicy;

  /**
   * \brief Type of scheduling policy
   */
  using SchedPriority = ara::com::internal::SchedPriority;

  /**
   * \brief Type alias for thread pool assignment configuration container
   */
  using ThreadPoolAssignmentConfigContainer = std::vector<ThreadPoolAssignmentConfig>;

  /**
   * \brief Pool-ID of the default thread-pool for the runtime.
   */
  static constexpr PoolId kDefaultThreadPoolId{0U};

  /**
     * \brief Number of threads of the default thread-pool.
     */
  static constexpr TaskCount kDefaultThreadPoolNumThreads{1U};

  /**
   * \brief Max. number of tasks of the default thread-pool.
   */
  static constexpr TaskCount kDefaultThreadPoolMaxTask{1024U};

  /**
   * \brief Minimum number of threads of a pool.
   */
  static constexpr ThreadCount kMinThreadsPerThreadPool{1U};

  /**
   * \brief Default constructor sets this configuration to false valid.
   */
  ThreadPoolConfig();

  /**
   * \brief Parametrize this thread-pool configuration.
   *
   * \param pool_id Unique ID for the ThreadPool object.
   * \param num_threads Number of threads to spawn in the container.
   * \param max_tasks Maximum number of tasks allowed that can be queued.
   */
  ThreadPoolConfig(const PoolId pool_id, const ThreadCount num_threads, const TaskCount max_tasks);

  /**
   * \brief Constructor with user-defined scheduler settings.
   *
   * \param pool_id Unique ID for the ThreadPool object.
   * \param num_threads Number of threads to spawn in the container.
   * \param max_tasks Maximum number of tasks allowed that can be queued.
   * \param scheduling_policy Configure the scheduler's policy for a certaub thread.
   * \param scheduling_priority Configure the scheduler's priority for a certain thread.
   */
  ThreadPoolConfig(const PoolId pool_id, const ThreadCount num_threads, const TaskCount max_tasks,
                   const ara::com::internal::RtSchedPolicy scheduling_policy,
                   const ara::com::internal::SchedPriority scheduling_priority);

  /**
   * \brief Check if this configuration has no errors.
   */
  bool IsValid() const noexcept;

  /**
   * \brief Get the pool identifier of this configuration.
   */
  PoolId GetPoolId() const noexcept;

  /**
   * \brief Get the scheduling policy used for the thread-pool and all of its threads.
   * \return The POSIX scheduling policy.
   */
  ara::com::internal::RtSchedPolicy GetSchedulingPolicy() const noexcept;

  /**
   * \brief Get the scheduling priority used for the thread-pool and all of its threads.
   * \return The POSIX scheduling priority.
   */
  ara::com::internal::SchedPriority GetSchedulingPriority() const noexcept;

  /**
   * \brief Get the number of worker threads configured.
   */
  ThreadCount GetNumberOfWorkerThreads() const noexcept;

  /**
   * \brief Get the number of maximum enqueueable tasks.
   */
  TaskCount GetMaxTasks() const noexcept;

  /**
   * \brief Check if this configuration contains user-defined POSIX scheduler settings.
   */
  bool HasUserDefinedSchedulerSettings() const noexcept;

  /**
   * \brief Get the thread pool assignment configurations
   */
  const ThreadPoolAssignmentConfigContainer& GetAssignmentConfigs() const noexcept;

  /**
   * \brief Add a new thread pool assignment configuration
   * \param assignment_config New thread pool assignment configuration to be added
   * \return The thread-pool configuration itself.
   */
  ThreadPoolConfig& operator<<(const ThreadPoolAssignmentConfig& assignment_config);

 private:
  /**
   * \brief Possible minimum for process priority.
   */
  static constexpr SchedPriority kMinPriority{1};

  /**
   * \brief Possible maximum for process priority.
   */
  static constexpr SchedPriority kMaxPriority{98};

  /**
   * \brief Check if scheduling stettings are valid.
   * \return true if scheduling settings are valid, false if not.
   */
  bool HasValidSchedulingSettings() const noexcept;

  /**
   * \brief Check if the number of specified worker threads is valid.
   * \param num_threads Number of threads to be checked against hardware / OS limitations
   */
  bool HasValidNumberOfWorkerThreads(const ThreadCount num_threads) const noexcept;

  /**
   * \brief Id of this thread-pool.
   */
  PoolId id_{};

  /**
   * \brief Scheduling policy
   */
  RtSchedPolicy scheduling_policy_{RtSchedPolicy::kFifo};

  /**
   * \brief Policy is set or not.
   */
  bool policy_set_{false};

  /**
   * \brief Scheduling priority
   */
  SchedPriority scheduling_priority_{kMinPriority};

  /**
   * \brief Number of worker threads.
   */
  ThreadCount num_threads_{};

  /**
   * \brief Maximum tasks for this thread-pool to be enqueued.
   * This is also the static size of the container.
   */
  TaskCount max_tasks_{};

  /**
   * \brief Valid configuration flag.
   */
  bool valid_{false};

  /**
   * \brief Thread pool assignment configurations
   */
  ThreadPoolAssignmentConfigContainer assignment_configs_;
};

}  // namespace configuration
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_CONFIG_H_
