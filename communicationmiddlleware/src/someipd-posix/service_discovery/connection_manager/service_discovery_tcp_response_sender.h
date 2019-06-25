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
/**        \file  service_discovery_tcp_response_sender.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_RESPONSE_SENDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_RESPONSE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "someipd-posix/packet_router/packet_router_interface.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpEndpoint;
class ServiceDiscoveryTcpConnection;

/**
 * \brief ServiceDiscoveryTcpResponseSender.
 */
class ServiceDiscoveryTcpResponseSender : public packet_router::PacketSink {
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
   * \brief Constructor of ServiceDiscoveryTcpResponseSender.
   *
   * \param endpoint A TCP endpoint this response sender belongs to.
   * \param connection A TCP connection this response sender belongs to.
   */
  ServiceDiscoveryTcpResponseSender(ServiceDiscoveryTcpEndpointPtr endpoint,
                                    ServiceDiscoveryTcpConnectionPtr connection);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryTcpResponseSender(const ServiceDiscoveryTcpResponseSender&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryTcpResponseSender& operator=(const ServiceDiscoveryTcpResponseSender&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryTcpResponseSender.
   */
  virtual ~ServiceDiscoveryTcpResponseSender();
  /**
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet) override;

 private:
  /**
   * \brief A TCP endpoint this response sender belongs to.
   */
  ServiceDiscoveryTcpEndpointPtr endpoint_;
  /**
   * \brief A TCP connection this response sender belongs to.
   */
  ServiceDiscoveryTcpConnectionPtr connection_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_RESPONSE_SENDER_H_
