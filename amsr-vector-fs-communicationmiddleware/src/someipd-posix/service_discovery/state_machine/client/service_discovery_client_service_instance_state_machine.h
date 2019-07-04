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
/**        \file  service_discovery_client_service_instance_state_machine.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_STATE_MACHINE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_STATE_MACHINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"
#include "someipd-posix/service_discovery/state_machine/service_discovery_service_instance_state_machine.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

/**
 * \brief ServiceDiscoveryClientServiceInstanceStateMachine abstract class.
 */
class ServiceDiscoveryClientServiceInstanceStateMachine : public ServiceDiscoveryServiceInstanceStateMachine {
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
  ServiceDiscoveryClientServiceInstanceStateMachine(std::uint16_t service_id, std::uint16_t instance_id,
                                                    std::uint8_t major_version, std::uint32_t minor_version,
                                                    vac::timer::TimerManager* timer_manager,
                                                    ServiceDiscoveryMessageSender* message_sender)
      : ServiceDiscoveryServiceInstanceStateMachine(service_id, instance_id, major_version, minor_version,
                                                    timer_manager, message_sender) {}

  /**
   * \brief Destructor.
   */
  virtual ~ServiceDiscoveryClientServiceInstanceStateMachine() = default;

  /**
   * \brief Called when a required service instance is requested.
   */
  virtual void OnServiceRequested() = 0;

  /**
   * \brief Called when a required service instance is released.
   */
  virtual void OnServiceReleased() = 0;

  /**
   * \brief Called on eventgroup subscription.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void OnSubscribeEventgroup(std::uint16_t eventgroup_id) = 0;

  /**
   * \brief Called on eventgroup subscription cancellation.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void OnStopSubscribeEventgroup(std::uint16_t eventgroup_id) = 0;

  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current event group subscription state.
   */
  virtual ServiceDiscoveryEventSubscriptionState GetEventgroupSubscriptionState(std::uint16_t eventgroup_id) const = 0;

  /**
   * \brief Gets the current subscription state of the given eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   *
   * \return Current eventgroup subscription state.
   */
  virtual std::size_t GetEventgroupSubscribersCount(std::uint16_t eventgroup_id) const = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_SERVICE_INSTANCE_STATE_MACHINE_H_
