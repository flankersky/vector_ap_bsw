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
/**        \file  service_discovery_client_service_instance.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <string>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_connection_manager.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace service_discovery {

class ServiceDiscovery;

/**
 * \brief ServiceDiscoveryClientServiceInstance.
 */
class ServiceDiscoveryClientServiceInstance
    : public std::enable_shared_from_this<ServiceDiscoveryClientServiceInstance>,
      public packet_router::PacketSink,
      public connection_manager::ServiceDiscoveryTcpConnectionStateChangeHandler {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryClientServiceInstance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version The major version of SOME/IP service instance.
   * \param minor_version The minor version of SOME/IP service instance.
   * \param service_discovery A service discovery.
   * \param conman A connection manager.
   * \param config A configuration.
   */
  ServiceDiscoveryClientServiceInstance(std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version,
                                        std::uint32_t minor_version, ServiceDiscovery* service_discovery,
                                        connection_manager::ServiceDiscoveryConnectionManager* conman,
                                        const configuration::Configuration* config);

  /**
   * \brief Connects to a remote provided service instance identified by its IP address and port numbers.
   *
   * \param tcp_address An IP address of a provided service instance.
   * \param tcp_port A TCP port number.
   * \param udp_address An IP address of a provided service instance.
   * \param udp_port A UDP port number.
   */
  virtual void Connect(const std::string& tcp_address, std::uint16_t tcp_port, const std::string& udp_address,
                       std::uint16_t udp_port);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryClientServiceInstance(const ServiceDiscoveryClientServiceInstance&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientServiceInstance& operator=(const ServiceDiscoveryClientServiceInstance&) = delete;

  /**
   * \brief Disconnects from the connected remote provided service instance.
   */
  virtual void Disconnect();
  /**
   * \brief Starts listening for multicast UDP events of a given eventgroup.
   *
   * \param address A multicast IP address to which events of the given eventgroup will be sent.
   * \param port A UDP port number.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void StartListenForMulticastEventgroup(const std::string& address, std::uint16_t port,
                                                 std::uint16_t eventgroup_id);
  /**
   * \brief Stops listening for multicast UDP events of a given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void StopListenForMulticastEventgroup(std::uint16_t eventgroup_id);

 protected:
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
   * \brief A service discovery.
   */
  ServiceDiscovery* service_discovery_;
  /**
   * \brief A connection manager.
   */
  connection_manager::ServiceDiscoveryConnectionManager* conman_;
  /**
   * \brief A configuration.
   */
  const configuration::Configuration* config_;
  /**
   * \brief The corresponding configuration of this client service instance.
   */
  const configuration::Configuration::RequiredServiceInstance* instance_config_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief An IP address of a provided service instance.
   */
  std::string tcp_address_;
  /**
   * \brief A TCP port number of a provided service instance.
   */
  std::uint16_t tcp_port_;
  /**
   * \brief An IP address of a provided service instance.
   */
  std::string udp_address_;
  /**
   * \brief A UDP port number of a provided service instance.
   */
  std::uint16_t udp_port_;
  /**
   * \brief A TCP sender.
   */
  connection_manager::ServiceDiscoveryTcpSenderPtr tcp_sender_;
  /**
   * \brief A UDP sender.
   */
  connection_manager::ServiceDiscoveryUdpSenderPtr udp_sender_;
  /**
   * \brief A UDP receiver.
   */
  connection_manager::ServiceDiscoveryUdpReceiverPtr udp_receiver_;
  /**
   * \brief Counts how many eventgroup subscriptions are active.
   */
  std::atomic<std::size_t> eventgroup_subscriptions_;
  /**
   * \brief Indicates whether a service is offered or not.
   */
  bool is_offered_;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_H_
