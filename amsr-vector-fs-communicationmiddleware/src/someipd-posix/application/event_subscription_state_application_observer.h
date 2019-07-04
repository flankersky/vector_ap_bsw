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
/**        \file  event_subscription_state_application_observer.h
 *        \brief  Used for notification on event subscription state changes.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_APPLICATION_OBSERVER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_APPLICATION_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/someip_posix_types.h"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state.h"

namespace someipd_posix {
namespace application {

/**
 * \brief An abstract observer-pattern base class for the notification of event subscription state changes.
 * This decouples a concrete observer-type from the ServiceDiscovery.
 */
class EventSubscriptionStateApplicationObserver {
 public:
  virtual ~EventSubscriptionStateApplicationObserver() = default;
  /**
   * \brief Notify about event subscription state change, in case an event subscription is done by the client.
   * The observer itself shall implement this pure virtual method.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param event_id The event identifier of the event with the subscription state change.
   * \param new_state The new subscription state for this event.
   */
  virtual void NotifyEventSubscriptionStateChange(
      const someip_posix_common::someip::ServiceId service_id,
      const someip_posix_common::someip::InstanceId instance_id, const someip_posix_common::someip::EventId event_id,
      const service_discovery::ServiceDiscoveryEventSubscriptionState new_state) = 0;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_APPLICATION_OBSERVER_H_
