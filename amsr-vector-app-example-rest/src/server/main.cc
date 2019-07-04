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
#include <vac/language/cpp14_backport.h>
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
 * \brief The Router which maps the incoming ara::rest::Requests to the corresponding ara::rest::Route handler
 * functions.
 */
ara::rest::Router router;

/**
 * \brief The Server uptime duration in seconds.
 */
const unsigned int kServerUptimeDuration = 999;

}  // namespace

/**
 * \brief An ara::rest request handler on application level.
 */
ara::rest::Route::Upshot LeftDoorPutHandler(const ara::rest::ServerRequest& req, ara::rest::ServerReply& rep,
                                            const ara::rest::Matches& matches) {
  ara::log::Logger& log = ara::log::CreateLogger("6", "LeftStatusGetHandler Context of rest server sample application");
  log.LogInfo() << "LeftDoorPutHandler called by: " << ara::rest::ToString(req.GetHeader().GetUri());
  (void)matches;
  // Trigger asynchronous GetObject call
  // blocks till the ara::rest::ogm::Object is ready to be processed
  ara::rest::Future<ara::rest::ogm::Object const&> f_data_received = req.GetObject();
  // This is a Put handler function to store the received data somewhere.
  auto& data_received(f_data_received.get());

  return ara::rest::Route::Upshot::Accept;
}

/**
 * \brief An ara::rest request handler on application level.
 */
ara::rest::Route::Upshot LeftStatusGetHandler(const ara::rest::ServerRequest& req, ara::rest::ServerReply& rep,
                                              const ara::rest::Matches& matches) {
  (void)req;
  (void)rep;
  (void)matches;
  ara::log::Logger& log = ara::log::CreateLogger("6", "LeftStatusGetHandler Context of rest server sample application");
  log.LogInfo() << "LeftStatusGetHandler called by: " << ara::rest::ToString(req.GetHeader().GetUri());

  // Do application specific things here.
  // e.g. create an ara::rest::ogm::Object with the Field Status and the Value Closed to transmit
  auto p_obj = ara::rest::ogm::Object::Make(ara::rest::ogm::Field::Make(
      "Status", ara::rest::ogm::String::Make(ara::rest::StringView("Closed", strlen("Closed")))));

  // Trigger the asynchronous transmission of the ara::rest::ServerReply
  ara::rest::Task<void> send_task = rep.Send(std::move(p_obj));

  return ara::rest::Route::Upshot::Accept;
}

/**
 * \brief Default handler function which gets called by the ara::rest::Router in case of no matching route.
 */
void DefaultHandler(const ara::rest::ServerRequest& req, ara::rest::ServerReply& rep) {
  ara::log::Logger& log = ara::log::CreateLogger("5", "DefaultHandler Context of rest server sample application");
  log.LogInfo() << "DefaultHandler called by: " << ara::rest::ToString(req.GetHeader().GetUri());

  rep.SetHeader(std::move(vac::language::make_unique<ara::rest::ReplyHeader>(200, req.GetHeader().GetUri())));

  // Do application specific things here.
  // e.g. create an ara::rest::ogm::Object.
  auto p_obj = ara::rest::ogm::Object::Make(ara::rest::ogm::Field::Make(
      "Status", ara::rest::ogm::String::Make(ara::rest::StringView("unknown", strlen("unknown")))));

  // Trigger the asynchronous transmission of the ara::rest::ServerReply
  ara::rest::Task<void> send_task = rep.Send(std::move(p_obj));

  // Application may wait till ara::rest::ServerReply is transmited.
  send_task.wait();
}

/**
 * \brief A simple ara::rest::Request dispatcher.
 * It dispatches HTTP Requests which comes from the ara::rest::Server to ara::rest::Router
 */
void Dispatcher(const ara::rest::ServerRequest& req, ara::rest::ServerReply& rep) {
  ara::log::Logger& log = ara::log::CreateLogger("4", "Dispatcher Context of rest server sample application");
  log.LogInfo() << "Dispatcher called for request: " << static_cast<uint32_t>(req.GetHeader().GetMethod())
                << " by: " << ara::rest::ToString(req.GetHeader().GetUri());
  // Forward the request to the Router
  router(req, rep);
}

/**
 * TODO
 * \param server_event
 */
void SubscriptionHandler(ara::rest::ServerEvent server_event) {
  (void)server_event;
  ara::log::Logger& log = ara::log::CreateLogger("3", "SubscriptionHandler Context of rest server sample application");
  log.LogInfo() << "SubscriptionHandler called";
}

/**
 * TODO
 * \param server_event
 * \param subscription_state
 */
void SubscriptionStateHandler(const ara::rest::ServerEvent& server_event,
                              ara::rest::SubscriptionState subscription_state) {
  ara::log::Logger& log =
      ara::log::CreateLogger("2", "SubscriptionStateHandler Context of rest server sample application");
  log.LogInfo() << "SubscriptionStateHandler called" << &server_event << &subscription_state;
}

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

  // Configure the ara::rest::Router instance by adding ara::rest::Route(s)
  router.EmplaceRoute(ara::rest::RequestMethod::kPut,
                      ara::rest::Pattern(ara::rest::StringView("/left/door", strlen("/left/door"))),
                      LeftDoorPutHandler);
  router.EmplaceRoute(ara::rest::RequestMethod::kGet,
                      ara::rest::Pattern(ara::rest::StringView("/left/status", strlen("/left/status"))),
                      LeftStatusGetHandler);
  router.SetDefaultHandler(DefaultHandler);

  // Create an ara::rest::Server instance
  ara::rest::Server server("my_server_conf_id", Dispatcher);

  // Install handler functions for events
  server.ObserveSubscriptions(SubscriptionHandler, SubscriptionStateHandler);

  // Start the ara::rest::Server instance
  log.LogInfo() << "Start Server";
  server.Start(ara::rest::StartupPolicy::kDetached);

#if defined(ENABLE_EXEC_MANAGER)
  // Report application state to execution management
  app_client.ReportApplicationState(ApplicationState::kRunning);
#endif  // defined(ENABLE_EXEC_MANAGER)

  // Wait for incoming requests, the ara::rest::Server instance runs asynchronously.
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
