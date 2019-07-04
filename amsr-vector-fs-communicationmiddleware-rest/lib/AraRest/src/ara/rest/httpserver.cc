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
/**        \file  httpserver.cc
 *        \brief  Header of the HTTP Server implementation of ARA::REST
 *
 *      \details  Contains the implementation of classes HttpServerConfig, HttpServerStatus, HttpServerRequestHandler,
 *                HttpServerRequestHandlerFactory, HttpServer, HttpServerRequest, HttpServerReply and HttpServerBinding.
 *                The implementation heavily relays on the Server Interface of ARA::REST.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <functional>
#include <future>

#include "ara/rest/allocator.h"
#include "ara/rest/config.h"
#include "ara/rest/future.h"
#include "ara/rest/httpserver.h"
#include "ara/rest/ogm/object.h"

namespace ara {
namespace rest {

/**
 * \brief Determines the corresponding RequestMethod from a given string
 * \param request_method_string The request method as a string
 * \return A RequestMethod object
 */
RequestMethod HttpServerRequestHandler::DetermineRequestMethod(String request_method_string) {
  ara::log::Logger& log = ara::log::CreateLogger("42", "ara::rest HttpServer additional");
  RequestMethod result{RequestMethod::kAny};
  if (request_method_string == "GET") {
    result = RequestMethod::kGet;
  } else if (request_method_string == "POST") {
    result = RequestMethod::kPost;
  } else if (request_method_string == "PUT") {
    result = RequestMethod::kPut;
  } else if (request_method_string == "DELETE") {
    result = RequestMethod::kDelete;
  } else if (request_method_string == "OPTIONS") {
    result = RequestMethod::kOptions;
  } else {
    log.LogError() << "HttpServer DetermineRequestMethod: Unkown RequestMethod: " << request_method_string;
  }
  return result;
}

/**
 * \brief Creates the ara::rest::ServerRequest and ara::rest:ServerReply objects and fills the ServerRequest with
 * content from the Poco::Net::HTTPServerRequest. The ServerReply object will be filled from an application.
 *
 * \param request The Poco::Net::HTTPServerRequest to get content from for the ara::rest::ServerRequest
 * \param response
 * \param http_server_reply
 * \return TODO
 */
std::pair<std::shared_ptr<ServerRequest>, std::shared_ptr<ServerReply>> HttpServerRequestHandler::CreateAraRestReqRep(
    Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response,
    std::shared_ptr<HttpServerReply> http_server_reply) {
  ara::log::Logger& log = ara::log::CreateLogger("42", "ara::rest HttpServer additional");
  log.LogDebug() << "CreateAraRestReqRep called.";
  (void)response;
  // ServerRequest
  Pointer<RequestHeader> request_header = vac::language::make_unique<RequestHeader>(
      DetermineRequestMethod(request.getMethod()),
      Uri::Builder(StringView(STRING_TO_STRINGVIEW(request.getURI()))).ToUri());

  Pointer<ogm::Object> request_object;

  std::istream& request_body = request.stream();
  String request_body_string = String(std::istreambuf_iterator<char>(request_body), {});
  if (request_body_string.length() > 0) {
    http_server_reply.get()->SetRequestObjectString(request_body_string);
    // create object out of request body
    request_object = serialize::Serializer::JsonToOgm(request_body_string);
  }

  std::shared_ptr<ServerRequest> rest_http_server_request =
      std::make_shared<ServerRequest>(std::move(request_header), std::move(request_object));
  // ServerReply
  std::shared_ptr<ServerReply> rest_http_server_reply = std::make_shared<ServerReply>();
  Pointer<ReplyHeader> reply_header = vac::language::make_unique<ReplyHeader>(
      200, Uri::Builder(StringView(STRING_TO_STRINGVIEW(request.getURI()))).ToUri());

  rest_http_server_reply.get()->SetHeader(std::move(reply_header));
  rest_http_server_reply.get()->SetServerReplyInterface(http_server_reply.get());

  return std::make_pair(rest_http_server_request, rest_http_server_reply);
}

/**********************************************************************************************************************
 *  HttpServerEvent
 *********************************************************************************************************************/

HttpServerEvent::HttpServerEvent(std::shared_ptr<Poco::Net::WebSocket> web_socket, Poco::Net::HTTPRequest& poco_request,
                                 Poco::Net::HTTPResponse& poco_response, Function<RequestHandlerType> hnd,
                                 WebSocketRequestHandler* ws_request_handler)
    : web_socket_(std::move(web_socket)),
      poco_request_(&poco_request),
      poco_response_(&poco_response),
      event_policy_(EventPolicy::kPeriodic),
      timing_(0),
      uri_(),
      hnd_(hnd),
      web_socket_request_handler_(ws_request_handler),
      log_(ara::log::CreateLogger("42", "ara::rest HttpServerEvent")) {
  if (event_policy_ == EventPolicy::kPeriodic) {
    log_.LogDebug() << "HttpServerEvent::HttpServerEvent: EventPolicy::kPeriodic";
  } else if (event_policy_ == EventPolicy::kTriggered) {
    log_.LogDebug() << "HttpServerEvent::HttpServerEvent: EventPolicy::kTriggered";
  }
}

HttpServerEvent::~HttpServerEvent() { StopPeriodicNotification(); }

void HttpServerEvent::NotifyTask() {
  log_.LogDebug() << "HttpServerEvent::NotifyTask called";
  // if the event should be handled periodic nothing needs to be done
  if (event_policy_ != EventPolicy::kPeriodic) {
    // create ServerRequest
    RequestHeader req_header(RequestMethod::kGet, uri_);
    ServerRequest req;
    req.SetHeader(vac::language::make_unique<RequestHeader>(req_header));
    // create ServerReply
    HttpServerReply* http_server_reply = new HttpServerReply(web_socket_);
    Pointer<ReplyHeader> rep_head = vac::language::make_unique<ReplyHeader>(Poco::Net::HTTPResponse::HTTP_OK, uri_);
    ServerReply rep(std::move(rep_head), ogm::Object::Make(), http_server_reply);

    // call custom request handler to send response (application has to call send, but server checks that it has to be
    // send via websocket)
    hnd_(req, rep);
  }
}

Task<void> HttpServerEvent::Notify() {
  log_.LogDebug() << "HttpServerEvent::Notify called";
  std::packaged_task<void()> notify_ptask(std::bind(&HttpServerEvent::NotifyTask, this));
  std::future<void> notify_ft = notify_ptask.get_future();
  Task<void> notify_task(std::move(notify_ft));

  notify_ptask();

  return notify_task;
}

void HttpServerEvent::UnsubscribeTask() {
  log_.LogDebug() << "HttpServerEvent::UnsubscribeTask called";
  // stop the periodic sending if the event is periodic
  StopPeriodicNotification();
  // create the unsubscribe request
  String uri_str = ToString(uri_);
  Pointer<ogm::Object> unsubscribe_ogm =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("unsubscribe"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                        ogm::Field::Make("Authorization", ogm::String::Make(CSTRING_TO_STRINGVIEW("<token>"))));
  // convert form ogm::Object to String
  String unsubscribe_message = serialize::Serializer::OgmToJson(unsubscribe_ogm);
  // send the message by the websockets
  int flags = Poco::Net::WebSocket::FRAME_TEXT;
  web_socket_->sendFrame(unsubscribe_message.data(), static_cast<int>(unsubscribe_message.size()), flags);
}

Task<void> HttpServerEvent::Unsubscribe() {
  // create the assync task for the unsubscribtion
  log_.LogDebug() << "HttpServerEvent::Unsubscribe called";
  std::packaged_task<void()> unsubscribe_ptask(std::bind(&HttpServerEvent::NotifyTask, this));
  std::future<void> unsubscribe_ft = unsubscribe_ptask.get_future();
  Task<void> unsubscribe_task(std::move(unsubscribe_ft));
  // execute the assync task
  unsubscribe_ptask();

  return unsubscribe_task;
}

std::shared_ptr<Poco::Net::WebSocket> HttpServerEvent::GetWebSocket() { return web_socket_; }

Poco::Net::HTTPRequest& HttpServerEvent::GetPocoRequest() { return *poco_request_; }

Poco::Net::HTTPResponse& HttpServerEvent::GetPocoResponse() { return *poco_response_; }

EventPolicy HttpServerEvent::GetEventPolicy() { return event_policy_; }

duration_t HttpServerEvent::GetTiming() { return timing_; }

Uri HttpServerEvent::GetUri() { return uri_; }

void HttpServerEvent::SetEventPolicy(EventPolicy event_policy) {
  event_policy_ = event_policy;
  if (event_policy_ == EventPolicy::kPeriodic) {
    stop_sending_periodic_ = false;
    periodic_thread_ = std::thread(std::bind(&HttpServerEvent::HandlePeriodicEvent, this,
                                             std::chrono::duration_cast<std::chrono::nanoseconds>(this->timing_),
                                             std::chrono::nanoseconds(0)));
  } else {
    StopPeriodicNotification();
  }
}

void HttpServerEvent::SetTiming(duration_t timing) { timing_ = timing; }

void HttpServerEvent::SetUri(Uri uri) { uri_ = uri; }

void HttpServerEvent::SetWebSocket(std::shared_ptr<Poco::Net::WebSocket> web_socket) { web_socket_ = web_socket; }

void HttpServerEvent::SendTask(String data) {
  log_.LogDebug() << "HttpServerEvent::SendTask called";
  int flags = Poco::Net::WebSocket::FRAME_TEXT;
  web_socket_.get()->sendFrame(data.data(), static_cast<int>(data.size()), flags);
}

Task<void> HttpServerEvent::Send(String data) {
  log_.LogDebug() << "HttpServerEvent::Send called. data: " << data;
  std::packaged_task<void()> send_ptask(std::bind(&HttpServerEvent::SendTask, this, data));
  std::future<void> send_ft = send_ptask.get_future();
  Task<void> send_task(std::move(send_ft));

  send_ptask();

  return send_task;
}

void HttpServerEvent::ConfirmSubscriptionTask() {
  String uri_str = ToString(uri_);
  Pointer<ogm::Object> subscribe_ogm =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("subscribe"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                        ogm::Field::Make("status", ogm::String::Make(CSTRING_TO_STRINGVIEW("ok"))));

  String confirm_message = serialize::Serializer::OgmToJson(subscribe_ogm);
  log_.LogDebug() << "HttpServerEvent::ConfirmSubscriptionTask called. confirm_message: " << confirm_message;

  int flags = Poco::Net::WebSocket::FRAME_TEXT;
  web_socket_->sendFrame(confirm_message.data(), static_cast<int>(confirm_message.size()), flags);
}

Task<void> HttpServerEvent::ConfirmSubscription() {
  log_.LogDebug() << "HttpServerEvent::ConfirmSubscription called.";
  std::packaged_task<void()> confirm_subscription_ptask(std::bind(&HttpServerEvent::ConfirmSubscriptionTask, this));
  std::future<void> confirm_subscription_ft = confirm_subscription_ptask.get_future();
  Task<void> confirm_subscription_task(std::move(confirm_subscription_ft));

  confirm_subscription_ptask();

  return confirm_subscription_task;
}

void HttpServerEvent::ConfirmUnsubscriptionTask() {
  log_.LogDebug() << "HttpServerEvent::ConfirmUnsubscriptionTask called.";

  String uri_str = ToString(uri_);
  Pointer<ogm::Object> unsubscribe_ogm =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("unsubscribe"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                        ogm::Field::Make("status", ogm::String::Make(CSTRING_TO_STRINGVIEW("ok"))));

  String confirm_message = serialize::Serializer::OgmToJson(unsubscribe_ogm);
  int flags = Poco::Net::WebSocket::FRAME_TEXT;
  web_socket_->sendFrame(confirm_message.data(), static_cast<int>(confirm_message.size()), flags);
}

Task<void> HttpServerEvent::ConfirmUnsubscription() {
  StopPeriodicNotification();
  std::packaged_task<void()> confirm_unsubscription_ptask(std::bind(&HttpServerEvent::ConfirmUnsubscriptionTask, this));
  std::future<void> confirm_unsubscription_ft = confirm_unsubscription_ptask.get_future();
  Task<void> confirm_unsubscription_task(std::move(confirm_unsubscription_ft));

  confirm_unsubscription_ptask();

  return confirm_unsubscription_task;
}

void HttpServerEvent::ConfirmResubscriptionTask() {
  StopPeriodicNotification();
  SetEventPolicy(event_policy_);
  String uri_str = ToString(uri_);
  Pointer<ogm::Object> resubscribe_ogm =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("reauthorize"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                        ogm::Field::Make("status", ogm::String::Make(CSTRING_TO_STRINGVIEW("ok"))));
  String confirm_message = serialize::Serializer::OgmToJson(resubscribe_ogm);
  int flags = Poco::Net::WebSocket::FRAME_TEXT;
  web_socket_->sendFrame(confirm_message.data(), static_cast<int>(confirm_message.size()), flags);
}

Task<void> HttpServerEvent::ConfirmResubscription() {
  std::packaged_task<void()> confirm_resubscription_ptask(std::bind(&HttpServerEvent::ConfirmResubscriptionTask, this));
  std::future<void> confirm_resubscription_ft = confirm_resubscription_ptask.get_future();
  Task<void> confirm_resubscription_task(std::move(confirm_resubscription_ft));

  confirm_resubscription_ptask();

  return confirm_resubscription_task;
}

void HttpServerEvent::HandlePeriodicEvent(std::chrono::nanoseconds interval, std::chrono::nanoseconds interval_limit) {
  RequestHeader req_header(RequestMethod::kGet, uri_);
  ServerRequest req;
  req.SetHeader(vac::language::make_unique<RequestHeader>(req_header));
  // create ServerReply
  HttpServerReply* http_server_reply = new HttpServerReply(web_socket_);
  Pointer<ReplyHeader> rep_head = vac::language::make_unique<ReplyHeader>(Poco::Net::HTTPResponse::HTTP_OK, uri_);
  ServerReply rep(std::move(rep_head), ogm::Object::Make(), http_server_reply);
  auto start_time = std::chrono::steady_clock::now();

  // call application request handler function periodicly
  while (((std::chrono::duration_cast<std::chrono::nanoseconds>((std::chrono::steady_clock::now() - start_time)) <
           std::chrono::nanoseconds((interval_limit.count()))) ||
          interval_limit.count() == 0) &&
         (!stop_sending_periodic_)) {
    auto start_working = std::chrono::steady_clock::now();
    // call custom request handler to send response (application has to call send, but server checks that it has to be
    hnd_(req, rep);
    auto sleep_time = (interval - (std::chrono::steady_clock::now() - start_working));
    if (sleep_time.count() > 0) {
      std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::nanoseconds>(sleep_time));
    }
  }
}

void HttpServerEvent::PropagateMovedPointer(ServerEvent* server_event_ptr) {
  web_socket_request_handler_->PropagateMovedPointer(server_event_ptr);
}

void HttpServerEvent::PropagateDeletedEvent(String uri) { web_socket_request_handler_->PropagateDeletedEvent(uri); }

void HttpServerEvent::StopPeriodicNotification() {
  if (event_policy_ == EventPolicy::kPeriodic) {
    if (!stop_sending_periodic_) {
      stop_sending_periodic_ = true;
      periodic_thread_.join();
    }
  }
}

/**********************************************************************************************************************
 *  HttpServerReply
 *********************************************************************************************************************/
HttpServerReply::HttpServerReply()
    : poco_response_(nullptr),
      already_send_(false),
      is_event_(false),
      ws_(nullptr),
      log_(ara::log::CreateLogger("42", "ara::rest HttpServerReply")) {}

HttpServerReply::HttpServerReply(Poco::Net::HTTPServerResponse* poco_response)
    : poco_response_(poco_response),
      already_send_(false),
      is_event_(false),
      log_(ara::log::CreateLogger("42", "ara::rest HttpServerReply")) {}

HttpServerReply::HttpServerReply(std::shared_ptr<Poco::Net::WebSocket> ws)
    : already_send_(false), is_event_(true), ws_(ws), log_(ara::log::CreateLogger("42", "ara::rest HttpServerReply")) {}

void HttpServerReply::SetPocoResponse(Poco::Net::HTTPServerResponse* poco_response) { poco_response_ = poco_response; }

void HttpServerReply::SendTask(String data_string, int status) {
  log_.LogDebug() << "HttpServerReply::SendTask called";
  if (is_event_) {
    int flags = Poco::Net::WebSocket::FRAME_TEXT;
    // TODO(hhz): Error handling
    ws_->sendFrame(data_string.data(), static_cast<int>(data_string.size()), flags);
  } else {
    // prepare response
    poco_response_->setContentType("application/json");
    poco_response_->setStatus(std::to_string(status));
    poco_response_->setContentLength(data_string.size());
    // the actual send returns a file stream which is filled directly with data_string
    poco_response_->send() << data_string;
  }
}

Task<void> HttpServerReply::Send(const Pointer<ogm::Object>& data, int status) {
  // convert ogm::object data into a string
  String data_string = serialize::Serializer::OgmToJson(std::move(data));

  // TODO(hhz): remove this?
  if (data_string.compare("{}") == 0) {
    data_string = "{\"Default\":\"response\"}";
  }

  log_.LogDebug() << "HttpServerReply::SendTask: Data to be send: " << data_string;
  // actual send task
  std::packaged_task<void()> send_data_ptask(std::bind(&HttpServerReply::SendTask, this, data_string, status));
  std::future<void> send_data_ft = send_data_ptask.get_future();
  Task<void> send_data_task(std::move(send_data_ft));
  already_send_ = true;

  send_data_ptask();

  return send_data_task;
}

Task<void> HttpServerReply::Send(Pointer<ogm::Object>&& data, int status) {
  // convert ogm::object data into a string
  String data_string = serialize::Serializer::OgmToJson(std::move(data));

  // TODO(hhz): remove this?
  if (data_string.compare("{}") == 0) {
    data_string = "{\"Default\":\"response\"}";
  }

  log_.LogDebug() << "HttpServerReply::SendTask: Data to be send: " << data_string;
  // actual send task
  std::packaged_task<void()> send_data_ptask(std::bind(&HttpServerReply::SendTask, this, data_string, status));
  std::future<void> send_data_ft = send_data_ptask.get_future();
  Task<void> send_data_task(std::move(send_data_ft));
  already_send_ = true;

  send_data_ptask();

  return send_data_task;
}

void HttpServerReply::RedirectTask(String uri) {
  poco_response_->redirect(uri, Poco::Net::HTTPResponse::HTTPStatus::HTTP_TEMPORARY_REDIRECT);
}

Task<void> HttpServerReply::Redirect(const Uri& uri) {
  std::packaged_task<void()> redirect_ptask(std::bind(&HttpServerReply::RedirectTask, this, ToString(uri)));
  std::future<void> redirect_ft = redirect_ptask.get_future();
  Task<void> redirect_task(std::move(redirect_ft));
  already_send_ = true;

  redirect_ptask();

  return redirect_task;
}

bool HttpServerReply::GetAlreadySend() { return already_send_; }

/**********************************************************************************************************************
 *  WebSocketRequestHandler
 *********************************************************************************************************************/

WebSocketRequestHandler::WebSocketRequestHandler(Function<SubscriptionHandlerType> shnd,
                                                 Function<SubscriptionStateHandlerType> sshnd,
                                                 Function<RequestHandlerType> hnd)
    : shnd_(shnd),
      sshnd_(sshnd),
      hnd_(hnd),
      log_(ara::log::CreateLogger("42", "ara::rest HttpServer WebSocketRequestHandler")) {
  if (!shnd) {
    log_.LogError()
        << "WebSocketRequestHandler::WebSocketRequestHandler: Custom Subscription Handler Function is not available!";
  }
  if (!sshnd) {
    log_.LogError()
        << "WebSocketRequestHandler::WebSocketRequestHandler: Custom Subscription State Handler Function is "
           "not available!";
  }
}

String WebSocketRequestHandler::GetTypeFieldFromWebSocketMessage(ogm::Object* ogm) {
  String result;
  if (ogm->HasField(CSTRING_TO_STRINGVIEW("type"))) {
    ogm::Object::Iterator type_iter = ogm->Find(CSTRING_TO_STRINGVIEW(("type")));
    StringView type_view(ogm::details::cast<ogm::String>(&(*type_iter).GetValue())->GetValue());
    STRINGVIEW_TO_STRING(result, type_view);
  } else {
    log_.LogError() << "WebSocketRequestHandler::GetTypeFieldFromWebSocketMessage: The OGM, determined from WebSocket "
                       "message, does not have a 'type' field!";
  }
  return result;
}

duration_t WebSocketRequestHandler::GetIntervalFieldFromWebSocketMessage(ogm::Object* ogm) {
  ogm::Object::Iterator interval_iter = ogm->Find(CSTRING_TO_STRINGVIEW("interval"));
  StringView interval_view(ogm::details::cast<ogm::String>(&(*interval_iter).GetValue())->GetValue());
  String sm_interval;
  STRINGVIEW_TO_STRING(sm_interval, interval_view);
  // set HttpServerEvent timing to read value
  return static_cast<duration_t>(std::chrono::milliseconds(atol(sm_interval.c_str())));
}

duration_t WebSocketRequestHandler::GetUpdateLimitFieldFromWebSocketMessage(ogm::Object* ogm) {
  ogm::Object::Iterator interval_iter = ogm->Find(CSTRING_TO_STRINGVIEW("updatelimit"));
  StringView interval_view(ogm::details::cast<ogm::String>(&(*interval_iter).GetValue())->GetValue());
  String sm_interval;
  STRINGVIEW_TO_STRING(sm_interval, interval_view);
  // set HttpServerEvent timing to read value
  return static_cast<duration_t>(std::chrono::milliseconds(atol(sm_interval.c_str())));
}

String WebSocketRequestHandler::GetUriFieldFromWebSocketMessage(ogm::Object* ogm) {
  if (ogm->HasField(CSTRING_TO_STRINGVIEW("event"))) {
    ogm::Object::Iterator uri_iter = ogm->Find(CSTRING_TO_STRINGVIEW("event"));
    StringView uri_view(ogm::details::cast<ogm::String>(&(*uri_iter).GetValue())->GetValue());
    String sm_uri;
    STRINGVIEW_TO_STRING(sm_uri, uri_view);
    // set HttpServerEvent timing to read value
    return sm_uri;
  } else {
    // TODO(hhz): Error response over websocket
    log_.LogError() << "WebSocketRequestHandler::handleRequest: "
                    << "No event in subscription message found!";
    return "";
  }
}

void WebSocketRequestHandler::HandleSubscriptionMessage(Pointer<ogm::Object> ws_message, Poco::Net::WebSocket ws,
                                                        Poco::Net::HTTPServerRequest& request,
                                                        Poco::Net::HTTPServerResponse& response) {
  // preparations
  EventPolicy event_policy = EventPolicy::kPeriodic;
  duration_t event_interval = static_cast<duration_t>(0);
  String uri;
  bool subscription_correct = false;
  // determine if periodic or triggered
  if (ws_message->HasField(CSTRING_TO_STRINGVIEW("interval"))) {
    // set HttpServerEvent to periodic
    event_policy = EventPolicy::kPeriodic;
    // read out the value of interval from ogm
    event_interval = GetIntervalFieldFromWebSocketMessage(ws_message.get());
    subscription_correct = true;
  } else if (ws_message->HasField(CSTRING_TO_STRINGVIEW("updatelimit"))) {
    // set HttpServerEvent to triggered
    event_policy = EventPolicy::kTriggered;
    // read out the value of updatelimit from ogm
    event_interval = GetUpdateLimitFieldFromWebSocketMessage(ws_message.get());
    subscription_correct = true;
  } else {
    subscription_correct = false;
    // TODO(hhz): Error response over websocket
    log_.LogError() << "WebSocketRequestHandler::handleRequest: "
                    << "No 'interval' and no 'updatelimit' in subscription message!";
  }
  // read out the value of uri from ogm
  String sm_uri = GetUriFieldFromWebSocketMessage(ws_message.get());
  if (!sm_uri.empty()) {
    uri = sm_uri;
    subscription_correct = true;
  } else {
    subscription_correct = false;
  }
  if (subscription_correct) {
    log_.LogDebug() << "WebSocketRequestHandler::handleRequest: create ServerEvent";
    // create HttpServerEvent
    Pointer<HttpServerEvent> http_server_event = vac::language::make_unique<HttpServerEvent>(
        std::make_shared<Poco::Net::WebSocket>(ws), request, response, hnd_, this);
    http_server_event->SetTiming(event_interval);
    http_server_event->SetUri(Uri::Builder(StringView(STRING_TO_STRINGVIEW(uri))).ToUri());
    http_server_event.get()->SetEventPolicy(event_policy);
    // create ServerEvent
    ServerEvent server_event =
        ServerEvent(SubscriptionState::kSubscribed,
                    vac::language::make_unique<Uri>(Uri::Builder(StringView(STRING_TO_STRINGVIEW(uri))).ToUri()),
                    std::move(http_server_event));
    // insert ServerEvent in the map
    log_.LogDebug() << "WebSocketRequestHandler::handleRequest: insert ServerEvent in the map";
    event_map_.insert(std::make_pair(uri, &server_event));
    log_.LogDebug() << "WebSocketRequestHandler::handleRequest: call Subscription handler";
    if (shnd_) {
      // call custom subscription handler function, important: owner is then the application, but
      // ServerEvent
      // will propagate its pointer back to the WebSocketRequestHandler
      shnd_(std::move(*event_map_[uri]));
      log_.LogDebug() << "WebSocketRequestHandler::handleRequest: called Subscription handler";
    } else {
      log_.LogError() << "WebSocketRequestHandler::handleRequest: Received event subscription but no "
                         "subscription handler given. Did you called Server::ObserveSubscriptions to make "
                         "Subscription Handler available?";
    }
  } else {
    log_.LogError() << "WebSocketRequestHandler::handleRequest: ServerEvent couldn't be created because "
                       "subscription message was not complete. One of the "
                       "following contents is missing: interval, updatelimit or event";
  }
}

void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                            Poco::Net::HTTPServerResponse& response) {
  // create Websocket to talk with client
  try {
    Poco::Net::WebSocket ws(request, response);
    char buffer[1024] = {0};
    int flags = 0;
    int bytes_received = 0;

    do {
      memset(buffer, 0, sizeof(buffer));
      // check if a new message is available
      if (ws.available()) {
        // receive message from websocket
        bytes_received = ws.receiveFrame(buffer, sizeof(buffer), flags);
        if (bytes_received > 0) {
          String buffer_string = String(buffer);
          log_.LogDebug() << "WebSocketRequestHandler::handleRequest: "
                          << "Frame received (length=" << bytes_received << ", flags=" << unsigned(flags) << ")"
                          << "\nWebSocketRequestHandler::handleRequest: "
                          << "Received message: " << buffer_string;
          // investigate message send via websocket (is it a event subscription)
          // TODO(hhz): Error handling, emanuel already did this i think
          Pointer<ogm::Object> ws_message = serialize::Serializer::JsonToOgm(buffer_string);
          String type = GetTypeFieldFromWebSocketMessage(ws_message.get());
          log_.LogDebug() << "WebSocketRequestHandler::handleRequest: type: " << type;
          if (type.compare("subscribe") == 0) {
            HandleSubscriptionMessage(std::move(ws_message), ws, request, response);
          } else if (type.compare("unsubscribe") == 0) {
            if (ws_message->HasField(CSTRING_TO_STRINGVIEW("event"))) {
              auto sm_uri = GetUriFieldFromWebSocketMessage(ws_message.get());
              // call subscription state handler
              if (sm_uri.compare("") != 0) {
                sshnd_(*event_map_[sm_uri], SubscriptionState::kCanceled);
              }
            }
          } else if (type.compare("reauthorize") == 0) {
            String sm_uri = GetUriFieldFromWebSocketMessage(ws_message.get());
            // call subscription state handler
            if (sm_uri.compare("") != 0) {
              sshnd_(*event_map_[sm_uri], SubscriptionState::kSubscribed);
            }
          } else {
            // TODO(hhz): Error response over websocket
            log_.LogError() << "WebSocketRequestHandler::handleRequest: Can't determine type of websocket message, is '"
                            << type << "' misspelled? ";
          }
        }
      }
    } while ((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
    log_.LogDebug() << "WebSocketRequestHandler::handleRequest: WebSocket connection closed.";
    // if websockets closed stop all periodicHandlers and clear the map
    for (std::map<String, ServerEvent*>::iterator it = event_map_.begin(); it != event_map_.end(); ++it) {
      event_map_[it->first]->StopPeriodicNotification();
    }
    event_map_.clear();
  } catch (Poco::Net::WebSocketException& exc) {
    switch (exc.code()) {
      case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
        response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
      // fallthrough
      case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
      case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
      case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
        response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.setContentLength(0);
        response.send();
        break;
    }
  }
}

void WebSocketRequestHandler::PropagateMovedPointer(ServerEvent* server_event_ptr) {
  // update map to match new pointer
  event_map_[ToString(server_event_ptr->GetUri())] = server_event_ptr;
}

void WebSocketRequestHandler::PropagateDeletedEvent(String uri) {
  // delete event from map
  event_map_.erase(uri);
}

/**********************************************************************************************************************
 *  HttpServerRequestHandler
 *********************************************************************************************************************/

HttpServerRequestHandler::HttpServerRequestHandler(Function<RequestHandlerType> hnd)
    : hnd_(hnd), log_(ara::log::CreateLogger("42", "ara::rest HttpServerRequestHandler")) {}

void HttpServerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                             Poco::Net::HTTPServerResponse& response) {
  // create ara::rest::ServerRequest and ara::rest::ServerReply and puts the content of the
  // Poco::Net::HTTPServerRequest into the ServerRequest object
  http_server_reply_ = std::make_shared<HttpServerReply>(&response);
  std::pair<std::shared_ptr<ServerRequest>, std::shared_ptr<ServerReply>> rest_pair =
      CreateAraRestReqRep(request, response, http_server_reply_);

  rest_http_server_request_ = rest_pair.first;
  rest_http_server_reply_ = rest_pair.second;

  // call handler function given to server ctor
  hnd_(*rest_http_server_request_, *rest_http_server_reply_);

  // if the application has not already send the reply
  if (!http_server_reply_.get()->GetAlreadySend()) {
    log_.LogDebug()
        << "HttpServerRequestHandler::handleRequest Application has not triggered the transmission of the reply. "
           "Default Reply will be transmitted.";
    Task<void> send_task =
        http_server_reply_.get()->Send(ogm::Object::Make(), rest_http_server_reply_.get()->GetHeader().GetStatus());

    send_task.wait();
  }
}

/**********************************************************************************************************************
 *  HttpServerRequestHandlerFactory
 *********************************************************************************************************************/

HttpServerRequestHandlerFactory::HttpServerRequestHandlerFactory(HttpServer* http_server) : http_server_(http_server) {}

Poco::Net::HTTPRequestHandler* HttpServerRequestHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) {
  if (request.find("Upgrade") != request.end() && Poco::icompare(request["Upgrade"], "websocket") == 0)
    return new WebSocketRequestHandler(http_server_->GetCustomSubscriptionHandler(),
                                       http_server_->GetCustomSubscriptionStateHandler(),
                                       http_server_->GetCustomRequestHandler());
  else
    return new HttpServerRequestHandler(http_server_->GetCustomRequestHandler());
}

/**********************************************************************************************************************
 *  HttpServer
 *********************************************************************************************************************/

HttpServer::HttpServer(config::ServerBinding binding, Function<RequestHandlerType> hnd, Allocator* alloc)
    : hnd_(hnd), stop_called_(false), log_(ara::log::CreateLogger("42", "ara::rest HttpServer")) {
  log_.LogDebug() << "HttpServer::HttpServer: "
                  << "HttpServer created (Binding ID: " << binding.Id_ << ") and Application Request Handler at "
                  << &hnd;
  String adress_port = binding.address_ + ":" + std::to_string(binding.port_);

  log_.LogDebug() << "HttpServer::HttpServer: "
                  << "Start HTTP Server to listen on " << adress_port;

  poco_http_server_ = vac::language::make_unique<Poco::Net::HTTPServer>(
      new HttpServerRequestHandlerFactory(this), Poco::Net::ServerSocket(Poco::Net::SocketAddress(adress_port)),
      new Poco::Net::HTTPServerParams);

  log_.LogDebug() << "HttpServer::HttpServer: "
                  << "Created HTTP Server Binding " << &alloc;
}

Task<void> HttpServer::Start(StartupPolicy policy) {
  std::future<void> ft_start_server;
  if (policy == StartupPolicy::kAttached) {
    // Start of server will block
    poco_http_server_->start();
    // fill the future to make use of it possible
    std::promise<void> p;
    ft_start_server = p.get_future();
    // block until DoStop has been called
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [&] { return stop_called_; });

    log_.LogDebug() << "HttpServer::Start: "
                    << "Stop has been called, thread no longer blocked";
    lk.unlock();
  } else if (policy == StartupPolicy::kDetached) {
    std::packaged_task<void()> start_task([&]() { poco_http_server_->start(); });
    ft_start_server = start_task.get_future();
    std::thread(std::move(start_task)).detach();
  } else {
    log_.LogError() << "HttpServer::Start: "
                    << "ERROR: Unkown Startup policy";
  }
  Task<void> start_task(std::move(ft_start_server));
  return start_task;
}

Task<void> HttpServer::Stop(ShutdownPolicy policy) {
  // notify that DoStop has been called
  stop_called_ = true;
  cv_.notify_one();
  std::future<void> ft_stop_server;
  if (policy == ShutdownPolicy::kForced) {
    // stop server immediately
    poco_http_server_->stopAll(true);
  } else if (policy == ShutdownPolicy::kGraceful) {
    // stop server when all connections finished
    poco_http_server_->stop();
  } else {
    log_.LogError() << "HttpServer::Stop: "
                    << "ERROR: Unkown Shutdown policy";
  }
  Task<void> stop_task(std::move(ft_stop_server));
  return stop_task;
}

void HttpServer::ObserveSubscriptions(const Function<SubscriptionHandlerType>& shnd,
                                      const Function<SubscriptionStateHandlerType>& sshnd) {
  shnd_ = shnd;
  sshnd_ = sshnd;
}

std::error_code HttpServer::GetError() const noexcept {
  std::error_code code;
  return code;
}

void HttpServer::ObserveError(const Function<void(std::error_code)>& hnd) {
  // TODO(hhz): implement observer error logic
  log_.LogDebug() << "HttpServer::ObserveError called to set handler: " << &hnd;
  log_.LogWarn() << "HttpServer::ObserveError is not implemented.";
}

const Function<RequestHandlerType> HttpServer::GetCustomRequestHandler() { return hnd_; }

const Function<SubscriptionHandlerType> HttpServer::GetCustomSubscriptionHandler() { return shnd_; }

const Function<SubscriptionStateHandlerType> HttpServer::GetCustomSubscriptionStateHandler() { return sshnd_; }

}  // namespace rest
}  // namespace ara
