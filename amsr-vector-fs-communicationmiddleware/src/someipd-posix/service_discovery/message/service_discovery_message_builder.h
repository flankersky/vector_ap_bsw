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
/**        \file  service_discovery_message_builder.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "someip-posix-common/someip/someip_posix_types.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {
namespace message {

/**
 * A Service Discovery message builder.
 * //TODO(PAASR-3104) extend this class
 */
class ServiceDiscoveryMessageBuilder {
 public:
  /**
   * Destructor
   */
  virtual ~ServiceDiscoveryMessageBuilder() = default;

  /**
   * \brief Creates SD entry and option containers for a negative SD message in response to a SubscribeEventgroup
   * request.
   *
   * \param service_id SOME/IP service identifier.
   * \param instance_id SOME/IP instance identifier.
   * \param major_version SOME/IP major version.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param counter A counter value.
   * \return A pair of a SD entry container and a SD option container.
   */
  static std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer> MakeSubscribeEventgroupNackMessage(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
      someip_posix_common::someip::MajorVersion major_version, someip_posix_common::someip::EventgroupId eventgroup_id,
      std::uint8_t counter);
};

}  // namespace message
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_MESSAGE_SERVICE_DISCOVERY_MESSAGE_BUILDER_H_
