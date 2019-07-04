/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set
 * out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  client/main.cc
 *        \brief  A minimal usage example of ara::rest
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <ara/rest/client.h>
#include <ara/rest/config.h>
#include <ara/rest/error_code.h>
#include <ara/rest/ogm/object.h>
#include <ara/rest/routing.h>
#include <ara/rest/serialize/serialize.h>
#include <ara/rest/uri.h>
#include <getopt.h>
#include <ara/log/logging.hpp>
#include <chrono>
#include <iostream>
#include <list>

#include "opt_parser.h"

#if defined(ENABLE_EXEC_MANAGER)
#include <ara/exec/application_client.hpp>
using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;
#endif  // defined(ENABLE_EXEC_MANAGER)

namespace {

/**
 * \brief ClientConfiguration
 */
const char* kClientConfiguration{"my_client_conf_id"};

/**
 * \brief ErrorObserver which gets called when an ara::rest::Client has an
 * error.
 */
void ErrorHandler(std::error_code error) {
  ara::log::Logger& log = ara::log::CreateLogger("2", "ErrorHandler Context of rest server sample application");
  log.LogError() << "An Client error occurred (Code " << error.value() << "): " << error.message();
  if (error.value() == static_cast<int>(ara::rest::ErrorCode::kNetworkError)) {
    exit(-1);
  }
}

}  // namespace

/**
 * \brief notification of new data or error messages arrive here
 * \param obj
 */
void notifyMe(const ara::rest::ogm::Object& obj) {
  (void)obj;
  static int counter = 0;

  if (counter >= 20) {
    counter = 0;
  }
  ara::log::Logger& log = ara::log::CreateLogger("notify1", "Main Context of rest event client sample application");

  log.LogDebug() << "Client1:Notifier1: I got notified about new data or an error " << counter;
  counter++;
}

/**
 * \brief notification of new data or error messages arrive here
 * \param obj
 */
void notifyMe2(const ara::rest::ogm::Object& obj) {
  (void)obj;
  static int counter = 0;
  ara::log::Logger& log = ara::log::CreateLogger("notify2", "Main Context of rest event client sample application");

  log.LogDebug() << "Client1:Notifier2: I got notified about new data or an error " << counter;
  counter++;
}
/**
 * \brief notification of new data or error messages arrive here
 * \param obj
 */
void notifyMe3(const ara::rest::ogm::Object& obj) {
  (void)obj;
  static int counter = 0;
  if (counter >= 10) {
    counter = 0;
  }
  ara::log::Logger& log = ara::log::CreateLogger("notify3", "Main Context of rest event client sample application");

  log.LogDebug() << "Client2:Notifier3: I got notified about new data or an error " << counter;
  counter++;
}

/**
 * \brief notification of new data or error messages arrive here
 * \param obj
 */
void notifyMe4(const ara::rest::ogm::Object& obj) {
  (void)obj;
  static int counter = 0;
  ara::log::Logger& log = ara::log::CreateLogger("notify4", "Main Context of rest event client sample application");

  log.LogDebug() << "Client2:Notifier4: I got notified about new data or an error " << counter;
  counter++;
}
/**
 * \brief gets notified if the state of the event is becoming be changed
 * \param state
 * \param event
 */
void state(const ara::rest::Event& event, ara::rest::SubscriptionState state) {
  ara::log::Logger& log = ara::log::CreateLogger("state", "Main Context of rest event client sample application");

  if (state == ara::rest::SubscriptionState::kSubscribed) {
    log.LogDebug() << "I got subscribed to the event " << ara::rest::ToString(event.GetUri()) << "!!!";
  } else if (state == ara::rest::SubscriptionState::kCanceled) {
    log.LogDebug() << "I got unsubscribed to the event " << ara::rest::ToString(event.GetUri()) << "!!!";
  } else {
    log.LogDebug() << "I got notified about an invalid state of the event " << ara::rest::ToString(event.GetUri())
                   << "!!!";
  }
}

static void ClientThread1() {
  // create a client
  ara::rest::Client client("my_client_conf_id");
  // subscribe a event
  ara::rest::Task<ara::rest::Event> send_client_task =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/left/*",
                                                                     strlen("http://127.0.0.1:9090/left/*")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(1000), notifyMe, state);

  ara::rest::Event event = std::move(send_client_task.get());

  // subscribe an other event
  std::this_thread::sleep_for(std::chrono::seconds(20));
  ara::rest::Task<ara::rest::Event> send_client_task2 =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/right/door",
                                                                     strlen("http://127.0.0.1:9090/right/door")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(100), notifyMe2, state);

  ara::rest::Event event2 = std::move(send_client_task2.get());
  // wait for a while before unsubscribing the first event
  std::this_thread::sleep_for(std::chrono::seconds(20));
  event.Unsubscribe().get();
  // wait for a while to receive messages for the subscribed event
  // before subscribe an other event
  std::this_thread::sleep_for(std::chrono::seconds(30));
  // subscribe an other event
  ara::rest::Task<ara::rest::Event> send_client_task3 =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/left/test",
                                                                     strlen("http://127.0.0.1:9090/left/test")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(1000), notifyMe, state);

  ara::rest::Event event3 = std::move(send_client_task3.get());
  // wait for a while to receive messages for the subscribed event
  // before stopping the client
  std::this_thread::sleep_for(std::chrono::seconds(300));
  // stop the client
  client.Stop();
}

static void ClientThread2() {
  // create client
  ara::rest::Client client("my_client_conf_id");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // subscribe a event
  ara::rest::Task<ara::rest::Event> send_client_task =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/left/*",
                                                                     strlen("http://127.0.0.1:9090/left/*")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(1000), notifyMe3, state);

  ara::rest::Event event = std::move(send_client_task.get());

  // subscribe an other event
  ara::rest::Task<ara::rest::Event> send_client_task2 =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/right/door",
                                                                     strlen("http://127.0.0.1:9090/right/door")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(100), notifyMe4, state);

  ara::rest::Event event2 = std::move(send_client_task2.get());
  // wait for a while before unsubscribing the first event
  std::this_thread::sleep_for(std::chrono::seconds(20));
  event.Unsubscribe().get();
  // wait for a while to receive messages for the subscribed event
  // before subscribe an other event
  std::this_thread::sleep_for(std::chrono::seconds(50));
  // subscribe an other event
  ara::rest::Task<ara::rest::Event> send_client_task3 =
      client.Subscribe(ara::rest::Uri::Builder(ara::rest::StringView("http://127.0.0.1:9090/right/test",
                                                                     strlen("http://127.0.0.1:9090/right/test")))
                           .ToUri(),
                       ara::rest::EventPolicy::kPeriodic, static_cast<ara::rest::duration_t>(1000), notifyMe3, state);

  ara::rest::Event event3 = std::move(send_client_task3.get());
  // wait for a while to receive messages for the subscribed event
  // before stopping the client
  std::this_thread::sleep_for(std::chrono::seconds(300));
  // stop the client
  client.Stop();
}

/**
 * \brief main
 */
int main(int argc, char* argv[]) {
  appl::parse::CommandLineArguments args = appl::parse::ParseArguments(argc, argv);
  ara::rest::config::ConfigurationHandler::config_file = args.cfg_path_;
  // Initialize the logging framework

  ara::log::Logger& log = ara::log::CreateLogger("1", "Main Context of rest event client sample application");
  ara::log::InitLogging("CLI", "Ara::rest event client sample application", ara::log::LogLevel::kDebug,
                        ara::log::LogMode::kConsole, "");

#if defined(ENABLE_EXEC_MANAGER)
  // Instantiate the ApplicationClient
  ApplicationClient app_client;
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER)

  // starting 2 clients in 2 separate threads
  std::thread client1 = std::thread(ClientThread1);
  std::thread client2 = std::thread(ClientThread2);
  // wait until both threads finished there job
  client1.join();
  client2.join();

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif  // defined(ENABLE_EXEC_MANAGER)

  return 0;
}
