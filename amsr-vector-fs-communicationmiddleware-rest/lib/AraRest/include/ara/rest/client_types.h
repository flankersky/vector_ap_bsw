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
/**        \file  client_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_TYPES_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iostream>
#include <memory>
#include <utility>

#include "ara/rest/endpoint.h"
#include "ara/rest/function.h"
#include "ara/rest/header.h"
#include "ara/rest/ogm/copy.h"
#include "ara/rest/ogm/object.h"
#include "ara/rest/pointer.h"
#include "ara/rest/task.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {

class HttpClient;  // fwd-decl

namespace ogm {
class Object;  // fwd-decl
}  // namespace ogm

class Event;  // fwd-decl
class Reply;  // fwd-decl

namespace client {

/// Denotes a callback function for notifications
using NotificationHandlerType = void(const ogm::Object&);

/// Denotes a callback to call if subscription status changes
using SubscriptionStateHandlerType = void(const Event&, SubscriptionState);
}  // namespace client

/** \brief Represents a client-side request message
 *
 * A request encapsulates all context information related to a client-side request
 * to be transmited to a server. A request is composed of at least a RequestMethod
 * and an Uri. Optionally a request payload in form of an object graph may be provided.
 *
 */
class Request {
 public:
  Request(const Request&) = delete;             ///< Non-copyable
  Request& operator=(const Request&) = delete;  ///< Non-copy-assignable

  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   */
  Request(RequestMethod met, const Uri& uri);
  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   */
  Request(RequestMethod met, Uri&& uri);
  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   * \param obj data payload of request message
   */
  Request(RequestMethod met, const Uri& uri, const Pointer<ogm::Object>& obj);
  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   * \param obj data payload of request message
   */
  Request(RequestMethod met, const Uri& uri, Pointer<ogm::Object>&& obj);
  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   * \param obj data payload of request message
   */
  Request(RequestMethod met, Uri&& uri, const Pointer<ogm::Object>& obj);
  /** \brief Constructs a Request
   *
   * \param met one of RequestMethod
   * \param uri a Uri
   * \param obj data payload of request message
   */
  Request(RequestMethod met, Uri&& uri, Pointer<ogm::Object>&& obj);

  /** \brief Return RequestMethod
   *
   * \return a RequestMethod
   */
  RequestMethod GetRequestMethod() const { return met_; }

  /** \brief Return Uri
   *
   * \return a Uri
   */
  Uri GetUri() const { return uri_; }

  /**
   * \brief Get a copy of ogm
   */
  Pointer<ogm::Object> GetObject() const { return ogm::Copy(obj_); }

  /**
   * \brief Has Object
   * TODO(x): HasObject The method is currently useless because this class creates dummy OGMs even with no payload
   */
  bool HasObject() const noexcept {
    bool retval{false};
    if (obj_) {
      retval = true;
    }
    return retval;
  }

 private:
  /**
   * \brief The RequestMethod
   */
  const RequestMethod met_;

  /**
   * \brief The Uri
   */
  const Uri uri_;

  /**
   * \brief The Uri
   */
  Pointer<ogm::Object> obj_;
};

/** \brief Represents a client-side reply message
 *
 * A reply encapsulates all context information related to a client-side reply
 * received from a server. A reply is composed of a message header and a message payload.
 *
 */
class Reply {
 public:
  Reply() = default;                                              ///< default, TODO(x):delte me
  explicit Reply(const ReplyHeader& header) : header_(header) {}  ///< Constructor
  explicit Reply(const ReplyHeader& header, Pointer<ogm::Object> obj)
      : header_(header), obj_(std::move(obj)) {}  ///< Constructor
  Reply(const Reply&) = delete;                   ///< Non-copyable
  Reply& operator=(const Reply&) = delete;        ///< Non-copy-assignable

  /** \brief Obtains the message header
   *
   *  Requests the message header from the endpoint.
   *  Accessing the message header is always synchronous.
   *
   *  \return a reference to a ReplyHeader
   */
  ReplyHeader const& GetHeader() const;
  /** \brief Obtains the reply message payload
   *
   *  Asynchronous request for the message payload. A binding may delay reading and parsing.
   *  of a message payload until explicitly requested.
   *  \return returns a task waiting for the message payload to be received.
   */
  Task<ogm::Object const&> GetObject() const;  // deserializes lazily

  /** \brief Obtains the reply message payload
   *
   *  Asynchronous request for the message payload. A binding may delay reading and parsing
   *  of a message payload until explicitly requested.
   *  As opposed to GetObject() this function transfers ownership of the payload to the user.
   *  \return returns a task waiting for the message payload to be received.
   */
  Task<Pointer<ogm::Object>> ReleaseObject();  // take ownership

 private:
  /**
   * \brief The ReplyHeader
   */
  ReplyHeader header_;

  /**
   * \brief The payload of the Reply
   */
  Pointer<ogm::Object> obj_;
};

/** \brief Represents the client-side of a logical event subscription
 *
 *  An Event represents the client-side of a logical event subscription.
 *  An instance of this object owns all resourced necessary to maintain
 *  a logical notification channel.
 *  An event can only be created by means of Client::Subscribe() and is
 *  permanently bound to a user-defined handler function for notification.
 *
 *  Event is copyable, copy-assignable, movable and move-assignable.
 */
class Event {
 public:
  /**
   * \brief Move Constructor
   * \param other
   */
  Event(Event&& other);

  /**
   * \brief move assignment
   *  \param other
   *  \return new_event
   */
  Event& operator=(Event&& other);
  /**
   * \brief destructor
   */
  ~Event();

  /**
   * \brief constructor
   * \param uri
   * \param notify
   * \param client_ptr
   * \param state_notifier
   *
   */
  explicit Event(Uri uri, const Function<client::NotificationHandlerType>& notify, HttpClient* client_ptr,
                 const Function<client::SubscriptionStateHandlerType> state_notifier);

  Event(const Event&) = delete;             ///< Non-copyable
  Event& operator=(const Event&) = delete;  ///< Non-copy-assignable

  /** \brief Cancels an event subscription by issuing a cancelation request
   *
   * A subscription can also be terminated (but not canceled) by destroying the correspond Event
   * object.
   *
   * \return a task waiting for cancellation which returns true on success.
   */
  Task<bool> Unsubscribe();

  /**
   * \brief Re-subscribes to an event
   * Resubscription to an already subscribed event is valid but has not user-visible effect.
   *
   * \return a task waiting for re-subscription to be finished which returns true on success
   */
  Task<bool> Resubscribe();

  /** \brief Returns the event Uri
   *
   *  \return the Uri corresponding to this event subscription
   */
  const Uri& GetUri() const noexcept;

  /**
   *  \brief Returns the current subscription state
   *  \return the current subscription state as perceived by the client
   */
  SubscriptionState GetSubscriptionState() const noexcept;

  /**
     * \brief returns the state_notifier function
     * \return state_notifier_
     */
  const Function<client::SubscriptionStateHandlerType>& GetStateNotifier();

  /** \brief Tests events for equality
   *  \param a an event
   *  \param b an event
   *  \return true if a and b are equal
   */
  friend bool operator==(const Event& a, const Event& b) noexcept;
  /** \brief Tests events for inequality
   *  \param a an event
   *  \param b an event
   *  \return true if a and b are unequal
   */
  friend bool operator!=(const Event& a, const Event& b) noexcept;
  /** \brief Tests events for their partial order
   *  Order criterion is implementation-defined.
   *  \param a an event
   *  \param b an event
   *  \return true if a less-than b
   */
  friend bool operator<(const Event& a, const Event& b) noexcept;
  /**
   * \brief can be used to update the state of the event
   * \param new_state: represents the new state
   */
  void setSubscribtionState(SubscriptionState new_state);

  /**
   * \brief getter of the notification function
   * \return function to notify
   */
  const Function<client::NotificationHandlerType>& GetNotify();
  /**
   * \brief returns the client_ptr
   * \return client_ptr_
   */
  /*std::shared_ptr<*/ HttpClient* GetClient();

 private:
  /**
   * \brief is processing the actual task of unsubscription
   */
  bool UnsubscribeTask();
  /**
   * \brief is processing the actual task of resubscription
   */
  bool ResubscribeTask();
  /**
   * \brief the URI of the event
   */
  Uri uri_;
  /**
   * \brief the function handling the notification of arrived data or error frames
   */
  Function<client::NotificationHandlerType> notify_;
  /**
   * \brief the pointer to the client containing this event
   *
   * is relevant for notification of an destroyed or moved event
   */
  HttpClient* client_ptr_ = nullptr;
  /**
   * \brief the function handling the notification of an change of the subscription state
   */
  Function<client::SubscriptionStateHandlerType> state_notifier_;
  /**
   * \brief the state of the subscription of the event
   */
  SubscriptionState state_ = SubscriptionState::kCanceled;
  /**
   * \brief contains an flag if the status has changed
   * is necessary to wait for the reply to an un-/resubsribe message
   */
  bool state_changed_;
  /**
   * \brief time to wait before checking if the subscriptionstate changed
   */
  const int time_to_wait_before_new_check = 10000;
};

/** \brief Represents a client session with a particular server
 *
 *  A client represents all resources related to communication. By configuration a
 *  client is bound to one particular transport binding. Although the configuration
 *  may bind a client to one particular server by default, a client may connect
 *  to arbitrary servers of the same transport protocol.
 *  A client manages the request/reply cycles to servers but also manages event subscriptions.
 *  How requests and replies are delivered, or the handling of events is depended on the
 *  capabilities and constraints of the transport protocol.
 *
 */
}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_TYPES_H_
