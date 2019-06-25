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
/**        \file  service_discovery_option.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OPTION_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <string>
#include <vector>

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief Represents a SOME/IP SD option.
 */
struct ServiceDiscoveryOption {
  std::uint8_t type_;    ///< Type of endpoint option
  std::string address_;  ///< IPv4 or IPv6 endpoint address
  std::uint8_t proto_;   ///< L4 protocol (TCP or UDP)
  std::uint16_t port_;   ///< L4 port number

  /**
   * \brief Compares SOME/IP SD options.
   *
   * \param other A SOME/IP SD option to compare to.
   * \return true if both options are equal and false otherwise.
   */
  bool operator==(const ServiceDiscoveryOption& other) const {
    return (type_ == other.type_) && (address_ == other.address_) && (proto_ == other.proto_) && (port_ == other.port_);
  }
};

/**
 * \brief A container for SD options.
 */
using ServiceDiscoveryOptionContainer = std::vector<ServiceDiscoveryOption>;

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OPTION_H_
