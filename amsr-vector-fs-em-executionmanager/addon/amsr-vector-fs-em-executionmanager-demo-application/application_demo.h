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
/**        \file  application_demo.cc
 *        \brief  A demo adaptive application to the execution management.
 *
 *      \details  Handles signals, reports its application states.
 *
 *********************************************************************************************************************/

#ifndef ADDON_AMSR_VECTOR_FS_EM_EXECUTIONMANAGER_DEMO_APPLICATION_APPLICATION_DEMO_H_
#define ADDON_AMSR_VECTOR_FS_EM_EXECUTIONMANAGER_DEMO_APPLICATION_APPLICATION_DEMO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <signal.h>

#include <ara/exec/application_client.hpp>
#include <ara/exec/state_client.hpp>
#include <ara/log/logging.hpp>
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace application_demo {

constexpr uint_least8_t kNumOfThreads = 2; /**< Number of Threads used in AdaptiveApplication. */

/**
 * \brief Main class representing the application
 */
class AdaptiveApplication {
 public:
  AdaptiveApplication();
  /**
   * \brief Constructor of the Adaptive Application
   *
   * \param is_state_client Inidicator if the application should work as machine state client or not
   *
   * \trace SWS_EM_01056
   */
  explicit AdaptiveApplication(bool is_state_client);
  virtual ~AdaptiveApplication() = default;

  /**
   * \brief Starts the application.
   */
  void Start();

 protected:
  /**
   * \brief Data Type containing the state to be saved across iterations of a cyclic task.
   */
  typedef struct { int my_data; /**< data */ } PeriodicWorkStateType;

  /**
   * \brief Lifecycle Method for Initialization.
   *
   * \trace SWS_EM_02031
   */
  void Initialize();

  /**
   * \brief Lifecycle Methods for Run Mode.
   *
   * \trace SWS_EM_02031
   */
  void Run();

  /**
   * \brief Lifecycle Methods for Shutdown.
   *
   * \trace SWS_EM_02031
   */
  void Shutdown();

 private:
  /**
   * \brief Member to report the application state to the Execution Management.
   */
  ara::exec::ApplicationClient application_client_;

  /**
   * \brief Member to report the machine state to the Execution Management.
   */
  ara::exec::StateClient state_client_;

  /**
   * \brief Flag to identify whether the application was requested to terminate, i.e., has received a SIGTERM
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief Vector to store all threads spawned by this application.
   */
  std::vector<std::thread> threads_;

  /**
   * \brief Inidicator if the application should work as machine state client or not
   */
  bool is_state_client_;

  /**
   * \brief Entry point of the thread receiving signals from the execution manager
   */
  void SignalHandlerThread();

  /**
   * \brief Entry point of a thread that executed periodic work.
   */
  void PeriodicThread();

  /**
   * \brief Actual workload to be executed periodically. The parameter contains the state to be preserved across
   * iterations.
   */
  void PeriodicWork(PeriodicWorkStateType& state);
};

}  // namespace application_demo

#endif  // ADDON_AMSR_VECTOR_FS_EM_EXECUTIONMANAGER_DEMO_APPLICATION_APPLICATION_DEMO_H_
