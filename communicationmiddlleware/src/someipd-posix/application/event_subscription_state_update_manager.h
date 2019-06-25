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
/**        \file  event_subscription_state_update_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_UPDATE_MANAGER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_UPDATE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>
#include "ara/log/logging.hpp"
#include "someipd-posix/application/event_subscription_state_application_observer.h"
#include "someipd-posix/service_discovery/service_discovery_event_subscription_state_observer.h"

namespace someipd_posix {
namespace application {

/**
 * \brief An observer of the service discovery and delegates incoming service discovery event subscription state updates
 * to the registered observers.
 */
class EventSubscriptionStateUpdateManager : public service_discovery::ServiceDiscoveryEventSubscriptionStateObserver {
 public:
  /**
   * \brief Default constructor
   */
  EventSubscriptionStateUpdateManager();
  /**
   * \brief Event subscription state observer.
   */
  using EventSubscriptionStateObserverPtr = std::shared_ptr<EventSubscriptionStateApplicationObserver>;
  /**
  * \brief Holds the registered observers, that want to be informed asynchronously about event subscription state
  * change.
  */
  using ApplicationsWithEventSubscriptions = std::vector<EventSubscriptionStateObserverPtr>;
  /**
   * \brief Gets called on an event subscription state change.
   * Realizes the pure virtual method of the observer interface from the service discovery.
   *
   * \param service_id The SOME/IP identifier of the service with the subscribed event.
   * \param instance_id The SOME/IP instance identifier of the service instance with the subscribed event.
   * \param event_id The subscribed event identifier with the event subscription change.
   * \param new_state Current event subscription state.
   */
  void OnEventSubscriptionStateChange(
      const someip_posix_common::someip::ServiceId service_id,
      const someip_posix_common::someip::InstanceId instance_id, const someip_posix_common::someip::EventId event_id,
      const service_discovery::ServiceDiscoveryEventSubscriptionState new_state) override;

  /**
   * \brief Add a client to the event subscription state watch list.
   *
   * \param application The observer that wants to be notified on every event subscription state updates.
   */
  void AddEventSubscriptionObserver(EventSubscriptionStateObserverPtr application);
  /**
   * \brief Remove an observer from the event subscription state watch list.
   *
   * \param application The observer that wants to stop any notification about event subscription state updates.
   */
  void DeleteEventSubscriptionObserver(EventSubscriptionStateObserverPtr application);

 private:
  /**
   * \brief Container of registered service discovery observers, that want to be informed
   * about event subscription state changes.
   */
  ApplicationsWithEventSubscriptions event_subscription_observers_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_EVENT_SUBSCRIPTION_STATE_UPDATE_MANAGER_H_
