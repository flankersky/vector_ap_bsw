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
 *        \brief
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_THREADPOOL_THREAD_POOL_H_
#define LIB_INCLUDE_VAC_THREADPOOL_THREAD_POOL_H_

#include <threadpool/work_unit.h>
#include <vac/testing/test_adapter.h>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "vac/container/static_list.h"
#include "vac/memory/leaky_array_allocator.h"

namespace vac {
namespace threadpool {

/**
 * \brief Implements a thread pool with a given capacity.
 */
template <class W>
class ThreadPool {
  static_assert(std::is_base_of<WorkUnit, W>::value, "W must inherit from WorkUnit");

 public:
  /// Typedef for the WorkQueue
  using WorkQueueType = vac::container::StaticList<W>;

  /**
   * \brief Builds a new thread pool and starts the worker threads.
   *
   * \param number_threads The number of worker threads to start.
   * \param length_list Length of the list containing the workunits.
   */
  explicit ThreadPool(size_t number_threads, typename WorkQueueType::size_type length_list) : running_(true) {
    work_queue_.reserve(length_list);
    threads_.reserve(number_threads);
    for (size_t i{0}; i < number_threads; ++i) {
      threads_.emplace_back(&ThreadPool::Worker, this);
    }
  }

  /**
    * \brief Copy Constructor
    */
  ThreadPool(const ThreadPool&) = delete;

  /**
   * \brief Copy Assignment
   */
  ThreadPool& operator=(const ThreadPool&) = delete;

  /**
   * \brief Move Constructor
   */
  ThreadPool(ThreadPool&&) = delete;

  /**
   * \brief Move Assignment
   */
  ThreadPool& operator=(ThreadPool&&) = delete;

  /**
   * \brief Destructor.
   *
   * Join all the working threads.
   */
  virtual ~ThreadPool() {
    for (std::thread& thread : threads_) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  /**
   * \brief Submit a WorkUnit to the Thread Pool.
   *
   * \param args arguments used to instantiate a new workUnit.
   *
   * \return true if the workUnit has been submitted successfully.
   */
  template <typename... Args>
  bool SubmitWork(Args&&... args) {
    std::unique_lock<std::mutex> lock(work_queue_mutex_);
    if (!work_queue_.full()) {
      work_queue_.emplace_back(std::forward<Args>(args)...);
      work_queue_condvar_.notify_one();
      return true;
    } else {
      // the queue is already full : return false.
      return false;
    }
  }

  /**
   * \brief Signal all worker threads to exit.
   *
   * Worker threads will exit once they complete their current work unit.
   */
  inline void Stop() {
    running_ = false;
    std::unique_lock<std::mutex> lock(work_queue_mutex_);
    work_queue_condvar_.notify_all();
  }

  /**
   * \brief Check if the queue is full or not.
   *
   * \return true if the queue is full.
   *
   * The queue is full and no other work can be submitted.
   */
  inline bool IsQueueFull() {
    std::unique_lock<std::mutex> lock(work_queue_mutex_);
    return work_queue_.full();
  }

 private:
  /**
   * \brief Implementation of the Worker Thread.
   *
   * Calls WorkOne as long as running_ == true.
   */
  virtual void Worker() {
    while (running_) {
      WorkOne();
    }
  }

  /**
   * \brief Execution of a single work unit.
   *
   * Get a WorkUnit from the work_queue_ and calls WorkUnit#Run() on it.
   * If no work is available, blocks on work_queue_mutex_ and waits for a notification on work_queue_condvar_.
   */
  void WorkOne() {
    std::unique_lock<std::mutex> lock(work_queue_mutex_);
    while (running_ && work_queue_.empty()) {
      work_queue_condvar_.wait(lock);
    }
    if (running_) {
      W work_unit = work_queue_.front();
      work_queue_.pop_front();
      lock.unlock();

      // execute the task in front of the queue
      work_unit.Run();
    }
  }

  /**
   * \brief The worker threads.
   */
  std::vector<std::thread, vac::memory::LeakyArrayAllocator<std::thread>> threads_;

  /**
   * \brief Mutex to protect concurrent access to the work queue.
   */
  std::mutex work_queue_mutex_;

  /**
   * \brief Condition Variable to protect concurrent access to the work queue.
   */
  std::condition_variable work_queue_condvar_;

  /**
   * \brief The work queue.
   */
  WorkQueueType work_queue_;

  /**
   * \brief Flag to signal threads whether they should terminate.
   */
  std::atomic_bool running_;

  FRIEND_TEST(ThreadPoolTestFixture, Capacity);
  FRIEND_TEST(ThreadPoolTestFixture, NoInitialWork);
  FRIEND_TEST(ThreadPoolTestFixture, Lifecycle);
  FRIEND_TEST(ThreadPool, SubmitWork);
  FRIEND_TEST(ThreadPool, ExecuteWork3);
  FRIEND_TEST(ThreadPool, SubmitInFullQueue);
  FRIEND_TEST(ThreadPool, StdException);
  FRIEND_TEST(ThreadPool, OtherException);
};

} /* namespace threadpool */
} /* namespace vac*/

#endif  // LIB_INCLUDE_VAC_THREADPOOL_THREAD_POOL_H_
