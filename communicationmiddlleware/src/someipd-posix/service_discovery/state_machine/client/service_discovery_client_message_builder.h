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
/**        \file  service_discovery_client_message_builder.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_MESSAGE_BUILDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/log/logging.hpp"
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

/**
 * \brief A Service Discovery Client message builder.
 */
class ServiceDiscoveryClientMessageBuilder {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryClientMessageBuilder.
   *
   * \param instance_config The configuration of a required service instance.
   */
  explicit ServiceDiscoveryClientMessageBuilder(
      const configuration::Configuration::RequiredServiceInstance* instance_config);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryClientMessageBuilder(const ServiceDiscoveryClientMessageBuilder&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClientMessageBuilder& operator=(const ServiceDiscoveryClientMessageBuilder&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryClientMessageBuilder.
   */
  virtual ~ServiceDiscoveryClientMessageBuilder() = default;
  /**
   * \brief Builds a SOME/IP-SD message containing a single FindService entry.
   *
   * \return A pair of SOME/IP-SD entries and options.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeFindServiceMessage() const;
  /**
   * \brief Builds a SOME/IP-SD message containing a single SubscribeEventgroup entry and the corresponding options.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   * \return A pair of SOME/IP-SD entries and options.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupMessage(
      std::uint16_t eventgroup_id, bool request_initial_events) const;
  /**
   * \brief Builds a SOME/IP-SD message containing a single StopSubscribeEventgroup entry and the corresponding options.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \return A pair of SOME/IP-SD entries and options.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeStopSubscribeEventgroupMessage(
      std::uint16_t eventgroup_id) const;

 private:
  /**
   * \brief Builds a SOME/IP-SD message containing a single (Stop)SubscribeEventgroup entry and the corresponding
   * options.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param ttl A TTL value.
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   * \return A pair of SOME/IP-SD entries and options.
   */
  std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeEventgroupMessage(
      std::uint16_t eventgroup_id, std::uint32_t ttl, bool request_initial_events) const;

  /**
   * \brief The configuration of a required service instance.
   */
  const configuration::Configuration::RequiredServiceInstance* instance_config_;
  /**
   * \brief An option type, either IPv4 or IPv6.
   */
  someip_posix_common::someip::SomeIpSdEndpointOptionType option_type_;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_MESSAGE_BUILDER_H_
