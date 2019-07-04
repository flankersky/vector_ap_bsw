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
/**        \file  service_discovery_message_builder.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/message/service_discovery_message_builder.h"
#include "someip-posix-common/someip/message.h"

namespace someipd_posix {
namespace service_discovery {
namespace message {

std::pair<ServiceDiscoveryEntryContainer, ServiceDiscoveryOptionContainer>
ServiceDiscoveryMessageBuilder::MakeSubscribeEventgroupNackMessage(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id,
    someip_posix_common::someip::MajorVersion major_version, someip_posix_common::someip::EventgroupId eventgroup_id,
    std::uint8_t counter) {
  namespace someip = someip_posix_common::someip;
  ServiceDiscoveryEntryContainer entries{};
  ServiceDiscoveryOptionContainer options{};
  ServiceDiscoveryEntry entry{};
  entry.type_ = someip::SomeIpSdEntryType::kSubscribeEventgroupAck;
  entry.service_id_ = service_id;
  entry.instance_id_ = instance_id;
  entry.major_version_ = major_version;
  entry.index_1st_opts_ = 0U;
  entry.index_2nd_opts_ = 0U;
  entry.number_1st_opts_ = 0U;
  entry.number_2nd_opts_ = 0U;
  entry.ttl_ = 0U;  // TTL must be zero in a SubscribeEventgroupNack entry
  entry.counter_ = counter;
  entry.eventgroup_id_ = eventgroup_id;
  entries.push_back(entry);
  return std::make_pair(entries, options);
}

}  // namespace message
}  // namespace service_discovery
}  // namespace someipd_posix
