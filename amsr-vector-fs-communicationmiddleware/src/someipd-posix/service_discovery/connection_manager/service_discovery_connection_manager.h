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
/**       \file  service_discovery_connection_manager.h
*        \brief  TODO
*
*      \details  TODO
*
*********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_CONNECTION_MANAGER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_CONNECTION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/reactor.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_udp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_policy_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

/**
 * \brief Represents an invalid port number.
 */
static constexpr someip_posix_common::someip::Port kInvalidPort = 0;

/**
 * \brief Represents an invalid IP address.
 */
static constexpr const char* kInvalidAddress = "";

/**
 * \brief A TCP sender pointer type.
 */
using ServiceDiscoveryTcpSenderPtr = std::unique_ptr<ServiceDiscoveryTcpSender>;
/**
 * \brief A TCP receiver pointer type.
 */
using ServiceDiscoveryTcpReceiverPtr = std::unique_ptr<ServiceDiscoveryTcpReceiver>;
/**
 * \brief An UDP sender pointer type.
 */
using ServiceDiscoveryUdpSenderPtr = std::unique_ptr<ServiceDiscoveryUdpSender>;
/**
 * \brief An UDP receiver pointer type.
 */
using ServiceDiscoveryUdpReceiverPtr = std::unique_ptr<ServiceDiscoveryUdpReceiver>;

/**
 * \brief ServiceDiscoveryConnectionManager.
 */
class ServiceDiscoveryConnectionManager {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryConnectionManager.
   *
   * \param config A configuration.
   * \param reactor A reactor.
   * \param packet_router A packet router.
   * \param timer_manager A timer manager.
   */
  ServiceDiscoveryConnectionManager(const configuration::Configuration* config,
                                    osabstraction::io::ReactorInterface* reactor,
                                    packet_router::PacketRouterInterface* packet_router,
                                    vac::timer::TimerManager* timer_manager);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryConnectionManager(const ServiceDiscoveryConnectionManager&) = delete;

  /**
   * \brief Delete copy assignment operator
   */
  ServiceDiscoveryConnectionManager& operator=(const ServiceDiscoveryConnectionManager&) = delete;
  /**
   * \brief Returns a new TCP sender.
   *
   * \param from_address A local IP address.
   * \param from_port A local port number.
   * \param to_address A remote IP address.
   * \param to_port A remote port number.
   * \return A TCP sender.
   */
  ServiceDiscoveryTcpSenderPtr GetTcpSender(const someip_posix_common::someip::IpAddress& from_address,
                                            someip_posix_common::someip::Port from_port,
                                            const someip_posix_common::someip::IpAddress& to_address,
                                            someip_posix_common::someip::Port to_port);
  /**
   * \brief Returns a new TCP receiver.
   *
   * \param address A local IP address.
   * \param port A local port number.
   * \return A TCP receiver.
   */
  ServiceDiscoveryTcpReceiverPtr GetTcpReceiver(const someip_posix_common::someip::IpAddress& address,
                                                someip_posix_common::someip::Port port);
  /**
   * \brief Indicates whether a passive TCP connection exist.
   *
   * \param from_address A local IP address.
   * \param from_port A local port number.
   * \param to_address A remote IP address.
   * \param to_port A remote port number.
   * \return true if the specified passive TCP connection exists and false otherwise.
   */
  bool HasTcpPassiveConnection(const someip_posix_common::someip::IpAddress& from_address,
                               someip_posix_common::someip::Port from_port,
                               const someip_posix_common::someip::IpAddress& to_address,
                               someip_posix_common::someip::Port to_port);
  /**
   * \brief Returns a new UDP sender.
   *
   * \param from_address A local IP address.
   * \param from_port A local port number.
   * \param to_address A remote IP address.
   * \param to_port A remote port number.
   * \return An UDP sender.
   */
  ServiceDiscoveryUdpSenderPtr GetUdpSender(const someip_posix_common::someip::IpAddress& from_address,
                                            someip_posix_common::someip::Port from_port,
                                            const someip_posix_common::someip::IpAddress& to_address,
                                            someip_posix_common::someip::Port to_port);
  /**
   * \brief Returns a new UDP receiver.
   *
   * \param address A local IP address.
   * \param port A local port number.
   * \return An UDP receiver.
   */
  ServiceDiscoveryUdpReceiverPtr GetUdpUnicastReceiver(const someip_posix_common::someip::IpAddress& address,
                                                       someip_posix_common::someip::Port port);
  /**
   * \brief Returns a new UDP receiver.
   *
   * \param local_address A local IP address.
   * \param multicast_address A multicast IP address.
   * \param multicast_port A multicast port number.
   * \return An UDP receiver.
   */
  ServiceDiscoveryUdpReceiverPtr GetUdpMulticastReceiver(
      const someip_posix_common::someip::IpAddress& local_address,
      const someip_posix_common::someip::IpAddress& multicast_address,
      someip_posix_common::someip::Port multicast_port);

 protected:
  /**
   * \brief A TCP endpoint pointer type.
   */
  using ServiceDiscoveryTcpEndpointPtr = std::shared_ptr<ServiceDiscoveryTcpEndpoint>;
  /**
   * \brief A TCP endpoint container type.
   */
  using ServiceDiscoveryTcpEndpointContainer = std::vector<ServiceDiscoveryTcpEndpointPtr>;
  /**
   * \brief A TCP endpoint container iterator type.
   */
  using ServiceDiscoveryTcpEndpointContainerIter = ServiceDiscoveryTcpEndpointContainer::iterator;
  /**
   * \brief An UDP endpoint pointer type.
   */
  using ServiceDiscoveryUdpEndpointPtr = std::shared_ptr<ServiceDiscoveryUdpEndpoint>;
  /**
   * \brief An UDP endpoint container type.
   */
  using ServiceDiscoveryUdpEndpointContainer = std::vector<ServiceDiscoveryUdpEndpointPtr>;
  /**
   * \brief An UDP endpoint container iterator type.
   */
  using ServiceDiscoveryUdpEndpointContainerIter = ServiceDiscoveryUdpEndpointContainer::iterator;
  /**
   * \brief Creates TCP and UDP endpoints according to the configuration that was passed in.
   */
  void CreateEndpoints();
  /**
   * \brief Looks up a TCP endpoint by its IP address and port number.
   *
   * \param address A local IP address.
   * \param port A local port number.
   * \return An iterator pointing into TCP endpoint container.
   */
  ServiceDiscoveryTcpEndpointContainerIter FindTcpEndpoint(const someip_posix_common::someip::IpAddress& address,
                                                           someip_posix_common::someip::Port port);
  /**
   * \brief Looks up an UDP endpoint by its IP address and port number.
   *
   * \param address A local IP address.
   * \param port A local port number.
   * \return An iterator pointing into UDP endpoint container.
   */
  ServiceDiscoveryUdpEndpointContainerIter FindUdpEndpoint(const someip_posix_common::someip::IpAddress& address,
                                                           someip_posix_common::someip::Port port);
  /**
   * \brief Sets up the UDP transmission trigger policy manager.
   *
   * \param config A configuration.
   */
  void SetupUdpTransmissionTriggerPolicyManager(const configuration::Configuration& config);
  /**
   * \brief A configuration.
   */
  const configuration::Configuration* config_;
  /**
   * \brief A reactor.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* packet_router_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A container of TCP endpoints.
   */
  ServiceDiscoveryTcpEndpointContainer tcp_endpoints_;
  /**
   * \brief A container of UDP endpoints.
   */
  ServiceDiscoveryUdpEndpointContainer udp_endpoints_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A UDP transmission trigger policy.
   */
  transmission_trigger::TransmissionTriggerPolicyManager udp_transmission_trigger_policy_manager_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_CONNECTION_MANAGER_H_
