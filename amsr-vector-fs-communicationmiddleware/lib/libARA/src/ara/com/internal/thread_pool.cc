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
/**        \file  thread_pool.cc
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/thread_pool.h"
#include <cassert>

namespace ara {
namespace com {
namespace internal {

ThreadPool::ThreadPool() : state_{State::kStopped}, logger_(ara::log::CreateLogger("ThreadPool", "")) {}

ThreadPool::Status ThreadPool::Initialize(const ThreadPoolConfig& config) {
  logger_.LogDebug() << __func__ << ": Thread-Pool ID: " << static_cast<std::uint32_t>(config.GetPoolId())
                     << ", number of worker threads: " << static_cast<std::uint32_t>(config.GetNumberOfWorkerThreads())
                     << ", maximum tasks: " << config.GetMaxTasks();

  if (state_ != State::kStopped) {
    // If the pool is running already a call to ThreadPool::Shutdown is
    // necessary.
    logger_.LogFatal() << "No re-initialization!";
    return Status::kFatal;
  }

  if (!config.IsValid()) {
    logger_.LogFatal() << "Configuration error for thread-pool ID: " << config.GetPoolId();
    return Status::kCfgError;
  }

  cfg_ = config;
  const auto num_threads = cfg_.GetNumberOfWorkerThreads();
  const auto max_tasks = cfg_.GetMaxTasks();
  workers_.reserve(num_threads);
  queue_.reserve(max_tasks);
  state_ = State::kRunning;
  SpawnWorkerThreads();
  return Status::kOk;
}

void ThreadPool::Shutdown() {
  logger_.LogDebug() << __func__;
  state_ = State::kStopped;
  // The following notification is used to get threads out of the blocking wait, in case they are idle.
  cv_.notify_all();

  for (auto& thread : workers_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void ThreadPool::SpawnWorkerThreads() {
  logger_.LogDebug() << __func__ << ", spawning number of worker threads: " << workers_.capacity();

  // Start all worker threads...
  for (std::size_t i{0U}; i < workers_.capacity(); ++i) {
    workers_.emplace_back(std::thread([this, i]() {
      logger_.LogDebug() << "Starting worker thread " << i;
      bool settings_applied{true};

      if (cfg_.HasUserDefinedSchedulerSettings()) {
        PosixThreadScheduler scheduler{cfg_.GetSchedulingPolicy(), cfg_.GetSchedulingPriority()};
        settings_applied = scheduler.ApplySettings(pthread_self());
        if (!settings_applied) {
          logger_.LogWarn() << "Couldn't apply user-specific real-time scheduler settings for this thread.";
        }
      }

      while (state_ == State::kRunning) {
        std::unique_ptr<Task> task{nullptr};
        std::unique_lock<std::mutex> lock(pool_mutex_);
        // Either one task is waiting for the worker thread or the thread-pool is shut down.
        cv_.wait(lock,
                 [this] { return (!queue_.empty() || (state_ != State::kRunning)); });  // idle until work comes in
        lock.unlock();
        if (state_ != State::kRunning) {
          break;
        }

        task = std::move(queue_.front());
        queue_.pop();
        if (task) {
          (*task)();  // Executes the work to do within this thread.
        } else {
          logger_.LogFatal() << "Couldn't process task because of nullptr address.";
        }
      }
    }));
  }
}

}  // namespace internal
}  // namespace com
}  // namespace ara
