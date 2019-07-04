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
/**        \file  service_discovery_udp_response_sender.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RESPONSE_SENDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RESPONSE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "someipd-posix/packet_router/packet_router_interface.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryUdpEndpoint;
class ServiceDiscoveryUdpConnection;

/**
 * \brief ServiceDiscoveryUdpResponseSender.
 */
class ServiceDiscoveryUdpResponseSender : public packet_router::PacketSink {
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
   * \brief Constructor of ServiceDiscoveryUdpResponseSender.
   *
   * \param endpoint An UDP endpoint this response sender belongs to.
   * \param connection An UDP connection this response sender belongs to.
   */
  ServiceDiscoveryUdpResponseSender(ServiceDiscoveryUdpEndpointPtr endpoint,
                                    ServiceDiscoveryUdpConnectionPtr connection);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryUdpResponseSender(const ServiceDiscoveryUdpResponseSender&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryUdpResponseSender& operator=(const ServiceDiscoveryUdpResponseSender&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryUdpResponseSender.
   */
  virtual ~ServiceDiscoveryUdpResponseSender();
  /**
   * \brief Sends a SOME/IP message to remote peer.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, packet_router::Packet packet) override;

 private:
  /**
   * \brief An UDP endpoint this response sender belongs to.
   */
  ServiceDiscoveryUdpEndpointPtr endpoint_;
  /**
   * \brief An UDP connection this response sender belongs to.
   */
  ServiceDiscoveryUdpConnectionPtr connection_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RESPONSE_SENDER_H_
