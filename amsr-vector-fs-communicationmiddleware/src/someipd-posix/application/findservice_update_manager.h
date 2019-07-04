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
/**        \file  findservice_update_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_UPDATE_MANAGER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_UPDATE_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>

#include "ara/log/logging.hpp"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "someipd-posix/service_discovery/service_discovery_offer_service_observer.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace application {

// Forward declaration to avoid unnecessary include
class FindServiceApplicationObserver;

/**
 * \brief Is an observer of the service discovery and delegates incoming service discovery updates
 * to the findservice jobs of the registered applications.
 */
class FindServiceUpdateManager : public service_discovery::ServiceDiscoveryOfferServiceObserver {
 public:
  /**
   * \brief Default constructor
   */
  FindServiceUpdateManager();
  /**
   * \brief FindService observer pointer.
   */
  using FindServiceApplicationObserverPtr = std::shared_ptr<FindServiceApplicationObserver>;
  /**
  * \brief Holds the registered observers (applications), that want to be informed asynchronously
  * about service offers and stopped offers.
  */
  using FindServiceApplicationObserverContainer = std::vector<FindServiceApplicationObserverPtr>;
  /**
   * \brief Gets called on a service instance offer, to inform StartFindService requests.
   * Realizes the pure virtual method of the observer interface from the service discovery.
   *
   * \param service_id The SOME/IP identifier of the offered service.
   * \param instance_id The SOME/IP instance identifier of the offered service instance.
   */
  void OnStartOfferServiceInstance(const someip_posix_common::someip::ServiceId service_id,
                                   const someip_posix_common::someip::InstanceId instance_id) override;
  /**
   * \brief Gets called on a stopped offer of a specific service instance.
   *  Realizes the pure virtual method of the observer interface from the service discovery.
   *
   * \param service_id The SOME/IP service identifier of the stopped service.
   * \param instance_id The SOME/IP instance identifier of the stopped service.
   */
  void OnStopOfferServiceInstance(const someip_posix_common::someip::ServiceId service_id,
                                  const someip_posix_common::someip::InstanceId instance_id) override;

  /**
   * \brief Add a client to the watch list, after the client requests asynchronous notification.
   *
   * \param application The observer that wants to be notified on every status update.
   * \param service_id The service identifier to watch.
   * \param instance_id The instance identifier to watch.
   */
  void AddFindServiceJob(FindServiceApplicationObserverPtr application,
                         someip_posix_common::someip::ServiceId service_id,
                         someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Delete a StartFindService job from one specific application.
   *
   * \param application The observer that wants to stop any notification about service updates.
   * \param service_id The service identifier to stop
   * \param instance_id The instance identifier to stop.
   */
  void DeleteFindServiceJob(FindServiceApplicationObserverPtr application,
                            someip_posix_common::someip::ServiceId service_id,
                            someip_posix_common::someip::InstanceId instance_id);
  /**
   * \brief Deletes one service discovery client completely.
   *
   * \param application The observer that wants to stop any notification about service updates.
   */
  void DeleteOfferServiceObserver(FindServiceApplicationObserverPtr application);

 private:
  /**
   * \brief Container of registered service discovery observers, that want to be informed
   * about service offers and stopped offers.
   */
  FindServiceApplicationObserverContainer discovery_observers_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_FINDSERVICE_UPDATE_MANAGER_H_
