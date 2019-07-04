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
/**        \file  service_discovery_event_subscription_state_observer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_OBSERVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief This interface provides a pure virtual method to inform a concrete observer on event subscriptions state
 * changes.
 */
class ServiceDiscoveryEventSubscriptionStateObserver {
 public:
  virtual ~ServiceDiscoveryEventSubscriptionStateObserver() = default;

  /**
   * \brief Gets called on an event subscription state change. Pure virtual method shall be implemented by a concrete
   * observer.
   *
   * \param service_id The SOME/IP identifier of the service with the subscribed event.
   * \param instance_id The SOME/IP instance identifier of the service instance with the subscribed event.
   * \param event_id The subscribed event identifier with the event subscription change.
   * \param new_state Current event subscription state.
   */
  virtual void OnEventSubscriptionStateChange(const std::uint16_t service_id, const std::uint16_t instance_id,
                                              const std::uint16_t event_id,
                                              const ServiceDiscoveryEventSubscriptionState new_state) = 0;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_OBSERVER_H_
