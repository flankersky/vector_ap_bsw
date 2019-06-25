/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector
 * Informatik GmbH. Vector Informatik GmbH grants to you only those rights as
 * set out in the license conditions. All other rights remain
 * with Vector Informatik GmbH. \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  service_discovery_message_sender.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <vector>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief ServiceDiscoveryMessageSender.
 */
class ServiceDiscoveryMessageSender {
 public:
  /**
   * \brief Destructor of ServiceDiscoveryMessageSender.
   */
  virtual ~ServiceDiscoveryMessageSender() = default;
  /**
   * \brief Sends a unicast SOME/IP SD message to the provided IP address and port.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   * \param entries A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  virtual void SendUnicastMessage(const std::string& to_address, std::uint16_t to_port,
                                  const ServiceDiscoveryEntryContainer& entries,
                                  const ServiceDiscoveryOptionContainer& options) = 0;
  /**
   * \brief Sends a multicast SOME/IP SD message to the provided IP address and port.
   *
   * \param entries A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  virtual void SendMulticastMessage(const ServiceDiscoveryEntryContainer& entries,
                                    const ServiceDiscoveryOptionContainer& options) = 0;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_MESSAGE_SENDER_H_
