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
/**        \file  server/main.cc
 *        \brief  A minimal usage example of ara::rest
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <ara/rest/config.h>
#include <ara/rest/ogm/copy.h>
#include <ara/rest/ogm/object.h>
#include <ara/rest/ogm/value.h>
#include <ara/rest/routing.h>
#include <ara/rest/routing/router.h>
#include <ara/rest/serialize/serialize.h>
#include <ara/rest/server.h>
#include <ara/rest/server_types.h>
#include <ara/rest/uri.h>
#include <getopt.h>
#include <chrono>
#include <iostream>

#include "opt_parser.h"

#if defined(ENABLE_EXEC_MANAGER)
#include <ara/exec/application_client.hpp>
using ApplicationClient = ara::exec::ApplicationClient;
using ApplicationState = ara::exec::ApplicationState;
#endif  // defined(ENABLE_EXEC_MANAGER)

namespace {
/**
 * \brief The Router which maps the incoming ara::rest::Requests to the
 * corresponding ara::rest::Route handler
 * functions.
 */
ara::rest::Router router;

/**
 * \brief The Server uptime duration in seconds.
 */
const unsigned int kServerUptimeDuration = 999;

}  // namespace

/**
 * \brief main
 */
int main(int argc, char* argv[]) {
  appl::parse::CommandLineArguments args = appl::parse::ParseArguments(argc, argv);
  ara::rest::config::ConfigurationHandler::config_file = args.cfg_path_;

  // Initalize the logging framework
  ara::log::InitLogging("SRV", "Ara::rest server sample application", ara::log::LogLevel::kDebug,
                        ara::log::LogMode::kConsole, "");
  ara::log::Logger& log = ara::log::CreateLogger("1", "Main Context of rest server sample application");

#if defined(ENABLE_EXEC_MANAGER)
  // Instantiate the ApplicationClient
  ApplicationClient app_client;
#endif  // defined(ENABLE_EXEC_MANAGER)

  // create the server
  ara::rest::Server server("my_server_conf_id", [](const ara::rest::ServerRequest& req, ara::rest::ServerReply& rep) {
    ara::rest::String str_uri(ara::rest::ToString(req.GetHeader().GetUri()));
    ara::rest::Pointer<ara::rest::ogm::Object> objNode = ara::rest::ogm::Object::Make(
        ara::rest::ogm::Field::Make("type",
                                    ara::rest::ogm::String::Make(ara::rest::StringView("data", strlen("data")))),
        ara::rest::ogm::Field::Make(
            "event", ara::rest::ogm::String::Make(ara::rest::StringView(str_uri.c_str(), str_uri.size()))),
        ara::rest::ogm::Field::Make("data",
                                    ara::rest::ogm::String::Make(ara::rest::StringView("Test1", strlen("Test1")))),
        ara::rest::ogm::Field::Make("timestamp",
                                    ara::rest::ogm::String::Make(ara::rest::StringView("1000", strlen("1000")))));
    rep.Send(objNode);
  });
  // start the server
  server.Start();

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER)

  // define what happens with the subscription requests
  server.ObserveSubscriptions(
      [](ara::rest::ServerEvent server_event) {
        static int counter = 0;
        // in this case the first seven requests will be confirmed
        if (counter == 0) {
          static ara::rest::ServerEvent event1 = ara::rest::ServerEvent(std::move(server_event));
          event1.ConfirmSubscription().get();
        } else if (counter == 1) {
          static ara::rest::ServerEvent event2 = ara::rest::ServerEvent(std::move(server_event));
          event2.ConfirmSubscription().get();
        } else if (counter == 2) {
          static ara::rest::ServerEvent event3 = ara::rest::ServerEvent(std::move(server_event));
          event3.ConfirmSubscription().get();
        } else if (counter == 3) {
          static ara::rest::ServerEvent event4 = ara::rest::ServerEvent(std::move(server_event));
          event4.ConfirmSubscription().get();
        } else if (counter == 4) {
          static ara::rest::ServerEvent event5 = ara::rest::ServerEvent(std::move(server_event));
          event5.ConfirmSubscription().get();
        } else if (counter == 5) {
          static ara::rest::ServerEvent event6 = ara::rest::ServerEvent(std::move(server_event));
          event6.ConfirmSubscription().get();
        } else if (counter == 6) {
          static ara::rest::ServerEvent event7 = ara::rest::ServerEvent(std::move(server_event));
          event7.ConfirmSubscription().get();
        }
        counter++;
      },
      [](ara::rest::ServerEvent& server_event, ara::rest::SubscriptionState subscription_state) {
        // define what happens if the subscriptionstate changes
        if (subscription_state != ara::rest::SubscriptionState::kSubscribed) {
          server_event.ConfirmUnsubscription().get();
        } else {
          server_event.ConfirmResubscription().get();
        }
      });

  // Wait for incoming requests, the ara::rest::Server instance runs
  // asynchronously.
  std::this_thread::sleep_for(std::chrono::seconds(kServerUptimeDuration));

  // Shut the ara::rest::Server instance down.
  log.LogInfo() << "Stopping Server";

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kTerminating);
#endif  // defined(ENABLE_EXEC_MANAGER)

  server.Stop();
  log.LogInfo() << "Stopped Server";
  return 0;
}
