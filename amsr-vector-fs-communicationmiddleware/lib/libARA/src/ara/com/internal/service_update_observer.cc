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
/**        \file  service_update_observer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/service_update_observer.h"
#include <cassert>

namespace ara {
namespace com {
namespace internal {

ara::com::FindServiceHandle ServiceUpdateObserver::AddObserver(ara::com::internal::ProxyId proxy_id,
                                                               ara::com::InstanceIdentifier instance_id,
                                                               ApplicationHandler handler) {
  // Protect concurrent modification of active_observers_ container.
  // Required in case of concurrent StartFindService / StopFindService calls from multiple threads
  std::lock_guard<std::mutex> guard(active_observers_lock_);

  // Current length of the map works as ID for multiple StartFindService jobs per ServiceProxy.
  const std::uint32_t uid = static_cast<std::uint32_t>(active_observers_.size());
  const auto ret =
      active_observers_.emplace(ara::com::FindServiceHandle{proxy_id, instance_id.GetInstanceId(), uid}, handler);
  assert(ret.second == true);
  return ret.first->first;
}

void ServiceUpdateObserver::DeleteObserver(const ara::com::FindServiceHandle& handle) {
  // Protect concurrent modification of active_observers_ container.
  // Required in case of concurrent StartFindService / StopFindService calls from multiple threads
  std::lock_guard<std::mutex> guard(active_observers_lock_);

  active_observers_.erase(handle);
}

void ServiceUpdateObserver::Clear() {
  // Protect concurrent modification of active_observers_ container.
  // Required in case of concurrent StartFindService / StopFindService calls from multiple threads
  std::lock_guard<std::mutex> guard(active_observers_lock_);
  active_observers_.clear();
}

}  // namespace internal
}  // namespace com
}  // namespace ara
