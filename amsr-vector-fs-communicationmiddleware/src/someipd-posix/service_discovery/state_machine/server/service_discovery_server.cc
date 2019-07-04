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
/**        \file  service_discovery_server.cc
 *        \brief  Service Discovery Server Service Instance State Machine implementation
 *
 *      \details  This class creates and contains the SD server state machine. It receives all events and forwards them
 *to the state machine owner.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server.h"
#include <sstream>
#include "someip-posix-common/someip/message.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServer::ServiceDiscoveryServer(std::uint16_t service_id, std::uint16_t instance_id,
                                               std::uint8_t major_version, std::uint32_t minor_version,
                                               const std::string& address, vac::timer::TimerManager* timer_manager,
                                               ServiceDiscoveryMessageSender* message_sender,
                                               std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance,
                                               const configuration::Configuration* config)
    : ServiceDiscoveryServerServiceInstanceStateMachine(service_id, instance_id, major_version, minor_version,
                                                        timer_manager, message_sender),
      message_builder_(address,
                       config->GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version)),
      event_manager_(timer_manager, address, service_instance, config,
                     config->GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version),
                     message_builder_, *message_sender),
      message_send_queue_(message_builder_, *message_sender, *timer_manager),
      state_owner_(state_pool_, timer_manager),
      context_(state_owner_, address,
               config->GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version),
               service_instance, event_manager_, message_builder_, *message_sender, message_send_queue_),
      logger_(ara::log::CreateLogger(GetLoggerContextId(service_id, instance_id, major_version, minor_version), "")) {
  state_owner_.SetContext(&context_);
}

void ServiceDiscoveryServer::OnStart() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;
}

void ServiceDiscoveryServer::OnStop() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;

  state_owner_.OnServiceDown();
  state_owner_.OnNetworkDown();
}

void ServiceDiscoveryServer::OnNetworkUp() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;

  state_owner_.OnNetworkUp();
}

void ServiceDiscoveryServer::OnNetworkDown() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;

  state_owner_.OnNetworkDown();
}

void ServiceDiscoveryServer::OnServiceUp() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;

  state_owner_.OnServiceUp();
}

void ServiceDiscoveryServer::OnServiceDown() {
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << std::hex << ": (" << service_id_ << "," << instance_id_ << ")"
                     << std::dec;

  state_owner_.OnServiceDown();
}

// Received Message Handling

void ServiceDiscoveryServer::OnUnicastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                              someip_posix_common::someip::Port from_port,
                                              const ServiceDiscoveryEntryContainer& entries,
                                              const ServiceDiscoveryOptionContainer& options) {
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << std::hex << " ("
                     << service_id_ << "," << instance_id_ << ")" << std::dec;
  for (auto it_entry = entries.cbegin(); it_entry != entries.cend(); ++it_entry) {
    // Skip foreign entries
    if (!IsMatchingEntry(*it_entry)) {
      logger_.LogDebug() << __func__ << ":" << __LINE__ << " - Skipped Unicast entry "
                         << " (" << it_entry->service_id_ << "," << it_entry->instance_id_ << ","
                         << static_cast<uint16_t>(it_entry->major_version_) << "," << it_entry->minor_version_ << ")"
                         << std::dec;
      continue;
    }
    switch (it_entry->type_) {
      case someip::SomeIpSdEntryType::kSubscribeEventgroup:
        if (it_entry->ttl_ > 0) {
          state_owner_.OnSubscribeEventgroupEntry(from_address, from_port, *it_entry, options);
        } else {
          state_owner_.OnStopSubscribeEventgroupEntry(from_address, from_port, *it_entry, options);
        }
        break;
      case someip::SomeIpSdEntryType::kFindService:
        // Note:  Currently this specification does not allow sending “FindService Entries” using
        // unicast. For compatibility reasons receiving such entries shall be supported.
        state_owner_.OnUnicastFindServiceEntry(from_address, from_port, *it_entry, options);
        break;
      default:
        break;
    }
  }
}

void ServiceDiscoveryServer::OnMulticastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                                someip_posix_common::someip::Port from_port,
                                                const ServiceDiscoveryEntryContainer& entries,
                                                const ServiceDiscoveryOptionContainer& options) {
  namespace someip = someip_posix_common::someip;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": from " << from_address << "," << from_port << std::hex << " ("
                     << service_id_ << "," << instance_id_ << ")" << std::dec;

  for (auto it_entry = entries.cbegin(); it_entry != entries.cend(); ++it_entry) {
    // Skip foreign entries
    if (!IsMatchingEntry(*it_entry)) {
      logger_.LogDebug() << __func__ << ":" << __LINE__ << " - Skipped Multicast entry "
                         << " (" << it_entry->service_id_ << "," << it_entry->instance_id_ << ","
                         << static_cast<uint16_t>(it_entry->major_version_) << "," << it_entry->minor_version_ << ")"
                         << std::dec;
      continue;
    }
    switch (it_entry->type_) {
      case someip::SomeIpSdEntryType::kFindService:
        state_owner_.OnMulticastFindServiceEntry(from_address, from_port, *it_entry, options);
        break;
      case someip::SomeIpSdEntryType::kSubscribeEventgroup:
        // Note:  Currently this specification does not allow sending “SubscribeEventgroup Entries” using
        // multicast. For compatibility reasons receiving such entries shall be supported.
        if (it_entry->ttl_ > 0) {
          state_owner_.OnSubscribeEventgroupEntry(from_address, from_port, *it_entry, options);
        } else {
          state_owner_.OnStopSubscribeEventgroupEntry(from_address, from_port, *it_entry, options);
        }
        break;
      default:
        break;
    }
  }
}

void ServiceDiscoveryServer::OnRebootDetected(const someip_posix_common::someip::IpAddress& from_address,
                                              someip_posix_common::someip::Port from_port) {
  event_manager_.UnsubscribeAllEventgroupsOnReboot(from_address, from_port);
}

bool ServiceDiscoveryServer::IsMatchingEntry(const ServiceDiscoveryEntry& entry) const {
  return (entry.service_id_ == service_id_) && ((entry.instance_id_ == someip_posix_common::someip::kInstanceIdAny) ||
                                                (entry.instance_id_ == instance_id_)) &&
         ((entry.major_version_ == someip_posix_common::someip::kMajorVersionAny) ||
          (entry.major_version_ == major_version_));
}

std::string ServiceDiscoveryServer::GetLoggerContextId(std::uint16_t service_id, std::uint16_t instance_id,
                                                       std::uint8_t major_version, std::uint32_t minor_version) {
  std::stringstream ss;
  ss << "ServiceDiscoveryServer" << std::hex << "(" << service_id << "," << instance_id << ","
     << static_cast<std::size_t>(major_version) << "," << minor_version << ")";
  return ss.str();
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
