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
/**        \file  someip_posix.cc
 *        \brief  SomeIP POSIX
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix/someip_posix.h"

#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/address/unix_domain_socket_address.h"
#include "osabstraction/io/network/socket/socket.h"
#include "osabstraction/io/network/socket/socket_eof_exception.h"
#include "osabstraction/io/network/socket/unix_domain_stream_socket.h"
#include "someip-posix-common/someipd_posix/control/message.h"

namespace someip_posix {

std::atomic<std::uint32_t> SomeIpPosix::socket_id_{0};

SomeIpPosix::SomeIpPosix(const someip_posix_common::config::SomeIpPosixConfigModel& config,
                         osabstraction::io::ReactorInterface* reactor)
    : reactor_(reactor),
      logger_(ara::log::CreateLogger("SomeIpPosix", "")),
      control_socket_(osabstraction::io::network::socket::UnixDomainDatagramSocket::CreateSocket(
          osabstraction::io::network::address::UnixDomainSocketAddress(
              std::string("/tmp/someip-posix-control-") + std::to_string(getpid()) + "-" +
              std::to_string(++socket_id_) + std::string(".socket")),
          osabstraction::io::network::address::UnixDomainSocketAddress(config.control_socket_path_))),
      routing_channel_id_{},
      someip_posix_application_{nullptr},
      someip_config_{config} {
  // Socket connector for the routing connection to the SomeIP daemon.

  using SocketConnector =
      osabstraction::io::network::socket::SocketConnector<osabstraction::io::network::socket::UnixDomainStreamSocket,
                                                          SomeIpPosix, &SomeIpPosix::RoutingSocketConnectorHandler>;

  SocketConnector routing_socket_connector(
      osabstraction::io::network::address::UnixDomainSocketAddress(config.routing_socket_path_), *this,
      true /* Use Blocking Mode for establishing the connection */);

  // Synchronously connect the routing_socket_
  routing_socket_connector.HandleWrite(routing_socket_connector.GetHandle());
  // control_socket_ always used synchronously, so no reactor registration here.
}

void SomeIpPosix::RoutingSocketConnectorHandler(
    osabstraction::io::network::socket::UnixDomainStreamSocket&& stream_socket) {
  this->routing_socket_ = std::move(stream_socket);
  this->ReadRoutingChannelId();

  if (someip_config_.processing_mode_ == someip_posix_common::config::SomeIpBindingProcessingMode::kPolling) {
    // Set it to the routing socket to non-blocking to achieve full polling mode.
    routing_socket_.SetBlockingMode(false);
  } else {
    // Registering an event handler is only necessary for threaded mode.
    this->reactor_->RegisterEventHandler(routing_socket_.GetHandle(), this, osabstraction::io::kReadEvent, nullptr);
  }
}

SomeIpPosix::~SomeIpPosix() {
  if (someip_config_.processing_mode_ != someip_posix_common::config::SomeIpBindingProcessingMode::kPolling) {
    reactor_->UnregisterEventHandler(routing_socket_.GetHandle(), osabstraction::io::kReadEvent);
  }
}

someip_posix_common::someip::ClientId SomeIpPosix::GetClientId() {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageHeader header{control::kVersion, control::MessageType::kGetClientIdRequest, 0 /* body length */,
                                routing_channel_id_};
  std::size_t n = control_socket_.Send(&header, sizeof(header));
  if (n != sizeof(header)) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
  control::MessageGetClientIdResponse response;
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&response, sizeof(response)}}};
  n = control_socket_.Receive(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(response))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Receive");
  }
  if (header.type_ != control::MessageType::kGetClientIdResponse) {
    // TODO(PAASR-608)
    throw std::runtime_error("invalid packet");
  }
  return response.client_id_;
}

void SomeIpPosix::ReleaseClientId(someip_posix_common::someip::ClientId client_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageReleaseClientIdRequest request{client_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kReleaseClientIdRequest, sizeof(request),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&request, sizeof(request)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(request))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::OfferService(someip_posix_common::someip::ServiceId service_id,
                               someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageOfferService body{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kOfferService, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::StopOfferService(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageStopOfferService body{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kStopOfferService, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::RequestService(someip_posix_common::someip::ServiceId service_id,
                                 someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageRequestService request{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kRequestService, sizeof(request),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&request, sizeof(request)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(request))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::ReleaseService(someip_posix_common::someip::ServiceId service_id,
                                 someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageReleaseService request{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kReleaseService, sizeof(request),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&request, sizeof(request)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(request))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::SubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                                 someip_posix_common::someip::InstanceId instance_id,
                                 someip_posix_common::someip::EventId event_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageSubscribeEvent body{service_id, instance_id, event_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kSubscribeEvent, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::UnsubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id,
                                   someip_posix_common::someip::EventId event_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageUnsubscribeEvent body{service_id, instance_id, event_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kUnsubscribeEvent, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::Send(someip_posix_common::someip::InstanceId instance_id,
                       someip_posix_common::someip::SomeIpPacket packet) {
  namespace routing = someip_posix_common::someipd_posix::routing;
  std::lock_guard<std::mutex> lock(routing_socket_lock_);
  routing::MessageHeader header{routing::kVersion, routing::MessageType::kSomeIP,
                                static_cast<std::uint32_t>(packet->size()), instance_id};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {packet->data(), packet->size()}}};
  std::size_t n = routing_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != sizeof(header) + packet->size()) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

someip_posix_common::someip::ServiceInstanceContainer SomeIpPosix::FindService(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageFindServiceRequest request{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kFindServiceRequest, sizeof(request),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&request, sizeof(request)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(request))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
  someip_posix_common::someip::ServiceInstanceContainer container(kMaxFindServiceResponseEntries);
  std::size_t max_response_size = container.size() * sizeof(someip_posix_common::someip::ServiceInstance);
  iov = {{{&header, sizeof(header)}, {container.data(), max_response_size}}};
  n = control_socket_.Receive(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n < sizeof(header)) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Receive");
  }
  if ((header.type_ != control::MessageType::kFindServiceResponse) ||
      ((n - sizeof(header)) % sizeof(someip_posix_common::someip::ServiceInstance) != 0)) {
    // TODO(PAASR-608)
    throw std::runtime_error("SomeIpPosix::FindService: invalid packet");
  }
  container.resize(header.length_ / sizeof(someip_posix_common::someip::ServiceInstance));
  return container;
}

void SomeIpPosix::StartFindService(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageStartFindService body{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kStartFindService, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::StopFindService(someip_posix_common::someip::ServiceId service_id,
                                  someip_posix_common::someip::InstanceId instance_id) {
  namespace control = someip_posix_common::someipd_posix::control;
  std::lock_guard<std::mutex> lock(control_socket_lock_);
  control::MessageStopFindService body{service_id, instance_id};
  control::MessageHeader header{control::kVersion, control::MessageType::kStopFindService, sizeof(body),
                                routing_channel_id_};
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = control_socket_.Send(osabstraction::io::network::socket::Socket::IovecContainer(iov));
  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Send");
  }
}

void SomeIpPosix::SetSomeipPosixApplication(SomeipPosixApplicationInterface* someip_posix_application) {
  someip_posix_application_ = someip_posix_application;
}

void SomeIpPosix::UnsetSomeipPosixApplication() { someip_posix_application_ = nullptr; }

bool SomeIpPosix::IsRunning() const { return someip_posix_running_; }

bool SomeIpPosix::ProcessNextPacket() { return HandleRead(routing_socket_.GetHandle()); }

bool SomeIpPosix::HandleRead(int handle) {
  try {
    routing_message_reader_.Read(&routing_socket_);
    if (routing_message_reader_.IsMessageAvailable()) {
      ProcessRoutingMessage(routing_message_reader_.GetNextMessage());
    }
    return EventHandler::HandleRead(handle);
  } catch (osabstraction::io::network::socket::SocketEOFException&) {
    logger_.LogError() << "The connection to the someipd-posix was lost.";
    someip_posix_running_ = false;
    if (shutdown_handler_) {
      shutdown_handler_->OnSomeIpPosixShutdown(*this);
    }
    return false;
  } catch (const std::system_error& e) {
    if (e.code().value() == ECONNRESET) {
      logger_.LogError() << "The connection to the someipd-posix was lost.";
      someip_posix_running_ = false;
      if (shutdown_handler_) {
        shutdown_handler_->OnSomeIpPosixShutdown(*this);
      }
      return false;
    } else if ((e.code().value() == EAGAIN) && (someip_config_.processing_mode_ ==
                                                someip_posix_common::config::SomeIpBindingProcessingMode::kPolling)) {
      // In full polling mode the routing socket is configured to be non-blocking. If there is no data on the socket,
      // POSIX systems will conventionally return -1 and errno EAGAIN.
      return false;
    } else {
      throw e;
    }
  }
}

void SomeIpPosix::ProcessRoutingMessage(someip_posix_common::someipd_posix::routing::MessageReader::Message&& message) {
  namespace routing = someip_posix_common::someipd_posix::routing;

  switch (message.header_.type_) {
    case routing::MessageType::kSomeIP: {
      if (someip_posix_application_ != nullptr) {
        someip_posix_common::someip::SomeIpPacket packet(new std::vector<std::uint8_t>(std::move(message.body_)));
        someip_posix_application_->HandleReceive(message.header_.instance_id_, std::move(packet));
      }

      break;
    }
    case routing::MessageType::kServiceDiscoveryOfferService: {
      if (someip_posix_application_) {
        someip_posix_common::someip::ServiceInstanceContainer container;
        container.resize(message.header_.length_ / sizeof(routing::MessageServiceDiscoveryOfferedServiceEntry));

        if (!container.empty()) {
          static_assert(sizeof(message.header_.length_) <= sizeof(std::size_t),
                        "Header length size is larger than std::size_t");
          std::memcpy(&container[0], &message.body_[0],
                      std::min(static_cast<std::size_t>(message.header_.length_),
                               (container.size() * sizeof(routing::MessageServiceDiscoveryOfferedServiceEntry))));
        }
        // Forwards the service list to the vector-specific ARA stack.
        // The service container is then translated into the type specified by ara::com.
        someip_posix_application_->HandleFindService(std::move(container));
      }

      break;
    }
    case routing::MessageType::kServiceDiscoveryEventSubscriptionState: {
      if (someip_posix_application_) {
        routing::MessageServiceDiscoveryEventSubscriptionStateEntry event_subscription_state_entry;

        static_assert(sizeof(message.header_.length_) <= sizeof(std::size_t),
                      "Header length size is larger than std::size_t");
        std::memcpy(&event_subscription_state_entry, &message.body_[0],
                    std::min(static_cast<std::size_t>(message.header_.length_),
                             sizeof(routing::MessageServiceDiscoveryEventSubscriptionStateEntry)));

        // Check subscription state for validity
        bool state_valid =
            (event_subscription_state_entry.state_ <=
             static_cast<std::uint16_t>(someip_posix_common::someip::SubscriptionState::kSubscriptionPending));
        if (state_valid) {
          someip_posix_common::someip::EventSubscriptionState event_subscription_state;
          event_subscription_state.service_id_ = event_subscription_state_entry.service_id_;
          event_subscription_state.instance_id_ = event_subscription_state_entry.instance_id_;
          event_subscription_state.event_id_ = event_subscription_state_entry.event_id_;
          event_subscription_state.state_ =
              static_cast<someip_posix_common::someip::SubscriptionState>(event_subscription_state_entry.state_);

          // Forwards the event details to the vector-specific ARA stack.
          // It is then translated into the type specified by ara::com.
          someip_posix_application_->HandleEventSubscriptionStateUpdate(event_subscription_state);
        } else {
          logger_.LogError() << "Invalid Event Subscription state received " << static_cast<std::uint16_t>(state_valid);
        }
      }
      break;
    }
    default: {
      // TODO(PAASR-608)
      logger_.LogError() << "unknown message type " << message.header_.type_;
      break;
    }
  }
}

void SomeIpPosix::ReadRoutingChannelId() {
  namespace routing = someip_posix_common::someipd_posix::routing;
  routing::MessageHeader header;
  routing::MessageChannelId body;
  std::array<struct iovec, 2> iov{{{&header, sizeof(header)}, {&body, sizeof(body)}}};
  std::size_t n = routing_socket_.Receive(osabstraction::io::network::socket::Socket::IovecContainer(iov));

  if (n != (sizeof(header) + sizeof(body))) {
    // TODO(PAASR-608)
    throw std::runtime_error("Socket::Receive");
  }
  if (header.type_ != routing::MessageType::kChannelId) {
    // TODO(PAASR-608)
    throw std::runtime_error("invalid packet");
  }
  routing_channel_id_ = body.channel_id_;
}

}  // namespace someip_posix
