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
/**        \file  service_discovery_server.h
 *        \brief  Service Discovery Server Service Instance State Machine implementation
 *
 *      \details  This class creates and contains the SD server state machine. It receives all events and forwards them
 *to the state machine owner.
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/server/eventgroup/service_discovery_server_event_manager.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_context.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_offer_service_send_queue.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_service_instance_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief ServiceDiscoveryServerServiceInstanceStateMachine.
 */
class ServiceDiscoveryServer : public ServiceDiscoveryServerServiceInstanceStateMachine {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryServerServiceInstanceStateMachine.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version The major version of SOME/IP service instance.
   * \param minor_version The minor version of SOME/IP service instance.
   * \param address The IP address of this server service instance.
   * \param timer_manager A timer manager.
   * \param message_sender A message sender.
   * \param service_instance A server service instance.
   * \param config A configuration.
   */
  ServiceDiscoveryServer(std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version,
                         std::uint32_t minor_version, const std::string& address,
                         vac::timer::TimerManager* timer_manager, ServiceDiscoveryMessageSender* message_sender,
                         std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance,
                         const configuration::Configuration* config);
  /**
   * \brief Delete copy constructor.
   */
  explicit ServiceDiscoveryServer(const ServiceDiscoveryServer& other) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServer& operator=(const ServiceDiscoveryServer& other) = delete;
  /**
   * \brief Delete move constructor.
   */
  explicit ServiceDiscoveryServer(const ServiceDiscoveryServer&& other) = delete;
  /**
   * \brief Delete move assignment operator.
   */
  ServiceDiscoveryServer& operator=(const ServiceDiscoveryServer&& other) = delete;
  /**
   * \brief Called on service instance startup.
   */
  void OnStart() override;
  /**
   * \brief Called on service instance shutdown.
   */
  void OnStop() override;
  /**
   * \brief Called when network is brought up.
   */
  void OnNetworkUp() override;
  /**
   * \brief Called when network is brought down.
   */
  void OnNetworkDown() override;
  /**
   * \brief Called when a provided service instance is brought up.
   */
  void OnServiceUp() override;
  /**
   * \brief Called when a provided service instance is brought down.
   */
  void OnServiceDown() override;
  /**
   * \brief Called on reception of a unicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnUnicastMessage(const someip_posix_common::someip::IpAddress& from_address,
                        someip_posix_common::someip::Port from_port, const ServiceDiscoveryEntryContainer& entries,
                        const ServiceDiscoveryOptionContainer& options) override;
  /**
   * \brief Called on reception of a multicast SOME/IP SD message.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entries A container of entries received in a SOME/IP SD message.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnMulticastMessage(const someip_posix_common::someip::IpAddress& from_address,
                          someip_posix_common::someip::Port from_port, const ServiceDiscoveryEntryContainer& entries,
                          const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Called on detection of reboot.
   *
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A port identifying sender's port.
   */
  void OnRebootDetected(const someip_posix_common::someip::IpAddress& from_address,
                        someip_posix_common::someip::Port from_port) override;

 protected:
  /**
   * \brief Checks if the entry targets this state machine or not.
   * The following checks are done:
   *  - entry service id matches that of the state machine,
   *  - entry instance id matches that of the state machine or is set to kInstanceIdAny,
   *  - entry major version matches that of the state machine or is set to kMajorVersionAny,
   *
   * \param entry An SD Entry.
   *
   * \return True if the entry matches this state machine; false otherwise.
   */
  bool IsMatchingEntry(const ServiceDiscoveryEntry& entry) const;

 private:
  /**
   * \brief Generates a unique logger context identifier for given SOME/IP service parameters.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A SOME/IP service major version.
   * \param minor_version A SOME/IP service minor version.
   * \return A string representing the logger context identifier for given SOME/IP service parameters.
   */
  static std::string GetLoggerContextId(std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version,
                                        std::uint32_t minor_version);

  /**
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder message_builder_;
  /**
   * \brief Event Subscription Manager.
   */
  eventgroup::ServiceDiscoveryServerEventManager event_manager_;
  /**
   * \brief Message send queue for delayed messages
   */
  ServiceDiscoveryServerOfferServiceSendQueue message_send_queue_;
  /**
   * \brief State Pool.
   */
  ServiceDiscoveryServerStatePool state_pool_;
  /**
   * \brief State Machine Owner.
   */
  ServiceDiscoveryServerStateOwner state_owner_;
  /**
   * \brief A container of available actions for ServiceDiscoveryServer.
   */
  ServiceDiscoveryServerContext context_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_H_
