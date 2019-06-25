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
/**        \file  application_manager.cc
 *         \brief Application manager Is used to register, unregister and control all applications available.
 *
 *         \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/application_manager.h"
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/address/unix_domain_socket_address.h"
#include "osabstraction/io/network/socket/socket.h"
#include "osabstraction/io/network/socket/unix_domain_stream_socket.h"

namespace someipd_posix {
namespace application {

ApplicationManager::ApplicationManager(const configuration::Configuration& config,
                                       osabstraction::io::ReactorInterface* reactor,
                                       packet_router::PacketRouterInterface* packet_router,
                                       service_discovery::ServiceDiscovery* service_discovery)
    : config_(config),
      reactor_(reactor),
      packet_router_(packet_router),
      application_event_handler_(this),
      service_discovery_(service_discovery),
      subLogger(ara::log::CreateLogger("ApplicationManager", "")),
      routing_socket_acceptor_(
          osabstraction::io::network::address::UnixDomainSocketAddress(config_.GetRoutingSocketPath()), *this),
      control_socket_(osabstraction::io::network::socket::UnixDomainDatagramSocket::CreateSocket(
          osabstraction::io::network::address::UnixDomainSocketAddress(config_.GetControlSocketPath()))) {
  reactor_->RegisterEventHandler(routing_socket_acceptor_.GetHandle(), &routing_socket_acceptor_,
                                 osabstraction::io::kReadEvent, nullptr);
  reactor_->RegisterEventHandler(control_socket_.GetHandle(), this, osabstraction::io::kReadEvent, nullptr);
  service_discovery_->RegisterServiceDiscoveryOfferServiceObserver(&find_service_update_manager_);
  service_discovery_->RegisterServiceDiscoveryEventSubscriptionStateObserver(&event_state_subscription_update_manager_);
}

ApplicationManager::~ApplicationManager() {
  reactor_->UnregisterEventHandler(routing_socket_acceptor_.GetHandle(), osabstraction::io::kReadEvent);
  reactor_->UnregisterEventHandler(control_socket_.GetHandle(), osabstraction::io::kReadEvent);
  for (auto& application : applications_) {
    reactor_->UnregisterEventHandler(application.second->GetChannelId(), osabstraction::io::kReadEvent);
  }
  service_discovery_->UnRegisterServiceDiscoveryOfferServiceObserver(&find_service_update_manager_);
  service_discovery_->UnRegisterServiceDiscoveryEventSubscriptionStateObserver(
      &event_state_subscription_update_manager_);
}

void ApplicationManager::OnApplicationShutdown(Application* application) {
  someip_posix_common::someip::ChannelId channel_id = application->GetChannelId();
  subLogger.LogInfo() << "application with channel id " << channel_id << " terminated connection";
  auto i = applications_.find(channel_id);
  if (i != applications_.end()) {
    ApplicationPtr app = i->second;

    // (visjak): does assert(app == application) hold here? please leave a comment.

    /* Clean up after an application is shut down. */
    for (auto const& id : app->GetClientIds()) {
      client_ids_.reset(id);
    }
    for (auto const& in : app->GetOfferedServiceInstances()) {
      service_discovery_->StopOfferService(in.service_id_, in.instance_id_);
    }
    for (auto const& ev : app->GetSubscribedEvents()) {
      service_discovery_->UnsubscribeEvent(ev.service_id_, ev.instance_id_, ev.event_id_, app);
    }
    for (auto const& in : app->GetRequestedServiceInstances()) {
      service_discovery_->ReleaseService(std::get<0>(in), std::get<1>(in));
    }
    packet_router_->CleanUpResponseRoutingTableEntries(app.get());
    // The next step of unregistering any existing observers for StartFindService is
    // very important, otherwise the application is called on service offers, but the application is shut down.
    find_service_update_manager_.DeleteOfferServiceObserver(app);
    event_state_subscription_update_manager_.DeleteEventSubscriptionObserver(app);
    applications_.erase(i);
  }
}

void ApplicationManager::AcceptApplication(osabstraction::io::network::socket::UnixDomainStreamSocket&& socket) {
  ApplicationPtr application = std::make_shared<Application>(packet_router_, std::move(socket));
  application->SetShutdownHandler([this](Application* app) { this->OnApplicationShutdown(app); });
  application->SetServiceDiscoveryPtr(service_discovery_);
  applications_.insert({application->GetChannelId(), application});
  reactor_->RegisterEventHandler(application->GetChannelId(), &application_event_handler_,
                                 osabstraction::io::kReadEvent, nullptr);
  subLogger.LogInfo() << "accepted new application with channel id " << application->GetChannelId();
}

bool ApplicationManager::HandleRead(someip_posix_common::someip::ChannelId handle) {
  namespace control = someip_posix_common::someipd_posix::control;
  osabstraction::io::network::address::SocketAddress remote_address;
  std::size_t n = static_cast<typename std::make_unsigned<ssize_t>::type>(
      control_socket_.Receive(control_packet_buffer_.data(), kMaxControlPacketSize, &remote_address));
  if (n > 0) {
    auto header = reinterpret_cast<const control::MessageHeader*>(control_packet_buffer_.data());
    std::size_t body_length = n - sizeof(control::MessageHeader);
    if (header->length_ == body_length) {
      ProcessControlPacket(header, remote_address);
    } else {
      subLogger.LogError() << " invalid control message received from application with channel id " << handle << "\n"
                           << " got body length " << body_length << " expected body length " << header->length_;
    }
  }
  return EventHandler::HandleRead(handle);
}

ApplicationManager::ApplicationPtr ApplicationManager::GetApplication(
    someip_posix_common::someip::ChannelId handle) const {
  auto it = applications_.find(handle);
  if (it == applications_.end()) {
    return ApplicationPtr();
  } else {
    return it->second;
  }
}

void ApplicationManager::ProcessControlPacket(
    const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& remote_address) {
  namespace control = someip_posix_common::someipd_posix::control;
  auto it = applications_.find(static_cast<ApplicationMap::key_type>(header->channel_id_));
  if (it != applications_.end()) {
    ApplicationPtr application = it->second;
    switch (header->type_) {
      case control::MessageType::kGetClientIdRequest:
        GetClientIdHandler(application, header, remote_address);
        break;
      case control::MessageType::kReleaseClientIdRequest:
        ReleaseClientIdHandler(application, header, remote_address);
        break;
      case control::MessageType::kOfferService:
        OfferServiceHandler(application, header, remote_address);
        break;
      case control::MessageType::kStopOfferService:
        StopOfferServiceHandler(application, header, remote_address);
        break;
      case control::MessageType::kFindServiceRequest:
        FindServiceHandler(application, header, remote_address);
        break;
      case control::MessageType::kStartFindService:
        StartFindServiceHandler(application, header, remote_address);
        break;
      case control::MessageType::kStopFindService:
        StopFindServiceHandler(application, header, remote_address);
        break;
      case control::MessageType::kSubscribeEvent:
        SubscribeEventHandler(application, header, remote_address);
        break;
      case control::MessageType::kUnsubscribeEvent:
        UnsubscribeEventHandler(application, header, remote_address);
        break;
      case control::MessageType::kRequestService:
        RequestService(application, header, remote_address);
        break;
      case control::MessageType::kReleaseService:
        ReleaseService(application, header, remote_address);
        break;
      default:
        subLogger.LogError() << "unknown control packet type " << header->type_;
        break;
    }
  } else {
    subLogger.LogError() << "unknown channel id " << header->channel_id_ << " message type " << header->type_;
  }
}

void ApplicationManager::GetClientIdHandler(ApplicationPtr application,
                                            const someip_posix_common::someipd_posix::control::MessageHeader* header,
                                            const osabstraction::io::network::address::SocketAddress& remote_address) {
  namespace control = someip_posix_common::someipd_posix::control;
  static_assert(sizeof(someip_posix_common::someip::ChannelId) <= sizeof(std::uint32_t),
                "int is too big for std::uint32_t !!!");
  if (header->length_ == 0) {
    auto next_client_id = GetFreeClientId();
    control::MessageGetClientIdResponse response{next_client_id};
    control::MessageHeader to_header{control::kVersion, control::MessageType::kGetClientIdResponse, sizeof(response),
                                     static_cast<std::uint32_t>(application->GetChannelId())};
    std::array<struct iovec, 2> iov{{
        {&to_header, sizeof(to_header)}, {&response, sizeof(response)},
    }};
    std::size_t n =
        control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov), remote_address);
    if (n != (sizeof(to_header) + sizeof(response))) {
      // TODO(PAASR-605)
      throw std::runtime_error("Socket::Send");
    }
    application->AddClientId(next_client_id);
  } else {
    subLogger.LogError() << "invalid GetClientIdRequest control message";
  }
}

void ApplicationManager::ReleaseClientIdHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageReleaseClientIdRequest)) {
    auto request = reinterpret_cast<const control::MessageReleaseClientIdRequest*>(header + 1);  // Jump over header
    client_ids_.reset(request->client_id_);
    application->DeleteClientId(request->client_id_);
  } else {
    subLogger.LogError() << "invalid ReleaseClientIdRequest control message";
  }
}

void ApplicationManager::OfferServiceHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageOfferService)) {
    auto request = reinterpret_cast<const control::MessageOfferService*>(header + 1);  // Jump over header
    const auto sid = request->service_id_;
    const auto inst = request->instance_id_;

    // Search within the service discovery if the same or any other application has already offered same service
    // instance id
    const auto& offered_services = service_discovery_->GetOfferedServices();
    auto it = std::find_if(offered_services.begin(), offered_services.end(),
                           [&sid, &inst](const service_discovery::ServiceDiscovery::OfferedService& other) {
                             return sid == other.service_id_ && inst == other.instance_id_;
                           });

    if (it == offered_services.end()) {
      service_discovery_->OfferService(request->service_id_, request->instance_id_, application);
      application->AddOfferedServiceInstance(request->service_id_, request->instance_id_);
    } else {
      // already exists
      subLogger.LogError() << "service (" << std::hex << sid << ", " << inst << std::dec
                           << ") is already offered, skipped.";
    }
  } else {
    subLogger.LogError() << "invalid OfferService control message";
  }
}

void ApplicationManager::StopOfferServiceHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageStopOfferService)) {
    auto request = reinterpret_cast<const control::MessageStopOfferService*>(header + 1);  // Jump over header
    const auto sid = request->service_id_;
    const auto inst = request->instance_id_;

    // Search only within this application if it already offered the service before
    const auto& offered_services = application->GetOfferedServiceInstances();
    auto it = std::find_if(offered_services.begin(), offered_services.end(),
                           [&sid, &inst](const Application::OfferedServiceInstanceEntry& other) {
                             return sid == other.service_id_ && inst == other.instance_id_;
                           });

    if (it != offered_services.end()) {
      service_discovery_->StopOfferService(sid, inst);
      application->DeleteOfferedServiceInstance(sid, inst);
    } else {
      // does not exist
      subLogger.LogError() << "service (" << std::hex << sid << ", " << inst << std::dec
                           << ") is not offered, skipped.";
    }
  } else {
    subLogger.LogError() << "invalid StopOfferService control message";
  }
}

void ApplicationManager::FindServiceHandler(ApplicationPtr application,
                                            const someip_posix_common::someipd_posix::control::MessageHeader* header,
                                            const osabstraction::io::network::address::SocketAddress& remote_address) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageFindServiceRequest)) {
    auto request = reinterpret_cast<const control::MessageFindServiceRequest*>(header + 1);  // Jump over header
    auto offered_services = service_discovery_->GetOfferedServices();
    std::vector<control::FindServiceEntry> response;
    for (auto const& service : offered_services) {
      if (service.service_id_ == request->service_id_) {
        if (request->instance_id_ == someip_posix_common::someip::kInstanceIdAny ||
            service.instance_id_ == request->instance_id_) {
          response.push_back({service.service_id_, service.instance_id_});
        }
      }
    }
    std::size_t response_size = response.size() * sizeof(control::FindServiceEntry);
    assert(std::numeric_limits<std::uint16_t>::max() >= response_size);
    control::MessageHeader to_header{control::kVersion, control::MessageType::kFindServiceResponse,
                                     static_cast<std::uint16_t>(response_size),
                                     static_cast<std::uint32_t>(application->GetChannelId())};
    std::array<struct iovec, 2> iov{{
        {&to_header, sizeof(to_header)}, {response.data(), response_size},
    }};
    std::size_t n =
        control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov), remote_address);
    if (n != (sizeof(to_header) + response_size)) {
      // TODO(PAASR-605)
      throw std::runtime_error("Socket::Send");
    }
  } else {
    subLogger.LogError() << "invalid FindServiceRequest control message";
  }
}

void ApplicationManager::StartFindServiceHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  // Type of `MessageStartFindService` is declared in message.h for detailed info
  // The length given must match the size of the message field
  const auto length = header->length_;
  if (length == sizeof(control::MessageStartFindService)) {
    const control::MessageFindServiceRequest* request =
        reinterpret_cast<const control::MessageFindServiceRequest*>(header + 1);  // Jump over header
    const someip_posix_common::someip::ServiceId service_id = request->service_id_;
    const someip_posix_common::someip::InstanceId instance_id = request->instance_id_;
    // Register this application in an observer-like pattern container, to inform about offered and stopped offered
    // services.
    find_service_update_manager_.AddFindServiceJob(application, service_id, instance_id);
  } else {
    // If the length of the message does not match the specification
    subLogger.LogError() << "invalid StartFindServiceRequest control message";
  }
}

void ApplicationManager::StopFindServiceHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  // Type of `MessageStopFindService` is declared in message.h for detailed info
  // The length given must match the size of the message field
  const auto length = header->length_;
  if (length == sizeof(control::MessageStopFindService)) {
    // get the message field, without the SOME/IP header
    const auto request = reinterpret_cast<const control::MessageStartFindService*>(header + 1);
    // De-Register this application in an observer-like pattern container, to inform about offered services,
    // that match the requested (explicit or any) instance from the proxy
    const someip_posix_common::someip::ServiceId service_id = request->service_id_;
    const someip_posix_common::someip::InstanceId instance_id = request->instance_id_;
    find_service_update_manager_.DeleteFindServiceJob(application, service_id, instance_id);
  } else {
    // If the length of the message does not match the specification
    subLogger.LogError() << "invalid StopFindServiceRequest control message";
  }
}

void ApplicationManager::SubscribeEventHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageSubscribeEvent)) {
    auto request = reinterpret_cast<const control::MessageSubscribeEvent*>(header + 1);
    if (application->GetSubscribedEvents().empty()) {
      // First event subscription -> Start monitoring
      event_state_subscription_update_manager_.AddEventSubscriptionObserver(application);
    }
    // Application::AddSubscribedEvent is called before ServiceDiscovery::SubscribeEvent so that the application starts
    // tracking the event subscription state changes.
    application->AddSubscribedEvent(request->service_id_, request->instance_id_, request->event_id_);
    service_discovery_->SubscribeEvent(request->service_id_, request->instance_id_, request->event_id_, application);
  } else {
    subLogger.LogError() << "invalid SubscribeEvent control message";
  }
}

void ApplicationManager::UnsubscribeEventHandler(
    ApplicationPtr application, const someip_posix_common::someipd_posix::control::MessageHeader* header,
    const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageUnsubscribeEvent)) {
    auto request = reinterpret_cast<const control::MessageUnsubscribeEvent*>(header + 1);
    service_discovery_->UnsubscribeEvent(request->service_id_, request->instance_id_, request->event_id_, application);
    application->DeleteSubscribedEvent(request->service_id_, request->instance_id_, request->event_id_);
    if (application->GetSubscribedEvents().empty()) {
      // Last event subscription -> Stop monitoring
      event_state_subscription_update_manager_.DeleteEventSubscriptionObserver(application);
    }
  } else {
    subLogger.LogError() << "invalid UnsubscribeEvent control message";
  }
}

void ApplicationManager::RequestService(ApplicationPtr application,
                                        const someip_posix_common::someipd_posix::control::MessageHeader* header,
                                        const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  subLogger.LogDebug() << __func__ << ":" << __LINE__;
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageRequestService)) {
    auto request = reinterpret_cast<const control::MessageRequestService*>(header + 1);
    const auto sid = request->service_id_;
    const auto inst = request->instance_id_;

    // Search within the service discovery if this service is already offered
    const auto& offered_services = service_discovery_->GetOfferedServices();
    auto it = std::find_if(offered_services.begin(), offered_services.end(),
                           [&sid, &inst](const service_discovery::ServiceDiscovery::OfferedService& other) {
                             return sid == other.service_id_ && inst == other.instance_id_;
                           });

    if (it != offered_services.end()) {
      service_discovery_->RequestService(request->service_id_, request->instance_id_);
      application->AddRequestedServiceInstance(request->service_id_, request->instance_id_);
    } else {
      subLogger.LogError() << "service (" << std::hex << sid << ", " << inst << std::dec
                           << ") is not offered, skipped.";
    }
  } else {
    subLogger.LogError() << "invalid RequestService control message";
  }
}

void ApplicationManager::ReleaseService(ApplicationPtr application,
                                        const someip_posix_common::someipd_posix::control::MessageHeader* header,
                                        const osabstraction::io::network::address::SocketAddress& /*remote_address*/) {
  subLogger.LogDebug() << __func__ << ":" << __LINE__;
  namespace control = someip_posix_common::someipd_posix::control;
  if (header->length_ == sizeof(control::MessageReleaseService)) {
    auto request = reinterpret_cast<const control::MessageReleaseService*>(header + 1);
    const auto sid = request->service_id_;
    const auto inst = request->instance_id_;

    // Search only within this application if it already requested the service before
    const auto& offered_services = application->GetRequestedServiceInstances();
    auto it = std::find_if(offered_services.begin(), offered_services.end(),
                           [&sid, &inst](const Application::RequestedServiceInstanceEntry& other) {
                             return sid == std::get<0>(other) && inst == std::get<1>(other);
                           });

    if (it != offered_services.end()) {
      service_discovery_->ReleaseService(request->service_id_, request->instance_id_);
      application->DeleteRequestedServiceInstance(request->service_id_, request->instance_id_);
    } else {
      // does not exist
      subLogger.LogError() << "service (" << std::hex << sid << ", " << inst << std::dec
                           << ") is not requested, skipped.";
    }
  } else {
    subLogger.LogError() << "invalid ReleaseService control message";
  }
}

someip_posix_common::someip::ClientId ApplicationManager::GetFreeClientId() {
  for (std::size_t pos = 0; pos < client_ids_.size(); ++pos) {
    if (!client_ids_[pos]) {
      client_ids_.set(pos);
      return static_cast<someip_posix_common::someip::ClientId>(pos);  // length of client_ids_ is limits to a uint16
    }
  }
  // TODO(PAASR-605)
  throw std::runtime_error("no free client ids left");
}

}  // namespace application
}  // namespace someipd_posix
