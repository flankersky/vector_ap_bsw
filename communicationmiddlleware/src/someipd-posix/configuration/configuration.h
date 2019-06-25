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
/**        \file  someipd-posix/configuration/configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_H_
#define SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <iterator>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration_types.h"

namespace someipd_posix {
namespace configuration {

/**
 * \brief Assembled configuration of all applications and the SOME/IP daemon.
 * This class represents the internal data model containing the necessary configuration
 * parameters for the SOME/IP daemon. This is explicitly separated from a certain
 * configuration intermediate representation such as JSON
 * to allow testing without additional mocks, interfaces etc.
 */
class Configuration : public ConfigurationTypesAndDefs {
 public:
  /**
   * \brief Constructor of Configuration.
   */
  Configuration();
  /**
   * \brief Returns the path of the control socket.
   *
   * \return Path to control socket.
   */
  const someip_posix_common::someip::SocketPath& GetControlSocketPath() const { return control_socket_path_; }
  /**
   * \brief Returns the path of the routing socket.
   *
   * \return Path to routing socket.
   */
  const someip_posix_common::someip::SocketPath& GetRoutingSocketPath() const { return routing_socket_path_; }
  /**
   * \brief Returns a container of all services.
   *
   * \return A container of services read from the configuration.
   */
  const ServiceContainer& GetServices() const { return services_; }
  /**
   * \brief Returns a service.
   *
   * \param id A SOME/IP service identifier.
   *
   * \return A service or nullptr.
   */
  const Service* GetService(someip_posix_common::someip::ServiceId id) const {
    const auto it =
        std::find_if(services_.cbegin(), services_.cend(), [id](const Service& s) { return (id == s.id_); });
    if (it != services_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns a method.
   *
   * \param service_id A SOME/IP service identifier.
   * \param method_id A SOME/IP method identifier.
   *
   * \return A method or nullptr.
   */
  const Method* GetMethod(someip_posix_common::someip::ServiceId service_id,
                          someip_posix_common::someip::MethodId method_id) const {
    auto s = GetService(service_id);
    if (s) {
      const auto it = std::find_if(s->methods_.cbegin(), s->methods_.cend(),
                                   [method_id](const Method& m) { return (method_id == m.id_); });
      if (it != s->methods_.end()) {
        return &*it;
      } else {
        return nullptr;
      }
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns an event.
   *
   * \param service_id A SOME/IP service identifier.
   * \param event_id A SOME/IP event identifier.
   *
   * \return An event or nullptr.
   */
  const Event* GetEvent(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::EventId event_id) const {
    auto s = GetService(service_id);
    if (s) {
      const auto it = std::find_if(s->events_.cbegin(), s->events_.cend(),
                                   [event_id](const Event& e) { return (event_id == e.id_); });
      if (it != s->events_.end()) {
        return &*it;
      } else {
        return nullptr;
      }
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns an eventgroup.
   *
   * \param service_id A SOME/IP service identifier.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return An eventgroup or nullptr.
   */
  const Eventgroup* GetEventgroup(someip_posix_common::someip::ServiceId service_id,
                                  someip_posix_common::someip::EventId eventgroup_id) const {
    auto s = GetService(service_id);
    if (s) {
      const auto it = std::find_if(s->eventgroups_.cbegin(), s->eventgroups_.cend(),
                                   [eventgroup_id](const Eventgroup& eg) { return (eventgroup_id == eg.id_); });
      if (it != s->eventgroups_.end()) {
        return &*it;
      } else {
        return nullptr;
      }
    } else {
      return nullptr;
    }
  }

  /**
   * \brief Applies the given visitor func on all field events contained in a specific eventgroup.
   *
   * \tparam EventVisitor A visitor function which is passed to field events.
   *
   * \param service_id A SOME/IP service identifier.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param func function to apply on each field event contained in the eventgroup.
   * The signature of the function should be equivalent to the following:
   * void func(const ConfigurationTypesAndDefs::Event*);
   */
  template <class EventVisitor>
  void ForeachEventgroupField(someip_posix_common::someip::ServiceId service_id,
                              someip_posix_common::someip::EventId eventgroup_id, EventVisitor func) const {
    auto eg = GetEventgroup(service_id, eventgroup_id);
    if (eg) {
      const auto& events = eg->events_;
      std::for_each(events.begin(), events.end(),
                    [this, service_id, &func](someip_posix_common::someip::EventId event_id) {
                      const auto event = GetEvent(service_id, event_id);
                      if (event && event->is_field_) {
                        func(event);
                      }
                    });
    }
  }
  /**
   * \brief Returns a container of all eventgroup ids of a specified service.
   *
   * \param service_id A SOME/IP service identifier.
   *
   * \return A container of all eventgroup ids.
   */
  std::vector<someip_posix_common::someip::EventId> GetEventgroupIds(
      someip_posix_common::someip::ServiceId service_id) const {
    auto s = GetService(service_id);
    std::vector<someip_posix_common::someip::EventId> eventgroups;
    if (s) {
      eventgroups.reserve(s->eventgroups_.size());
      std::transform(s->eventgroups_.cbegin(), s->eventgroups_.cend(), std::back_inserter(eventgroups),
                     [](const Eventgroup& eg) { return eg.id_; });
    }
    return eventgroups;
  }
  /**
   * \brief Returns a container of all required service instances.
   *
   * \return A container of required services.
   */
  const RequiredServiceInstanceContainer& GetRequiredServiceInstances() const { return required_service_instances_; }
  /**
   * \brief Returns a required service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A major version of a SOME/IP service.
   * \param minor_version A minor version of a SOME/IP service.
   * \return A required service instance or nullptr.
   */
  const RequiredServiceInstance* GetRequiredServiceInstance(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
      someip_posix_common::someip::MajorVersion major_version,
      someip_posix_common::someip::MinorVersion minor_version) const {
    const auto it =
        std::find_if(required_service_instances_.cbegin(), required_service_instances_.cend(),
                     [service_id, instance_id, major_version, minor_version](const RequiredServiceInstance& psi) {
                       return (service_id == psi.service_id_ && instance_id == psi.instance_id_ &&
                               major_version == psi.major_version_);
                     });
    if (it != required_service_instances_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns a required service instance SD eventgroup.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A major version of a SOME/IP service.
   * \param minor_version A minor version of a SOME/IP service.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return A required service instance SD eventgroup or nullptr.
   */
  const RequiredServiceInstanceSdEventgroup* GetRequiredServiceInstanceSdEventgroup(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
      someip_posix_common::someip::MajorVersion major_version, someip_posix_common::someip::MinorVersion minor_version,
      someip_posix_common::someip::EventId eventgroup_id) const {
    const auto rsi = GetRequiredServiceInstance(service_id, instance_id, major_version, minor_version);
    if (!rsi) {
      return nullptr;
    }
    const auto it = std::find_if(
        rsi->service_discovery_.eventgroups_.cbegin(), rsi->service_discovery_.eventgroups_.cend(),
        [eventgroup_id](const RequiredServiceInstanceSdEventgroup& rsieg) { return (eventgroup_id == rsieg.id_); });
    if (it != rsi->service_discovery_.eventgroups_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns a container of all provided service instances.
   *
   * \return A container of provided services.
   */
  const ProvidedServiceInstanceContainer& GetProvidedServiceInstances() const { return provided_service_instances_; }
  /**
   * \brief Returns a provided service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A major version of a SOME/IP service.
   * \param minor_version A minor version of a SOME/IP service.
   *
   * \return A provided service instance or nullptr.
   */
  const ProvidedServiceInstance* GetProvidedServiceInstance(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
      someip_posix_common::someip::MajorVersion major_version,
      someip_posix_common::someip::MinorVersion minor_version) const {
    const auto it =
        std::find_if(provided_service_instances_.cbegin(), provided_service_instances_.cend(),
                     [service_id, instance_id, major_version, minor_version](const ProvidedServiceInstance& psi) {
                       return (service_id == psi.service_id_ && instance_id == psi.instance_id_ &&
                               major_version == psi.major_version_);
                     });
    if (it != provided_service_instances_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns a provided service instance SD eventgroup.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A major version of a SOME/IP service.
   * \param minor_version A minor version of a SOME/IP service.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return A provided service instance SD eventgroup or nullptr.
   */
  const ProvidedServiceInstanceSdEventgroup* GetProvidedServiceInstanceSdEventgroup(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
      someip_posix_common::someip::MajorVersion major_version, someip_posix_common::someip::MinorVersion minor_version,
      someip_posix_common::someip::EventId eventgroup_id) const {
    const auto psi = GetProvidedServiceInstance(service_id, instance_id, major_version, minor_version);
    if (!psi) {
      return nullptr;
    }
    const auto it = std::find_if(
        psi->service_discovery_.eventgroups_.cbegin(), psi->service_discovery_.eventgroups_.cend(),
        [eventgroup_id](const ProvidedServiceInstanceSdEventgroup& psieg) { return (eventgroup_id == psieg.id_); });
    if (it != psi->service_discovery_.eventgroups_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns a container of network endpoints.
   *
   * \return A container of network endpoints.
   */
  const NetworkEndpointContainer& GetNetworkEndpoints() const { return network_endpoints_; }
  /**
   * \brief Returns a network endpoint.
   *
   * \param address An IP address.
   *
   * \return A network endpoint or nullptr.
   */
  const NetworkEndpoint* GetNetworkEndpoint(const someip_posix_common::someip::IpAddress& address) const {
    const auto it = std::find_if(network_endpoints_.cbegin(), network_endpoints_.cend(),
                                 [&address](const NetworkEndpoint& ne) { return (address == ne.address_); });
    if (it != network_endpoints_.end()) {
      return &*it;
    } else {
      return nullptr;
    }
  }
  /**
   * \brief Returns parameters for static SD.
   *
   * \return Parameters for static SD.
   */
  const StaticServiceDiscovery& GetStaticServiceDiscovery() const { return static_service_discovery_; }
  /**
   * \brief Determines whether an SD endpoint with the specified IP address and port exists or not.
   *
   * \param address A unicast IP address.
   * \param port A port number.
   * \return true if the specified SD endpoint exists and false otherwise.
   */
  bool HasSdAddressAndPort(const someip_posix_common::someip::IpAddress& address,
                           someip_posix_common::someip::Port port) const;
  /**
   * \brief Returns all eventgroups of a SOME/IP service which contain the specified event.
   *
   * \param service_id A SOME/IP service identifier.
   * \param event_id An event identifier.
   * \return A container of eventgroups which contain the specified event.
   */
  EventgroupIdContainer EventToEventgroups(someip_posix_common::someip::ServiceId service_id,
                                           someip_posix_common::someip::EventId event_id) const;
  /**
   * \brief Adding services to the global configuration. This only adds services that are
   * not already part of the global configuration.
   * \param services The collection to insert into the global configuration.
   * \return The configuration that is appended by the services to add.
   */
  Configuration& operator<<(const ServiceContainer& services);

  /**
   * \brief Merging and storing the network endpoint configuration of an application into this configuration object.
   * \param network_endpoints The network endpoints to add to the global configuration.
   * \return The configuration that is appended by the network endpoints to add.
   */
  Configuration& operator<<(const NetworkEndpointContainer& network_endpoints);
  /**
   * \brief Adding service instances that are required by an application to the global configuration.
   * \param required_service_instances The required service instances to add to the global configuration.
   * \return The configuration that is appended by the required service instances to add.
   */
  Configuration& operator<<(const RequiredServiceInstanceContainer& required_service_instances);
  /**
   * \brief Adding service instances that are provided by an application to the global configuration.
   * \param provided_service_instances The provided service instances to add to the global configuration.
   * \return The configuration that is appended by the provided service instances to add.
   */
  Configuration& operator<<(const ProvidedServiceInstanceContainer& provided_service_instances);
  /**
   * \brief Storing the static service discovery settings of one application into the configuration object.
   * \param static_service_discovery A static SD configuration to add to the global config.
   * \return The configuration that is appended by a static service discovery.
   */
  Configuration& operator<<(const StaticServiceDiscovery& static_service_discovery) {
    this->static_service_discovery_ = static_service_discovery;
    return *this;
  }

  /**
   * \brief Check if two method/event/event groups collections for one service interface differ
   * from each other.
   * \tparam ContainerT Deduced type of both containers to compare.
   * \remark This method allows re-ordering of elements within its container.
   * \param container1 The first container of methods / events
   * \param container2 The second container of methods to compare to the first one.
   * \return true if both method containers are equal; returns false if:
   * - The size of the method/event collections differ.
   * - The protocols for the same method/event ID differ.
   */
  template <typename ContainerT>
  bool CheckEquality(const ContainerT& container1, const ContainerT& container2) const;

 protected:
  /**
   * \brief An instance of a logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A path to a control socket.
   */
  someip_posix_common::someip::SocketPath control_socket_path_;
  /**
   * \brief A path to a routing socket.
   */
  someip_posix_common::someip::SocketPath routing_socket_path_;
  /**
   * \brief Required service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
  /**
   * \brief Provided service instances.
   */
  ProvidedServiceInstanceContainer provided_service_instances_;
  /**
   * \brief Network endpoints collected from the application configurations.
   */
  NetworkEndpointContainer network_endpoints_;
  /**
   * \brief Static SD parameters.
   */
  StaticServiceDiscovery static_service_discovery_;
  /**
   * \brief Collection of all required AND provided services read from the configuration.
   */
  ServiceContainer services_;
};

}  // namespace configuration
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_H_
