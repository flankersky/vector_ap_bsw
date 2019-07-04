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
/**        \file  thread_pool.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_THREAD_POOL_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_THREAD_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <condition_variable>
#include <memory>
#include <thread>
#include <utility>
#include <vector>
#include "ara/com/configuration/thread_pool_config.h"
#include "ara/com/internal/posix_scheduler.h"
#include "ara/com/internal/static_queue.h"
#include "ara/log/logging.hpp"
#include "vac/container/static_vector.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Models one abstract task to be implemented by a derived class. Concrete
 * methods of a skeleton derive from this base-class.
 * These pending requests are then stored in a global request queue for later execution.
 */
class Task {
 public:
  /**
   * \brief This destructor shall be virtual to prevent undefined behavior on deletion of
   * a derived object through a pointer to its base.
   */
  virtual ~Task() = default;

  /**
   * \brief Pure virtual method, to be realized by derived classes.
   */
  virtual void operator()() = 0;
};

/**
 * \brief The ThreadPool contains a number of threads that process incoming
 * requests.
 * \remark This version is checked by gcc's thread sanitizer
 * (option `-fsanitize=thread`).
 */
class ThreadPool {
 public:
  /**
   * \brief Initialization status of this thread pool.
   */
  enum class Status { kOk, kCfgError, kFatal };

  /**
   * \brief State to control worker thread excecution.
   */
  enum class State { kStopped, kRunning };

  /**
   * \brief ThreadPool configuration type
   */
  using ThreadPoolConfig = ara::com::configuration::ThreadPoolConfig;

  /**
   * \brief Forward type
   */
  using PoolId = typename ThreadPoolConfig::PoolId;

  /**
   * \brief Worker thread count
   */
  using ThreadCount = typename ThreadPoolConfig::ThreadCount;

  /**
   * \brief Type-alias for the task count
   */
  using TaskCount = typename ThreadPoolConfig::TaskCount;

  /**
   * \brief Default constructor
   */
  ThreadPool();

  /**
   * \brief If the ThreadPool object goes out of scope / gets destructed, shut down all worker threads and join
   * all threads contained for proper termination.
   */
  ~ThreadPool() { Shutdown(); }

  /**
   * \brief Initializes the ThreadPool by spawning the worker threads.
   * \note This will return immediately if the thread pool has running threads
   * already.
   * \note No thread-safety guarantee
   *
   * \param config The thread-pool's config including:
   * * pool id,
   * * number of worker threads
   * * number of tasks
   * * (optional) scheduler priority
   * * (optional) scheduler policy
   * \return status of initialization
   * Status::kOk - spawning threads was successful.
   * Status::kCfgError - spawning threads not possible due to configuration
   * error or thread-pool already started.
   */
  Status Initialize(const ara::com::configuration::ThreadPoolConfig& config);

  /**
   * \brief Shuts down the complete ThreadPool including all threads contained.
   * This will terminate threads even if there are pending tasks in the queue.
   */
  void Shutdown();

  /**
   * \brief Add a task to the queue for the worker threads to process.
   *
   * \tparam Task Deduced type
   * \param task to add to the queue for later processing within a worker
   * thread.
   * \return true if the task could be enqueued, false if the maximum number of
   * queued tasks has exceeded.
   */
  template <typename Task>
  bool AddTask(Task&& task) {
    bool enqueued{true};
    if (state_ == State::kRunning) {
      {
        using Task_ = typename std::decay<Task>::type;  // Without any decoration
        std::unique_ptr<Task_> task_{new Task_(std::move(task))};
        enqueued = queue_.push(std::move(task_));
      }
      cv_.notify_one();

      if (!enqueued) {
        logger_.LogError() << "Maximum number of tasks for Thread-Pool with ID "
                           << static_cast<std::uint16_t>(cfg_.GetPoolId()) << " has been reached.";
      }
    } else {
      logger_.LogError() << "Thread-Pool with ID " << static_cast<std::uint16_t>(cfg_.GetPoolId())
                         << " is not running.";
      enqueued = false;
    }

    return enqueued;
  }

  /**
   * \brief Getter for the pool id.
   */
  PoolId GetPoolId() const noexcept { return cfg_.GetPoolId(); }

  /**
   * \brief Type-alias that specifies a queue for the tasks to process.
   */
  using TaskQueue = ara::com::internal::StaticQueue<std::unique_ptr<Task>>;

  /**
   * \brief Getter for the number of worker threads.
   * This number will not change during runtime.
   *
   * \return The number of worker threads.
   */
  typename TaskQueue::size_type GetNumberOfWorkerThreads() const noexcept { return workers_.size(); }

  // Protected allows access to the following attributes for testing
  // to get rid of keyword friend.
 protected:
  /**
   * \brief Condition variable used to do a blocking wait until
   * a tasked is pending for a worker thread or the thread-pool has been shut
   * down.
   */
  std::condition_variable cv_;

  /**
   * \brief Task queue the worker threads access
   */
  TaskQueue queue_;

  /**
   * \brief Mutex for the conditional wait within the worker threads.
   */
  std::mutex pool_mutex_;

  /**
   * \brief State of this thread pool for threads to access and terminate if the
   * state_ is kStopped.
   */
  std::atomic<State> state_;

 private:
  // Thread-Pools shall be non-copyable / non-moveable
  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool(const ThreadPool&& other) = delete;
  ThreadPool& operator=(const ThreadPool& other) = delete;
  ThreadPool& operator=(ThreadPool&& other) = delete;

  /**
   * \brief Called by the initialization routine to spawn the worker threads.
   */
  void SpawnWorkerThreads();

  /**
   * \brief Configuration for this thread-pool.
   */
  ThreadPoolConfig cfg_;

  /**
   * \brief Type-alias for the worker thread container.
   */
  using WorkerThreadContainer = vac::container::StaticVector<std::thread>;

  /**
   * \brief Worker threads contained within this thread-pool.
   */
  WorkerThreadContainer workers_;

  /**
   * \brief Reference to the logger for this thread-pool.
   */
  ara::log::Logger& logger_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_THREAD_POOL_H_
