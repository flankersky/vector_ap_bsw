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
 * \brief ErrorObserver which gets called when an ara::rest::Client has an error.
 */
void ErrorHandler(std::error_code error) {
  ara::log::Logger& log = ara::log::CreateLogger("2", "ErrorHandler Context of rest server sample application");
  log.LogError() << "An Client error occurred: " << error.value();
  if (error.value() == static_cast<int>(ara::rest::ErrorCode::kNetworkError)) {
    exit(-1);
  }
}

}  // namespace

/**
 * \brief main
 */
int main(int argc, char* argv[]) {
  appl::parse::CommandLineArguments args = appl::parse::ParseArguments(argc, argv);
  ara::rest::config::ConfigurationHandler::config_file = args.cfg_path_;

  // Initalize the logging framework
  ara::log::InitLogging("CLI", "Ara::rest client sample application", ara::log::LogLevel::kDebug,
                        ara::log::LogMode::kConsole, "");
  ara::log::Logger& log = ara::log::CreateLogger("1", "Main Context of rest server sample application");

#if defined(ENABLE_EXEC_MANAGER)
  // Instantiate the ApplicationClient
  ApplicationClient app_client;
#endif  // defined(ENABLE_EXEC_MANAGER)

  // Setup an ara::rest::Client instance
  ara::rest::Client client(kClientConfiguration);
  log.LogInfo() << "Installing Error Observer.";
  client.ObserveError(ErrorHandler);

  // The ara::rest::Uri used for the ara::rest::Requests.
  ara::rest::Uri uri_left_door =
      ara::rest::Uri::Builder(
          ara::rest::StringView("http://127.0.0.1:9090/left/door", strlen("http://127.0.0.1:9090/left/door")))
          .ToUri();
  ara::rest::Uri uri_left_status =
      ara::rest::Uri::Builder(
          ara::rest::StringView("http://127.0.0.1:9090/left/status", strlen("http://127.0.0.1:9090/left/door")))
          .ToUri();
  // The example code manages all ara::rest::Request, ara::rest::Task, and ara::rest::Reply instances in std::list
  // containers.
  std::list<ara::rest::Request> requests;
  std::list<ara::rest::Task<ara::rest::Pointer<ara::rest::Reply>>> f_replies;
  std::list<ara::rest::Pointer<ara::rest::Reply>> replies;
  // Create ara::rest::Request(s)
  requests.emplace_back(ara::rest::RequestMethod::kGet, uri_left_door);
  requests.emplace_back(ara::rest::RequestMethod::kPut, uri_left_status,
                        ara::rest::ogm::Object::Make(ara::rest::ogm::Field::Make(
                            "test", ara::rest::ogm::String::Make(ara::rest::StringView("unit", strlen("unit"))))));

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER)

  // Transmit the ara::rest::Request(s) and add the resulting ara::rest::Task(s) into a list.
  for (auto& req : requests) {
    log.LogInfo() << "Send request";
    f_replies.emplace_back(client.Send(req));
  }

  // Iterate over all ara::rest::Task(s) and check whether the ara::rest::Reply is ready to be consumed.
  for (auto& rep : f_replies) {
    // Note: An Reply handler function could be installed, e.g. with rep.then(ReplyHandlerFunction).

    if (ara::rest::FutureStatus::ready == rep.wait_for(std::chrono::milliseconds(100))) {
      // The ara::rest::Reply is ready to consume. Get it from the ara::rest::Task object and add it to a list.
      replies.emplace_back(rep.get());
      log.LogInfo() << "A reply is ready";
    }
    if (f_replies.size() == replies.size()) {
      // all replies are ready
      log.LogInfo() << "All replies are ready";
      break;
    }
  }

  // Consume the ara::rest::Reply instances.
  for (auto& p_rep : replies) {
    log.LogInfo() << "Printing the replies:"
                  << " Status: " << p_rep->GetHeader().GetStatus()
                  << " Uri: " << ara::rest::ToString(p_rep->GetHeader().GetUri());
  }

  // Stop the ara::rest::Client instance
  log.LogInfo() << "Stopping Client";

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif  // defined(ENABLE_EXEC_MANAGER)

  client.Stop();
  log.LogInfo() << "Stopped Client";
  return 0;
}
