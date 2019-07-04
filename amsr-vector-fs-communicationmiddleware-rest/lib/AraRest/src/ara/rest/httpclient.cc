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
/**        \file  httpclient.cc
 *        \brief  Implementation of the HttpClient binding for ara::rest
 *
 *      \details  Implementation of the HttpClient binding for ara::rest
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <iostream>
#include <sstream>
#include <string>

#include "Poco/Net/NetException.h"
#include "ara/rest/config.h"
#include "ara/rest/error_code.h"
#include "ara/rest/exception.h"
#include "ara/rest/httpclient.h"
#include "ara/rest/serialize/serialize.h"

namespace ara {
namespace rest {

String HttpClient::DeterminePocoHttpRequestMethod(RequestMethod req_method) {
  String result;
  if (req_method == RequestMethod::kGet) {
    result = Poco::Net::HTTPRequest::HTTP_GET;
  } else if (req_method == RequestMethod::kPost) {
    result = Poco::Net::HTTPRequest::HTTP_POST;
  } else if (req_method == RequestMethod::kPut) {
    result = Poco::Net::HTTPRequest::HTTP_PUT;
  } else if (req_method == RequestMethod::kDelete) {
    result = Poco::Net::HTTPRequest::HTTP_DELETE;
  } else if (req_method == RequestMethod::kOptions) {
    result = Poco::Net::HTTPRequest::HTTP_OPTIONS;
  } else {
    result = "";
  }
  return result;
}
HttpClient::~HttpClient() { stopWebsockets(); }

void HttpClient::stopWebsockets() {
  // if a websocket was created
  if (websocket_ != nullptr) {
    log_.LogDebug() << "stopWebsockets: stopping the web sockets of the HttpClient:";
    // stop the thread receiving the frames and is notifying the application
    perform_receiving_thread_ = false;
    receiving_thread_.join();
    // send the connection close frame to the server
    String test = "{}";
    websocket_->sendFrame(test.data(), static_cast<int>(test.size()), 8);
    websocket_ = nullptr;
  }
}

HttpClient::HttpClient(const String& config, Allocator* allocator)
    : log_(ara::log::CreateLogger("42", "ara::rest httpclient")) {
  (void)allocator;

  log_.LogDebug() << "HttpClient: Start HTTP Client Binding with config: " << config;
  // Get configuration
  static config::ConfigurationHandler& config_handler = config::ConfigurationHandler::Instance();
  config::ClientBinding client_binding_ = config_handler.GetClientBinding(config_handler.GetClientBindingIds(config));

  poco_http_session_ = vac::language::make_unique<Poco::Net::HTTPClientSession>();

  if (client_binding_.has_proxy_) {
    try {
      poco_http_session_->setProxy(client_binding_.proxy_.Adress_, static_cast<uint16_t>(client_binding_.proxy_.Port_));
      if (client_binding_.proxy_.Has_credentials_) {
        poco_http_session_->setProxyCredentials(client_binding_.proxy_.Username_, client_binding_.proxy_.Password_);
      } else {
        log_.LogInfo() << "HttpClient: No proxy credentials given.";
      }
      log_.LogDebug() << "HttpClient: SetProxy: " << client_binding_.proxy_.Adress_ << ":"
                      << static_cast<uint16_t>(client_binding_.proxy_.Port_)
                      << " TransportProtocol: " << client_binding_.transport_protocol_;
    } catch (Poco::IllegalStateException&) {
      SetError(std::error_code{static_cast<int>(ErrorCode::kDevelopmentError), std::generic_category()});
    }
  }
}

Task<Pointer<Reply>> HttpClient::Send(const Request& req) {
  // Create packaged task for SendTask.
  // packaged_task passes the parameters by value to the "function". So to pass reference we need to wrap the variable
  // you are passing  with std::ref or std::cref
  std::packaged_task<Pointer<Reply>(const Request&)> send_ptask(std::bind(&HttpClient::SendTask, this, std::cref(req)));
  std::future<Pointer<Reply>> send_ptask_ft = send_ptask.get_future();
  Task<Pointer<Reply>> send_task(std::move(send_ptask_ft));

  // Call packaged task.
  send_ptask(std::move(req));
  return send_task;
}

Pointer<Reply> HttpClient::SendTask(const Request& req) {
  String host_data;
  STRINGVIEW_TO_STRING(host_data, req.GetUri().GetHost());
  log_.LogDebug() << "Send: RequestMethod: " << static_cast<uint32_t>(req.GetRequestMethod())
                  << " to host: " << host_data << ":" << static_cast<uint16_t>(req.GetUri().GetPort());

  const bool object_present{!req.GetObject()->IsEmpty()};  // TODO(x) : Switch back to HasObject when HasObject function
                                                           // is fixed. ATM: HasObject returns true every time
  String obj_serialized;
  if (object_present) {
    obj_serialized = serialize::Serializer::OgmToJson(std::move(req.GetObject()));
  }

  String host_str;
  STRINGVIEW_TO_STRING(host_str, req.GetUri().GetHost());
  /* Configure Transmission */
  if (poco_http_session_->getHost() != host_str) {  // TODO(x): remove this when each requests has its own session
    poco_http_session_->setHost(host_data);
  }

  if (poco_http_session_->getPort() !=
      req.GetUri().GetPort()) {  // TODO(x): remove this when each requests has its own session
    poco_http_session_->setPort(static_cast<uint16_t>(req.GetUri().GetPort()));
  }

  Poco::Net::HTTPRequest poco_request(
      DeterminePocoHttpRequestMethod(req.GetRequestMethod()), ToString(req.GetUri(), Uri::Part::PathAndQuery),
      Poco::Net::HTTPMessage::HTTP_1_1);  // TODO(x): use client_binding_.transport_protocol_;

  if (object_present) {
    poco_request.setContentLength(obj_serialized.size());
    poco_request.setContentType("application/json");
  } else {
    poco_request.setContentLength(0);
  }
  poco_request.setKeepAlive(true);

  try {
    /* Transmit */
    std::ostream& poco_request_payload =
        poco_http_session_->sendRequest(poco_request);  // sends request, returns open stream

    if (object_present) {
      poco_request_payload << obj_serialized;  // sends the serialized object
    }

    if (poco_request_payload.bad() || poco_request_payload.fail()) {
      SetError(std::error_code{static_cast<int>(ErrorCode::kNetworkError), std::generic_category()});
      poco_http_session_->reset();
    } else {
      /* Response */
      Poco::Net::HTTPResponse poco_response;
      try {
        std::istream& poco_response_payload = poco_http_session_->receiveResponse(poco_response);
        if (poco_response_payload.bad() || poco_response_payload.fail()) {
          SetError(std::error_code{static_cast<int>(ErrorCode::kNetworkError), std::generic_category()});
          poco_http_session_->reset();
        } else {
          // convert poco response to ara::rest:Reply.
          Pointer<Reply> return_value(ConvertResponse(poco_response, req.GetUri(), poco_response_payload));
          return return_value;
        }
      } catch (Poco::Exception&) {
        /* catch poco network exceptions */
        SetError(std::error_code{static_cast<int>(ErrorCode::kNetworkError), std::generic_category()});
      }
    }
  } catch (Poco::Net::ConnectionRefusedException e) {
    log_.LogError() << e.what();
    SetError(std::error_code{static_cast<int>(ErrorCode::kNetworkError), std::generic_category()});
  }

  /* failed getting correct reply back */
  return Pointer<Reply>();
}

Task<void> HttpClient::Stop(ShutdownPolicy policy) {
  std::future<void> ft_stop_client;
  if (policy == ShutdownPolicy::kForced) {
    log_.LogDebug() << "DoStop: Stop HTTP Client Binding with policy forced";
    poco_http_session_->reset();
  } else if (policy == ShutdownPolicy::kGraceful) {
    log_.LogDebug() << "DoStop: Stop HTTP Client Binding with policy graceful";
    poco_http_session_->reset();
  } else {
    log_.LogDebug() << "DoStop: ShutdownPolicy unknown";
  }
  // stops the websocket connection
  stopWebsockets();

  Task<void> stop_client(std::move(ft_stop_client));
  return stop_client;
}
void HttpClient::PropagateMovedPointer(Event* new_event_ptr) {
  // check if the URI is added top the event_map
  // and set the event pointer to the new one
  if (event_map_.find(ToString(new_event_ptr->GetUri())) != event_map_.end()) {
    event_map_[ToString(new_event_ptr->GetUri())] = new_event_ptr;
  }
}
void HttpClient::PropagateDeletedEvent(Event* event) {
  // check if the URI is added top the event_map
  if (event_map_.find(ToString(std::move(event->GetUri()))) != event_map_.end()) {
    // check if the event pointer is the same
    if (event_map_[ToString(event->GetUri())] == event) {
      // erase the entry of the map
      event_map_.erase(ToString(event->GetUri()));
    }
  }
}

void HttpClient::sendWithWebsocket(String message) {
  char buffer[1024] = {0};
  std::copy(message.begin(), message.end(), buffer);
  websocket_->sendFrame(buffer, 1024, 0);
}

void HttpClient::HandleStatusReply(Pointer<ogm::Object> received_obj) {
  ogm::Object::Iterator it_event = received_obj->Find(CSTRING_TO_STRINGVIEW("event"));
  String event;
  StringView event_view(ogm::details::cast<ogm::String>(&(*it_event).GetValue())->GetValue());
  STRINGVIEW_TO_STRING(event, event_view);
  ogm::Object::Iterator it_type = received_obj->Find(CSTRING_TO_STRINGVIEW("type"));
  String type;
  StringView type_view(ogm::details::cast<ogm::String>(&(*it_type).GetValue())->GetValue());
  STRINGVIEW_TO_STRING(type, type_view);
  ogm::Object::Iterator it_status = received_obj->Find(CSTRING_TO_STRINGVIEW("status"));
  String status;
  StringView staus_view = ogm::details::cast<ogm::String>(&(*it_status).GetValue())->GetValue();
  STRINGVIEW_TO_STRING(status, staus_view);

  // check for status "ok"
  if (status.compare("ok") == 0) {
    // check for subscription or reauthorization frame
    if (type.compare("subscribe") == 0 || type.compare("reauthorize") == 0) {
      // subscribe or reauthorizes frames received
      event_map_[event]->setSubscribtionState(SubscriptionState::kSubscribed);
    } else if (type.compare("unsubscribe") == 0) {
      // unsubscribe frame received
      event_map_[event]->setSubscribtionState(SubscriptionState::kCanceled);
    } else {
      // if the type is not un-/subscribe or reauthorize
      // but the key status contains "ok" the frame is strange
      // therefore the subscriptionstate is set to invalid
      event_map_[event]->setSubscribtionState(SubscriptionState::kInvalid);
    }
  } else {
    // if the status is not "ok" the subscriptionstate is set to invalid
    event_map_[event]->setSubscribtionState(SubscriptionState::kInvalid);
  }
}

void HttpClient::HandleUnsubscribeReception(Pointer<ogm::Object> received_obj) {
  ogm::Object::Iterator it_event = received_obj->Find(CSTRING_TO_STRINGVIEW("event"));
  String event;
  StringView event_view(ogm::details::cast<ogm::String>(&(*it_event).GetValue())->GetValue());
  STRINGVIEW_TO_STRING(event, event_view);
  event_map_[event]->setSubscribtionState(SubscriptionState::kCanceled);
  Pointer<ogm::Object> objNode =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("unsubscribe"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(event)))),
                        ogm::Field::Make("status", ogm::String::Make(CSTRING_TO_STRINGVIEW("OK"))));
  String json_for_unsubscription = serialize::Serializer::OgmToJson(objNode);
  char buffer[1024] = {0};
  std::copy(json_for_unsubscription.begin(), json_for_unsubscription.end(), buffer);
  websocket_->sendFrame(buffer, 1024, 0);
}

void HttpClient::HandleReceivedData(Pointer<ogm::Object> received_obj) {
  ogm::Object::Iterator it_event = received_obj->Find(CSTRING_TO_STRINGVIEW("event"));
  String event;
  StringView event_view(ogm::details::cast<ogm::String>(&(*it_event).GetValue())->GetValue());
  STRINGVIEW_TO_STRING(event, event_view);
  if (received_obj->HasField(CSTRING_TO_STRINGVIEW("timestamp"))) {
    // data frame received
    // the notifier function of the application will be called
    event_map_[event]->GetNotify()(*received_obj);
  } else if (received_obj->HasField(CSTRING_TO_STRINGVIEW("code"))) {
    // error frame received
    // the notifier function of the application will be called
    event_map_[event]->GetNotify()(*received_obj);
  } else {
    // all other cases leads to unknown message type
    log_.LogError() << "receiveAndNotify Received unknown message type";
  }
}

void HttpClient::HandleReceivedPayload(Pointer<ogm::Object>&& received_obj) {
  // check for the key type and the key event in the frame to make sure it is a valid frame
  if (received_obj->HasField(CSTRING_TO_STRINGVIEW("type")) && received_obj->HasField(CSTRING_TO_STRINGVIEW("event"))) {
    // safe the value of the type and the event in an variable
    ogm::Object::Iterator it_event = received_obj->Find(CSTRING_TO_STRINGVIEW("event"));
    String event;
    StringView event_view(ogm::details::cast<ogm::String>(&(*it_event).GetValue())->GetValue());
    STRINGVIEW_TO_STRING(event, event_view);
    ogm::Object::Iterator it_type = received_obj->Find(CSTRING_TO_STRINGVIEW("type"));
    String type;
    StringView type_view(ogm::details::cast<ogm::String>(&(*it_type).GetValue())->GetValue());
    STRINGVIEW_TO_STRING(type, type_view);
    // check if the eventuri is contained by the map
    if (event_map_.find(event) != event_map_.end()) {
      if (event.compare(ToString(event_map_[event]->GetUri())) == 0) {
        // cfor server replay to un-/subscribe or reauthorize request
        if (received_obj->HasField(CSTRING_TO_STRINGVIEW("status"))) {
          HandleStatusReply(std::move(received_obj));
          // if no status in included but the message type is unsubscribe the received frame
          // means the server is not providing further informations to the event
          // Therefore the state becomes canceled and the replay with status "ok" gets send to the server
        } else if (type.compare("unsubscribe") == 0) {
          // unsubscribe frame received
          // the server isn't providing any further informations to this event
          // there for the reply with status "ok" will be send
          // and the subscription state will be canceled
          HandleUnsubscribeReception(std::move(received_obj));
        } else if (received_obj->HasField(CSTRING_TO_STRINGVIEW("data"))) {
          HandleReceivedData(std::move(received_obj));
        } else {
          // all other cases leads to unknown message type
          log_.LogError() << "receiveAndNotify Received unknown message type";
        }
      } else {
        // no event registered to the given event by the message
        log_.LogError() << "receiveAndNotify Event is not in the map of known events";
      }
    }
  } else {
    // frame kind is not known
    log_.LogError() << "receiveAndNotify Unknown frame received";
  }
}

std::vector<String> HttpClient::GenerateStringVectorOfPayloads(String received_frame) {
  // the delimiter the payloads are separated with
  const String kDelimiter = "\n";
  size_t pos;
  // a vector of all the received payloads with in this frame
  std::vector<String> received_payloads = std::vector<String>();
  // split all the payloads and add them to the vector defined before
  while ((pos = received_frame.find(kDelimiter)) != std::string::npos) {
    String payload = received_frame.substr(0, pos);
    received_payloads.push_back(payload);
    std::cout << payload << std::endl;
    received_frame.erase(0, pos + kDelimiter.length());
  }
  // add the last payload to the vector
  // necessary if there is no terminating \n at the end of the frame
  if (received_frame.size() > 0) {
    received_payloads.push_back(received_frame);
  }
  return received_payloads;
}

void HttpClient::receiveAndNotify(std::shared_ptr<Poco::Net::WebSocket> websocket) {
  while (perform_receiving_thread_) {
    if (websocket->available() > 0) {
      int flags;
      char receive_buff[1024] = {0};
      websocket->receiveFrame(receive_buff, sizeof(receive_buff), flags);
      if ((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CLOSE) {
        perform_receiving_thread_ = false;
      }
      // create string from received frame
      String received_frame = String(receive_buff);
      std::vector<String> received_payloads = GenerateStringVectorOfPayloads(received_frame);
      // looping the vector to handle all payloads
      for (std::vector<String>::iterator it = received_payloads.begin(); it != received_payloads.end(); ++it) {
        // convert the payload to an object
        Pointer<ogm::Object> received_obj = (serialize::Serializer::JsonToOgm(*it));

        HandleReceivedPayload(std::move(received_obj));
      }
    }
  }
}

Event HttpClient::SubscribeTask(const Uri& uri, EventPolicy policy, duration_t time,
                                const Function<client::NotificationHandlerType>& notify,
                                const Function<client::SubscriptionStateHandlerType>& state) {
  if (websocket_ == nullptr) {
    // if there is no websocket created yet it will be created one
    // therefore each client has just one websocket
    try {
      Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/", Poco::Net::HTTPMessage::HTTP_1_1);
      Poco::Net::HTTPResponse response;
      String host_str;
      STRINGVIEW_TO_STRING(host_str, uri.GetHost());
      poco_http_session_->setHost(host_str);
      poco_http_session_->setPort(static_cast<uint16_t>(uri.GetPort()));
      Poco::Net::WebSocket m_psock(*poco_http_session_, request, response);
      websocket_ = vac::language::make_unique<Poco::Net::WebSocket>(m_psock);
      websocket_->setReceiveTimeout(Poco::Timespan(10, 0, 0, 0, 0));
      event_map_ = std::map<String, Event*>();
      // start data reception
      receiving_thread_ = std::thread(std::bind(&HttpClient::receiveAndNotify, this, websocket_));
    } catch (Poco::Net::ConnectionRefusedException e) {
      log_.LogError() << e.what();
      SetError(std::error_code{static_cast<int>(ErrorCode::kNetworkError), std::generic_category()});
    }
  }
  Uri uri_for_map(uri);
  if (event_map_.find(ToString(uri)) != event_map_.end()) {
    // if the URI is already added to the map add # + map.size()
    std::stringstream new_uri;
    new_uri << ToString(uri) << "#" << event_map_.size();
    uri_for_map = Uri::Builder(StringView(STRING_TO_STRINGVIEW(new_uri.str()))).ToUri();
  }

  String uri_str = ToString(uri_for_map);
  // it will be added
  Event result(uri_for_map, notify, this, state);
  event_map_.insert(std::make_pair(uri_str, &result));
  Pointer<ogm::Object> objNode;

  String time_count = std::to_string(time.count());
  // create the subscription request
  if (policy == EventPolicy::kPeriodic) {
    // set the given time to the key "interval"
    objNode =
        ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("subscribe"))),
                          ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                          ogm::Field::Make("interval", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(time_count)))),
                          ogm::Field::Make("Authorization", ogm::String::Make(CSTRING_TO_STRINGVIEW("<token>"))));
  } else {
    // set the given time to the key "updatelimit"
    objNode = ogm::Object::Make(
        ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("subscribe"))),
        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
        ogm::Field::Make("updatelimit", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(time_count)))),
        ogm::Field::Make("Authorization", ogm::String::Make(CSTRING_TO_STRINGVIEW("<token>"))));
  }
  // convert the ogm::Object to a String
  String json_for_subscription = serialize::Serializer::OgmToJson(objNode);
  // create char[] from the String
  char buffer[1024] = {0};
  std::copy(json_for_subscription.begin(), json_for_subscription.end(), buffer);
  // send the frame by the websocket
  websocket_->sendFrame(buffer, 1024, 0);
  return result;
}

Task<Event> HttpClient::Subscribe(const Uri& uri, EventPolicy policy, duration_t time,
                                  const Function<client::NotificationHandlerType>& notify,
                                  const Function<client::SubscriptionStateHandlerType>& state) {
  // create the assync task
  std::packaged_task<Event()> subscribe_ptask(
      std::bind(&HttpClient::SubscribeTask, this, std::cref(uri), policy, time, notify, state));
  Task<Event> subscribe_ft(subscribe_ptask.get_future());
  // execute the created task assync
  subscribe_ptask();
  return subscribe_ft;
}

Pointer<Reply> HttpClient::ConvertResponse(const Poco::Net::HTTPResponse& from, const Uri& uri,
                                           std::istream& obj_serialized) {
  // Construct the ReplyHeader
  ReplyHeader header;
  header.SetStatus(from.getStatus());
  header.SetUri(uri);

  // Deserialize the ogm payload
  std::istreambuf_iterator<char> eos;
  std::string str(std::istreambuf_iterator<char>(obj_serialized), eos);
  Pointer<ogm::Object> ogm = serialize::Serializer::JsonToOgm(str);

  // Create the reply
  Pointer<Reply> rep = vac::language::make_unique<Reply>(header, std::move(ogm));
  return rep;
}

}  // namespace rest
}  // namespace ara
