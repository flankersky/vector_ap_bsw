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
/**        \file  diag_test_mediator.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DIAG_MEDIATOR_APPLICATION_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DIAG_MEDIATOR_APPLICATION_H_

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
#include "data/did_service/did_operation_factory_impl.h"
#include "data/routine_service/routine_operation_factory_impl.h"
#include "service_interfaces/dm_ipc.h"

namespace diag_test_mediator {

class DiagMediatorApplication final {
 public:
  DiagMediatorApplication();
  ~DiagMediatorApplication();

  DiagMediatorApplication(const DiagMediatorApplication&) = delete;
  DiagMediatorApplication(DiagMediatorApplication&&) = delete;
  const DiagMediatorApplication& operator=(const DiagMediatorApplication&) = delete;
  const DiagMediatorApplication& operator=(DiagMediatorApplication&&) = delete;

  /**
   * \brief Initialization of ara::com interface and activation of the IPC service.
   * \param args command line arguments.
   */
  void Initialize(CommandLineArguments args);

  /**
   * \brief Lifecycle Methods for Run Mode.
   */
  void Run();

  /**
   * \brief Shutdown stops the before with Initialize published IPC service.
   * \remarks Must not be called before or without calling Initialize.
   * \throws runtime_error when ara::com runtime was not correctly initialized (by Initialize)
   */
  void Shutdown();

 private:
  /**
   * \brief Entry point of the thread receiving signals from the execution manager
   */
  void SignalHandlerThread();

  /**
   * Joins all threads owned by this instance
   */
  void JoinAllThreads();

 private:
  /**
   * \brief Number of Threads used by Mediator.
   * The Mediator Application currently uses only one additional thread for the signal handler.
   * (The thread is initialized by DiagTestMediator::Initialize;
   * the signal handler thread itself is DiagTestMediator::SignalHandlerThread.)
   */
  static constexpr uint_least8_t kNumOfThreads = 1;

  /**
   * \brief Flag to identify whether the application was requested to terminate, i.e., has received a SIGTERM
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief Vector to store all threads spawned by this application.
   */
  std::array<std::thread, kNumOfThreads> threads_;

  /**
   * The factory that is used to create read/write DID polling tasks
   */
  data::did_service::DidOperationFactoryImpl did_op_factory_;

  /**
   * The factory that is used to create routine polling tasks
   */
  data::routine_service::RoutineOperationFactoryImpl rid_op_factory_;

  /**
   * \brief Mediator skeleton implementation
   */
  service_interfaces::DM_IPC mediator_interface_impl_;
};

}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DIAG_MEDIATOR_APPLICATION_H_
