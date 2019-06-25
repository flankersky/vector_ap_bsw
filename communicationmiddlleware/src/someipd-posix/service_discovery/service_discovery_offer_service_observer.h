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
/**        \file  service_discovery_offer_service_observer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief This interface provides a pure virtual method to inform a concrete observer on service offers and
 * stopped offers.
 */
class ServiceDiscoveryOfferServiceObserver {
 public:
  virtual ~ServiceDiscoveryOfferServiceObserver() = default;

  /**
   * \brief Gets called on a service instance offer. Pure virtual method shall be
   * implemented by a concrete observer.
   *
   * \param service_id The offered service SOME/IP identifier.
   * \param instance_id The SOME/IP instance identifier of the service.
   */
  virtual void OnStartOfferServiceInstance(const std::uint16_t service_id, const std::uint16_t instance_id) = 0;
  /**
   * \brief Gets called on a stopped offer of a specific service instance. Pure virtual method shall be
   * implemented by a concrete observer.
   *
   * \param service_id The stopped service SOME/IP identifier.
   * \param instance_id The SOME/IP instance identifier of the stopped service.
   */
  virtual void OnStopOfferServiceInstance(const std::uint16_t service_id, const std::uint16_t instance_id) = 0;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_OFFER_SERVICE_OBSERVER_H_
