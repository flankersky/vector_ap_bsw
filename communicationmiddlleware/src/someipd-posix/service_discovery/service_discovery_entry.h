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
/**        \file  service_discovery_entry.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENTRY_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENTRY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <vector>

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief Represents a SOME/IP SD endpoint option.
 */
struct ServiceDiscoveryEntry {
  std::uint8_t type_;             ///< SOME/IP SD entry type
  std::uint16_t service_id_;      ///< SOME/IP service identifier
  std::uint16_t instance_id_;     ///< SOME/IP instance identifier
  std::uint8_t major_version_;    ///< Major version of service
  std::uint32_t minor_version_;   ///< Minor version of service (unused for
                                  ///  SubscribeEventgroup/StopSubscribeEventgroup and
                                  ///  SubscribeEventgroupAck/SubscribeEventgroupNack entries)
  std::uint8_t index_1st_opts_;   ///< Index of the first option in the first option run
  std::uint8_t index_2nd_opts_;   ///< Index of the first option in the second option run
  std::uint8_t number_1st_opts_;  ///< Number of options in the first option array
  std::uint8_t number_2nd_opts_;  ///< Number of options in the second option array
  std::uint32_t ttl_;             ///< Time-to-live
  std::uint8_t counter_;          ///< Counter (valid only for SubscribeEventgroup/StopSubscribeEventgroup entries)
  std::uint16_t
      eventgroup_id_;  ///< Eventgroup identifier (valid only for SubscribeEventgroup/StopSubscribeEventgroup entries)

  /**
   * \brief Compares SOME/IP SD entries.
   *
   * \param other A SOME/IP SD entry to compare to.
   * \return true if both entries are equal and false otherwise.
   */
  bool operator==(const ServiceDiscoveryEntry& other) const {
    return (type_ == other.type_) && (service_id_ == other.service_id_) && (instance_id_ == other.instance_id_) &&
           (major_version_ == other.major_version_) && (minor_version_ == other.minor_version_) &&
           (index_1st_opts_ == other.index_1st_opts_) && (index_2nd_opts_ == other.index_2nd_opts_) &&
           (number_1st_opts_ == other.number_1st_opts_) && (number_2nd_opts_ == other.number_2nd_opts_) &&
           (ttl_ == other.ttl_) && (counter_ == other.counter_) && (eventgroup_id_ == other.eventgroup_id_);
  }
};

/**
 * \brief A container for SD entries.
 */
using ServiceDiscoveryEntryContainer = std::vector<ServiceDiscoveryEntry>;

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENTRY_H_
