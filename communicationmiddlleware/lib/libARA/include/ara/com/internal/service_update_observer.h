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
/**        \file  service_update_observer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_UPDATE_OBSERVER_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_UPDATE_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <map>
#include <mutex>
#include "ara/com/handle_type.h"
#include "ara/com/internal/types.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief ServiceUpdateObserver has the purpose to manage asynchronous FindService jobs of one ServiceProxy.
 */
class ServiceUpdateObserver {
 public:
  /**
   * \brief Shortcut for the ara::com FindServiceHandler std::function type
   */
  using ApplicationHandler = FindServiceHandler<HandleType>;

  /**
   * \brief In theory, each ServiceProxy can request multiple StartFindService requests.
   * Thus we need a map to find the correct StartFindService job on a possible update.
   */
  using FindServiceUpdateJobs = std::map<ara::com::FindServiceHandle, ApplicationHandler>;

  /**
   * \brief Because one object of this type will reside statically in one ServiceProxy, the constructor shall not
   * contain any logic to test.
   */
  ServiceUpdateObserver() = default;

  /**
   * \brief Default destructor.
   */
  ~ServiceUpdateObserver() = default;

  /**
   * \brief Add an observer on StartFindService
   *
   * \param proxy_id The ara::com Proxy identifier.
   * \param instance_id The ara::com instance identifier.
   * \param handler The callback to store for later notification.
   *
   * \return A FindServiceHandle managed by this object, to identify and stop the correct FindService update job later
   * on.
   */
  ara::com::FindServiceHandle AddObserver(ara::com::internal::ProxyId proxy_id,
                                          ara::com::InstanceIdentifier instance_id, ApplicationHandler handler);

  /**
   * \brief Remove an observer on StopFindService
   */
  void DeleteObserver(const ara::com::FindServiceHandle& handle);

  /**
   * \brief Removes all active FindService jobs from this ServiceProxy.
   */
  void Clear();

  /**
   * \brief Is called from a concrete binding on service updates (offers / stopped offers).
   * This method in here doesn't contain any logic. "Filters" will be applied in the binding.
   *
   * \param handle The FindServiceHandle to trace back to the concrete StartFindService call that was made.
   * \param handles The container of handles which will be forwarded to the callback.
   */
  void OnUpdate(const ara::com::FindServiceHandle& handle,
                ara::com::ServiceHandleContainer<ara::com::HandleType> handles) {
    FindServiceUpdateJobs::iterator ret = active_observers_.find(handle);
    assert(ret != active_observers_.end());
    // Call the handler specified by the user and deliver a container of updated  service instance handles.
    // From here it acts the same as for the one-shot FindService variant (synchronous case).
    ret->second(handles);
  }

  /**
   * \brief Getter for a reference to the container with active asynchronous FindService jobs for a concrete
   * ServiceProxy.
   *
   * \return The container with active FindService jobs.
   */
  FindServiceUpdateJobs& GetObservers() { return active_observers_; }

 private:
  /// Current asynchronous FindService jobs for this ServiceProxy.
  FindServiceUpdateJobs active_observers_;

  /// Mutex to protect active_observers_ container
  std::mutex active_observers_lock_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_UPDATE_OBSERVER_H_
