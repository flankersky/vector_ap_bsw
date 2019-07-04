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
/**        \file  service_discovery_server_message_builder.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>
#include "osabstraction/io/network/address/ip_socket_address.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/message/service_discovery_message_builder.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder_interface.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * A Service Discovery Server message builder.
 */
class ServiceDiscoveryServerMessageBuilder : public ServiceDiscoveryServerMessageBuilderInterface {
 public:
  /**
   * Constructor
   *
   * \param address The IP address of this server service instance.
   * \param config Provided Service configuration.
   */
  ServiceDiscoveryServerMessageBuilder(const std::string& address,
                                       const configuration::Configuration::ProvidedServiceInstance* config);

  /**
   * Destructor
   */
  virtual ~ServiceDiscoveryServerMessageBuilder() = default;

  /**
   * \brief Creates SD entry and option containers for an OfferService SD message.
   *
   * \return A pair of a SD entry container and a SD option container.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeOfferServiceMessage() override;

  /**
   * \brief Creates SD entry and option containers for an StopOfferService SD message.
   *
   * \return A pair of a SD entry container and a SD option container.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeStopOfferServiceMessage() override;

  /**
   * \brief Creates SD entry and option containers for a positive SD message in response to a SubscribeEventgroup
   * request.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param ttl A TTL value.
   * \param counter A counter value.
   * \return A pair of a SD entry container and a SD option container.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupAckMessage(
      std::uint16_t eventgroup_id, std::uint32_t ttl, std::uint8_t counter) override;

  /**
   * \brief Creates SD entry and option containers for a negative SD message in response to a SubscribeEventgroup
   * request.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param counter A counter value.
   * \return A pair of a SD entry container and a SD option container.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupNackMessage(
      std::uint16_t eventgroup_id, std::uint8_t counter) override;

 protected:
  /**
   * \brief Creates SD entry and option containers for a Stop/OfferService SD message.
   *
   * \param ttl TTL of the Offer packet.
   * For StopOfferService messages, ttl must be set to 0, and for OfferService messages, ttl must be > 0,
   *
   * \return A pair of a SD entry container and a SD option container.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeOfferServiceMessage(std::uint32_t ttl);

 private:
  /**
   * \brief Alias for ServiceDiscoveryMessageBuilder.
   */
  using ServiceDiscoveryMessageBuilder = service_discovery::message::ServiceDiscoveryMessageBuilder;
  /**
   * \brief The configuration of a provided service instance.
   */
  const configuration::Configuration::ProvidedServiceInstance* instance_config_;
  /**
   * \brief The corresponding port mapping of this server service instance.
   */
  const configuration::Configuration::NetworkEndpointPortMapping* port_mapping_config_;
  /**
   * \brief TCP message option type.
   */
  std::uint8_t tcp_option_type_{};
  /**
   * \brief UDP message option type.
   */
  std::uint8_t udp_option_type_{};
  /**
   * \brief Event multicast message option type.
   */
  std::uint8_t multicast_option_type_{};
};

} /* namespace server */
} /* namespace state_machine */
} /* namespace service_discovery */
} /* namespace someipd_posix */

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_H_
