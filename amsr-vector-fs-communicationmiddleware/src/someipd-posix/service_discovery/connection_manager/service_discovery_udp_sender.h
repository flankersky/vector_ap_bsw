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
/**        \file  service_discovery_udp_sender.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_SENDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "someipd-posix/packet_router/packet_router_interface.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryUdpEndpoint;
class ServiceDiscoveryUdpConnection;

/**
 * \brief ServiceDiscoveryUdpSender.
 */
class ServiceDiscoveryUdpSender : public packet_router::PacketSink {
 public:
  /**
   * \brief An UDP endpoint pointer type.
   */
  using ServiceDiscoveryUdpEndpointPtr = std::shared_ptr<ServiceDiscoveryUdpEndpoint>;
  /**
   * \brief An UDP connection pointer type.
   */
  using ServiceDiscoveryUdpConnectionPtr = ServiceDiscoveryUdpConnection*;
  /**
   * \brief Constructor of ServiceDiscoveryUdpSender.
   *
   * \param endpoint An UDP endpoint this sender belongs to.
   * \param connection An UDP connection this sender belongs to.
   */
  ServiceDiscoveryUdpSender(ServiceDiscoveryUdpEndpointPtr endpoint, ServiceDiscoveryUdpConnectionPtr connection);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryUdpSender(const ServiceDiscoveryUdpSender&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryUdpSender& operator=(const ServiceDiscoveryUdpSender&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryUdpSender.
   */
  virtual ~ServiceDiscoveryUdpSender();
  /**
   * \brief Returns the remote IP address.
   *
   * \return A remote IP address.
   */
  const someip_posix_common::someip::IpAddress& GetRemoteAddress() const;
  /**
   * \brief Returns the remote port number.
   *
   * \return A remote port number.
   */
  someip_posix_common::someip::Port GetRemotePort() const;
  /**
   * \brief Registers a required SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void RegisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                       someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Unregisters a required SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void UnregisterRequiredServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                         someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet) override;

 private:
  /**
   * \brief Represents a required SOME/IP service instance.
   */
  using RequiredServiceInstance =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Represents a container of required SOME/IP service instances.
   */
  using RequiredServiceInstanceContainer = std::vector<RequiredServiceInstance>;
  /**
   * \brief An UDP endpoint this sender belongs to.
   */
  ServiceDiscoveryUdpEndpointPtr endpoint_;
  /**
   * \brief An UDP connection this sender belongs to.
   */
  ServiceDiscoveryUdpConnectionPtr connection_;
  /**
   * \brief A container of registered required SOME/IP service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_SENDER_H_
