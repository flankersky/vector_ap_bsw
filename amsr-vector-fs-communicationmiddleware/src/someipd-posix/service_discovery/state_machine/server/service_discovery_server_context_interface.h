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
/**        \file  service_discovery_server_context_interface.h
 *        \brief  Service Discovery Server State Machine Context Interface
 *
 *      \details  Service Discovery Server State Machine Context Interface
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>
#include <vector>
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

enum class ServiceDiscoveryServerStateHandle;

/**
 * \brief ServiceDiscoveryServerInterface interface for SD server state machine context.
 * The state machine context implementation must provide all functionalities required by different state machine states.
 */
class ServiceDiscoveryServerContextInterface {
 public:
  virtual ~ServiceDiscoveryServerContextInterface() = default;

  /**
   * \brief Checks the current network state
   *
   * \return true of network is up, false otherwise
   */
  virtual bool IsNetworkUp() const = 0;

  /**
   * \brief Checks the current service state
   *
   * \return true of service is requested, false if released
   */
  virtual bool IsServiceUp() const = 0;

  /**
   * \brief request a state change
   *
   * \param handle The handle for the requested next state.
   */
  virtual void RequestStateChange(const ServiceDiscoveryServerStateHandle handle) = 0;

  /**
   * \brief Gets the requested next state set by \see RequestStateChange
   *
   * \return The requested next state
   */
  virtual ServiceDiscoveryServerStateHandle GetRequestedNextState() const = 0;

  /**
   * \brief Start a timer with the given period.
   *
   * \param period Timer period.
   * \param periodic True if the timer runs periodically, false if the timer should start as one shot.
   */
  virtual void StartTimer(vac::timer::Timer::Clock::duration period, bool periodic) = 0;

  /**
   * \brief Stop the currently running timer.
   */
  virtual void StopTimer() = 0;

  /**
   * \brief gets the configuration minimum delay of initial FindService entry within Initial Wait Phase.
   *
   * \return The minimum initial wait delay duration.
   */
  virtual vac::timer::Timer::Clock::duration GetInitialDelayMin() const = 0;

  /**
   * \brief gets the configuration maximum delay of initial FindService entry within Initial Wait Phase.
   *
   * \return The maximum initial wait delay duration.
   */
  virtual vac::timer::Timer::Clock::duration GetInitialDelayMax() const = 0;

  /**
   * \brief calculates a random value between [InitialDelayTimeMin, InitialDelayTimeMax] and returns it
   *
   * \return A random value for initial wait delay duration.
   */
  virtual vac::timer::Timer::Clock::duration GetInitialDelayRandom() const = 0;

  /**
   * \brief Gets the configuration repetition base period used to sending OfferService packets within Repetition Phase.
   *
   * \return The repetition base period.
   */
  virtual vac::timer::Timer::Clock::duration GetInitialRepetitionBaseDelay() const = 0;

  /**
   * \brief Gets the configuration maximum number of repetitions in Repetition Phase.
   *
   * \return The configuration maximum number of repetitions.
   */
  virtual std::uint32_t GetInitialRepetitionMaxCount() const = 0;

  /**
   * \brief Gets the configuration cycle period for sending OfferService packets within Main Phase.
   *
   * \return The cycle OfferService period.
   */
  virtual vac::timer::Timer::Clock::duration GetCyclicOfferServiceDelay() const = 0;

  /**
   * \brief Gets the configuration Maximum delay of OfferService entry as a response to a multicast FindService entry
   *
   * \return The maximum request response delay.
   */
  virtual vac::timer::Timer::Clock::duration GetRequestResponseDelayMax() const = 0;

  /**
   * \brief Gets the configuration Minimum delay of OfferService entry as a response to a multicast FindService entry
   *
   * \return The minimum request response delay.
   */
  virtual vac::timer::Timer::Clock::duration GetRequestResponseDelayMin() const = 0;

  /**
   * \brief Calculates a random value between [RequestResponseDelayMin, RequestResponseDelayMax] and returns it
   *
   * \return A random value for request response delay duration.
   */
  virtual vac::timer::Timer::Clock::duration GetRequestResponseDelayRandom() const = 0;

  /**
   * \brief Starts a provided service instance.
   */
  virtual void StartServiceInstance() = 0;

  /**
   * \brief Stops a provided service instance.
   */
  virtual void StopServiceInstance() = 0;

  /**
   * \brief Schedules sending a multicast OfferService SOME/IP SD message after the specified delay.
   *
   * \param delay The delay after which the message should be sent.  If zero, the message is sent immediately.
   */
  virtual void SendMulticastOfferServiceMessage(
      vac::timer::Timer::Clock::duration delay = vac::timer::Timer::Clock::duration::zero()) = 0;

  /**
   * \brief Schedules sending a unicast OfferService SOME/IP SD message after the specified delay.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   * \param delay The delay after which the message should be sent.  If zero, the message is sent immediately.
   */
  virtual void SendUnicastOfferServiceMessage(
      const std::string& to_address, std::uint16_t to_port,
      vac::timer::Timer::Clock::duration delay = vac::timer::Timer::Clock::duration::zero()) = 0;

  /**
   * \brief Sends a multicast StopOfferService SOME/IP SD message.
   */
  virtual void SendMulticastStopOfferServiceMessage() = 0;

  /**
   * \brief Sends a unicast StopOfferService SOME/IP SD message.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   */
  virtual void SendUnicastStopOfferServiceMessage(const std::string& to_address, std::uint16_t to_port) = 0;

  /**
   * \brief Adds a new subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  virtual void SubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                        const ServiceDiscoveryEntry& entry,
                                        const ServiceDiscoveryOptionContainer& options) = 0;

  /**
   * \brief Removes a subscriber for a given eventgroup.
   *
   * \param from_address An IP address identifying the subscriber.
   * \param from_port A port identifying the subscriber.
   * \param entry A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  virtual void UnsubscribeEventgroupEntry(const std::string& from_address, std::uint16_t from_port,
                                          const ServiceDiscoveryEntry& entry,
                                          const ServiceDiscoveryOptionContainer& options) = 0;

  /**
   * \brief Removes all eventgroup subscribers.
   */
  virtual void UnsubscribeAllEventgroupEntries() = 0;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_CONTEXT_INTERFACE_H_
