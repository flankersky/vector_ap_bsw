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
/**        \file  packet_router.cc
 *        \brief  Packet router
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/packet_router/packet_router.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <algorithm>
#include <ios>
#include <utility>
#include "ara/log/logging.hpp"

#include "someip-posix-common/someip/message_builder.h"
#include "someip-posix-common/someip/message_verification.h"

namespace someipd_posix {
namespace packet_router {

PacketRouter::PacketRouter(const configuration::Configuration* config)
    : config_(config), logger_(ara::log::CreateLogger("PacketRouter", "")) {}

void PacketRouter::AddRequestRoute(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id,
                                   std::shared_ptr<PacketSink> to) {
  logger_.LogDebug() << "add new request route for (" << std::hex << service_id << ", " << instance_id << ")"
                     << std::dec;
  request_routing_table_.insert({{service_id, instance_id}, to});
}

void PacketRouter::DeleteRequestRoute(someip_posix_common::someip::ServiceId service_id,
                                      someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << "delete request route for (" << std::hex << service_id << ", " << instance_id << ")"
                     << std::dec;
  CleanUpFieldCacheTableEntries(service_id, instance_id);
  request_routing_table_.erase({service_id, instance_id});
}

void PacketRouter::AddEventRoute(someip_posix_common::someip::ServiceId service_id,
                                 someip_posix_common::someip::InstanceId instance_id,
                                 someip_posix_common::someip::EventId event_id, std::shared_ptr<PacketSink> to) {
  logger_.LogDebug() << "add new event route for (" << std::hex << service_id << ", " << instance_id << ", " << event_id
                     << ")" << std::dec;
  auto it_cont = event_routing_table_.find({service_id, instance_id, event_id});
  if (it_cont == event_routing_table_.end()) {
    event_routing_table_.insert({{service_id, instance_id, event_id}, PacketSinkContainer{to}});
  } else {
    it_cont->second.push_back(to);
  }
}

void PacketRouter::DeleteEventRoute(someip_posix_common::someip::ServiceId service_id,
                                    someip_posix_common::someip::InstanceId instance_id,
                                    someip_posix_common::someip::EventId event_id,
                                    std::shared_ptr<packet_router::PacketSink> to) {
  logger_.LogDebug() << "delete event route for (" << std::hex << service_id << ", " << instance_id << ", " << event_id
                     << ")" << std::dec;
  auto it_cont = event_routing_table_.find({service_id, instance_id, event_id});
  if (it_cont != event_routing_table_.end()) {
    auto it_sink = std::find_if(it_cont->second.begin(), it_cont->second.end(),
                                [to](std::shared_ptr<PacketSink>& other) { return other == to; });
    if (it_sink != it_cont->second.end()) {
      it_cont->second.erase(it_sink);
      // Destroy packet sink container if it's empty
      if (it_cont->second.empty()) {
        event_routing_table_.erase(it_cont);
      }
    }
  }
}

void PacketRouter::AddEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                                      someip_posix_common::someip::InstanceId instance_id,
                                      someip_posix_common::someip::EventgroupId eventgroup_id,
                                      std::shared_ptr<PacketSink> to) {
  logger_.LogDebug() << "add new eventgroup route for (" << std::hex << service_id << ", " << instance_id << ", "
                     << eventgroup_id << ")" << std::dec;
  auto it_cont = eventgroup_routing_table_.find({service_id, instance_id, eventgroup_id});
  if (it_cont == eventgroup_routing_table_.end()) {
    eventgroup_routing_table_.insert({{service_id, instance_id, eventgroup_id}, PacketSinkContainer{to}});
  } else {
    it_cont->second.push_back(to);
  }
}

void PacketRouter::DeleteEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                                         someip_posix_common::someip::InstanceId instance_id,
                                         someip_posix_common::someip::EventgroupId eventgroup_id,
                                         std::shared_ptr<packet_router::PacketSink> to) {
  logger_.LogDebug() << "delete eventgroup route for (" << std::hex << service_id << ", " << instance_id << ", "
                     << eventgroup_id << ")" << std::dec;
  auto it_cont = eventgroup_routing_table_.find({service_id, instance_id, eventgroup_id});
  if (it_cont != eventgroup_routing_table_.end()) {
    auto it_sink = std::find_if(it_cont->second.begin(), it_cont->second.end(),
                                [to](std::shared_ptr<PacketSink>& other) { return other == to; });
    if (it_sink != it_cont->second.end()) {
      it_cont->second.erase(it_sink);
      // Destroy packet sink container if it's empty
      if (it_cont->second.empty()) {
        eventgroup_routing_table_.erase(it_cont);
      }
    }
  }
}

void PacketRouter::Forward(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
                           Packet packet) {
  const someip_posix_common::someip::SomeIpMessageHeader& header = packet->GetHeader();

  switch (header.message_type_) {
    case someip_posix_common::someip::SomeIpMessageType::kNotification: {
      const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
          someip_posix_common::someip::EventMessageVerification::DoInfrastructuralChecks(header)};

      if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
        ForwardEvent(instance_id, packet);
      } else {
        logger_.LogError() << "Event message verification failed with return code " << verify_return_code << " - ("
                           << std::hex << header.service_id_ << ", " << instance_id << "): "
                           << "event ID: " << header.method_id_ << ", session ID: " << header.session_id_ << std::dec
                           << ". No forwarding.";
      }

      break;
    }
    case someip_posix_common::someip::SomeIpMessageType::kResponse: {
      const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
          someip_posix_common::someip::ResponseMessageVerification::DoInfrastructuralChecks(header)};

      if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
        ForwardResponse(instance_id, packet);
      } else {
        logger_.LogError() << "Method response message verification failed with return code " << verify_return_code
                           << " - (" << std::hex << header.service_id_ << ", " << instance_id << "): "
                           << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_ << std::dec
                           << ". No forwarding.";
      }

      break;
    }
    case someip_posix_common::someip::SomeIpMessageType::kRequest: {
      const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
          someip_posix_common::someip::RequestMessageVerification::DoInfrastructuralChecks(header)};

      if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
        ForwardRequest(instance_id, from, packet);
      } else {
        someip_posix_common::someip::SomeIpPacket error_res{
            someip_posix_common::someip::SomeIpPacket{new someip_posix_common::someip::PacketBuffer{}}};
        error_res =
            someip_posix_common::someip::CreateSomeIpErrorHeader(verify_return_code, header, std::move(error_res));
        // Use the packet for the error message - swap it with the original request.
        packet.reset(new someip_posix_common::someip::SomeIpMessage(std::move(*error_res)));
        from->Forward(instance_id, packet);
        logger_.LogError() << "Method request message verification failed with return code " << verify_return_code
                           << " - (" << std::hex << header.service_id_ << ", " << instance_id << "): "
                           << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_ << std::dec
                           << ". Sending error response.";
      }

      break;
    }
    case someip_posix_common::someip::SomeIpMessageType::kRequestNoReturn: {
      const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
          someip_posix_common::someip::RequestNoReturnMessageVerification::DoInfrastructuralChecks(header)};

      if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
        ForwardRequestNoReturn(instance_id, from, packet);
      } else {
        // No error response message shall be sent for fire & forget methods. Print an error logging message only.
        logger_.LogError() << "Fire & forget message verification failed with return code " << verify_return_code
                           << " - (" << std::hex << header.service_id_ << ", " << instance_id << "): "
                           << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_ << std::dec
                           << ". No forwarding.";
      }

      break;
    }
    case someip_posix_common::someip::SomeIpMessageType::kError: {
      const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
          someip_posix_common::someip::ErrorMessageVerification::DoInfrastructuralChecks(header)};

      if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
        ForwardResponse(instance_id, packet);
      } else {
        logger_.LogError() << "Generic error response message verification failed with return code "
                           << verify_return_code << ". Will not route this error response to a binding instance."
                           << " - (" << std::hex << header.service_id_ << ", " << instance_id << "): "
                           << "method/event ID: " << header.method_id_ << ", session ID: " << header.session_id_
                           << std::dec << ". No forwarding.";
      }

      break;
    }
    default: {
      logger_.LogError() << "unknown SOME/IP message type " << std::hex
                         << static_cast<std::size_t>(header.message_type_) << std::dec;
      break;
    }
  }
}

void PacketRouter::SendInitialEvent(const someip_posix_common::someip::ServiceId service_id,
                                    const someip_posix_common::someip::InstanceId instance_id,
                                    const someip_posix_common::someip::EventId event_id, PacketSink& sink) {
  const auto event = config_->GetEvent(service_id, event_id);
  if (event && event->is_field_) {
    auto it_cache = field_cache_table_.find({service_id, instance_id});
    if (it_cache != field_cache_table_.end()) {
      auto& field_cache = it_cache->second;
      auto packet = field_cache.GetValue(event_id);
      if (packet) {
        sink.Forward(instance_id, packet);
      }
    }
  }
}

void PacketRouter::SendInitialEvents(const someip_posix_common::someip::ServiceId service_id,
                                     const someip_posix_common::someip::InstanceId instance_id,
                                     const someip_posix_common::someip::EventgroupId eventgroup_id,
                                     PacketSink* tcp_packet_sink, PacketSink* udp_packet_sink) {
  using Event = someipd_posix::configuration::ConfigurationTypesAndDefs::Event;

  auto it_cache = field_cache_table_.find({service_id, instance_id});
  if (it_cache != field_cache_table_.end()) {
    auto& field_cache = it_cache->second;
    config_->ForeachEventgroupField(service_id, eventgroup_id, [this, &field_cache, tcp_packet_sink, udp_packet_sink,
                                                                service_id, instance_id](const Event* event) {
      auto packet = field_cache.GetValue(event->id_);
      if (packet) {
        switch (event->proto_) {
          case configuration::ConfigurationTypesAndDefs::Protocol::kTCP:
            if (tcp_packet_sink) {
              tcp_packet_sink->Forward(instance_id, packet);
            }
            break;
          case configuration::ConfigurationTypesAndDefs::Protocol::kUDP:
            if (udp_packet_sink) {
              udp_packet_sink->Forward(instance_id, packet);
            }
            break;
          default:
            break;
        }
      }
    });
  }
}

void PacketRouter::CleanUpRequestRoutingTableEntries(const PacketSink* sink) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  field_cache_table_.clear();
  for (auto it = request_routing_table_.begin(); it != request_routing_table_.end(); ++it) {
    if (it->second.get() == sink) {
      it = request_routing_table_.erase(it);
    }
  }
}

void PacketRouter::CleanUpResponseRoutingTableEntries(const PacketSink* sink) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  response_routing_table_.erase(
      std::remove_if(response_routing_table_.begin(), response_routing_table_.end(),
                     [sink](const ResponseRoute& other) { return other.from_.get() == sink; }),
      response_routing_table_.end());
}

void PacketRouter::CleanUpEventRoutingTableEntries(const PacketSink* sink) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (auto it_ev = event_routing_table_.begin(); it_ev != event_routing_table_.end();) {
    for (auto it_sink = it_ev->second.begin(); it_sink != it_ev->second.end();) {
      if (it_sink->get() == sink) {
        it_sink = it_ev->second.erase(it_sink);
      } else {
        ++it_sink;
      }
    }
    if (it_ev->second.empty()) {
      it_ev = event_routing_table_.erase(it_ev);
    } else {
      ++it_ev;
    }
  }
}

void PacketRouter::CleanUpEventgroupRoutingTableEntries(const PacketSink* sink) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (auto it_eg = eventgroup_routing_table_.begin(); it_eg != eventgroup_routing_table_.end();) {
    for (auto it_sink = it_eg->second.begin(); it_sink != it_eg->second.end();) {
      if (it_sink->get() == sink) {
        it_sink = it_eg->second.erase(it_sink);
      } else {
        ++it_sink;
      }
    }
    if (it_eg->second.empty()) {
      it_eg = eventgroup_routing_table_.erase(it_eg);
    } else {
      ++it_eg;
    }
  }
}

void PacketRouter::CleanUpFieldCacheTableEntries(const someip_posix_common::someip::ServiceId service_id,
                                                 const someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  field_cache_table_.erase({service_id, instance_id});
}

void PacketRouter::ForwardRequest(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
                                  Packet packet) {
  auto header = packet->GetHeader();

  const auto service_id = header.service_id_;
  const auto method_id = header.method_id_;
  const auto client_id = header.client_id_;
  const auto session_id = header.session_id_;
  auto it = request_routing_table_.find({service_id, instance_id});
  if (it != request_routing_table_.end()) {
    response_routing_table_.push_back({service_id, instance_id, client_id, session_id, from});
    it->second->Forward(instance_id, packet);
  } else {
    // Assume that the service and method is known, but the application is not
    // ready yet, as there was no entry in the request_routing_table for this
    // service instance.
    someip_posix_common::someip::SomeIpReturnCode return_code{someip_posix_common::someip::SomeIpReturnCode::kNotReady};
    const auto* cfg_service = config_->GetService(service_id);
    const auto* cfg_method = config_->GetMethod(service_id, method_id);

    if (cfg_service) {
      if (!cfg_method) {
        // If the service is known, but the method is not known to the configuration model.
        return_code = someip_posix_common::someip::SomeIpReturnCode::kUnknownMethod;
      }
    } else {
      // In case the complete service is not known at all.
      return_code = someip_posix_common::someip::SomeIpReturnCode::kUnknownService;
    }

    someip_posix_common::someip::SomeIpPacket error_res{
        someip_posix_common::someip::SomeIpPacket{new someip_posix_common::someip::PacketBuffer{}}};
    error_res = someip_posix_common::someip::CreateSomeIpErrorHeader(return_code, header, std::move(error_res));
    // Use the packet for the error message - swap it with the original request.
    packet.reset(new someip_posix_common::someip::SomeIpMessage(std::move(*error_res)));
    from->Forward(instance_id, packet);

    logger_.LogError() << "no route was found for request (" << std::hex << service_id << ", " << instance_id << ", "
                       << method_id << ", " << client_id << ", " << session_id << ", "
                       << static_cast<std::uint16_t>(return_code) << ")" << std::dec;
  }
}

void PacketRouter::ForwardRequestNoReturn(someip_posix_common::someip::InstanceId instance_id,
                                          std::shared_ptr<PacketSink> from, Packet packet) {
  (void)from;
  auto header = packet->GetHeader();
  const auto service_id = header.service_id_;
  const auto method_id = header.method_id_;
  const auto client_id = header.client_id_;
  const auto session_id = header.session_id_;
  auto it = request_routing_table_.find({service_id, instance_id});
  if (it != request_routing_table_.end()) {
    it->second->Forward(instance_id, packet);
  } else {
    logger_.LogError() << "no route was found for request (" << std::hex << service_id << ", " << instance_id << ", "
                       << method_id << ", " << client_id << ", " << session_id << ")" << std::dec;
  }
}

void PacketRouter::ForwardResponse(someip_posix_common::someip::InstanceId instance_id, Packet packet) {
  auto header = packet->GetHeader();
  const auto service_id = header.service_id_;
  const auto method_id = header.method_id_;
  const auto client_id = header.client_id_;
  const auto session_id = header.session_id_;
  auto it = std::find_if(response_routing_table_.begin(), response_routing_table_.end(),
                         [service_id, instance_id, client_id, session_id](const ResponseRoute& rr) {
                           return ((rr.service_id_ == service_id) && (rr.instance_id_ == instance_id) &&
                                   (rr.client_id_ == client_id) && (rr.session_id_ == session_id));
                         });
  if (it != response_routing_table_.end()) {
    it->from_->Forward(instance_id, packet);
    response_routing_table_.erase(it);
  } else {
    logger_.LogDebug() << "response (" << std::hex << service_id << ", " << instance_id << ", " << method_id << ", "
                       << client_id << ", " << session_id << ") could not be routed" << std::dec;
  }
}

void PacketRouter::ForwardEvent(someip_posix_common::someip::InstanceId instance_id, Packet packet) {
  auto header = packet->GetHeader();
  const auto service_id = header.service_id_;
  const auto method_id = header.method_id_;
  logger_.LogDebug() << "event (" << std::hex << service_id << ", " << instance_id << ", " << method_id << ")"
                     << std::dec;
  /* Forward to single event subscribers */
  logger_.LogDebug() << "routing to event subscribers";
  auto it_cont_e = event_routing_table_.find({service_id, instance_id, method_id});
  if (it_cont_e != event_routing_table_.end()) {
    std::for_each(it_cont_e->second.begin(), it_cont_e->second.end(),
                  [instance_id, packet](std::shared_ptr<PacketSink> to) { to->Forward(instance_id, packet); });
  }
  /* Forward to eventgroup subscribers */
  logger_.LogDebug() << "routing to eventgroup subscribers";
  for (auto eventgroup_id : config_->EventToEventgroups(service_id, method_id)) {
    logger_.LogDebug() << std::hex << "event " << method_id << " maps to eventgroup " << eventgroup_id << std::dec;
    auto it_cont_eg = eventgroup_routing_table_.find({service_id, instance_id, eventgroup_id});
    if (it_cont_eg != eventgroup_routing_table_.end()) {
      std::for_each(it_cont_eg->second.begin(), it_cont_eg->second.end(),
                    [instance_id, packet](std::shared_ptr<PacketSink> to) { to->Forward(instance_id, packet); });
    }
  }

  // Field Notification ? Cache it
  const auto event = config_->GetEvent(service_id, method_id);
  assert(event && "No such event found");
  if (event && event->is_field_) {
    // Create Cache if not found
    auto result =
        field_cache_table_.insert({{service_id, instance_id}, PacketCache<someip_posix_common::someip::EventId>{}});

    // Update value in cache
    auto& field_cache = result.first->second;
    field_cache.InsertOrAssign(method_id, packet);
  }
}

const PacketRouter::FieldCacheTable& PacketRouter::GetFieldCacheTable() { return field_cache_table_; }

}  // namespace packet_router
}  // namespace someipd_posix
