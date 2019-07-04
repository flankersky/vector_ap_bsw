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
/**        \file  httpserver.h
 *        \brief  Header of the HTTP Server implementation of ARA::REST
 *
 *      \details  Contains the Headers of classes HttpServerConfig, HttpServerStatus, HttpServerRequestHandler,
 *                HttpServerRequestHandlerFactory, HttpServer, HttpServerRequest, HttpServerReply and HttpServerBinding.
 *                The implementation heavily relays on the Server Interface of ARA::REST.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_HTTPSERVER_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_HTTPSERVER_H_

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <vac/language/cpp14_backport.h>
#include <ara/log/logging.hpp>
#include <condition_variable>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "ara/rest/config.h"
#include "ara/rest/endpoint.h"
#include "ara/rest/function.h"
#include "ara/rest/header.h"
#include "ara/rest/serialize/serialize.h"
#include "ara/rest/server_event_interface.h"
#include "ara/rest/server_interface.h"
#include "ara/rest/server_reply_interface.h"
#include "ara/rest/server_types.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {

class ServerRequest;  //< fwd
class ServerReply;    //< fwd

class WebSocketRequestHandler;  //< fwd
class HttpServer;               //< fwd

/**********************************************************************************************************************
 *  HttpServerEvent
 *********************************************************************************************************************/

/**
 * TODO
 */
class HttpServerEvent : public ServerEventInterface {
 public:
  /**
  * Default CTOR
  */
  HttpServerEvent() = default;

  /**
  * Constructor
  * \param web_socket
  * \param poco_request
  * \param poco_response
  * \param hnd
  * \param ws_request_handler
  */
  HttpServerEvent(std::shared_ptr<Poco::Net::WebSocket> web_socket, Poco::Net::HTTPRequest& poco_request,
                  Poco::Net::HTTPResponse& poco_response, Function<RequestHandlerType> hnd,
                  WebSocketRequestHandler* ws_request_handler);

  /**
    * Destructor
    */
  ~HttpServerEvent();

  /** \brief Issues a change notification to its corresponding Server
   *
   *  \return a task waiting for the notification to complete. True denotes a successful notification.
   */
  Task<void> Notify();

  /** \brief Cancels an event subscription by issuing a cancelation request.
   *
   * A subscription can also be terminated (but not canceled) by destroying the correspond Event
   * object.
   */
  Task<void> Unsubscribe();

  /**
   * TODO
   * \return
   */
  std::shared_ptr<Poco::Net::WebSocket> GetWebSocket();

  /**
   * TODO
   * \return
   */
  Poco::Net::HTTPRequest& GetPocoRequest();

  /**
   * TODO
   * \return
   */
  Poco::Net::HTTPResponse& GetPocoResponse();

  /**
   * TODO
   * \return
   */
  EventPolicy GetEventPolicy();

  /**
   * TODO
   * \return
   */
  duration_t GetTiming();

  /**
   * TODO
   * \return
   */
  Uri GetUri();

  /**
   * TODO
   * \param event_policy
   */
  void SetEventPolicy(EventPolicy event_policy);

  /**
   * TODO
   * \param timing
   */
  void SetTiming(duration_t timing);

  /**
   * TODO
   * \param uri
   */
  void SetUri(Uri uri);

  /**
   * TODO
   * \param web_socket
   */
  void SetWebSocket(std::shared_ptr<Poco::Net::WebSocket> web_socket);

  /**
   * \brief sends data to the client via websocket
   *
   * Sends data via websocket to the corresponding client to notify about an event
   *
   * \param data
   * \return
   */
  Task<void> Send(String data);

  /**
   * TODO
   * \return
   */
  Task<void> ConfirmSubscription();

  /**
   * TODO
   * \return
   */
  Task<void> ConfirmUnsubscription();

  /**
   * TODO
   * \return
   */
  Task<void> ConfirmResubscription();

  /**
   * TODO
   * \param server_event_ptr
   */
  void PropagateMovedPointer(ServerEvent* server_event_ptr);

  /**
   * TODO
   * \param uri
   */
  void PropagateDeletedEvent(String uri);
  /**
   * \brief notifies the Event about an stoped server
   */
  void StopPeriodicNotification();

 private:
  /**
   * TODO
   * \param interval is the periodic time the event should be send to the client.
   * \param interval_limit is the duration of the subscription.
   */
  void HandlePeriodicEvent(std::chrono::nanoseconds interval, std::chrono::nanoseconds interval_limit);

  /**
   * \brief send task for asychron call of Send
   *
   */
  void SendTask(String data);

  /**
   * TODO
   */
  void ConfirmSubscriptionTask();

  /**
   * TODO
   */
  void ConfirmUnsubscriptionTask();

  /**
   * TODO
   */
  void ConfirmResubscriptionTask();

  /**
   * \brief notify task for asychron call of Notify
   */
  void NotifyTask();

  /**
   * \brief unsubscribe task for asychron call of Unsubscribe
   */
  void UnsubscribeTask();

  /**
   * \brief unsubscribe task for asychron call of Unsubscribe
   */
  void ResubscribeTask();

  /**
   * THe Websocket to be used for communication
   */
  std::shared_ptr<Poco::Net::WebSocket> web_socket_;

  /**
   * The corospending Poco::Net::HTTPRequest (from the Websocket upgrade handshake)
   */
  Poco::Net::HTTPRequest* poco_request_;

  /**
   * The correspending Poco::Net::HTTPResponse (from the Websocket upgrade handshake)
   */
  Poco::Net::HTTPResponse* poco_response_;

  /**
   * The event policy for this event
   */
  EventPolicy event_policy_;

  /**
   * Timing for perdiodic or triggered events
   */
  duration_t timing_;

  /**
   * TODO
   */
  Uri uri_;

  /**
   * The ServerRequest handler function
   */
  Function<RequestHandlerType> hnd_;

  /**
   * TODO
   */
  WebSocketRequestHandler* web_socket_request_handler_;

  /**
  * \brief The logger instance
  */
  ara::log::Logger& log_;

  /**
   * \brief Thread which is performing the periodic sending
   */
  std::thread periodic_thread_;
  /**
   * \brief flag for stopping the periodic sending thread
   */
  bool stop_sending_periodic_ = false;
};

/**********************************************************************************************************************
 *  HttpServerReply
 *********************************************************************************************************************/

/**
 * Implementation of ServerReplyInterface
 */
class HttpServerReply : public ServerReplyInterface {
 public:
  /**
    * Default CTOR
    */
  HttpServerReply();

  /**
   * CTOR
   * \param poco_response
   */
  explicit HttpServerReply(Poco::Net::HTTPServerResponse* poco_response);

  /**
   * CTOR for event reply
   * \param ws
   */
  explicit HttpServerReply(std::shared_ptr<Poco::Net::WebSocket> ws);

  explicit HttpServerReply(const HttpServerReply&) = delete;    ///< Non-copyable
  HttpServerReply& operator=(const HttpServerReply&) = delete;  ///< Non-copy-assignable

  /**
   * Destructor
   */
  ~HttpServerReply() = default;

  /** \brief Send a reply to the peer that has issued the request
   *
   *  If this function is not invoked explicitly, the endpoint will transmit a default reply.
   *  If Redirect() has been before called, these functions must be used.
   *
   * \param data payload to be transmitted
   * \param status
   * \return a task waiting for the transmission to complete
   */
  Task<void> Send(const Pointer<ogm::Object>& data = {}, int status = 200);

  /** \brief Send a reply to the peer that has issued the request
   *
   *  Same a other Send(), only with move semantics
   *
   * \param data payload to be transmitted
   * \param status
   * \return a task waiting for the transmission to complete
   */
  Task<void> Send(Pointer<ogm::Object>&& data, int status);

  /** \brief Issues a redirect command to the connected client
   *
   * Must not be called after Send().
   *
   * \param uri location to redirect to
   */
  Task<void> Redirect(const Uri& uri);

  /**
   * \brief Set the pointer to the corrospending  Poco::Net::HTTPServerResponse
   * \param poco_response
   */
  void SetPocoResponse(Poco::Net::HTTPServerResponse* poco_response);

  /**
   * TODO
   * \param object_string
   */
  inline void SetRequestObjectString(String object_string) { request_object_string_ = object_string; }

  /**
   * TODO
   * \return
   */
  inline String GetRequestObjectString() { return request_object_string_; }

  /**
   * \brief Helper function to asynchronously send a reply
   * \param data_string
   * \param status
   */
  void SendTask(String data_string, int status);

  /**
   * \brief Helpfer function to asynchronously redirect a request
   * \param uri
   */
  void RedirectTask(String uri);

  /**
   * \brief Get boolean member variable which determines if the server application already has send the reply
   * \return The boolean member variable already_send_
   */
  bool GetAlreadySend();

 private:
  /**
   * \brief Pointer the the associated poco response object
   */
  Poco::Net::HTTPServerResponse* poco_response_;

  /**
   * \brief Boolean to mark if send has already been called, so the server will not send a reply if the application has
   * already send it
   */
  bool already_send_;

  /**
   * TODO
   */
  String request_object_string_;

  /**
   * TODO
   */
  const bool is_event_;

  /**
   * Websocket
   */
  std::shared_ptr<Poco::Net::WebSocket> ws_;

  /**
   * \brief The logger instance
   */
  ara::log::Logger& log_;
};

/**********************************************************************************************************************
 *  WebSocketRequestHandler
 *********************************************************************************************************************/

/**
 * TODO
 */
class WebSocketRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  /**
   * Constructor default
   */
  WebSocketRequestHandler() = default;
  /**
   * Constructor
   * \param shnd Subscription handler function
   * \param sshnd SubscriptionState handler function
   * \param hnd ServerRequest handler function
   */
  WebSocketRequestHandler(Function<SubscriptionHandlerType> shnd, Function<SubscriptionStateHandlerType> sshnd,
                          Function<RequestHandlerType> hnd);

  /**
   * Handles the upgrade to Websocket Handshake and creates a event if the message, received from the websocket, is a
   * subscription
   * \param request The Poco::Net::HTTPServerRequest Object which is received via the upgrade to websocket handshake
   * \param response The Poco::Net::HTTPServerResponse Object which is created via the upgrade to websocket handshake
   */
  void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

  /**
   * TODO
   * \param server_event_ptr
   */
  void PropagateMovedPointer(ServerEvent* server_event_ptr);

  /**
   * TODO
   * \param uri
   */
  void PropagateDeletedEvent(String uri);

 private:
  /**
   * \brief is handling the reception of a message with the type subscribe
   * \param ws_message the received message
   * \param ws the websocket the event subscribtion was received from
   * \param request
   * \param response
   */
  void HandleSubscriptionMessage(Pointer<ogm::Object> ws_message, Poco::Net::WebSocket ws,
                                 Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
  /**
   * TODO
   * \param ogm
   * \return
   */
  String GetTypeFieldFromWebSocketMessage(ogm::Object* ogm);

  /**
   * \brief reads interval time of the ogm
   * \param ogm received object
   * \return returns the time in microseconds read from ogm
   */
  duration_t GetIntervalFieldFromWebSocketMessage(ogm::Object* ogm);

  /**
     * \brief reads update_limit time of the ogm
     * \param ogm received object
     * \return returns the time in microseconds read from ogm
     */
  duration_t GetUpdateLimitFieldFromWebSocketMessage(ogm::Object* ogm);

  /**
   * \brief reads uri time of the ogm
   * \param ogm received object
   * \return returns the uri read from ogm
   */
  String GetUriFieldFromWebSocketMessage(ogm::Object* ogm);

  /**
   * The Subscription handler function
   */
  Function<SubscriptionHandlerType> shnd_;

  /**
   * The SubscriptionState handler function
   */
  Function<SubscriptionStateHandlerType> sshnd_;

  /**
   * The ServerRequest handler function
   */
  Function<RequestHandlerType> hnd_;

  /**
   * TODO
   */
  std::map<String, ServerEvent*> event_map_;

  /**
   * \brief The logger instance
   */
  ara::log::Logger& log_;
};

/**********************************************************************************************************************
 *  HttpServerRequestHandler
 *********************************************************************************************************************/

/**
 * Executes the handleRequest Method imidiatly to handle a http server request
 */
class HttpServerRequestHandler : public Poco::Net::HTTPRequestHandler {
 public:
  /**
   * \brief Default Constructor
   */
  HttpServerRequestHandler() = default;

  /**
   * Constructor
   * \param hnd Handler function given by the application
   */
  explicit HttpServerRequestHandler(Function<RequestHandlerType> hnd);

  /**
   * \brief Performs the complete handling of the HTTP request connection. As soon as the handleRequest() method
   * returns, the request handler object is destroyed. A new HTTPRequestHandler object will be created for each new HTTP
   * request that is received by the HTTPServer.
   * \param request represents a server-side HTTP request
   * \param response represents a server-side HTTP response
   */
  void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

  /**
   * \brief Pointer to the corrospending ara::rest::HttpServerReply
   */
  std::shared_ptr<ServerReply> rest_http_server_reply_;

  /**
   * \brief Pointer to the corrospending ara::rest::HttpServerRequest
   */
  std::shared_ptr<ServerRequest> rest_http_server_request_;

  /**
   * \brief Pointer to the corresponding HTTPServerReply Object
   */
  std::shared_ptr<HttpServerReply> http_server_reply_;

  /**
   * Handler function given by the application
   */
  Function<RequestHandlerType> hnd_;

  /**
   * Stores the request body as a string
   */
  String request_body_;

  /**
  * \brief The logger instance
  */
  ara::log::Logger& log_;

 private:
  std::pair<std::shared_ptr<ServerRequest>, std::shared_ptr<ServerReply>> CreateAraRestReqRep(
      Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response,
      std::shared_ptr<HttpServerReply> http_server_reply);

  RequestMethod DetermineRequestMethod(String request_method_string);
};

/**********************************************************************************************************************
 *  HttpServerRequestHandlerFactory
 *********************************************************************************************************************/

/**
 * \brief Creates new HttpServerRequestHandler to handle new HTTP requests
 */
class HttpServerRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  /**
   * \brief  Creates the HttpServerRequestHandlerFactory and saves given HttpServer Pointer
   * \param http_server
   */
  explicit HttpServerRequestHandlerFactory(HttpServer* http_server);

  /**
   * \brief  Creates a new request handler for the given HTTP request. The method should inspect the given
   * HTTPServerRequest object (e.g., method and URI) and create an appropriate HTTPRequestHandler object to handle the
   * request.
   * \param request represents a server-side HTTP request
   * \return Pointer to an request handler
   */
  virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

  /**
   * Handler function given by the application
   */
  HttpServer* http_server_;
};

/**********************************************************************************************************************
 *  HttpServer
 *********************************************************************************************************************/

/**
 * \brief Implements the HTTP Server as proposed by the http server impl interface
 */
class HttpServer : public ServerInterface {
 public:
  /**
   * Deleted default constructor
   */
  HttpServer() = delete;

  /**
   * \brief Constructor
   * \param binding Server binding with configuration informations
   * \param hnd Custom handler function to handle a request
   * \param alloc
   */
  HttpServer(config::ServerBinding binding, Function<RequestHandlerType> hnd, Allocator* alloc = GetDefaultAllocator());
  /**
   * \brief Destructor
   */
  ~HttpServer() = default;

  /**
   * \brief Start the HttpServer with respect to given policy
   * \param policy Determines wheter to start  a server in a separate execution context or in the execution context of
   * the endpoint owner
   * \return Returns a task
   */
  Task<void> Start(StartupPolicy policy = StartupPolicy::kDetached);

  /**
   * \brief Stop the HttpServer with respect to given policy
   * \param policy Determines whether to stop gracefully or forced
   * \return Returns a Task containing the ServerStatus response of HttpServer::StopServer()
   */
  Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful);

  /**
   * TODO
   * \param shnd
   * \param sshnd
   */
  void ObserveSubscriptions(const Function<SubscriptionHandlerType>& shnd,
                            const Function<SubscriptionStateHandlerType>& sshnd);

  /** \brief Obtain server status
   * \return a reference to the server Status
   */
  std::error_code GetError() const noexcept;

  /** \brief Observe status changes
   *
   *  \param hnd user-defined handler function to to called on status changes
   */
  void ObserveError(const Function<void(std::error_code)>& hnd);

  /**
   * TODO
   * \return
   */
  const Function<RequestHandlerType> GetCustomRequestHandler();

  /**
   * TODO
   * \return
   */
  const Function<SubscriptionHandlerType> GetCustomSubscriptionHandler();

  /**
   * TODO
   * \return
   */
  const Function<SubscriptionStateHandlerType> GetCustomSubscriptionStateHandler();

 private:
  /**
   * \brief Instance of an Poco Http Server
   */
  Pointer<Poco::Net::HTTPServer> poco_http_server_;

  /**
   * \brief Holds the custom request handler function given in the Constructor
   */
  Function<RequestHandlerType> hnd_;

  /**
   * Saves the custom subscription method, which should be called for every subscription event
   */
  Function<SubscriptionHandlerType> shnd_;

  /**
   * Saves the custom subscription handle method, which should be called for every change in a subscription event
   */
  Function<SubscriptionStateHandlerType> sshnd_;

  /**
   * \brief Mutex for the condition variable
   */
  std::mutex m_;

  /**
   * \brief A conidition variable to notify about Stop beeing called
   */
  std::condition_variable cv_;

  /**
   * \brief Boolean to determine if Stop was called
   */
  bool stop_called_;

  /**
  * \brief The logger instance
  */
  ara::log::Logger& log_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_HTTPSERVER_H_
