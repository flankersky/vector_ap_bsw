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
/**        \file  application.cc
 *        \brief  Application
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/application.h"

#include <array>
#include <cassert>
#include <cerrno>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket_eof_exception.h"
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/service_discovery/service_discovery.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace application {

Application::Application(packet_router::PacketRouterInterface* packet_router,
                         osabstraction::io::network::socket::UnixDomainStreamSocket&& socket)
    : packet_router_(packet_router), subLogger(ara::log::CreateLogger("Application", "")), socket_(std::move(socket)) {
  WriteRoutingChannelId();
}

Application::~Application() = default;

void Application::Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet) {
  someip_posix_common::someipd_posix::routing::MessageHeader header{
      someip_posix_common::someipd_posix::routing::kVersion,
      someip_posix_common::someipd_posix::routing::MessageType::kSomeIP,
      static_cast<std::uint32_t>(packet->GetTotalSize()), instance_id};
  std::array<struct iovec, 1> iov{{{&header, sizeof(header)}}};
  SendRoutingMessage(IovecContainer(iov), packet->GetIovecContainer());
}

void Application::NotifyOfferService(const someip_posix_common::someip::ServiceId service_id,
                                     const someip_posix_common::someip::InstanceId instance_id) {
  namespace routing = someip_posix_common::someipd_posix::routing;  // namespace is used for the response message

  // We first have to look, if there is any pending StartFindService jobs for the service offered,
  // and return a set of requested service instances.
  const auto optional_active_jobs = GetActiveJobsFor(service_id);

  if (optional_active_jobs.has_value()) {
    const auto& active_jobs = optional_active_jobs.value().get();
    for (const auto& findservice_request : active_jobs) {
      const someip_posix_common::someip::InstanceId requested_instance_id{findservice_request};
      // Will return all services (independent of the ID) from the SD.
      const service_discovery::ServiceDiscovery::OfferedServiceContainer& offered_services =
          service_discovery_->GetOfferedServices();
      // Send a subset container for one service id only.
      std::vector<routing::MessageServiceDiscoveryOfferedServiceEntry> response;
      response.reserve(offered_services.size());  // To avoid re-allocation.
      // Filter again, build a subset of services and only push back the service instances, that match the criteria,
      // because the container `offered_services_` holds all  service instances, regardless of the service ID.
      for (const auto& service_instance : offered_services) {
        if (service_instance.service_id_ == service_id) {
          if ((requested_instance_id == someip_posix_common::someip::kInstanceIdAny) ||
              ((requested_instance_id == service_instance.instance_id_) && (requested_instance_id == instance_id))) {
            // Filter for the service, to ensure, that there are no service instance entries, that differ in the service
            // id. For each updated service, one container of the service instances available is sent out.
            response.push_back({service_instance.service_id_, service_instance.instance_id_});
          }
        }
      }

      // The last entry is for identification purposes, to identify which service instance has been updated.
      // This last entry is useful, if all services have been stopped.
      // It is placed at the back, so we are able to use pop_back in the ara::com middleware.
      response.push_back(
          {service_id, requested_instance_id});  // Identification entry to trace back to the StartFindService request.

      // Send out the notification once per service id
      ForwardServiceDiscoveryUpdate(response);
    }
  }
}

void Application::NotifyStopOfferService(const someip_posix_common::someip::ServiceId service_id,
                                         const someip_posix_common::someip::InstanceId instance_id) {
  NotifyOfferService(service_id, instance_id);
}

void Application::NotifyEventSubscriptionStateChange(
    const someip_posix_common::someip::ServiceId service_id, const someip_posix_common::someip::InstanceId instance_id,
    const someip_posix_common::someip::EventId event_id,
    const service_discovery::ServiceDiscoveryEventSubscriptionState new_state) {
  namespace routing = someip_posix_common::someipd_posix::routing;  // namespace is used for the response message

  if (!subscribed_events_.empty()) {
    for (const auto& subscribed_event : subscribed_events_) {
      if ((subscribed_event.service_id_ == service_id) && (subscribed_event.instance_id_ == instance_id) &&
          (subscribed_event.event_id_ == event_id)) {
        // Create the event subscription state change notification
        routing::MessageServiceDiscoveryEventSubscriptionStateEntry response;
        response.service_id_ = service_id;
        response.instance_id_ = instance_id;
        response.event_id_ = event_id;
        response.state_ = static_cast<std::uint16_t>(new_state);

        // Send out the notification
        ForwardServiceDiscoveryEventSubscriptionStateUpdate(response);
      }
    }
  }
}

bool Application::HandleRead(int /*handle*/) {
  try {
    message_reader_.Read(&socket_);
    if (message_reader_.IsMessageAvailable()) {
      ProcessRoutingMessage(message_reader_.GetNextMessage());
    }
    return true;
  } catch (osabstraction::io::network::socket::SocketEOFException&) {
    if (shutdown_handler_) {
      shutdown_handler_(this);
    }
    return false;
  } catch (const std::system_error& e) {
    if (e.code().value() == ECONNRESET) {  // A connection was forcibly closed by a peer
      if (shutdown_handler_) {
        shutdown_handler_(this);
      }
      return false;
    } else {
      throw e;
    }
  }
}

void Application::SetServiceDiscoveryPtr(const someipd_posix::service_discovery::ServiceDiscovery* service_discovery) {
  assert(service_discovery != nullptr);
  service_discovery_ = service_discovery;
}

void Application::ProcessRoutingMessage(MessageReader::Message&& message) {
  switch (message.header_.type_) {
    case someip_posix_common::someipd_posix::routing::MessageType::kSomeIP: {
      packet_router::Packet packet(new SomeIpMessage(std::move(message.body_)));
      packet_router_->Forward(message.header_.instance_id_, shared_from_this(), packet);
    } break;
    default:
      // TODO(PAASR-605)
      subLogger.LogError() << "unknown message type " << message.header_.type_;
      break;
  }
}

void Application::ForwardServiceDiscoveryUpdate(std::vector<MessageServiceDiscoveryOfferedServiceEntry>& response) {
  namespace routing = someip_posix_common::someipd_posix::routing;  // namespace is used for the response message
  std::size_t response_size = response.size() * sizeof(routing::MessageServiceDiscoveryOfferedServiceEntry);
  assert(std::numeric_limits<std::uint16_t>::max() >=
         response_size);  // check if the response size exceeds the value range of uint16
  // Assemble the routing header, which is sent to the client...
  routing::MessageHeader header{routing::kVersion, routing::MessageType::kServiceDiscoveryOfferService,
                                static_cast<std::uint16_t>(response_size), static_cast<std::uint16_t>(GetChannelId())};
  // Assemble the complete message from the header and the data field.
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {response.data(), response_size}}};
  // Notify the registered client about the service instance update,
  // by sending a message with all the services via the routing channel.
  SendRoutingMessage(IovecContainer(iov));
}

void Application::ForwardServiceDiscoveryEventSubscriptionStateUpdate(
    MessageServiceDiscoveryEventSubscriptionStateEntry& response) {
  namespace routing = someip_posix_common::someipd_posix::routing;  // namespace is used for the response message
  std::size_t response_size = sizeof(routing::MessageServiceDiscoveryEventSubscriptionStateEntry);
  assert(std::numeric_limits<std::uint16_t>::max() >=
         response_size);  // check if the response size exceeds the value range of uint16
  // Assemble the routing header, which is sent to the client...
  routing::MessageHeader header{routing::kVersion, routing::MessageType::kServiceDiscoveryEventSubscriptionState,
                                static_cast<std::uint16_t>(response_size), static_cast<std::uint16_t>(GetChannelId())};
  subLogger.LogDebug() << __func__ << ", response_size = " << response_size;
  // Assemble the complete message from the header and the data field.
  std::array<struct iovec, 2> iov{{
      {&header, sizeof(header)}, {&response, response_size},
  }};
  // Notify the registered client about the event subscription state update,
  // by sending a message with the event subscription state details via the routing channel.
  SendRoutingMessage(osabstraction::io::network::socket::Socket::IovecContainer(iov));
}

void Application::WriteRoutingChannelId() {
  someip_posix_common::someipd_posix::routing::MessageChannelId body{static_cast<std::uint32_t>(socket_.GetHandle())};
  someip_posix_common::someipd_posix::routing::MessageHeader header{
      someip_posix_common::someipd_posix::routing::kVersion,
      someip_posix_common::someipd_posix::routing::MessageType::kChannelId, sizeof(body),
      someip_posix_common::someip::kInstanceIdAny,
  };
  std::array<struct iovec, 2> iov{{
      {&header, sizeof(header)}, {&body, sizeof(body)},
  }};
  SendRoutingMessage(osabstraction::io::network::socket::Socket::IovecContainer(iov));
}

void Application::SendRoutingMessage(IovecContainer iovc) {
  using socket = osabstraction::io::network::socket::Socket;
  std::size_t sent_bytes{0U};
  const auto bytes_to_send = socket::GetDataLengthOfIovecContainer(iovc);
  try {
    sent_bytes = socket_.Send(iovc);
  } catch (const std::system_error& e) {
    if (e.code() == std::errc::broken_pipe) {
      // Drop packet and let the HandleRead method handle connection termination
      subLogger.LogDebug() << __func__ << ":" << __LINE__ << ": drop packet";
      return;
    } else {
      throw;
    }
  }
  if (sent_bytes != bytes_to_send) {
    // TODO(PAASR-605)
    throw std::runtime_error("Socket::Send");
  }
}

void Application::SendRoutingMessage(IovecContainer header_iovc, IovecContainer body_iovc) {
  using socket = osabstraction::io::network::socket::Socket;
  std::size_t sent_bytes{0U};
  const auto bytes_to_send =
      socket::GetDataLengthOfIovecContainer(header_iovc) + socket::GetDataLengthOfIovecContainer(body_iovc);
  try {
    sent_bytes = socket_.Send(header_iovc);
    sent_bytes += socket_.Send(body_iovc);
  } catch (const std::system_error& e) {
    if (e.code() == std::errc::broken_pipe) {
      // Drop packet and let the HandleRead method handle connection termination
      subLogger.LogDebug() << __func__ << ":" << __LINE__ << ": drop packet";
      return;
    } else {
      throw;
    }
  }
  if (sent_bytes != bytes_to_send) {
    // TODO(PAASR-605)
    throw std::runtime_error("Socket::Send");
  }
}

}  // namespace application
}  // namespace someipd_posix
