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
/**        \file  service_discovery_udp_receiver.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RECEIVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RECEIVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include <vector>
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryUdpEndpoint;

/**
 * \brief ServiceDiscoveryUdpReceiver.
 */
class ServiceDiscoveryUdpReceiver {
 public:
  /**
   * \brief An UDP endpoint pointer type.
   */
  using ServiceDiscoveryUdpEndpointPtr = std::shared_ptr<ServiceDiscoveryUdpEndpoint>;
  /**
   * \brief Constructor of ServiceDiscoveryUdpReceiver.
   *
   * \param endpoint An UDP endpoint this receiver belongs to.
   */
  explicit ServiceDiscoveryUdpReceiver(ServiceDiscoveryUdpEndpointPtr endpoint);
  /**
   * \brief Destructor of ServiceDiscoveryUdpReceiver.
   */
  virtual ~ServiceDiscoveryUdpReceiver();
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
   * \brief Registers a provided SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void RegisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                       someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Unregisters a provided SOME/IP service instance.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   */
  void UnregisterProvidedServiceInstance(someip_posix_common::someip::ServiceId service_id,
                                         someip_posix_common::someip::InstanceId instance_id);

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
   * \brief Represents a provided SOME/IP service instance.
   */
  using ProvidedServiceInstance =
      std::pair<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;
  /**
   * \brief Represents a container of provided SOME/IP service instances.
   */
  using ProvidedServiceInstanceContainer = std::vector<ProvidedServiceInstance>;
  /**
   * \brief An UDP endpoint this receiver belongs to..
   */
  ServiceDiscoveryUdpEndpointPtr endpoint_;
  /**
   * \brief A container of registered required SOME/IP service instances.
   */
  RequiredServiceInstanceContainer required_service_instances_;
  /**
   * \brief A container of registered provided SOME/IP service instances.
   */
  ProvidedServiceInstanceContainer provided_service_instances_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_UDP_RECEIVER_H_
