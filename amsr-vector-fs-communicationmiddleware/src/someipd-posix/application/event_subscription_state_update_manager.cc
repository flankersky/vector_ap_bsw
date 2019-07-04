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
/**        \file  event_subscription_state_update_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/event_subscription_state_update_manager.h"
#include <algorithm>
#include "someipd-posix/application/event_subscription_state_application_observer.h"

namespace someipd_posix {
namespace application {

EventSubscriptionStateUpdateManager::EventSubscriptionStateUpdateManager()
    : logger_(ara::log::CreateLogger("EventSubscriptionStateUpdateManager", "")) {}

void EventSubscriptionStateUpdateManager::OnEventSubscriptionStateChange(
    const someip_posix_common::someip::ServiceId service_id, const someip_posix_common::someip::InstanceId instance_id,
    const someip_posix_common::someip::EventId event_id,
    const service_discovery::ServiceDiscoveryEventSubscriptionState new_state) {
  for (auto& observer : event_subscription_observers_) {
    observer->NotifyEventSubscriptionStateChange(service_id, instance_id, event_id, new_state);
  }
}

void EventSubscriptionStateUpdateManager::AddEventSubscriptionObserver(EventSubscriptionStateObserverPtr application) {
  logger_.LogDebug() << __func__;

  const auto it = std::find(event_subscription_observers_.begin(), event_subscription_observers_.end(), application);
  if (it == event_subscription_observers_.end()) {
    // One application / client is only registered once as an observer.
    // Each observer may then have multiple event subscriptions.
    event_subscription_observers_.push_back(application);
  }
}

void EventSubscriptionStateUpdateManager::DeleteEventSubscriptionObserver(
    EventSubscriptionStateObserverPtr application) {
  logger_.LogDebug() << __func__;

  auto it = std::find(event_subscription_observers_.begin(), event_subscription_observers_.end(), application);
  if (it != event_subscription_observers_.end()) {
    event_subscription_observers_.erase(it);
  }
}

}  // namespace application
}  // namespace someipd_posix
