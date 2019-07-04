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
/**        \file  httpclient.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_HTTPCLIENT_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_HTTPCLIENT_H_
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/WebSocket.h>
#include <vac/language/cpp14_backport.h>
#include <ara/log/logging.hpp>
#include <map>
#include <memory>
#include <vector>

#include "ara/rest/allocator.h"
#include "ara/rest/client.h"
#include "ara/rest/client_interface.h"
#include "ara/rest/client_types.h"
#include "ara/rest/config.h"

namespace ara {
namespace rest {
namespace test {
class HttpClientTest_ConvertReply_Test;  // fwd-decl
}  // namespace test

/**
 * TODO
 */
class HttpClient : public ClientInterface {
  friend class test::HttpClientTest_ConvertReply_Test;  /// for testing
 private:
  /**
   * \brief flag for stopping the thread below
   */
  bool perform_receiving_thread_ = true;
  /**
   * \brief thread which is receiving the messages of the server and is handling them
   */
  std::thread receiving_thread_;

  /**
     * \brief This Task is handling the subscription and is returning the event
     * \param uri
     * \param policy
     * \param time
     * \return event subscribed by the task
     */
  Event SubscribeTask(const Uri& uri, EventPolicy policy, duration_t time,
                      const Function<client::NotificationHandlerType>& notify,
                      const Function<client::SubscriptionStateHandlerType>& state);

 public:
  /**
   * TODO
   * \param config
   * \param allocator
   */
  explicit HttpClient(const String& config, Allocator* allocator = GetDefaultAllocator());

  /**
   * \brief destructor
   */
  ~HttpClient() override;

  /**
   * Send client request asynchronously
   * \param req
   * \return a task waiting for the server reply to be received.
   */
  Task<Pointer<Reply>> Send(const Request& req) override;

  /**
   * Requests a client shutdown
   * \param policy
   * \return
   */
  Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful) override;
  /**
   * \brief is stopping the websockets.
   */
  void stopWebsockets();

  /**
   * \brief send message using the websockets
   * \param message
   */
  void sendWithWebsocket(String message);

  /**
   * \brief Performs an event subscription
   *
   *  \param uri the event to subscribe to
   *  \param policy the notification policy
   *  \param time time bound as a parameter of the notification policy
   *  \param notify user-defined event notification handler function
   *  \param state user-define subscription state observer function
   *  \return a task waiting for the Event construction and subscription Reply.
   */
  Task<Event> Subscribe(const Uri& uri, EventPolicy policy, duration_t time,
                        const Function<client::NotificationHandlerType>& notify,
                        const Function<client::SubscriptionStateHandlerType>& state = {}) override;

  /**
   * \brief
   * \param new_event_ptr
   */
  void PropagateMovedPointer(Event* new_event_ptr);
  /**
   * \brief removes Event from map
   * \param event
   */
  void PropagateDeletedEvent(Event* event);

 private:
  /**
   * \brief is handling a received data frame
   * \param received_obj: the ogm::Object which was received
   */
  void HandleReceivedData(Pointer<ogm::Object> received_obj);
  /**
   * \brief is handling a received unsubscribe request
   * \param received_obj: the ogm::Object which was received
   */
  void HandleUnsubscribeReception(Pointer<ogm::Object> received_obj);
  /**
   * \brief is handling a received Status replay
   * \param received_obj: the ogm::Object which was received
   */
  void HandleStatusReply(Pointer<ogm::Object> received_obj);
  /**
   * \brief is generating the a std::vector with all the payloads received
   * \param received_frame: the frame received in the String format
   * \return a vector of the payloads as Strings
   */
  std::vector<String> GenerateStringVectorOfPayloads(String received_frame);
  /**
   * \brief is handling the received ogm::Object
   * \param received_obj object to handle
   */
  void HandleReceivedPayload(Pointer<ogm::Object>&& received_obj);

  /**
   * Converts an ara::rest::RequestMethod to poco request method representation
   * \param req_method RequestMethod to convert
   * \return String representation
   */
  String DeterminePocoHttpRequestMethod(RequestMethod req_method);

  /**
   * \brief thread receiving the frames
   * \param websocket
   */
  void receiveAndNotify(std::shared_ptr<Poco::Net::WebSocket> websocket);
  /**
   * convert poco response to ara::rest:Response
   * \param from
   * \param uri
   * \param obj_serialized
   * \returns a Pointer to the ara::rest:Reply
   */
  Pointer<Reply> ConvertResponse(const Poco::Net::HTTPResponse& from, const Uri& uri, std::istream& obj_serialized);

  /**
   * \brief Function to accomplish tasks to send requests asynchronously
   *  \return a reply pointer
   */
  Pointer<Reply> SendTask(const Request& req);
  /**
   * Instance of an Poco Http Session
   */
  Pointer<Poco::Net::HTTPClientSession> poco_http_session_;
  /**
   * \brief the websocket is used to process the events
   */
  std::shared_ptr<Poco::Net::WebSocket> websocket_ = nullptr;
  /**
   * \brief a map linking the instance of the event(class) to an uri(event of an message)
   */
  std::map<String, Event*> event_map_;
  /**
   * \brief
   */
  config::ClientBinding client_binding_;

  /**
   * \brief The logger instance
   */
  ara::log::Logger& log_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_HTTPCLIENT_H_
