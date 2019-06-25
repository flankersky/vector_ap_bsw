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
/**        \file  service_discovery_server_context.h
 *        \brief  Service Discovery Server State Machine Context
 *
 *      \details  Context class contains all required interfaces to be called by any state machine state
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_message_sender.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "someipd-posix/service_discovery/service_discovery_server_service_instance.h"
#include "someipd-posix/service_discovery/state_machine/server/eventgroup/service_discovery_server_event_manager.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_offer_service_send_queue.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state_owner.h"
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief ServiceDiscoveryServerContext Implementation of ServiceDiscoveryServerContextInterface
 */
class ServiceDiscoveryServerContext : public ServiceDiscoveryServerContextInterface {
 public:
  /**
   * \brief Constructor
   *
   * \param state_owner StateOwner pointer
   * \param address The IP address of this server service instance.
   * \param config Provided Service configuration.
   * \param service_instance A server service instance.
   * \param event_manager An event manager
   * \param message_builder An SD Server message builder.
   * \param message_sender A message sender.
   * \param message_send_queue Message send queue.
   */
  explicit ServiceDiscoveryServerContext(ServiceDiscoveryServerStateOwner& state_owner, const std::string& address,
                                         const configuration::Configuration::ProvidedServiceInstance* config,
                                         std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance,
                                         eventgroup::ServiceDiscoveryServerEventManager& event_manager,
                                         ServiceDiscoveryServerMessageBuilder& message_builder,
                                         ServiceDiscoveryMessageSender& message_sender,
                                         ServiceDiscoveryServerOfferServiceSendQueue& message_send_queue);

  /**
   * \brief Delete copy constructor.
   */
  ServiceDiscoveryServerContext(const ServiceDiscoveryServerContext&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryServerContext& operator=(const ServiceDiscoveryServerContext&) = delete;

  /**
   * \brief Checks the current network state
   *
   * \return true of network is up, false otherwise
   */
  bool IsNetworkUp() const override;

  /**
   * \brief Checks the current service state
   *
   * \return true of service is requested, false if released
   */
  bool IsServiceUp() const override;

  /**
   * \brief request a state change
   *
   * \param handle The handle for the requested next state.
   */
  void RequestStateChange(const ServiceDiscoveryServerStateHandle handle) override;

  /**
   * \brief return the requested next state set by \see RequestStateChange
   *
   * \return the requested next state
   */
  ServiceDiscoveryServerStateHandle GetRequestedNextState() const override;

  /**
   * \brief Start a timer with the given period.
   *
   * \param period timer period.
   * \param periodic true if the timer runs periodically, false if the timer should start as one shot.
   */
  void StartTimer(vac::timer::Timer::Clock::duration period, bool periodic) override;

  /**
   * \brief Stop the currently running timer.
   */
  void StopTimer() override;

  /**
   * \brief gets the configuration minimum delay of initial FindService entry within Initial Wait Phase.
   *
   * \return The minimum initial wait delay duration.
   */
  vac::timer::Timer::Clock::duration GetInitialDelayMin() const override;

  /**
   * \brief gets the configuration maximum delay of initial FindService entry within Initial Wait Phase.
   *
   * \return The maximum initial wait delay duration.
   */
  vac::timer::Timer::Clock::duration GetInitialDelayMax() const override;

  /**
   * \brief calculates a random value between [InitialDelayTimeMin, InitialDelayTimeMax] and returns it.
   *
   * \return A random value for initial wait delay duration.
   */
  vac::timer::Timer::Clock::duration GetInitialDelayRandom() const override;

  /**
   * \brief Gets the configuration repetition base period used to sending OfferService packets within Repetition
   * Phase.
   *
   * \return The repetition base period in nanoseconds.
   */
  vac::timer::Timer::Clock::duration GetInitialRepetitionBaseDelay() const override;

  /**
   * \brief Gets the configuration maximum number of repetitions in Repetition Phase.
   *
   * \return The configuration maximum number of repetitions.
   */
  std::uint32_t GetInitialRepetitionMaxCount() const override;

  /**
   * \brief Gets the configuration cycle period for sending OfferService packets within Main Phase.
   *
   * \return The cycle OfferService period.
   */
  vac::timer::Timer::Clock::duration GetCyclicOfferServiceDelay() const override;

  /**
   * \brief Gets the configuration Maximum delay of OfferService entry as a response to a multicast FindService entry
   *
   * \return The maximum request response delay.
   */
  vac::timer::Timer::Clock::duration GetRequestResponseDelayMax() const override;

  /**
   * \brief Gets the configuration Minimum delay of OfferService entry as a response to a multicast FindService entry
   *
   * \return The minimum request response delay.
   */
  vac::timer::Timer::Clock::duration GetRequestResponseDelayMin() const override;

  /**
   * \brief calculates a random value between [RequestResponseDelayMin, RequestResponseDelayMax] and returns it
   *
   * \return A random value for request response delay duration
   */
  vac::timer::Timer::Clock::duration GetRequestResponseDelayRandom() const override;

  /**
   * \brief Starts a provided service instance.
   */
  void StartServiceInstance() override;

  /**
   * \brief Stops a provided service instance.
   */
  void StopServiceInstance() override;

  /**
   * \brief Schedules sending a multicast OfferService SOME/IP SD message after the specified delay.
   *
   * \param delay The delay after which the message should be sent.  If zero, the message is sent immediately.
   */
  void SendMulticastOfferServiceMessage(
      vac::timer::Timer::Clock::duration delay = vac::timer::Timer::Clock::duration::zero()) override;

  /**
   * \brief Schedules sending a unicast OfferService SOME/IP SD message after the specified delay.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   * \param delay The delay after which the message should be sent.  If zero, the message is sent immediately.
   */
  void SendUnicastOfferServiceMessage(
      const std::string& to_address, std::uint16_t to_port,
      vac::timer::Timer::Clock::duration delay = vac::timer::Timer::Clock::duration::zero()) override;

  /**
   * \brief Sends a multicast StopOfferService SOME/IP SD message.
   */
  void SendMulticastStopOfferServiceMessage() override;

  /**
   * \brief Sends a unicast StopOfferService SOME/IP SD message.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   */
  void SendUnicastStopOfferServiceMessage(const std::string& to_address, std::uint16_t to_port) override;

  /**
   * \brief Adds a new subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void SubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                const ServiceDiscoveryEntry& entry,
                                const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Removes a subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void UnsubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                  const ServiceDiscoveryEntry& entry,
                                  const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Removes all eventgroup subscribers.
   */
  void UnsubscribeAllEventgroupEntries() override;

 private:
  /**
   * \brief state owner
   */
  ServiceDiscoveryServerStateOwner& state_owner_;
  /**
   * \brief A server service instance.
   */
  std::shared_ptr<ServiceDiscoveryServerServiceInstance> service_instance_;
  /**
   * \brief The corresponding configuration of this server service instance.
   */
  const configuration::Configuration::ProvidedServiceInstance* instance_config_;
  /**
   * \brief The corresponding port mapping of this server service instance.
   */
  const configuration::Configuration::NetworkEndpointPortMapping* port_mapping_config_;
  /**
   * \brief An event manager
   */
  eventgroup::ServiceDiscoveryServerEventManager& event_manager_;
  /**
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilder& message_builder_;
  /**
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSender& message_sender_;
  /**
   * \brief Message send queue for delayed messages
   */
  ServiceDiscoveryServerOfferServiceSendQueue& message_send_queue_;
  /**
   * \brief Requested next state.
   */
  ServiceDiscoveryServerStateHandle requested_next_state_{ServiceDiscoveryServerStateHandle::kDownPhase};
  /**
   * \brief calculated random value between [InitialDelayMin, InitialDelayMax].
   */
  vac::timer::Timer::Clock::duration initial_delay_random_;
  /**
   * \brief calculated random value between [RequestResponseDelayMin, RequestResponseDelayMax].
   */
  vac::timer::Timer::Clock::duration request_response_delay_random_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_H_
