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
/**        \file  client_types.cc
 *        \brief  Contains implementation of Client related types
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/client_types.h"
#include <iostream>
#include "ara/rest/httpclient.h"
#include "ara/rest/serialize/serialize.h"

namespace ara {
namespace rest {

Request::Request(RequestMethod met, const Uri& uri) : met_(met), uri_(uri), obj_(ogm::Object::Make()) {}

Request::Request(RequestMethod met, Uri&& uri) : met_(met), uri_(std::move(uri)), obj_(ogm::Object::Make()) {}

Request::Request(RequestMethod met, const Uri& uri, const Pointer<ogm::Object>& obj)
    : met_(met), uri_(uri), obj_(ara::rest::ogm::Copy(obj)) {}

Request::Request(RequestMethod met, Uri&& uri, const Pointer<ogm::Object>& obj)
    : met_(met), uri_(uri), obj_(ara::rest::ogm::Copy(obj)) {}

Request::Request(RequestMethod met, const Uri& uri, Pointer<ogm::Object>&& obj)
    : met_(met), uri_(uri), obj_(std::move(obj)) {}

Request::Request(RequestMethod met, Uri&& uri, Pointer<ogm::Object>&& obj)
    : met_(met), uri_(std::move(uri)), obj_(std::move(obj)) {}

const ReplyHeader& Reply::GetHeader() const { return header_; }

Task<const ogm::Object&> Reply::GetObject() const {
  std::packaged_task<ogm::Object const&()> get_object_task([this]() -> ogm::Object const& { return *obj_; });
  get_object_task();
  Task<ogm::Object const&> get_object(std::move(get_object_task.get_future()));
  return get_object;
}

Event::Event(Uri uri, const Function<client::NotificationHandlerType>& notify, HttpClient* client_ptr,
             const Function<client::SubscriptionStateHandlerType> state_notifyer)
    : uri_(uri), notify_(notify), client_ptr_(client_ptr), state_notifier_(state_notifyer) {}

Event::Event(Event&& other)
    : notify_(std::move(other.notify_)),
      state_notifier_(std::move(other.GetStateNotifier())),
      state_(std::move(other.GetSubscriptionState())) {
  uri_ = std::move(other.uri_);
  client_ptr_ = std::move(other.client_ptr_);
  // tell the client that the pointer of the event was moved
  client_ptr_->PropagateMovedPointer(this);
}
Event::~Event() {  // tell the client that the event was deleted
  client_ptr_->PropagateDeletedEvent(this);
}

Event& Event::operator=(Event&& other) {
  if (this != &other) {
    std::swap(uri_, other.uri_);
    std::swap(client_ptr_, other.client_ptr_);
    notify_ = std::move(other.notify_);
    state_notifier_ = std::move(other.GetStateNotifier());
    state_ = std::move(other.GetSubscriptionState());
    // tell the client that the pointer of the event was moved
    client_ptr_->PropagateMovedPointer(this);
    other.client_ptr_->PropagateMovedPointer(&other);
  }
  return *this;
}

const Function<client::SubscriptionStateHandlerType>& Event::GetStateNotifier() { return state_notifier_; }

HttpClient* Event::GetClient() { return client_ptr_; }

bool Event::UnsubscribeTask() {
  // set subscriptionstate to invalid
  state_ = SubscriptionState::kInvalid;
  // set state_changed to false to wait until the client received
  // the replay message with status ok
  state_changed_ = false;
  // create the ogm::Object for the unsubscription message
  String uri_str = ToString(uri_);
  Pointer<ogm::Object> objNode =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("unsubscribe"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))));
  // convert the message to a string
  String json_for_unsubscription = serialize::Serializer::OgmToJson(objNode);
  // send the message by using the websocket of the client
  client_ptr_->sendWithWebsocket(json_for_unsubscription);
  // wait for the reply
  while (!state_changed_) {
    usleep(time_to_wait_before_new_check);
  }
  // check if state changed to canceled
  if (state_ == SubscriptionState::kCanceled) {
    return true;
  } else {
    return false;
  }
}

Task<bool> Event::Unsubscribe() {
  // create async task
  std::packaged_task<bool()> unsubscribe_ptask(std::bind(&Event::UnsubscribeTask, this));
  Task<bool> unsubscribe_ft(unsubscribe_ptask.get_future());
  // execute async task
  unsubscribe_ptask();

  return unsubscribe_ft;
}

const Uri& Event::GetUri() const noexcept { return uri_; }

void Event::setSubscribtionState(SubscriptionState new_state) {
  // check if state is different than before

  if (state_ != new_state) {
    // if it is change the state to the new one
    // call the state notifier
    state_ = new_state;
    state_notifier_(*this, state_);
    // set state_changed to true (relevant for unsubscription and resubscription)
    state_changed_ = true;
  }
}

bool Event::ResubscribeTask() {
  // set subscriptionstate to invalid
  state_ = SubscriptionState::kInvalid;
  // set state_changed to false to wait until the client received
  // the replay message with status ok
  state_changed_ = false;
  // create the ogm::Object for the unsubscription message
  String uri_str = ToString(uri_);
  Pointer<ogm::Object> objNode =
      ogm::Object::Make(ogm::Field::Make("type", ogm::String::Make(CSTRING_TO_STRINGVIEW("reauthorize"))),
                        ogm::Field::Make("event", ogm::String::Make(StringView(STRING_TO_STRINGVIEW(uri_str)))),
                        ogm::Field::Make("Authorization", ogm::String::Make(CSTRING_TO_STRINGVIEW("<token>"))));
  // convert the message to a string
  String json_for_resubscription = serialize::Serializer::OgmToJson(objNode);
  // send the message by using the websocket of the client
  client_ptr_->sendWithWebsocket(json_for_resubscription);
  // wait for the reply
  while (!state_changed_) {
    usleep(time_to_wait_before_new_check);
  }
  // check if state changed to subscribed
  if (state_ == SubscriptionState::kSubscribed) {
    return true;
  } else {
    return false;
  }
}

const Function<client::NotificationHandlerType>& Event::GetNotify() { return notify_; }

SubscriptionState Event::GetSubscriptionState() const noexcept { return state_; }

Task<bool> Event::Resubscribe() {
  // create async task
  std::packaged_task<bool()> resubscribe_ptask(std::bind(&Event::ResubscribeTask, this));
  Task<bool> resubscribe_ft(resubscribe_ptask.get_future());
  // execute async task
  resubscribe_ptask();

  return resubscribe_ft;
}

}  // namespace rest
}  // namespace ara
