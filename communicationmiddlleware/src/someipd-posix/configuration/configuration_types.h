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
/**        \file  configuration_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_TYPES_H_
#define SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>
#include <vector>
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someipd_posix {
namespace configuration {

/**
 * \brief Generic function that checks two containers if their elements are the same
 * and they both have the same size.
 * \tparam ContainerT Deduced type of the container
 * \param container1 The first container that is compared to the second one.
 * \param container2 The second container that is compared to the first one.
 * \return true if both containers are of the same size and the elements contained
 * are equal; false if the containers are not of the same size or one of the elements differ.
 */
template <typename ContainerT>
bool Compare(const ContainerT& container1, const ContainerT& container2) {
  bool equal{true};

  if (container1.size() != container2.size()) {
    equal = false;
  } else {
    for (const auto& element1 : container1) {
      const auto it = std::find(container2.cbegin(), container2.cend(), element1);
      if (it == container2.cend()) {
        equal = false;
        break;  // Comparing further is not an option, break on first inequality.
      }
    }
  }

  return equal;
}

/**
 * \brief Configuration declarations other configuration-related classes
 * may derive from for usage.
 */
struct ConfigurationTypesAndDefs {
  /**
   * \brief Represents an invalid port.
   */
  static constexpr someip_posix_common::someip::Port kInvalidPort = 0U;
  /**
   * \brief Represents an invalid IP address.
   */
  static constexpr const char* kInvalidAddress = "";
  /**
   * \brief Represents any service.
   */
  static constexpr someip_posix_common::someip::ServiceId kServiceIdAny = 0xFFFFU;
  /**
   * \brief Represents any service instance.
   */
  static constexpr someip_posix_common::someip::InstanceId kInstanceIdAny = 0xFFFFU;
  /**
   * \brief Represents any major version of a service.
   */
  static constexpr someip_posix_common::someip::MajorVersion kMajorVersionAny = 0xFFU;
  /**
   * \brief Represents any minor version of a service.
   */
  static constexpr someip_posix_common::someip::MinorVersion kMinorVersionAny = 0xFFFFFFFFU;
  /**
   * \brief Layer 4 protocols.
   */
  enum class Protocol {
    kTCP,  ///< TCP protocol
    kUDP   ///< UDP protocol
  };
  /**
   * \brief The value of event multicast threshold that disables multicast event communication.
   *
   *  TPS_MANI_03018
   */
  static constexpr std::uint32_t kEventMulticastThresholdUseOnlyUnicast = 0U;
  /**
   * \brief Represents a UDP transmission trigger mode.
   */
  enum class TransmissionTriggerMode {
    kTransmissionTriggerModeAlways,  ///< Always triggers the transmission
    kTransmissionTriggerModeNever    ///< Never triggers the transmission
  };
  /**
   * \brief Represents a UDP transmission trigger.
   */
  struct MethodEventTransmissionTrigger {
    bool is_valid_;                     ///< Indicates whether the following parameters are valid
    TransmissionTriggerMode mode_;      ///< Trigger mode
    std::chrono::nanoseconds timeout_;  ///< Trigger timeout (valid only for mode 'never')
  };
  /**
   * \brief Represents a SOME/IP method.
   */
  struct Method {
    /**
     * \brief Compare two objects of type Method based on their ID and their transport protocol in use.
     * \param other The other method to compare to this object.
     * \return true if id and proto are the same; false if not.
     */
    bool operator==(const Method& other) const noexcept { return (id_ == other.id_) && (proto_ == other.proto_); }

    someip_posix_common::someip::MethodId id_;   ///< Method identifier
    Protocol proto_;                             ///< Protocol used by method
    MethodEventTransmissionTrigger tx_trigger_;  ///< Transmission trigger parameters
  };
  /**
   * \brief Method container.
   */
  using MethodContainer = std::vector<Method>;
  /**
   * \brief Represents a SOME/IP event.
   */
  struct Event {
    /**
     * \brief Compare two objects of type Event based on their ID and their transport protocol in use.
     *
     * \param other The event to compare with.
     * \return true if event ID, is_field option and proto are the same; false if one of them differ.
     */
    bool operator==(const Event& other) const noexcept {
      return (id_ == other.id_) && (is_field_ == other.is_field_) && (proto_ == other.proto_);
    }

    someip_posix_common::someip::EventId id_;    ///< Event identifier
    bool is_field_;                              ///< Specifies whether event is a field
    Protocol proto_;                             ///< Protocol used by event
    MethodEventTransmissionTrigger tx_trigger_;  ///< Transmission trigger parameters
  };
  /**
   * \brief Event container.
   */
  using EventContainer = std::vector<Event>;
  /**
   * \brief Event identifier container.
   */
  using EventIdContainer = std::vector<someip_posix_common::someip::EventId>;
  /**
   * \brief Represents a SOME/IP eventgroup.
   */
  struct Eventgroup {
    /**
     * \brief Compare two objects of type Eventgroup based on their ID and the event IDs contained.
     */
    bool operator==(const Eventgroup& other) const noexcept {
      bool equal{true};
      if (events_.size() != other.events_.size()) {
        equal = false;
      } else {
        for (const auto& event_ref_id : events_) {
          const auto it = std::find(other.events_.cbegin(), other.events_.cend(), event_ref_id);
          if (it == other.events_.cend()) {
            equal = false;
          }
        }
      }
      return (id_ == other.id_) && equal;
    }

    someip_posix_common::someip::EventId id_;  ///< Eventgroup identifier
    EventIdContainer events_;                  ///< Events belonging to eventgroup
  };
  /**
   * \brief Eventgroup container.
   */
  using EventgroupContainer = std::vector<Eventgroup>;
  /**
   * \brief Eventgroup container.
   */
  using EventgroupIdContainer = std::vector<someip_posix_common::someip::EventId>;
  /**
   * \brief Represents a SOME/IP service.
   */
  struct Service {
    /**
     * \brief Comparing two services by their SOME/IP identifiers, the major and minor version.
     *
     * \param rhs The service to compare with this instance.
     * \return true if all parameters (id, major, minor version) of one service match; false if not.
     */
    inline bool operator==(const Service& rhs) const {
      return (id_ == rhs.id_) && (major_version_ == rhs.major_version_) && (minor_version_ == rhs.minor_version_);
    }

    someip_posix_common::someip::ServiceId id_;                ///< Service identifier
    someip_posix_common::someip::MajorVersion major_version_;  ///< Major version of service
    someip_posix_common::someip::MinorVersion minor_version_;  ///< Minor version of service
    MethodContainer methods_;                                  ///< Methods belonging to service
    EventContainer events_;                                    ///< Events belonging to service
    EventgroupContainer eventgroups_;                          ///< Eventgroups belonging to service
  };
  /**
   * \brief Service container.
   */
  using ServiceContainer = std::vector<Service>;
  /**
   * \brief Represents a mapping to a network endpoint port.
   */
  struct NetworkEndpointPortMapping {
    /**
     * \brief Network endpoints are compared by their addresses and ports.
     *
     * \param other the NetworkEndpointPortMapping object to compare with.
     * \return true if all the parameters are the same; false if one of them differ.
     */
    inline bool operator==(const NetworkEndpointPortMapping& other) const {
      return (address_.compare(other.address_) == 0) && (udp_port_ == other.udp_port_) &&
             (tcp_port_ == other.tcp_port_) &&
             (event_multicast_address_.compare(other.event_multicast_address_) == 0) &&
             (event_multicast_port_ == other.event_multicast_port_);
    }

    someip_posix_common::someip::IpAddress address_;                  ///< IP address of a network endpoint
    someip_posix_common::someip::Port udp_port_;                      ///< UDP port
    someip_posix_common::someip::Port tcp_port_;                      ///< TCP port
    someip_posix_common::someip::IpAddress event_multicast_address_;  ///< Event multicast IP address
    someip_posix_common::someip::Port event_multicast_port_;          ///< Event multicast port
  };
  /**
   * \brief Container for network endpoint port mappings.
   */
  using NetworkEndpointPortMappingContainer = std::vector<NetworkEndpointPortMapping>;
  /**
   * \brief Represents a SOME/IP service instance.
   */
  struct ServiceInstance {
    /**
     * \brief Compare this service instance with another one.
     *
     * \param other The service instance to compare with.
     * \return true if service ID, instance ID, major version and minor version are the same;
     * false if one of these IDs or the versions differ.
     */
    inline bool operator==(const ServiceInstance& other) const noexcept {
      return ((service_id_ == other.service_id_) && (instance_id_ == other.instance_id_) &&
              (major_version_ == other.major_version_) && (minor_version_ == other.minor_version_));
    }

    someip_posix_common::someip::ServiceId service_id_;        ///< Service identifier
    someip_posix_common::someip::InstanceId instance_id_;      ///< Minor version of service instance
    someip_posix_common::someip::MajorVersion major_version_;  ///< Major version of service instance
    someip_posix_common::someip::MinorVersion minor_version_;  ///< Minor version of service instance
  };
  /**
   * \brief Container of service instances.
   */
  using ServiceInstanceContainer = std::vector<ServiceInstance>;
  /**
   * \brief SD eventgroup parameters for a required service instance.
   */
  struct RequiredServiceInstanceSdEventgroup {
    /**
     * \brief Compares two RequiredServiceInstanceSdEventgroup objects.
     *
     * \param other The instance to compare with.
     * \return true if all parameters are the same; false if one of them differ.
     */
    inline bool operator==(const RequiredServiceInstanceSdEventgroup& other) const noexcept {
      return (id_ == other.id_) && (ttl_ == other.ttl_) &&
             (request_response_delay_min_ns_ == other.request_response_delay_min_ns_) &&
             (request_response_delay_max_ns_ == other.request_response_delay_max_ns_);
    }

    someip_posix_common::someip::EventId id_;      ///< Eventgroup identifier
    someip_posix_common::someip::Ttl ttl_;         ///< TTL in SubscribeEventgroup entry in seconds
    std::uint64_t request_response_delay_min_ns_;  ///< Minimum delay of SubscribeEventgroup entry
    std::uint64_t request_response_delay_max_ns_;  ///< Maximum delay of SubscribeEventgroup entry
  };
  /**
   * \brief Container of SD eventgroup parameters for a required service.
   */
  using RequiredServiceInstanceSdEventgroupContainer = std::vector<RequiredServiceInstanceSdEventgroup>;
  /**
   * \brief SD parameters for a required service.
   */
  struct RequiredServiceInstanceServiceDiscovery {
    /**
     * \brief Compare the service discovery parameters from two required service instances.
     *
     * \param other The RequiredServiceInstanceServiceDiscovery to compare this instance with.
     * \return true if all attributes of two instances match; false if one of them are not equal.
     */
    inline bool operator==(const RequiredServiceInstanceServiceDiscovery& other) const {
      return (ttl_ == other.ttl_) && (initial_repetitions_max_ == other.initial_repetitions_max_) &&
             (initial_delay_min_ns_ == other.initial_delay_min_ns_) &&
             (initial_delay_max_ns_ == other.initial_delay_max_ns_) &&
             (initial_repetitions_base_delay_ns_ == other.initial_repetitions_base_delay_ns_) &&
             (someipd_posix::configuration::Compare(eventgroups_, other.eventgroups_));
    }

    someip_posix_common::someip::Ttl ttl_;                      ///< TTL in FindService entry in seconds
    std::uint32_t initial_repetitions_max_;                     ///< Maximum number of repetitions in Repetition Phase
    std::uint64_t initial_delay_min_ns_;                        ///< Minimum delay of initial FindService entry
    std::uint64_t initial_delay_max_ns_;                        ///< Maximum delay of initial FindService entry
    std::uint64_t initial_repetitions_base_delay_ns_;           ///< Base delay of FindService entry in Repetition Phase
    RequiredServiceInstanceSdEventgroupContainer eventgroups_;  ///< SD eventgroup parameters
  };
  /**
   * \brief Represents a required SOME/IP service instance.
   */
  struct RequiredServiceInstance : ServiceInstance {
    NetworkEndpointPortMapping port_mapping_;                    ///< Mapping to a network endpoint port
    RequiredServiceInstanceServiceDiscovery service_discovery_;  ///< SD parameters
  };
  /**
   * \brief Container of required service instances.
   */
  using RequiredServiceInstanceContainer = std::vector<RequiredServiceInstance>;
  /**
   * \brief SD eventgroup parameters for a provided service instance.
   */
  struct ProvidedServiceInstanceSdEventgroup {
    someip_posix_common::someip::EventId id_;  ///< Eventgroup identifier
    someip_posix_common::someip::Ttl ttl_;     ///< TTL in SubscribeEventgroupAck entry in seconds
    std::uint32_t
        event_multicast_threshold_;  ///< Specifies after how many subscriptions events will be sent via multicast
    std::uint64_t request_response_delay_min_ns_;  ///< Minimum delay of OfferService entry as a response to a multicast
                                                   /// FindService entry
    std::uint64_t request_response_delay_max_ns_;  ///< Maximum delay of OfferService entry as a response to a multicast
                                                   /// FindService entry
  };
  /**
   * \brief Container of SD eventgroup parameters for a provided service.
   */
  using ProvidedServiceInstanceSdEventgroupContainer = std::vector<ProvidedServiceInstanceSdEventgroup>;
  /**
   * \brief SD parameters for a provided service.
   */
  struct ProvidedServiceInstanceServiceDiscovery {
    someip_posix_common::someip::Ttl ttl_;             ///< TTL in OfferService entry in seconds
    std::uint32_t initial_repetitions_max_;            ///< Maximum number of repetitions in Repetition Phase
    std::uint64_t initial_delay_min_ns_;               ///< Minimum delay of initial FindService entry
    std::uint64_t initial_delay_max_ns_;               ///< Maximum delay of initial FindService entry
    std::uint64_t initial_repetitions_base_delay_ns_;  ///< Base delay of FindService entry in Repetition Phase
    std::uint64_t cyclic_offer_delay_ns_;              ///< Delay between OfferService entries sent in Main Phase
    std::uint64_t request_response_delay_min_ns_;      ///< Minimum delay of a response to a multicast request
    std::uint64_t request_response_delay_max_ns_;      ///< Maximum delay of a response to a multicast request
    ProvidedServiceInstanceSdEventgroupContainer eventgroups_;  ///< SD eventgroup parameters
  };
  /**
   * \brief Represents a provided SOME/IP service instance.
   */
  struct ProvidedServiceInstance : ServiceInstance {
    NetworkEndpointPortMappingContainer port_mappings_;          ///< Mappings to network endpoint ports
    ProvidedServiceInstanceServiceDiscovery service_discovery_;  ///< SD parameter
  };
  /**
   * \brief Container of provided service instances.
   */
  using ProvidedServiceInstanceContainer = std::vector<ProvidedServiceInstance>;
  /**
   * \brief Contains service discovery parameters for a network endpoint.
   */
  struct NetworkEndpointServiceDiscovery {
    someip_posix_common::someip::IpAddress multicast_address_;  ///< Multicast IP address for multicast SD messages
    someip_posix_common::someip::Port port_;                    ///< UDP multicast port
  };
  /**
   * \brief Contains transmission trigger parameters for a network endpoint.
   *
   * Shall be valid only for UDP protocol.
   */
  struct NetworkEndpointPortTransmissionTrigger {
    bool is_valid_;  ///< Indicates whether the following parameters are valid
    someip_posix_common::someip::TransmissionTriggerBufferSize
        buffer_size_;  ///< Maximum length of bytes allowed to be buffered before transmission
  };
  /**
   * \brief Reprsents a network endpoint port.
   */
  struct NetworkEndpointPort {
    someip_posix_common::someip::Port port_;              ///< Port number
    Protocol proto_;                                      ///< Layer4 protocol
    someip_posix_common::someip::SocketOptions options_;  ///< Quality of Service option etc.
    NetworkEndpointPortTransmissionTrigger tx_trigger_;   ///< Transmission trigger parameters
  };
  /**
   * \brief Container of network endpoint ports.
   */
  using NetworkEndpointPortContainer = std::vector<NetworkEndpointPort>;
  /**
   * \brief Rerpesents a network endpoint.
   */
  struct NetworkEndpoint {
    someip_posix_common::someip::IpAddress address_;     ///< IP address
    someip_posix_common::someip::MTU mtu_;               ///< Maximum transmission unit
    NetworkEndpointServiceDiscovery service_discovery_;  ///< Service discovery parameters
    NetworkEndpointPortContainer ports_;                 ///< Ports
  };
  /**
   * \brief Container of network endpoints.
   */
  using NetworkEndpointContainer = std::vector<NetworkEndpoint>;
  /**
   * \brief Represents a remote network endpoint.
   */
  struct RemoteNetworkEndpoint {
    someip_posix_common::someip::IpAddress address_;       ///< IP address
    someip_posix_common::someip::Port tcp_port_;           ///< TCP port number
    someip_posix_common::someip::Port udp_port_;           ///< UDP port number
    ServiceInstanceContainer required_service_instances_;  ///< Container of required service instances
  };
  /**
   * \brief Container of remote network endpoints.
   */
  using RemoteNetworkEndpointContainer = std::vector<RemoteNetworkEndpoint>;
  /**
   * \brief Static SD parameters.
   */
  struct StaticServiceDiscovery {
    bool enable_;                               ///< Specifies whether the static SD is enabled or not
    RemoteNetworkEndpointContainer endpoints_;  ///< Container of remote network endpoints
  };
};

}  // namespace configuration
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_CONFIGURATION_CONFIGURATION_TYPES_H_
