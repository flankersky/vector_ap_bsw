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
/**        \file  server.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_H_

#include <ara/log/logging.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "ara/rest/client.h"
#include "ara/rest/endpoint.h"
#include "ara/rest/header.h"
#include "ara/rest/ogm/object.h"
#include "ara/rest/pointer.h"
#include "ara/rest/routing/pattern.h"
#include "ara/rest/server_event_interface.h"
#include "ara/rest/server_interface.h"
#include "ara/rest/server_reply_interface.h"
#include "ara/rest/server_types.h"
#include "ara/rest/task.h"
#include "ara/rest/types.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {

namespace ogm {
class Object;  //< fwd
}

class Request;        //< fwd
class ServerRequest;  //< fwd
class ServerReply;    //< fwd

class ServerInterface;  //< fwd pimpl

/**********************************************************************************************************************
 *  ServerEvent
 *********************************************************************************************************************/

/** \brief Represents the server side of an event subscription
 *
 *  This is a handle type that represents a single event subscription.
 *  In particular it owns all resources related to it. If it gets
 *  destroyed, the subscription is uniliterally and instantly terminated.
 *
 *  ServerEvent is copyable, copy-assignable, movable and move-assignable.
 */
class ServerEvent {
 public:
  /**
   * Default Constructor
   */
  ServerEvent() = default;

  /**
   * Constructor
   * \param sub_state
   * \param uri
   * \param server_event_interface
   */
  ServerEvent(SubscriptionState sub_state, Pointer<Uri> uri, Pointer<ServerEventInterface> server_event_interface);

  /**
   * Move Constrcutor
   * \param other
   */
  ServerEvent(ServerEvent&& other);

  /**
  * Move assignment
  * \param other
  * \return
  */
  ServerEvent& operator=(ServerEvent&& other);

  /**
   * Destructor
   */
  ~ServerEvent();

  ServerEvent(const ServerEvent&) = delete;             ///< Non-copyable
  ServerEvent& operator=(const ServerEvent&) = delete;  ///< Non-copy-assignable

  /** \brief Issues a change notification to its corresponding Server
   *
   *  Each server-side event has at least one corresponding client-side event.
   *  Notify does NOT notify these clients. It notifies its corresponding server
   *  of potential changes to the data referenced by the event (URI).
   *
   *  Depending on the EventPolicy, this may cause an actual notification to a client.
   *  Along with the notification a data payload is transmitted. The payload
   *  is exactly the result of a GET request to the Uri specified during
   *  subscription.
   *
   *  Notify returns a task that waits for the notification to complete.
   *  This can take a while since potentially a local GET request is issued and its payload
   *  is transmitted to a subscriber. The task yields true if the notification succeeded.
   *  A notification is successful if all actions that are triggered by it have been completed
   *  successfully.
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

  /** \brief Denotes the current subscription state
   *
   *  \return the current subscription state as perceived by the client
   */
  SubscriptionState GetSubscriptionState() const noexcept;

  /** \brief Returns the event Uri
   *
   *  \return the Uri corresponding to this event subscription
   */
  const Uri& GetUri() const noexcept;

  /**
   * Sets the event SubscriptionState
   * @param sstate
   */
  void SetSubscriptionState(SubscriptionState sstate);

  /**
   * Sets the event Uri
   * @param uri
   */
  void SetUri(Pointer<Uri> uri);

  /**
   * \brief sends data to the client via websocket
   *
   * Sends data via websocket to the coresponding client to notify about an event
   *
   * \param data
   */
  Task<void> Send(String data);

  /**
   * \brief confirms the event subscription from client
   * \return subscription confirmation task
   */
  Task<void> ConfirmSubscription();

  /**
   * \brief confirms the event subscription from client
   * \return unsubscription confirmation task
   */
  Task<void> ConfirmUnsubscription();

  /**
  * \brief confirms the event resubscription from client
  * \return resubscription confirmation task
  */
  Task<void> ConfirmResubscription();

  /**
   * \brief Tests events for equality
   *  \param event_a an event
   *  \param event_b an event
   *  \return true if a and b are equal
   */
  friend bool operator==(const ServerEvent& event_a, const ServerEvent& event_b) noexcept {
    bool result = false;
    if (event_a.GetUri().GetHost().compare(event_b.GetUri().GetHost()) == 0) {
      String uri_string_a = ToString(event_a.GetUri(), Uri::Part::Path);
      StringView str_a(STRING_TO_STRINGVIEW(uri_string_a));
      Pattern pat_a(str_a);

      String uri_string_b = ToString(event_b.GetUri(), Uri::Part::Path);
      StringView str_b(STRING_TO_STRINGVIEW(uri_string_b));
      Pattern pat_b(str_b);

      result = (pat_a == pat_b);
    }
    return result;
  }

  /**
   * \brief Tests events for inequality
   *  \param event_a an event
   *  \param event_b an event
   *   \return true if event_a less-than event_b
   */
  friend bool operator!=(const ServerEvent& event_a, const ServerEvent& event_b) noexcept {
    return !(event_a == event_b);
  }
  /**
   * \brief Tests events for their partial order
   *  \param event_a an event
   *  \param event_b an event
   *  \return true if a and b are unequal
   */
  friend bool operator<(const ServerEvent& event_a, const ServerEvent& event_b) noexcept {
    bool result = false;
    if (event_a.GetUri().GetHost().compare(event_b.GetUri().GetHost()) == 0) {
      String uri_string_a = ToString(event_a.GetUri(), Uri::Part::Path);
      StringView str_a(STRING_TO_STRINGVIEW(uri_string_a));
      Pattern pat_a(str_a);

      String uri_string_b = ToString(event_b.GetUri(), Uri::Part::Path);
      StringView str_b(STRING_TO_STRINGVIEW(uri_string_b));
      Pattern pat_b(str_b);

      result = (pat_a < pat_b);
    }
    return result;
  }
  /**
   * \brief propagates that the server is stoped or at least the websocket connection is closed
   */
  void StopPeriodicNotification();

 private:
  /**
   * \brief subscription state
   */
  SubscriptionState subscription_state_;

  /**
   * \brief Uri pointer
   */
  Pointer<Uri> uri_;

  /**
   * \brief server event interface pointer
   */
  Pointer<ServerEventInterface> server_event_interface_;

  /**
  * \brief The logger instance
  */
  ara::log::Logger& log_;
};

/**********************************************************************************************************************
 *  ServerRequest
 *********************************************************************************************************************/

/**
 * \brief Represents the server-side of a request issued by a client.
 *
 * A ServerRequest owns all context data related to this request. ServerRequests cannot be
 * instantiated by the user but are created by a server and provided to the user.
 * Constructors are implementation-defined.
 *
 */
class ServerRequest {
 public:
  explicit ServerRequest(const ServerRequest&) = delete;    ///< Non-copyable
  ServerRequest& operator=(const ServerRequest&) = delete;  ///< Non-copy-assignable

  /**
   * \brief Default Constructor
   */
  ServerRequest() = default;

  /**
   * \brief Constructor
   * \param request_header
   * \param object
   */
  ServerRequest(Pointer<RequestHeader> request_header, Pointer<ogm::Object> object);

  /**
   * \brief Provides access to the message header
   *  Requests the message header from the endpoint.
   *  Accessing the message header is always synchronous.
   *  \return a reference to a RequestHeader
   */
  RequestHeader const& GetHeader() const;
  /**
   * \brief Obtains the request message payload
   *  Asynchronous request for the message payload.
   *  \return returns a task waiting for the message payload to be received.
   */
  Task<ogm::Object const&> GetObject() const;

  /**
   * \brief Helpfer function to asynchronously get the member object_
   * \return
   */
  ogm::Object const& GetObjectTask() const;

  /**
   * \brief Obtains the reply message payload
   *  Asynchronous request for the message payload. A binding may delay reading and parsing
   *  of a message payload until explicitly requested.
   *  As opposed to GetObject() this function transfers ownership of the payload to the user.
   *  \return returns a task waiting for the message payload to be received.
   */
  Task<Pointer<ogm::Object>> ReleaseObject();

  /**
   * \brief Helpfer function to asynchronously release the member object_ (message payload)
   * \return
   */
  Pointer<ogm::Object> ReleaseObjectTask();

  /**
   * \brief Sets the linked RequestHeader
   * \param request_header
   */
  void SetHeader(Pointer<RequestHeader> request_header);

  /**
   * \brief Sets the linked ogm::Object
   * \param object
   */
  void SetObject(Pointer<ogm::Object> object);

 private:
  /**
   * \brief Pointer to the RequestHeader
   */
  Pointer<RequestHeader> request_header_;
  /**
   * \brief Pointer to the ogm::Object
   */
  Pointer<ogm::Object> object_;
};

/**********************************************************************************************************************
 *  ServerReply
 *********************************************************************************************************************/

/** \brief Represents the server-side of a reply issued back to a client.
 *
 * A ServerReply owns all context data related to this reply. ServerReply
 * cannot be instantiated by the user but is created by a server and provided to the user.
 * Constructors are implementation-defined.
 *
 *
 */
class ServerReply {
 public:
  explicit ServerReply(const ServerReply&) = delete;    ///< Non-copyable
  ServerReply& operator=(const ServerReply&) = delete;  ///< Non-copy-assignable

  /**
   * \brief Default Constructor
   */
  ServerReply() = default;

  /**
   * \brief Constructor
   * \param reply_header
   * \param object
   */
  ServerReply(Pointer<ReplyHeader> reply_header, Pointer<ogm::Object> object);

  /**
   * \brief Contstructor
   * \param reply_header
   * \param object
   * \param server_reply_interface
   */
  ServerReply(Pointer<ReplyHeader> reply_header, Pointer<ogm::Object> object,
              ServerReplyInterface* server_reply_interface);

  /**
   * \brief Provides access to the reply message header
   * \return a reference to a RequestHeader
   */
  ReplyHeader& GetHeader();

  /**
   * \brief Returns the Pointer to the corrospending ogm::Object
   * \return
   */
  Pointer<ogm::Object> GetObject();

  /** \brief Send a reply to the peer that has issued the request
   *
   *  If this function is not invoked explicitly, the endpoint will transmit a default reply.
   *  If Redirect() has been before called, these functions must be used.
   *
   * \param data payload to be transmitted
   * \return a task waiting for the transmission to complete
   */
  Task<void> Send(const Pointer<ogm::Object>& data = {});

  /** \brief Send a reply to the peer that has issued the request
   *
   *  Same a other Send(), only with move semantics
   *
   * \param data payload to be transmitted
   * \return a task waiting for the transmission to complete
   */
  Task<void> Send(Pointer<ogm::Object>&& data);

  /** \brief Issues a redirect command to the connected client
   *
   * Must not be called after Send().
   *
   * \param uri location to redirect to
   */
  Task<void> Redirect(const Uri& uri);

  /**
   * \brief Sets the linkes ReplyHeader
   * \param reply_header
   */
  void SetHeader(Pointer<ReplyHeader> reply_header);

  /**
   * \brief Sets the linkes ServerReplyInterface, which determines which ServerReply Object is to be used (e.g.
   * HTTPServerReply)
   * \param server_reply_interface
   */
  void SetServerReplyInterface(ServerReplyInterface* server_reply_interface);

 private:
  /**
   * \brief Pointer to the ReplyHeader
   */
  Pointer<ReplyHeader> reply_header_;
  /**
   * \brief Pointer to the Object
   */
  Pointer<ogm::Object> object_;

  /**
   * \brief Assosicated ServerReplyInterface which determines which ServerReply Object is to be used (e.g.
   * HTTPServerReply)
   */
  ServerReplyInterface* server_reply_interface_;
};

/**********************************************************************************************************************
 *  Server
 *********************************************************************************************************************/

/** \brief Server class to be instantiated by an application
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */
class Server {
 public:
  Server(const Server&) = delete;             ///< Server is non-copy-constructible
  Server& operator=(const Server&) = delete;  ///< Server is non-copy-assignable

  /** \brief Constructs a server
   *
   *  \param conf identifier for a corresponding configuration record
   *  \param hnd request handler function
   *  \param alloc allocator for dynamic memory
   */
  Server(const String& conf, const Function<RequestHandlerType>& hnd, Allocator* alloc = GetDefaultAllocator());

  /** \brief Instruct a server to begin serving clients.
   *
   *  A server does not serve anything unless Start() is invoked. A server
   *  can be started within the execution context of the caller or within its
   *  own execution context (usually this is a thread). If StartupPolicy::kAttached,
   *  then Start() blocks its caller and only returns of Stop() is called.
   *  If Startuppolicy::kDetached, Start() does not block its caller but returns
   *  a task for synchronization. The caller may wait on the task, which blocks
   *  until Stop() is invoked.
   *
   *  \param policy denotes whether caller is blocked or not.
   *  \return a task waiting for Stop() to be invoked
   */
  Task<void> Start(StartupPolicy policy = StartupPolicy::kDetached);

  /** \brief Instructs a server to stop serving clients
   *
   *  A client can be stopped either as fast as possible or "gracefully".
   *  If ShutdownPolicy::kForced then all connections are terminates instantly
   *  and all ongoing processes shall be terminated as fast as possible.
   *  If ShutdownPolicy::kGraceful then a server will stop accepting new
   *  requests but will wait until all requests have been served.
   *
   *  \param policy denotes how server is stopped.
   *  \return return type
   */
  Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful);

  /** \brief Registers a user-defined subscription handler
   *
   * A server can handle event subscriptions by default. Unless a user-defined
   * handler function is registered explicitly, event subscriptions are not visible
   * to the user. This implies that subscriptions with EventPolicy::kTriggered
   * never receive notifications.
   *
   * A user-defined subscription handler function will be called for each subscription
   * and is passed a ServerEvent which uniquely identifies the subscription.
   *
   * The user-defined handler function is not invoked if the server considers
   * the subscription invalid.
   *
   * A user-defined subscription state handler function may be provided
   * which is called every time the state of a subscription changes. This function
   * is passed the correponding ServerEvent and its new state.
   * As opposed to RegisterSubscriptionHandler(), this provided function is
   * called for every state change.
   *
   * \param shnd a subscription handler function
   * \param sshnd a subscription state handler function
   */
  void ObserveSubscriptions(const Function<SubscriptionHandlerType>& shnd,
                            const Function<SubscriptionStateHandlerType>& sshnd);

  /** \brief Obtain server status
   * \return a reference to the server Status
   */
  std::error_code GetError() const noexcept;

  /**
   * \brief Sets the server status
   * \param error
   */
  void SetError(std::error_code error);

  /** \brief Observe status changes
   *
   *  \param hnd user-defined handler function to to called on status changes
   */
  void ObserveError(const Function<void(std::error_code)>& hnd);

 private:
  /**
   * \brief Stops all Server Bindings listed in member server_list_ with associated stop functions
   * \param policy
   * \return
   */
  void StopAllServerBindings(ShutdownPolicy policy);

  /**
   * \brief Starts all Server Bindings listed in member server_list_ with associated start functions
   * \param policy
   */
  void StartAllServerBindings(StartupPolicy policy);

  /**
   * \brief Contains all known server bindings created by ServerFactory
   */
  std::vector<std::unique_ptr<ServerInterface>> server_list_;

  /**
   * \brief Client error
   */
  std::error_code error_;

  /**
   * \brief Error observer function
   */
  Function<void(std::error_code)> error_observer_;

  /**
   * \brief The logger instance
   */
  ara::log::Logger& log_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_H_
