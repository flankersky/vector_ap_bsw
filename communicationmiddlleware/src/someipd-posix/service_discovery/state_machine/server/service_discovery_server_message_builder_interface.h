/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_server_message_builder_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * Interface for Service Discovery Server message builder.
 */
class ServiceDiscoveryServerMessageBuilderInterface {
 public:
  /**
   * \brief Destructor
   */
  virtual ~ServiceDiscoveryServerMessageBuilderInterface() = default;

  /**
   * \brief Creates SD entry and option containers for an OfferService SD message.
   *
   * \return A pair of a SD entry container and a SD option container.
   */
  virtual std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeOfferServiceMessage() = 0;

  /**
   * \brief Creates SD entry and option containers for an StopOfferService SD message.
   *
   * \return A pair of a SD entry container and a SD option container.
   */
  virtual std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeStopOfferServiceMessage() = 0;

  /**
   * \brief Creates SD entry and option containers for a positive SD message in response to a SubscribeEventgroup
   * request.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param ttl A TTL value.
   * \param counter A counter value.
   * \return A pair of a SD entry container and a SD option container.
   */
  virtual std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupAckMessage(
      std::uint16_t eventgroup_id, std::uint32_t ttl, std::uint8_t counter) = 0;

  /**
   * \brief Creates SD entry and option containers for a negative SD message in response to a SubscribeEventgroup
   * request.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param counter A counter value.
   * \return A pair of a SD entry container and a SD option container.
   */
  virtual std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupNackMessage(
      std::uint16_t eventgroup_id, std::uint8_t counter) = 0;
};

} /* namespace server */
} /* namespace state_machine */
} /* namespace service_discovery */
} /* namespace someipd_posix */

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_MESSAGE_BUILDER_INTERFACE_H_
