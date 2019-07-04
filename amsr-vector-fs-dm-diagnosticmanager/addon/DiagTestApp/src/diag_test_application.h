/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  diag_test_application.h
 *        \brief  Main class representing the application.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_DIAG_TEST_APPLICATION_H_
#define ADDON_DIAGTESTAPP_SRC_DIAG_TEST_APPLICATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <signal.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <vector>

#include "arguments_parser.h"
#include "service_interfaces/data_identifier_4711.h"
#include "service_interfaces/data_identifier_F190.h"
#include "service_interfaces/generic_uds_service.h"
#include "service_interfaces/routine_control_3009.h"

namespace diag_test_application {

constexpr uint_least8_t kNumOfThreads = 1; /**< Number of Threads used in DiagTestApplication. */

/**
 * \brief Main class representing the application
 */
class DiagTestApplication final {
 public:
  DiagTestApplication();
  ~DiagTestApplication() = default;

  DiagTestApplication(const DiagTestApplication&) = delete;
  DiagTestApplication(const DiagTestApplication&&) = delete;
  DiagTestApplication operator=(const DiagTestApplication&) = delete;
  DiagTestApplication operator=(const DiagTestApplication&&) = delete;

  /**
   * \brief Lifecycle Method for Initialization.
   * \param args command line arguments.
   */
  void Initialize(CommandLineArguments args);

  /**
   * \brief Lifecycle Methods for Run Mode.
   */
  void Run();

  /**
   * \brief Lifecycle Methods for Shutdown.
   */
  void Shutdown();

 private:
  /**
   * \brief Entry point of the thread receiving signals from the execution manager
   */
  void SignalHandlerThread();

  /**
   * \brief Flag to identify whether the application was requested to terminate, i.e., has received a SIGTERM
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief Vector to store all threads spawned by this application.
   */
  std::vector<std::thread> threads_;

  /**
   * \brief GenericUDSService skeleton instantiation
   */
  service_interfaces::GenericUDSService generic_uds_service_;

  /**
   * \brief DataElement4711 skeleton instantiation
   */
  service_interfaces::DataIdentifier4711 data_element_4711_;

  /**
   * \brief DataElementF190 skeleton instantiation
   */
  service_interfaces::DataIdentifierF190 data_element_F190_;

  /**
   * \brief RountingControl3009 skeleton instantiation
   */
  service_interfaces::RoutineControl3009 routine_control_3009_;
};

}  // namespace diag_test_application

#endif  // ADDON_DIAGTESTAPP_SRC_DIAG_TEST_APPLICATION_H_
