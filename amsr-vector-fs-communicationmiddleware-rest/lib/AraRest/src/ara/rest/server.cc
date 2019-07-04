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
/**        \file  server.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/server.h"

#include <vac/language/cpp14_backport.h>
#include <ara/log/logging.hpp>
#include <iostream>
#include <memory>
#include <utility>

#include "ara/rest/config.h"
#include "ara/rest/server_factory.h"

namespace ara {
namespace rest {

/**********************************************************************************************************************
 *  ServerEvent
 *********************************************************************************************************************/

ServerEvent::ServerEvent(SubscriptionState sub_state, Pointer<Uri> uri,
                         Pointer<ServerEventInterface> server_event_interface)
    : subscription_state_(sub_state),
      uri_(std::move(uri)),
      server_event_interface_(std::move(server_event_interface)),
      log_(ara::log::CreateLogger("42", "ara::rest ServerEvent")) {
  if (!server_event_interface_) {
    log_.LogError() << "ServerEvent::ServerEvent ServerEventInterface Pointer is not set in ServerEvent!";
  }
}

ServerEvent::ServerEvent(ServerEvent&& other)
    : subscription_state_(std::move(other.subscription_state_)),
      uri_(std::move(other.uri_)),
      server_event_interface_(std::move(other.server_event_interface_)),
      log_(ara::log::CreateLogger("42", "ara::rest ServerEvent")) {
  other.subscription_state_ = SubscriptionState::kInvalid;
  other.uri_ = nullptr;
  other.server_event_interface_ = nullptr;
  // propagate moved pointer to WebSocketRequestHandlersourc
  server_event_interface_->PropagateMovedPointer(this);
}

ServerEvent& ServerEvent::operator=(ServerEvent&& other) {
  if (this != &other) {
    subscription_state_ = std::move(other.subscription_state_);
    other.subscription_state_ = SubscriptionState::kInvalid;
    uri_.swap(other.uri_);
    server_event_interface_.swap(other.server_event_interface_);

    // update both pointers
    server_event_interface_->PropagateMovedPointer(this);
    other.server_event_interface_->PropagateMovedPointer(&other);
  }
  return *this;
}

ServerEvent::~ServerEvent() {
  if (server_event_interface_) {
    // TODO(hhz): Remove this output// TODO(hhz): Remove when Segfault because of static ServerEvent in ServerTests is
    // not longer a problem when Segfault because of static ServerEvent in ServerTests is not longer a problem
    log_.LogDebug()
        << "INFO: The following Segmentation fault occurs because of the limited lifetime of the ServerEvent (which is "
           "expanded via 'static') in the "
           "Server.WebSocketServer Test. This Output is here to verify the segmentation fault is caused by this issue. "
           "It shouldn't prevent the tests from beeing successfull. If one test fails, it is most likely not the fault "
           "of this segmentation fault!";
    server_event_interface_->PropagateDeletedEvent(ToString(*uri_));
  }
}

Task<void> ServerEvent::Notify() { return server_event_interface_.get()->Notify(); }

Task<void> ServerEvent::Unsubscribe() {
  // first, set the SubscriptionState to canceled
  subscription_state_ = SubscriptionState::kCanceled;
  // then call the Unsubscribe method of the corresponding server binding to let client know
  return server_event_interface_->Unsubscribe();
}

SubscriptionState ServerEvent::GetSubscriptionState() const noexcept { return subscription_state_; }

const Uri& ServerEvent::GetUri() const noexcept { return *uri_; }

void ServerEvent::SetSubscriptionState(SubscriptionState sstate) { subscription_state_ = sstate; }

void ServerEvent::SetUri(Pointer<Uri> uri) { uri_ = std::move(uri); }

Task<void> ServerEvent::Send(String data) { return server_event_interface_->Send(data); }

Task<void> ServerEvent::ConfirmSubscription() { return server_event_interface_->ConfirmSubscription(); }

Task<void> ServerEvent::ConfirmUnsubscription() { return server_event_interface_->ConfirmUnsubscription(); }

Task<void> ServerEvent::ConfirmResubscription() { return server_event_interface_->ConfirmResubscription(); }
void ServerEvent::StopPeriodicNotification() { server_event_interface_->StopPeriodicNotification(); }

/**********************************************************************************************************************
 *  ServerRequest
 *********************************************************************************************************************/

ServerRequest::ServerRequest(Pointer<RequestHeader> request_header, Pointer<ogm::Object> object)
    : request_header_(std::move(request_header)), object_(std::move(object)) {}

RequestHeader const& ServerRequest::GetHeader() const { return *request_header_; }

ogm::Object const& ServerRequest::GetObjectTask() const { return *object_; }

Task<ogm::Object const&> ServerRequest::GetObject() const {
  std::packaged_task<ogm::Object const&()> get_object_task(std::bind(&ServerRequest::GetObjectTask, this));
  get_object_task();
  std::future<ogm::Object const&> get_object_ft = get_object_task.get_future();
  Task<ogm::Object const&> get_object(std::move(get_object_ft));
  return get_object;
}

Pointer<ogm::Object> ServerRequest::ReleaseObjectTask() { return std::move(object_); }

Task<Pointer<ogm::Object>> ServerRequest::ReleaseObject() {
  std::packaged_task<Pointer<ogm::Object>()> release_object_ptask(std::bind(&ServerRequest::ReleaseObjectTask, this));
  std::future<Pointer<ogm::Object>> release_object_ft = release_object_ptask.get_future();
  Task<Pointer<ogm::Object>> release_object_task(std::move(release_object_ft));
  return release_object_task;
}

void ServerRequest::SetHeader(Pointer<RequestHeader> request_header) { request_header_ = std::move(request_header); }

void ServerRequest::SetObject(Pointer<ogm::Object> object) { object_ = std::move(object); }

/**********************************************************************************************************************
 *  ServerReply
 *********************************************************************************************************************/

ServerReply::ServerReply(Pointer<ReplyHeader> reply_header, Pointer<ogm::Object> object)
    : reply_header_(std::move(reply_header)), object_(std::move(object)) {}

ServerReply::ServerReply(Pointer<ReplyHeader> reply_header, Pointer<ogm::Object> object,
                         ServerReplyInterface* server_reply_interface)
    : reply_header_(std::move(reply_header)),
      object_(std::move(object)),
      server_reply_interface_(server_reply_interface) {}

ReplyHeader& ServerReply::GetHeader() { return *reply_header_; }

Pointer<ogm::Object> ServerReply::GetObject() { return std::move(object_); }

void ServerReply::SetHeader(Pointer<ReplyHeader> reply_header) { reply_header_ = std::move(reply_header); }

void ServerReply::SetServerReplyInterface(ServerReplyInterface* server_reply_interface) {
  server_reply_interface_ = server_reply_interface;
}

Task<void> ServerReply::Send(const Pointer<ogm::Object>& data) {
  return server_reply_interface_->Send(data, this->GetHeader().GetStatus());
}

Task<void> ServerReply::Send(Pointer<ogm::Object>&& data) {
  return server_reply_interface_->Send(data, this->GetHeader().GetStatus());
}

Task<void> ServerReply::Redirect(const Uri& uri) { return server_reply_interface_->Redirect(uri); }

/**********************************************************************************************************************
 *  Server
 *********************************************************************************************************************/

Server::Server(const String& conf, const Function<RequestHandlerType>& hnd, Allocator* alloc)
    : server_list_(), log_(ara::log::CreateLogger("42", "ara::rest Server")) {
  log_.LogDebug() << "Server instance created.";

  // Get Binding ID for server
  static config::ConfigurationHandler& config_handler = config::ConfigurationHandler::Instance();
  std::vector<String> binding_ids = config_handler.GetServerBindingIds(conf);

  for (auto& binding : binding_ids) {
    config::ServerBinding server_binding = config_handler.GetServerBinding(binding);
    ServerFactory* server_factory = ServerFactory::Get();
    server_list_.emplace_back(server_factory->CreateServer(server_binding.type_, server_binding, hnd, alloc));
  }
}

void Server::StartAllServerBindings(StartupPolicy policy) {
  for (auto& server_binding : server_list_) {
    server_binding.get()->Start(policy);
  }
}

void Server::StopAllServerBindings(ShutdownPolicy policy) {
  for (auto& server_binding : server_list_) {
    server_binding.get()->Stop(policy);
  }
}

Task<void> Server::Start(StartupPolicy policy) {
  std::future<void> start_all_ft = std::async(std::launch::async, &Server::StartAllServerBindings, this, policy);

  Task<void> start_all_task(std::move(start_all_ft));
  return start_all_task;
}

Task<void> Server::Stop(ShutdownPolicy policy) {
  std::future<void> stop_all_ft = std::async(std::launch::async, &Server::StopAllServerBindings, this, policy);

  Task<void> stop_all_task(std::move(stop_all_ft));
  return stop_all_task;
}

void Server::ObserveSubscriptions(const Function<SubscriptionHandlerType>& shnd,
                                  const Function<SubscriptionStateHandlerType>& sshnd) {
  for (auto& server_binding : server_list_) {
    server_binding.get()->ObserveSubscriptions(shnd, sshnd);
  }
}

std::error_code Server::GetError() const noexcept { return error_; }

void Server::SetError(std::error_code error) {
  error_ = error;
  if (error_observer_) {
    error_observer_(error_);
  }
}

void Server::ObserveError(const Function<void(std::error_code)>& hnd) { error_observer_ = hnd; }

}  // namespace rest
}  // namespace ara
