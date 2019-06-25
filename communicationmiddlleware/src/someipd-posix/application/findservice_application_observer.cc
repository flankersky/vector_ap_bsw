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
/**        \file  findservice_application_observer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/findservice_application_observer.h"

namespace someipd_posix {
namespace application {

void FindServiceApplicationObserver::RegisterStartFindServiceJob(someip_posix_common::someip::ServiceId service_id,
                                                                 someip_posix_common::someip::InstanceId instance_id) {
  auto it = findservice_jobs_.find(service_id);
  if (it == findservice_jobs_.end()) {
    std::set<someip_posix_common::someip::InstanceId> instance_set;
    instance_set.emplace(instance_id);
    findservice_jobs_.emplace(service_id, instance_set);
  } else {
    it->second.emplace(instance_id);
  }
}

void FindServiceApplicationObserver::DeRegisterStartFindServiceJob(
    someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id) {
  auto it = findservice_jobs_.find(service_id);
  if (it != findservice_jobs_.end()) {
    it->second.erase(instance_id);
  }
}

FindServiceApplicationObserver::OptionalFindServiceSet FindServiceApplicationObserver::GetActiveJobsFor(
    someip_posix_common::someip::ServiceId service_id) {
  auto it = findservice_jobs_.find(service_id);
  OptionalFindServiceSet instances;

  if (it != findservice_jobs_.end()) {
    instances.emplace(it->second);
  }

  return instances;
}

}  // namespace application
}  // namespace someipd_posix
