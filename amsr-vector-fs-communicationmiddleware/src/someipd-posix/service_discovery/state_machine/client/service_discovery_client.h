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
/**        \file  service_discovery_client.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_client_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_manager.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_machine.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_context.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_message_builder.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_offer.h"
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_service_instance_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

/**
 * \brief A Service Discovery client.
 */
class ServiceDiscoveryClient : public ServiceDiscoveryClientServiceInstanceStateMachine,
                               public find_service::ServiceDiscoveryClientStateMachineContext,
                               public eventgroup::ServiceDiscoveryClientEventgroupManagerContext {
 public:
  /**
   * \brief A timer delay.
   */
  using find_service::ServiceDiscoveryClientStateMachineContext::TimerDelay;
  // TODO(PAASR-1034)
  /**
   * \brief Constructor of ServiceDiscoveryClient.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP service instance identifier.
   * \param major_version A SOME/IP service major version.
   * \param minor_version A SOME/IP service minor version.
   * \param timer_manager A timer manager.
   * \param message_sender A message sender.
   * \param service_instance A SOME/IP service instance.
   * \param config A configuration.
   * \param context ServiceDiscoveryClient context instance.
   */
  ServiceDiscoveryClient(std::uint16_t service_id, std::uint16_t instance_id, std::uint8_t major_version,
                         std::uint32_t minor_version, vac::timer::TimerManager* timer_manager,
                         ServiceDiscoveryMessageSender* message_sender,
                         std::shared_ptr<ServiceDiscoveryClientServiceInstance> service_instance,
                         const configuration::Configuration* config, ServiceDiscoveryClientContext* context);
  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryClient(const ServiceDiscoveryClient&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryClient& operator=(const ServiceDiscoveryClient&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryClient.
   */
  virtual ~ServiceDiscoveryClient() = default;
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
   * \brief Called when the service has been requested.
   */
  void OnServiceRequested() override;
  /**
   * \brief Called when the service has been released.
   */
  void OnServiceReleased() override;
  /**
   * \brief Initiates the subscription of a SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnSubscribeEventgroup(std::uint16_t eventgroup_id) override;
  /**
   * \brief Cancels the subscription of a SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnStopSubscribeEventgroup(std::uint16_t eventgroup_id) override;
  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current event group subscription state.
   */
  ServiceDiscoveryEventSubscriptionState GetEventgroupSubscriptionState(std::uint16_t eventgroup_id) const override;
  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current eventgroup subscription state.
   */
  std::size_t GetEventgroupSubscribersCount(std::uint16_t eventgroup_id) const override;
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
   * \brief Updates the availability of the service.
   *
   * \param is_available Indicates the availability of the service.
   */
  void SetServiceAvailable(bool is_available) override;
  /**
   * \brief Returns the TTL value from the last matching OfferService entry.
   *
   * \return TTL value.
   */
  TimerDelay GetOfferServiceTtl() const override;
  /**
   * \brief Returns the minimum time value to delay the transmission of a FindService entry.
   *
   * \return Minimum delay.
   */
  TimerDelay GetFindServiceInitialDelayMin() const override;
  /**
   * \brief Returns the maximum time value to delay the transmission of a FindService entry.
   *
   * \return Maximum delay.
   */
  TimerDelay GetFindServiceInitialDelayMax() const override;
  /**
   * \brief Returns the maximum number of transmissions of FindService entry.
   *
   * \return Maximum number of repetitions.
   */
  std::size_t GetFindServiceRepetitionsMax() const override;
  /**
   * \brief Returns the base delay for the transmission of FindService entry.
   *
   * \return Base delay.
   */
  TimerDelay GetFindServiceRepetitionsBaseDelay() const override;
  /**
   * \brief Transmits a FindService entry.
   */
  void SendFindService() override;
  /**
   * \brief Initiates the transmission of a SubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   */
  void SendSubscribeEventgroup(std::uint16_t eventgroup_id, bool request_initial_events) override;
  /**
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void SendStopSubscribeEventgroup(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupSubscribed(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupUnsubscribed(std::uint16_t eventgroup_id) override;
  /**
   * \brief Called when a SOME/IP eventgroup subscription is pending.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  void OnEventgroupSubscriptionPending(std::uint16_t eventgroup_id) override;

 private:
  /**
   * \brief A container of received offers.
   */
  using ServiceDiscoveryClientOfferContainer = std::vector<ServiceDiscoveryClientOffer>;
  /**
   * \brief Called on reception of a matching OfferService message.
   *
   * \param offer Represents a complete OfferService message.
   */
  void OnOfferMatch(ServiceDiscoveryClientOffer offer);
  /**
   * \brief Searches for matching OfferService entries and their corresponding options in a single received SOME/IP SD
   * message.
   *
   * \param from_address The IP address of the sending service discovery.
   * \param from_port The UDP port of the sending service discovery.
   * \param entries A container of entries contained in a received SOME/IP SD message.
   * \param options A container of options contained in a received SOME/IP SD message.
   * \param is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   * \return A container of all matching offers together with their description.
   */
  ServiceDiscoveryClientOfferContainer FindOffers(const std::string& from_address, std::uint16_t from_port,
                                                  const ServiceDiscoveryEntryContainer& entries,
                                                  const ServiceDiscoveryOptionContainer& options, bool is_multicast);
  /**
   * \brief Processes all SubscribeEventgroupAck and StopSubscribeEventgroup entries contained in a single received
   * SOME/IP SD message.
   *
   * \param from_address The IP address of the sending service discovery.
   * \param from_port The UDP port of the sending service discovery.
   * \param entries A container of entries contained in a received SOME/IP SD message.
   * \param options A container of options contained in a received SOME/IP SD message.
   */
  void ProcessEventgroupSubscribeAcksAndNacks(const std::string& from_address, std::uint16_t from_port,
                                              const ServiceDiscoveryEntryContainer& entries,
                                              const ServiceDiscoveryOptionContainer& options);
  /**
   * \brief Initiates connection establishment to a remote service provider found through a received offer.
   */
  void Connect();
  /**
   * \brief Terminates connection to a remote service provider.
   */
  void Disconnect();
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
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A service instance responsible for communication handling.
   */
  std::shared_ptr<ServiceDiscoveryClientServiceInstance> service_instance_;
  /**
   * \brief The configuration of the required service instance.
   */
  const configuration::Configuration::RequiredServiceInstance* instance_config_;
  /**
   * \brief A state machine responsible for finding a matching service provider.
   */
  find_service::ServiceDiscoveryClientStateMachine state_machine_;
  /**
   * \brief A state machine responsible for SOME/IP eventgroup subscriptions.
   */
  eventgroup::ServiceDiscoveryClientEventgroupManager eventgroup_manager_;
  /**
   * \brief Currently active service offer.
   */
  ServiceDiscoveryClientOffer active_offer_;
  /**
   * \brief A builder for SOME/IP SD messages.
   */
  ServiceDiscoveryClientMessageBuilder message_builder_;
  /**
   * \brief Indicates how many times the service has been requested.
   */
  std::atomic<std::size_t> requested_counter_;
  /**
   * \brief Indicates whether the service is available.
   */
  bool is_available_;
  /**
   * \brief ServiceDiscoveryClient Context, that receives event subscription state updates.
   */
  ServiceDiscoveryClientContext* context_;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_H_
