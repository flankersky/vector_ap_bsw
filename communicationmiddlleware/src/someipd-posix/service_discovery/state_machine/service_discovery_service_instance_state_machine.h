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
/**        \file  service_discovery_service_instance_state_machine.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVICE_DISCOVERY_SERVICE_INSTANCE_STATE_MACHINE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVICE_DISCOVERY_SERVICE_INSTANCE_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_message_sender.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {

/**
 * \brief ServiceDiscoveryServiceInstanceStateMachine.
 */
class ServiceDiscoveryServiceInstanceStateMachine {
 public:
  /**
   * \brief Constructor.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version The major version of SOME/IP service instance.
   * \param minor_version The minor version of SOME/IP service instance.
   * \param timer_manager A timer manager.
   * \param message_sender A message sender.
   */
  ServiceDiscoveryServiceInstanceStateMachine(std::uint16_t service_id, std::uint16_t instance_id,
                                              std::uint8_t major_version, std::uint32_t minor_version,
                                              vac::timer::TimerManager* timer_manager,
                                              ServiceDiscoveryMessageSender* message_sender)
      : service_id_(service_id),
        instance_id_(instance_id),
        major_version_(major_version),
        minor_version_(minor_version),
        timer_manager_(timer_manager),
        message_sender_(message_sender) {}

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryServiceInstanceStateMachine(const ServiceDiscoveryServiceInstanceStateMachine&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServiceInstanceStateMachine& operator=(const ServiceDiscoveryServiceInstanceStateMachine&) = delete;
  /**
   * \brief Destructor.
   */
  virtual ~ServiceDiscoveryServiceInstanceStateMachine() = default;
  /**
   * \brief Returns the service identifier of a SOME/IP service instance.
   *
   * \return A service identifier.
   */
  std::uint16_t GetServiceId() const { return service_id_; }
  /**
   * \brief Returns the service instance identifier of a SOME/IP service instance.
   *
   * \return A service instance identifier.
   */
  std::uint16_t GetInstanceId() const { return instance_id_; }
  /**
   * \brief Returns the major version of a SOME/IP service instance.
   *
   * \return Major version of SOME/IP service instance.
   */
  std::uint8_t GetMajorVersion() const { return major_version_; }
  /**
   * \brief Returns the minor version of a SOME/IP service instance.
   *
   * \return Minor version of SOME/IP service instance.
   */
  std::uint32_t GetMinorVersion() const { return minor_version_; }
  /**
   * \brief Called on service instance startup.
   */
  virtual void OnStart() = 0;
  /**
   * \brief Called on service instance shutdown.
   */
  virtual void OnStop() = 0;
  /**
   * \brief Called when network is brought up.
   */
  virtual void OnNetworkUp() = 0;
  /**
   * \brief Called when network is brought down.
   */
  virtual void OnNetworkDown() = 0;
  /**
   * \brief Called on reception of a unicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnUnicastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                someip_posix_common::someip::Port from_port,
                                const ServiceDiscoveryEntryContainer& entries,
                                const ServiceDiscoveryOptionContainer& options) = 0;
  /**
   * \brief Called on reception of a multicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnMulticastMessage(const someip_posix_common::someip::IpAddress& from_address,
                                  someip_posix_common::someip::Port from_port,
                                  const ServiceDiscoveryEntryContainer& entries,
                                  const ServiceDiscoveryOptionContainer& options) = 0;

  /**
   * \brief Called on detection of reboot.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   */
  virtual void OnRebootDetected(const someip_posix_common::someip::IpAddress& from_address,
                                someip_posix_common::someip::Port from_port) = 0;

 protected:
  /**
   * \brief A SOME/IP service identifier.
   */
  std::uint16_t service_id_;
  /**
   * \brief A SOME/IP service instance identifier.
   */
  std::uint16_t instance_id_;
  /**
   * \brief The major version of a SOME/IP service instance.
   */
  std::uint8_t major_version_;
  /**
   * \brief The minor version of a SOME/IP service instance.
   */
  std::uint32_t minor_version_;
  /**
   * \brief A timer manager.
   */
  vac::timer::TimerManager* timer_manager_;
  /**
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSender* message_sender_;
};

}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVICE_DISCOVERY_SERVICE_INSTANCE_STATE_MACHINE_H_
