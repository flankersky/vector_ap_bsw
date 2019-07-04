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
/**        \file  service_discovery_server_service_instance.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SERVER_SERVICE_INSTANCE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SERVER_SERVICE_INSTANCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_connection_manager.h"

namespace someipd_posix {
namespace service_discovery {

class ServiceDiscovery;

/**
 * \brief ServiceDiscoveryServerServiceInstance.
 */
class ServiceDiscoveryServerServiceInstance
    : public std::enable_shared_from_this<ServiceDiscoveryServerServiceInstance>,
      public packet_router::PacketSink,
      public connection_manager::ServiceDiscoveryTcpConnectionStateChangeHandler {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryServerServiceInstance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version The major version of SOME/IP service instance.
   * \param minor_version The minor version of SOME/IP service instance.
   * \param address An IP address of a provided service instance.
   * \param packet_router A packet router.
   * \param conman A connection manager.
   * \param config A configuration.
   */
  ServiceDiscoveryServerServiceInstance(std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version,
                                        std::uint32_t minor_version, const std::string& address,
                                        packet_router::PacketRouterInterface* packet_router,
                                        connection_manager::ServiceDiscoveryConnectionManager* conman,
                                        const configuration::Configuration* config);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryServerServiceInstance(const ServiceDiscoveryServerServiceInstance&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServerServiceInstance& operator=(const ServiceDiscoveryServerServiceInstance&) = delete;
  /**
   * \brief Starts a provided service instance.
   */
  virtual void Start();

  /**
   * \brief Stops a provided service instance.
   */
  virtual void Stop();

  /**
   * \brief Adds a new subscriber for a given eventgroup.
   *
   * \param tcp_address An IP address of a provided service instance.
   * \param tcp_port A TCP port number.
   * \param udp_address An IP address of a provided service instance.
   * \param udp_port A UDP port number.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void SubscribeEventgroup(const std::string& tcp_address, std::uint16_t tcp_port,
                                   const std::string& udp_address, std::uint16_t udp_port, std::uint16_t eventgroup_id);

  /**
   * \brief Removes a subscriber for a given eventgroup.
   *
   * \param tcp_address An IP address of a provided service instance.
   * \param tcp_port A TCP port number.
   * \param udp_address An IP address of a provided service instance.
   * \param udp_port A UDP port number.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void UnsubscribeEventgroup(const std::string& tcp_address, std::uint16_t tcp_port,
                                     const std::string& udp_address, std::uint16_t udp_port,
                                     std::uint16_t eventgroup_id);

  /**
   * \brief Tests whether a TCP connection with the specified remote IP address and remote port exist or not.
   *
   * \param address A remote IP address.
   * \param port A remote port number.
   * \return true if the specified connection exists and false otherwise.
   */
  virtual bool HasTcpConnection(const std::string& address, std::uint16_t port) const;

 private:
  /**
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(std::uint16_t instance_id, packet_router::Packet packet) override;
  /**
   * \brief Called when a TCP connection is established.
   *
   * \param sender A TCP sender.
   */
  void OnConnect(connection_manager::ServiceDiscoveryTcpSender* sender) override;
  /**
   * \brief Called when a TCP connection is closed.
   *
   * \param sender A TCP sender.
   */
  void OnDisconnect(connection_manager::ServiceDiscoveryTcpSender* sender) override;
  /**
   * \brief A container type for TCP eventgroup subscriptions.
   */
  using TcpEventgroupSubscriptionContainer = std::vector<connection_manager::ServiceDiscoveryTcpSenderPtr>;
  /**
   * \brief A container type for UDP eventgroup subscriptions.
   */
  using UdpEventgroupSubscriptionContainer = std::vector<connection_manager::ServiceDiscoveryUdpSenderPtr>;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* packet_router_;
  /**
   * \brief A connection manager.
   */
  connection_manager::ServiceDiscoveryConnectionManager* conman_;
  /**
   * \brief A configuration.
   */
  const configuration::Configuration* config_;
  /**
   * \brief The corresponding configuration of this server service instance.
   */
  const configuration::Configuration::ProvidedServiceInstance* instance_config_;
  /**
   * \brief The corresponding port mapping of this server service instance.
   */
  const configuration::Configuration::NetworkEndpointPortMapping* port_mapping_config_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A TCP receiver.
   */
  connection_manager::ServiceDiscoveryTcpReceiverPtr tcp_receiver_;
  /**
   * \brief A UDP receiver.
   */
  connection_manager::ServiceDiscoveryUdpReceiverPtr udp_receiver_;
  /**
   * \brief A UDP multicast sender.
   */
  connection_manager::ServiceDiscoveryUdpSenderPtr udp_mcast_sender_;
  /**
   * \brief A map from a SOME/IP eventgroup to TCP eventgroup subscriptions.
   */
  std::unordered_map<std::uint16_t, TcpEventgroupSubscriptionContainer> tcp_eventgroup_subscriptions_;
  /**
   * \brief A map from a SOME/IP eventgroup to UDP eventgroup subscriptions.
   */
  std::unordered_map<std::uint16_t, UdpEventgroupSubscriptionContainer> udp_eventgroup_subscriptions_;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_SERVER_SERVICE_INSTANCE_H_
