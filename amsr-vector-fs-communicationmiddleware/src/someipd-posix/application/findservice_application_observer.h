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
/**        \file  findservice_application_observer.h
 *        \brief  Used for notification on and management of active asynchronous StartFindService jobs.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_APPLICATION_OBSERVER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_APPLICATION_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <set>
#include "someip-posix-common/someip/someip_posix_types.h"
#include "vac/memory/optional.h"

namespace someipd_posix {

namespace service_discovery {
// Forward declaration to prevent include here.
class ServiceDiscovery;
}

namespace application {

/**
 * \brief An abstract observer-pattern base class for the notification of service updates.
 * This decouples a concrete observer-type from the ServiceDiscovery.
 */
class FindServiceApplicationObserver {
 public:
  virtual ~FindServiceApplicationObserver() {}
  /**
   * \brief Notify about a service update (offer),
   * in case an asynchronous StartFindService() is requested by the client.
   * \remark This method is called by the ServiceDiscovery per application
   * and the updated service instance once. The observer itself shall implement
   * this pure virtual method.
   *
   * \param service_id The Service identifier which has been offered.
   * \param instance_id The instance identifier from the StartFindService call.
   */
  virtual void NotifyOfferService(const someip_posix_common::someip::ServiceId service_id,
                                  const someip_posix_common::someip::InstanceId instance_id) = 0;

  /**
   * \brief Notify about a service update (stopped offer),
   * in case an asynchronous StartFindService() is requested by the client.
   * \remark This method is called by the ServiceDiscovery per application and
   * the updated service instance once.
   *
   * \param service_id The service identifier of the stopped service.
   * \param instance_id The instance identifier of the stopped service.
   */
  virtual void NotifyStopOfferService(const someip_posix_common::someip::ServiceId service_id,
                                      const someip_posix_common::someip::InstanceId instance_id) = 0;

  /**
   * \brief Register an asynchronous listener job for StartFindService
   *
   * Possible scenario:
   *
   * StartFindServiceJob(sid = 1, instance_id = any)
   * or
   * StartFindServiceJob(sid = 1, instance_id = 3)
   *
   * The map is grouped into service IDs. Each key
   * represents a group of FindService requests.
   *
   * key: service ID = 1
   * |- instance ID = any
   * |- instance ID = 3
   * key: service ID = 4
   * |- instance ID = 3
   * |- instance ID = 5
   * |- instance ID = 9
   * key: service ID = 5
   * |- instance ID = any
   * |- instance ID = 99
   *
   * \param service_id The service identifier to look for.
   * \param instance_id The instance identifier to start looking for.
   */
  void RegisterStartFindServiceJob(someip_posix_common::someip::ServiceId service_id,
                                   someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Remove an active FindService job for a given service (key of map) and instance ID (value of map).
   *
   * \param service_id The service identifier to stop looking for.
   * \param instance_id The instance identifier to stop looking for.
   */
  void DeRegisterStartFindServiceJob(someip_posix_common::someip::ServiceId service_id,
                                     someip_posix_common::someip::InstanceId instance_id);

 protected:
  /// Type-alias for a set of service instance IDs an application searches for.
  using SearchedInstances = std::set<someip_posix_common::someip::InstanceId>;

  /// Type-alias for giving back a set of service instances on the notification of an offered service.
  using OptionalFindServiceSet = vac::memory::optional<std::reference_wrapper<SearchedInstances>>;

  /**
   * \brief Get all pending StartFindService jobs for one service identifier.
   *
   * Possible scenario:
   * First job: Service ID 1, Instance ID any (0xFFFF)
   * Second job: Service ID 1, Instance ID 4
   *
   * \param service_id The service identifier to search for active StartFindService requests.
   * \return A set of find service jobs for one SOME/IP service ID. Could be empty due to a service that is offered and
   * there are no background searches registered from this application.
   */
  OptionalFindServiceSet GetActiveJobsFor(someip_posix_common::someip::ServiceId service_id);

 private:
  /// FindService jobs for one application as a type
  /// Key: SOME/IP service identifier
  /// Value: Set of FindService jobs for this service identifier.
  using FindServiceJobsOfApplication = std::map<someip_posix_common::someip::ServiceId, SearchedInstances>;

  /// Pending FindService jobs of one client based on the service ID.
  /// The key is the service identifier.
  FindServiceJobsOfApplication findservice_jobs_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_APPLICATION_OBSERVER_H_
