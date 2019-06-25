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
/**        \file  service_discovery_endpoint_observer.h
 *        \brief  This interface provides pure virtual methods to handle actions on reception of a SOME/IP SD message
 *
 *
 *********************************************************************************************************************/
#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_OBSERVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
namespace someipd_posix {
namespace service_discovery {

/**
 * \brief This interface provides pure virtual methods to handle actions on reception of a SOME/IP SD message
 */
class ServiceDiscoveryEndpointObserver {
 public:
  virtual ~ServiceDiscoveryEndpointObserver() = default;

  /**
   * \brief Function to handle reception of a unicast SOME/IP SD message.
   *
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A contaliner of options received in a SOME/IP SD message.
   */
  virtual void OnUnicastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                                  const someip_posix_common::someip::IpAddress& from_address,
                                  someip_posix_common::someip::Port from_port,
                                  const ServiceDiscoveryEntryContainer& entries,
                                  const ServiceDiscoveryOptionContainer& options) = 0;
  /**
   * \brief Function to handle reception of a multicast SOME/IP SD message.
   *
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnMulticastSdMessage(const someip_posix_common::someip::IpAddress& my_address,
                                    const someip_posix_common::someip::IpAddress& from_address,
                                    someip_posix_common::someip::Port from_port,
                                    const ServiceDiscoveryEntryContainer& entries,
                                    const ServiceDiscoveryOptionContainer& options) = 0;

  /**
   * \brief Function to handle reboot detection for unicast and  multicast SOME/IP SD message.
   * \param my_address The unicast IP address of the SD endpoint which received the SD message.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   */
  virtual void OnRebootDetected(const someip_posix_common::someip::IpAddress& my_address,
                                const someip_posix_common::someip::IpAddress& from_address,
                                someip_posix_common::someip::Port from_port) = 0;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_OBSERVER_H_
