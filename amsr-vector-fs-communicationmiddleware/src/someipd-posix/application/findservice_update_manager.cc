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
/**        \file  findservice_update_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/findservice_update_manager.h"

#include <algorithm>
#include "someipd-posix/application/findservice_application_observer.h"

namespace someipd_posix {
namespace application {

FindServiceUpdateManager::FindServiceUpdateManager()
    : logger_(ara::log::CreateLogger("FindServiceUpdateManager", "")) {}

void FindServiceUpdateManager::OnStartOfferServiceInstance(const someip_posix_common::someip::ServiceId service_id,
                                                           const someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << " - service ID: " << service_id << ", instance ID: " << instance_id;

  for (auto& observer : discovery_observers_) {
    observer->NotifyOfferService(service_id, instance_id);
  }
}

void FindServiceUpdateManager::OnStopOfferServiceInstance(const someip_posix_common::someip::ServiceId service_id,
                                                          const someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << " - service ID: " << service_id << ", instance ID: " << instance_id;

  for (auto& observer : discovery_observers_) {
    observer->NotifyStopOfferService(service_id, instance_id);
  }
}

void FindServiceUpdateManager::AddFindServiceJob(FindServiceApplicationObserverPtr application,
                                                 someip_posix_common::someip::ServiceId service_id,
                                                 someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << " - service ID: " << service_id << ", instance ID: " << instance_id;

  application->RegisterStartFindServiceJob(service_id, instance_id);
  // Register the application as observer, if not done already.
  const auto it = std::find(discovery_observers_.begin(), discovery_observers_.end(), application);
  if (it == discovery_observers_.end()) {
    // One application / client is only registered once as an observer.
    // Each observer may then have multiple StartFindService jobs pending.
    discovery_observers_.push_back(application);
  }
}

void FindServiceUpdateManager::DeleteFindServiceJob(FindServiceApplicationObserverPtr application,
                                                    someip_posix_common::someip::ServiceId service_id,
                                                    someip_posix_common::someip::InstanceId instance_id) {
  logger_.LogDebug() << __func__ << " - service ID: " << service_id << ", instance ID: " << instance_id;

  application->DeRegisterStartFindServiceJob(service_id, instance_id);
}

void FindServiceUpdateManager::DeleteOfferServiceObserver(FindServiceApplicationObserverPtr application) {
  logger_.LogDebug() << __func__;

  auto it = std::find(discovery_observers_.begin(), discovery_observers_.end(), application);
  if (it != discovery_observers_.end()) {
    discovery_observers_.erase(it);
  }
}

}  // namespace application
}  // namespace someipd_posix
