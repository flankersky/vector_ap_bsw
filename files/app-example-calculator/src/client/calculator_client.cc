/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  calculator_client.cc
 *        \brief  Main entry point of the calculatorClient, used for testing the calulator service.
 *
 *      \details  -
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/com/future.h>
#include <ara/com/runtime.h>
#include <ara/com/types.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <ara/exec/application_client.hpp>
#include <ara/log/logging.hpp>
#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include "vector/calculatorService/calculatorInterface.h"
#include "vector/calculatorService/calculatorInterface_proxy.h"

#ifdef PERSISTENCY_ON
#include <ara/per/fileproxyaccessorfactory.h>
#include <ara/per/keyvaluestorage.h>
#include <ara/per/perexceptions.h>
#include <vac/memory/optional.h>
#include <vac/memory/three_phase_allocator.h>
static vac::memory::optional<ara::per::KeyValueStorage> datastorage;
static vac::memory::optional<ara::per::FileProxyAccessUniquePtr<ara::per::WriteAccessor>> raw_file;
static vac::memory::optional<ara::per::FileProxyFactoryUnqiuePtr> fpafi;
static const char* kPathToVar = "../var";
#endif

using namespace ara::log;  // NOLINT 'using' for ara::log should be OK.

using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;

/* Forward declarations */
void FindServiceHandler(ara::com::ServiceHandleContainer<ara::com::HandleType> calculator_services);
void DivisionByZeroHandler(void);
void DivideResultHandler(void);
void Initialize(void);
void SignalHandler(void);
void StoreData(uint32_t data_to_store, std::string key_to_store);

ara::com::FindServiceHandle find_service_handle;
std::shared_ptr<vector::calculatorService::CalculatorInterfaceProxy> calculator_service_proxy;
std::atomic<bool> calculator_server_found(false);
/**
 * \brief Flag to identify whether the application was requested to terminate, i.e., has received a SIGTERM
 */
std::atomic_bool exit_requested_(false);

/**
 * \brief Vector to store all threads spawned by this application.
 */
std::vector<std::thread> threads_;

/**
 * \brief Prints the usage message.
 *
 * \param argc argument count
 * \param argv command line arguments
 */
static void Usage(const int argc, char* argv[]) {
  if (argc > 0) {
    std::cerr << "usage: " << argv[0] <<
        R"([-h] -c <config file path>
           -h                            Print this message and exit.
           -c <config file path>         Specify the location of the configuration file.
        )";
  } else {
    std::cerr << "usage: calculatorClient "
              <<
        R"([-h] -c <config file path>
           -h                            Print this message and exit.
           -c <config file path>         Specify the location of the configuration file.
        )";
  }
}

/**
 * \brief Parses command line arguments.
 *
 * \param argc Command line argument count.
 * \param argv Array of pointers to command line arguments.
 * \return Parsed arguments.
 */
static std::string ParseArguments(int argc, char* argv[]) {
  int c;
  while ((c = getopt(argc, argv, "hec:")) != -1) {
    switch (c) {
      case 'h':
        Usage(argc, argv);
        exit(EXIT_SUCCESS);
        break;
      case 'c':
        return std::string(optarg);
      default:
        Usage(argc, argv);
        exit(EXIT_FAILURE);
        break;
    }
  }
  Usage(argc, argv);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  InitLogging("CCLI", "Adaptive Calculator Client", LogLevel::kVerbose, LogMode::kConsole, "");
  LogInfo() << "Starting calculatorClient";

  // Instantiate the ApplicationClient
  ApplicationClient app_client;

  // Loading config file and initialize runtime
  LogInfo() << "Initializing runtime...";
  ara::com::Runtime::getInstance().Initialize(ParseArguments(argc, argv));
  Initialize();
  // Wait until calculatorSerice is found
  while (!calculator_server_found.load(std::memory_order_seq_cst) && !exit_requested_) {
  }
  // Stop searching for further services
  vector::calculatorService::CalculatorInterfaceProxy::StopFindService(find_service_handle);

  // Subscribe to events and set handlers
  calculator_service_proxy->divisionByZero.SetReceiveHandler([]() { DivisionByZeroHandler(); });
  calculator_service_proxy->divisionByZero.Subscribe(ara::com::EventCacheUpdatePolicy::kLastN, 1);

  // Subscribe to divideResult field (notify event)
  calculator_service_proxy->divideResult.SetReceiveHandler([]() { DivideResultHandler(); });
  calculator_service_proxy->divideResult.Subscribe(ara::com::EventCacheUpdatePolicy::kLastN, 1);

  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);

  Logger& logger_ctx_calc{CreateLogger("CXC1", "Context for calculator method invocation")};

  // Calling methods of the calculator service asynchronous
  vector::calculatorService::uint32 operand1 = 2;
  vector::calculatorService::uint32 operand2 = 4;
  ara::com::Future<vector::calculatorService::proxy::methods::Add::Output> add_future =
      calculator_service_proxy->add(operand1, operand2);
  auto divide_future = calculator_service_proxy->divide(operand1, operand2);
  operand2 = 0;
  auto divide_future2 = calculator_service_proxy->divide(operand1, operand2);
  operand1 = 16;
  operand2 = 2;
  auto divide_future3 = calculator_service_proxy->divide(operand1, operand2);

  // Print out results
  uint32_t result_add = add_future.get().result;
  logger_ctx_calc.LogInfo() << "Result add: " << result_add;
  uint32_t result_divide = divide_future.get().result;
  logger_ctx_calc.LogInfo() << "Result divide: " << result_divide;
  uint32_t result_divide2 = divide_future2.get().result;
  logger_ctx_calc.LogInfo() << "Result divide_2: " << result_divide2;
  logger_ctx_calc.LogInfo() << "Result from divideResult field: "
                            << **(calculator_service_proxy->divideResult.GetCachedSamples().begin());
  uint32_t result_divide3 = divide_future3.get().result;
  logger_ctx_calc.LogInfo() << "Result divide_3: " << result_divide3;
  logger_ctx_calc.LogInfo() << "Result from divideResult field: "
                            << **(calculator_service_proxy->divideResult.GetCachedSamples().begin());
  operand1 = 8;
  operand2 = 4;
  auto subtract_future = calculator_service_proxy->subtract(operand1, operand2);
  logger_ctx_calc.LogInfo() << "Result subtract: " << subtract_future.get().result;

  // Store the data
  StoreData(result_add, "Result-add");
  StoreData(result_divide, "Result-divide");
  StoreData(result_divide2, "Result-divide2");
  StoreData(result_divide3, "Result-divide3");

  while (!exit_requested_) {
    // Do some work here
    LogInfo() << "Nothing to do";
    // Yield the CPU to another thread
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  // Unsubscribe from divisionByZero event
  calculator_service_proxy->divisionByZero.Unsubscribe();
  // Unsubscribe from divideResult field notify event
  calculator_service_proxy->divideResult.Unsubscribe();
  calculator_service_proxy.reset();

  LogInfo() << "Finished client operation";
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);

  LogInfo() << "Terminating";

  return 0;
}

/**
 * \brief Example usage of ara::per
 */
void StoreData(uint32_t data_to_store, std::string key_to_store) {
#ifdef PERSISTENCY_ON
  try {
    // open storage location
    std::string path_to_KVS_file(kPathToVar);
    if (!datastorage.has_value()) {
      datastorage.emplace(path_to_KVS_file);
    }
    // Stores the value to the database
    ara::per::KvsType entry(data_to_store);
    entry.SetKey(key_to_store);
    datastorage->SetValue(key_to_store, entry);

    datastorage->SyncToStorage();

    // read the data back
    ara::per::KvsType data = datastorage->GetValue(key_to_store);

    if (ara::per::KvsType::Status::kSuccess == data.GetStatus()) {
      LogInfo() << "calculatorClient: #INFO: Data is ready to read.";
      // We expect the data is an unsigned integer
      uint32_t restored_value = data.GetUInt();
      LogInfo() << "calculatorClient: #INFO: The read value from: " << data.GetKey() << " is: " << restored_value;

    } else {
      LogInfo() << "Data NOT ready";
    }
  } catch (const ara::per::ExceptionLogicError& e) {
    LogError() << "Persistency exception caught! Message = " << e.what();
  } catch (const ara::per::ExceptionStorageLocationNotFound& e) {
    LogError() << "Persistency exception caught! Message = " << e.what();
  } catch (const ara::per::ExceptionPhysicalStorageError& e) {
    LogError() << "Persistency exception caught! Message = " << e.what();
  } catch (const std::exception& e) {
    LogError() << "Persistency caught std::exception! Message = " << e.what();
  } catch (...) {
    LogError() << "Caught unknown exception!";
  }

  // Access to raw file
  std::string path_to_raw_file(kPathToVar);
  if (!fpafi.has_value()) {
    fpafi.emplace(ara::per::CreateFileAccessorFactory(path_to_raw_file));
  }
  if (!raw_file.has_value()) {
    raw_file.emplace(
        fpafi->operator->()->CreateWriteAccess("calculatorRawData", ara::per::BasicOperations::OpenMode::out));
  }
  if (raw_file->operator->()->good()) {
    LogInfo() << "Opened file: " << path_to_raw_file;
    // Write data to file
    LogInfo() << "Writing data to file";
    (**raw_file) << "calculator data: " << std::to_string(data_to_store) << "\n"
                 << "some other data: 123"
                 << "\n";

    raw_file->operator->()->flush();
    LogInfo() << "Closed file: " << path_to_raw_file;
  } else {
    LogError() << "Cannot open file: " << path_to_raw_file;
  }
#endif
}

/**
  * \brief  Function to initialize the calculator client
  */
void Initialize(void) {
  /* Block all signals for this thread. Signal mask will be inherited by subsequent threads. */
  sigset_t signals;
  sigfillset(&signals);
  pthread_sigmask(SIG_SETMASK, &signals, NULL);
  /* spawn a new signal handler thread*/
  threads_.emplace_back(SignalHandler);

  // Search for calulatorService. When a calculatorService instance is found, the function FindServiceHandler gets
  // called
  LogInfo() << "Searching for calculatorService";
  find_service_handle = vector::calculatorService::CalculatorInterfaceProxy::StartFindService(FindServiceHandler);
}

/**
  * \brief Signal handler function for SIGTERM
  */
void SignalHandler(void) {
  sigset_t signal_set;
  int sig = -1;
  sigemptyset(&signal_set);        /* Empty the set of signals */
  sigaddset(&signal_set, SIGTERM); /* Add only SIGTERM to set */
  LogInfo() << "SignalHandler started";
  while (sig != SIGTERM) {
    sigwait(&signal_set, &sig);
    LogInfo() << "Received signal number:" << sig;
  }
  LogInfo() << "Received signal SIGTERM";
  exit_requested_ = true;
}
/**
  * \brief  Function gets called when a calculatorService instance is found
  * \param  calculator_services Contains all found calculatorService instances
  */
void FindServiceHandler(ara::com::ServiceHandleContainer<ara::com::HandleType> calculator_services) {
  if (calculator_services.size() == 0) {
    LogInfo() << "No calculatorService instance found";
    return;
  } else if (calculator_services.size() == 1) {
    LogInfo() << "Found one calculatorService instance";
    // Get proxy instance
    calculator_service_proxy =
        std::make_shared<vector::calculatorService::CalculatorInterfaceProxy>(calculator_services[0]);
    calculator_server_found.store(true);
  } else {
    // The example doesn't handle this case
    LogInfo() << "Found multiple instances of calculatorService";
  }
}

/**
  * \brief  Function gets called when a divisionByZero event occurs
  */
void DivisionByZeroHandler(void) { LogError() << "Division by zero!"; }

/**
  * \brief  Function gets called when a divideResult notify event occurs
  *         (everytime the server instance writes to the divideResult field).
  */
void DivideResultHandler(void) {
  // Check if new events are placed into the local cache
  if (calculator_service_proxy->divideResult.Update()) {
    LogInfo() << "Last valid division result is: "
              << **(calculator_service_proxy->divideResult.GetCachedSamples().begin());
  }
}
