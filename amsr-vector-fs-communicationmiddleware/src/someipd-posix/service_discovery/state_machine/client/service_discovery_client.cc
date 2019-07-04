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
/**        \file  service_discovery_client.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client.h"
#include <algorithm>
#include <cassert>
#include <ios>
#include <stdexcept>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

ServiceDiscoveryClient::ServiceDiscoveryClient(std::uint16_t service_id, std::uint16_t instance_id,
                                               std::uint8_t major_version, std::uint32_t minor_version,
                                               vac::timer::TimerManager* timer_manager,
                                               ServiceDiscoveryMessageSender* message_sender,
                                               std::shared_ptr<ServiceDiscoveryClientServiceInstance> service_instance,
                                               const configuration::Configuration* config,
                                               ServiceDiscoveryClientContext* context)
    : ServiceDiscoveryClientServiceInstanceStateMachine(service_id, instance_id, major_version, minor_version,
                                                        timer_manager, message_sender),
      logger_{ara::log::CreateLogger(GetLoggerContextId(service_id, instance_id, major_version, minor_version), "")},
      service_instance_{service_instance},
      instance_config_(config->GetRequiredServiceInstance(service_id, instance_id, major_version, minor_version)),
      state_machine_{this, timer_manager},
      eventgroup_manager_{config->GetEventgroupIds(service_id), this, timer_manager},
      message_builder_{instance_config_},
      requested_counter_{0},
      is_available_{false},
      context_{context} {}

void ServiceDiscoveryClient::OnStart() { logger_.LogDebug() << __func__ << ":" << __LINE__; }

void ServiceDiscoveryClient::OnStop() { logger_.LogDebug() << __func__ << ":" << __LINE__; }

void ServiceDiscoveryClient::OnNetworkUp() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_machine_.OnNetworkUp();
}

void ServiceDiscoveryClient::OnNetworkDown() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  state_machine_.OnNetworkDown();
}

void ServiceDiscoveryClient::OnServiceRequested() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": requested counter " << requested_counter_.load();
  ++requested_counter_;
  /* Only the first request shall be forwarded to the state machine. */
  if (requested_counter_ == 1) {
    state_machine_.OnServiceRequested();
    if (is_available_) {
      Connect();
    }
  }
}

void ServiceDiscoveryClient::OnServiceReleased() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": requested counter " << requested_counter_.load();
  if (requested_counter_ > 0) {
    --requested_counter_;
    /* Only the last release shall be forwarded to the state machine. */
    if (requested_counter_ == 0) {
      if (is_available_) {
        Disconnect();
      }
      state_machine_.OnServiceReleased();
      eventgroup_manager_.OnServiceReleased();
    }
  } else {
    throw std::logic_error{"ServiceDiscoveryClient: service is already released"};
  }
}

void ServiceDiscoveryClient::OnSubscribeEventgroup(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  eventgroup_manager_.OnSubscribe(eventgroup_id);
}

void ServiceDiscoveryClient::OnStopSubscribeEventgroup(std::uint16_t eventgroup_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  eventgroup_manager_.OnUnsubscribe(eventgroup_id);
}

ServiceDiscoveryEventSubscriptionState ServiceDiscoveryClient::GetEventgroupSubscriptionState(
    std::uint16_t eventgroup_id) const {
  return eventgroup_manager_.GetEventgroupSubscriptionState(eventgroup_id);
}

std::size_t ServiceDiscoveryClient::GetEventgroupSubscribersCount(std::uint16_t eventgroup_id) const {
  return eventgroup_manager_.GetEventgroupSubscribersCount(eventgroup_id);
}

void ServiceDiscoveryClient::OnUnicastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                              someip_posix_common::someip::Port from_port,
                                              const ServiceDiscoveryEntryContainer& entries,
                                              const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << " #entries "
                     << entries.size() << " #options " << options.size();
  auto offers = FindOffers(from_address, from_port, entries, options, false);
  std::for_each(offers.cbegin(), offers.cend(),
                [this](const ServiceDiscoveryClientOffer& o) { this->OnOfferMatch(o); });
  ProcessEventgroupSubscribeAcksAndNacks(from_address, from_port, entries, options);
}

void ServiceDiscoveryClient::OnMulticastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                                someip_posix_common::someip::Port from_port,
                                                const ServiceDiscoveryEntryContainer& entries,
                                                const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << " #entries "
                     << entries.size() << " #options " << options.size();
  auto offers = FindOffers(from_address, from_port, entries, options, true);
  /* Process each matching service offer */
  std::for_each(offers.cbegin(), offers.cend(),
                [this](const ServiceDiscoveryClientOffer& o) { this->OnOfferMatch(o); });
}

void ServiceDiscoveryClient::OnRebootDetected(const someip_posix_common::someip::IpAddress& from_address,
                                              someip_posix_common::someip::Port from_port) {
  logger_.LogDebug() << __func__ << ":" << __LINE__
                     << ": OnStopOfferService reboot detection for address :" << from_address << " Port :" << from_port;
  active_offer_.Invalidate();
  state_machine_.OnStopOfferService();
  eventgroup_manager_.OnStopOfferService();
}

void ServiceDiscoveryClient::SetServiceAvailable(bool is_available) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": available " << std::boolalpha << is_available << "("
                     << is_available_ << ")" << std::noboolalpha << " requested counter " << requested_counter_.load();
  if ((requested_counter_ > 0) && (is_available_ != is_available)) {
    is_available_ = is_available;
    if (is_available_) {
      Connect();
    } else {
      Disconnect();
    }
  }
}

ServiceDiscoveryClient::TimerDelay ServiceDiscoveryClient::GetOfferServiceTtl() const {
  if (active_offer_.IsValid()) {
    return std::chrono::seconds{active_offer_.GetEntry().ttl_};
  } else {
    return std::chrono::seconds{0};
  }
}

ServiceDiscoveryClient::TimerDelay ServiceDiscoveryClient::GetFindServiceInitialDelayMin() const {
  return std::chrono::nanoseconds{instance_config_->service_discovery_.initial_delay_min_ns_};
}

ServiceDiscoveryClient::TimerDelay ServiceDiscoveryClient::GetFindServiceInitialDelayMax() const {
  return std::chrono::nanoseconds{instance_config_->service_discovery_.initial_delay_max_ns_};
}

std::size_t ServiceDiscoveryClient::GetFindServiceRepetitionsMax() const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  return instance_config_->service_discovery_.initial_repetitions_max_;
}

ServiceDiscoveryClient::TimerDelay ServiceDiscoveryClient::GetFindServiceRepetitionsBaseDelay() const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  return std::chrono::nanoseconds{instance_config_->service_discovery_.initial_repetitions_base_delay_ns_};
}

void ServiceDiscoveryClient::SendFindService() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  auto message = message_builder_.MakeFindServiceMessage();
  message_sender_->SendMulticastMessage(message.first, message.second);
}

void ServiceDiscoveryClient::SendSubscribeEventgroup(std::uint16_t eventgroup_id, bool request_initial_events) {
  assert(active_offer_.IsValid());
  auto sd_ep = active_offer_.GetSdEndpoint();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " to <" << sd_ep.first << "," << sd_ep.second << ">"
                     << " eventgroup " << std::hex << eventgroup_id << std::dec << " request initial events "
                     << std::boolalpha << request_initial_events << std::noboolalpha;
  auto message = message_builder_.MakeSubscribeEventgroupMessage(eventgroup_id, request_initial_events);
  message_sender_->SendUnicastMessage(sd_ep.first, sd_ep.second, message.first, message.second);
}

void ServiceDiscoveryClient::SendStopSubscribeEventgroup(std::uint16_t eventgroup_id) {
  assert(active_offer_.IsValid());
  auto sd_ep = active_offer_.GetSdEndpoint();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " to <" << sd_ep.first << "," << sd_ep.second << ">"
                     << " eventgroup " << std::hex << eventgroup_id << std::dec;
  auto message = message_builder_.MakeStopSubscribeEventgroupMessage(eventgroup_id);
  message_sender_->SendUnicastMessage(sd_ep.first, sd_ep.second, message.first, message.second);
}

void ServiceDiscoveryClient::OnEventgroupSubscribed(std::uint16_t eventgroup_id) {
  using cfg = configuration::Configuration;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  const auto ret = eventgroup_manager_.GetEventgroupMulticastEndpoint(eventgroup_id);
  const auto mcast_addr = ret.first;
  const auto mcast_port = ret.second;
  if (mcast_port != cfg::kInvalidPort) {
    service_instance_->StartListenForMulticastEventgroup(mcast_addr, mcast_port, eventgroup_id);
  }
  context_->OnEventgroupSubscribed(service_id_, instance_id_, eventgroup_id);
}

void ServiceDiscoveryClient::OnEventgroupUnsubscribed(std::uint16_t eventgroup_id) {
  using cfg = configuration::Configuration;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": eventgroup " << std::hex << eventgroup_id << std::dec;
  const auto ret = eventgroup_manager_.GetEventgroupMulticastEndpoint(eventgroup_id);
  const auto mcast_addr = ret.first;
  const auto mcast_port = ret.second;
  if (mcast_port != cfg::kInvalidPort) {
    service_instance_->StopListenForMulticastEventgroup(eventgroup_id);
  }
  context_->OnEventgroupUnsubscribed(service_id_, instance_id_, eventgroup_id);
}

void ServiceDiscoveryClient::OnEventgroupSubscriptionPending(std::uint16_t eventgroup_id) {
  context_->OnEventgroupSubscriptionPending(service_id_, instance_id_, eventgroup_id);
}

void ServiceDiscoveryClient::OnOfferMatch(ServiceDiscoveryClientOffer offer) {
  (void)offer;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (offer.IsStopOffer()) {
    active_offer_.Invalidate();
    state_machine_.OnStopOfferService();
    eventgroup_manager_.OnStopOfferService();
  } else {
    active_offer_ = offer;
    state_machine_.OnOfferService();
    eventgroup_manager_.OnOfferService(offer.IsMulticast());
  }
}

ServiceDiscoveryClient::ServiceDiscoveryClientOfferContainer ServiceDiscoveryClient::FindOffers(
    const std::string& from_address, std::uint16_t from_port, const ServiceDiscoveryEntryContainer& entries,
    const ServiceDiscoveryOptionContainer& options, bool is_multicast) {
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << " #entries "
                     << entries.size() << " #options " << options.size();
  ServiceDiscoveryClientOfferContainer offers;
  for (const auto& entry : entries) {
    if ((entry.type_ == someip::SomeIpSdEntryType::kOfferService) && (entry.service_id_ == service_id_) &&
        (entry.instance_id_ == instance_id_) &&
        ((major_version_ == someip::kMajorVersionAny) || (entry.major_version_ == major_version_)) &&
        ((minor_version_ == someip::kMinorVersionAny) || (entry.minor_version_ == minor_version_))) {
      ServiceDiscoveryClientOffer offer{from_address, from_port, entry, options, is_multicast};
      if (offer.IsValid()) {
        offers.push_back(std::move(offer));
      }
    }
  }
  return offers;
}

void ServiceDiscoveryClient::ProcessEventgroupSubscribeAcksAndNacks(const std::string& from_address,
                                                                    std::uint16_t from_port,
                                                                    const ServiceDiscoveryEntryContainer& entries,
                                                                    const ServiceDiscoveryOptionContainer& options) {
  using cfg = configuration::Configuration;
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << " #entries "
                     << entries.size() << " #options " << options.size();
  for (const auto& entry : entries) {
    // Skip foreign entries
    if ((entry.service_id_ != service_id_) || (entry.instance_id_ != instance_id_) ||
        ((major_version_ != someip::kMajorVersionAny) && (entry.major_version_ != major_version_))) {
      continue;
    }
    // We are interested only in 'SubscribeEventgroupAck' or 'SubscribeEventgroupNack' messages
    if (entry.type_ == someip::SomeIpSdEntryType::kSubscribeEventgroupAck) {
      if (entry.ttl_ > 0) {
        assert((entry.number_1st_opts_ <= 1) && (entry.number_2nd_opts_ == 0));
        if (entry.number_1st_opts_ > 0) {
          const ServiceDiscoveryOption& option = options[entry.index_1st_opts_];
          eventgroup_manager_.OnSubscribeEventgroupAck(entry.eventgroup_id_, option.address_, option.port_,
                                                       std::chrono::seconds{entry.ttl_});
        } else {
          eventgroup_manager_.OnSubscribeEventgroupAck(entry.eventgroup_id_, cfg::kInvalidAddress, cfg::kInvalidPort,
                                                       std::chrono::seconds{entry.ttl_});
        }
      } else {
        eventgroup_manager_.OnSubscribeEventgroupNack(entry.eventgroup_id_);
      }
    }
  }
}

void ServiceDiscoveryClient::Connect() {
  assert(active_offer_.IsValid());
  auto tcp_ep = active_offer_.GetTcpEndpoint();
  auto udp_ep = active_offer_.GetUdpEndpoint();
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": tcp <" << tcp_ep.first << "," << tcp_ep.second << "> udp <"
                     << udp_ep.first << "," << udp_ep.second << ">";
  service_instance_->Connect(tcp_ep.first, tcp_ep.second, udp_ep.first, udp_ep.second);
}

void ServiceDiscoveryClient::Disconnect() { service_instance_->Disconnect(); }

std::string ServiceDiscoveryClient::GetLoggerContextId(std::uint16_t service_id, std::uint16_t instance_id,
                                                       std::uint8_t major_version, std::uint32_t minor_version) {
  std::stringstream ss;
  ss << "ServiceDiscoveryClient" << std::hex << "(" << service_id << "," << instance_id << ","
     << static_cast<std::size_t>(major_version) << "," << minor_version << ")";
  return ss.str();
}

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
