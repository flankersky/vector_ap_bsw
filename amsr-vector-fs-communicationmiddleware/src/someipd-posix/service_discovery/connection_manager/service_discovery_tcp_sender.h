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
/**        \file  service_discovery_tcp_sender.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SENDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>

#include "ara/log/logging.hpp"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_connection_state_change_handler.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpEndpoint;
class ServiceDiscoveryTcpConnection;

/**
 * \brief ServiceDiscoveryTcpSender.
 */
class ServiceDiscoveryTcpSender : public packet_router::PacketSink {
 public:
  /**
   * \brief A TCP endpoint pointer type.
   */
  using ServiceDiscoveryTcpEndpointPtr = std::shared_ptr<ServiceDiscoveryTcpEndpoint>;
  /**
   * \brief A TCP connection pointer type.
   */
  using ServiceDiscoveryTcpConnectionPtr = ServiceDiscoveryTcpConnection*;
  /**
   * \brief A TCP connection state change handler pointer type.
   */
  using ServiceDiscoveryTcpConnectionStateChangeHandlerPtr = ServiceDiscoveryTcpConnectionStateChangeHandler*;
  /**
   * \brief Constructor of ServiceDiscoveryTcpSender.
   *
   * \param endpoint A TCP endpoint this sender belongs to.
   * \param connection A TCP connection this sender belongs to.
   */
  ServiceDiscoveryTcpSender(ServiceDiscoveryTcpEndpointPtr endpoint, ServiceDiscoveryTcpConnectionPtr connection);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryTcpSender(const ServiceDiscoveryTcpSender&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryTcpSender& operator=(const ServiceDiscoveryTcpSender&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryTcpSender.
   */
  virtual ~ServiceDiscoveryTcpSender();
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
   * \brief Sets a TCP connection state change handler.
   *
   * \param handler A TCP connection state change handler.
   */
  void SetConnectionStateChangeHandler(ServiceDiscoveryTcpConnectionStateChangeHandlerPtr handler);
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
  /**
   * \brief Notifies a TCP connection state change handler about a state change.
   */
  void Notify();

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
   * \brief A TCP endpoint this sender belongs to.
   */
  ServiceDiscoveryTcpEndpointPtr endpoint_;
  /**
   * \brief A TCP connection this sender belongs to.
   */
  ServiceDiscoveryTcpConnectionPtr connection_;
  /**
   * \brief A TCP connection state change handler.
   */
  ServiceDiscoveryTcpConnectionStateChangeHandlerPtr handler_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A container of registered required SOME/IP service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SENDER_H_
