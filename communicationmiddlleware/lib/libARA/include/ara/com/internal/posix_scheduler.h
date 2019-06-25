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
/**        \file  posix_scheduler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_POSIX_SCHEDULER_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_POSIX_SCHEDULER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sched.h>
#include <cstdint>
#include <thread>

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Available scheduling policies for one thread-pool and its threads.
 * Currently there are two real-time priorities possible.
 */
enum class RtSchedPolicy : int { kFifo = SCHED_FIFO, kRoundRobin = SCHED_RR };

/**
 * \brief Scheduler priority for the worker threads of a thread-pool.
 */
using SchedPriority = int;

/**
 * \brief Models POSIX scheduling constraints in a class.
 * Acts as a helper class to apply scheduler settings (policy and priority) for a given process / thread.
 */
class PosixThreadScheduler {
 public:
  /**
   * \brief Default constructor sets the valid flag to false.
   */
  PosixThreadScheduler();

  /**
   * \brief Constructor initializes the policy and priority in use for a given process / thread.
   * \param policy The scheduler policy (SCHED_RR, SCHED_FIFO)
   * \param priority The scheduler priority to apply (1 to 98)
   */
  PosixThreadScheduler(RtSchedPolicy policy, SchedPriority priority) : policy_{policy}, priority_{priority} {}

  /**
   * \brief Applies the scheduler configuration for the current thread within this method is called.
   *
   * \note Refer to
   * - http://en.cppreference.com/w/cpp/thread/thread/native_handle
   * - http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/thread/thread/native_handle.html
   * \param posix_handle Native handle of this thread.
   * \return true if the scheduler settings could be applied, false if there's an error..
   */
  bool ApplySettings(const pthread_t posix_handle);

 private:
  /**
   * \brief POSIX scheduling policy for a specific process / thread.
   */
  RtSchedPolicy policy_{};

  /**
   * \brief POSIX scheduling priority for a specific process / thread.
   */
  SchedPriority priority_{};
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_POSIX_SCHEDULER_H_
