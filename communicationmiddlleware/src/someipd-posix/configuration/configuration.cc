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
/**        \file  someipd-posix/configuration/configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/configuration/configuration.h"
#include <limits>

namespace someipd_posix {
namespace configuration {

constexpr someip_posix_common::someip::Port ConfigurationTypesAndDefs::kInvalidPort;

constexpr const char* ConfigurationTypesAndDefs::kInvalidAddress;

Configuration::Configuration() : logger_(ara::log::CreateLogger("Configuration", "")) {}

bool Configuration::HasSdAddressAndPort(const someip_posix_common::someip::IpAddress& address,
                                        someip_posix_common::someip::Port port) const {
  for (const auto& nec : network_endpoints_) {
    if (nec.address_ == address && nec.service_discovery_.port_ == port) {
      return true;
    }
  }
  return false;
}

Configuration::EventgroupIdContainer Configuration::EventToEventgroups(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::EventId event_id) const {
  EventgroupIdContainer eventgroup_ids{};
  auto service = GetService(service_id);
  if (service) {
    for (const auto& eg : service->eventgroups_) {
      auto it = std::find(eg.events_.cbegin(), eg.events_.cend(), event_id);
      if (it != eg.events_.end()) {
        eventgroup_ids.push_back(eg.id_);
      }
    }
  }
  return eventgroup_ids;
}

Configuration& Configuration::operator<<(const ServiceContainer& services) {
  for (const auto& service_to_insert : services) {
    const auto it = std::find_if(services_.cbegin(), services_.cend(),
                                 [&](const Service& service) { return (service == service_to_insert); });

    if (it != services_.cend()) {
      const Service& existing_service = *it;
      logger_.LogDebug() << "Duplicate of service found in configuration. Will not insert service with ID: "
                         << service_to_insert.id_
                         << ", major version: " << static_cast<int>(service_to_insert.major_version_)
                         << ", minor version: " << service_to_insert.minor_version_;

      // Mount additional consistency checks here, if the services are equal
      const bool methods_equal = CheckEquality(existing_service.methods_, service_to_insert.methods_);
      assert(methods_equal &&
             "Inconsistent method data definitions for one service interface across multiple application "
             "configurations. Please correct the ARXML model of your "
             "applications.");

      const bool events_equal = CheckEquality(existing_service.events_, service_to_insert.events_);
      assert(events_equal &&
             "Inconsistent event data definitions for one service interface across multiple application "
             "configurations. Please correct the ARXML model of your "
             "applications or revert any manual modifications to application-gen.json.");

      const bool event_groups_equal = CheckEquality(existing_service.eventgroups_, service_to_insert.eventgroups_);
      assert(event_groups_equal &&
             "Inconsistent event group data definitions for one service interface across multiple application "
             "configurations. Please correct the ARXML model of your "
             "applications or revert any manual modifications to application-gen.json.");
    } else {
      // Only insert this service if it doesn't already exists.
      services_.push_back(service_to_insert);
    }
  }
  return *this;
}

Configuration& Configuration::operator<<(const NetworkEndpointContainer& network_endpoints) {
  // Before inserting a certain network endpoint (nep) from the application configuration,
  // check if there is already one entry within the global configuration.
  for (const auto& app_ep : network_endpoints) {
    auto it = std::find_if(network_endpoints_.begin(), network_endpoints_.end(),
                           [&](const NetworkEndpoint& nep) { return (nep.address_ == app_ep.address_); });

    if (it != network_endpoints_.end()) {
      // No need to push back the complete network endpoint,
      // because it already exists in the global configuration.
      NetworkEndpoint& existing_nep = *it;

      // MTU consistency check
      const bool mtu_consistency = (existing_nep.mtu_ == app_ep.mtu_);
      assert(mtu_consistency &&
             "MTU deviation in network endpoints that have the same IP address. Please correct the configs of your "
             "app so that the MTUs of one network endpoint have the identical MTU.");

      // SD parameters consistency check
      const bool sd_consistency =
          (existing_nep.service_discovery_.multicast_address_ == app_ep.service_discovery_.multicast_address_) &&
          (existing_nep.service_discovery_.port_ == app_ep.service_discovery_.port_);
      assert(sd_consistency && "Service discovery multicast address deviation for one network endpoint.");

      // Look further for the port configurations of this endpoint to see if there are ports to add to the
      // existing nep.
      for (const NetworkEndpointPort& port_cfg : app_ep.ports_) {
        const auto it_nep = std::find_if(
            existing_nep.ports_.cbegin(), existing_nep.ports_.cend(),
            [&](const NetworkEndpointPort& existing_port_cfg) {
              return ((port_cfg.port_ == existing_port_cfg.port_) && (port_cfg.proto_ == existing_port_cfg.proto_));
            });

        // If this port does not yet exist for this network endpoint...
        if (it_nep == existing_nep.ports_.cend()) {
          existing_nep.ports_.push_back(port_cfg);
          logger_.LogDebug() << "Adding port number " << port_cfg.port_
                             << " to existing network endpoint: " << existing_nep.address_;
        } else {
          logger_.LogDebug() << "Port configuration already exists for this network endpoint.";
        }
      }
    } else {
      network_endpoints_.push_back(app_ep);
    }
  }

  return *this;
}

Configuration& Configuration::operator<<(const RequiredServiceInstanceContainer& required_service_instances) {
  for (const auto& rsi_to_insert : required_service_instances) {
    const auto it = std::find_if(required_service_instances_.cbegin(), required_service_instances_.cend(),
                                 [&](const RequiredServiceInstance& rsi) { return (rsi == rsi_to_insert); });

    if (it == required_service_instances_.cend()) {
      required_service_instances_.push_back(rsi_to_insert);
    } else {
      // Compare further if there are deviations between the settings of one required service instance used by multiple
      // applications..
      assert((it->port_mapping_ == rsi_to_insert.port_mapping_) &&
             "Port mapping of the same RequiredServiceInstance differ.");
      assert((it->service_discovery_ == rsi_to_insert.service_discovery_) &&
             "SD parameters of the same RequiredServiceInstance differ.");
    }
  }

  return *this;
}

Configuration& Configuration::operator<<(const ProvidedServiceInstanceContainer& provided_service_instances) {
  for (const auto& psi_to_insert : provided_service_instances) {
    // One required service instance can only exist once!
    const auto it = std::find_if(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                                 [&](const ProvidedServiceInstance& psi) { return (psi == psi_to_insert); });
    assert((it == provided_service_instances_.cend()) && "Multiple definition of the same provided service instance.");
    provided_service_instances_.push_back(psi_to_insert);
  }

  return *this;
}

template <typename ContainerT>
bool Configuration::CheckEquality(const ContainerT& container1, const ContainerT& container2) const {
  return someipd_posix::configuration::Compare(container1, container2);
}

}  // namespace configuration
}  // namespace someipd_posix
