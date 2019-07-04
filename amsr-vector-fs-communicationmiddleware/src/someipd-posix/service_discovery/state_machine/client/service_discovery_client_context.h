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
/**        \file  service_discovery_client_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_CONTEXT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

/**
 * \brief An observer-pattern interface for the notifications of eventgroup subscription state updates.
 */
class ServiceDiscoveryClientContext {
 public:
  virtual ~ServiceDiscoveryClientContext() = default;
  /**
   * \brief Called when an eventgroup is subscribed.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  virtual void OnEventgroupSubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                                      const std::uint16_t eventgroup_id) = 0;
  /**
   * \brief Called when an eventgroup is unsubscribed.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  virtual void OnEventgroupUnsubscribed(const std::uint16_t service_id, const std::uint16_t instance_id,
                                        const std::uint16_t eventgroup_id) = 0;
  /**
   * \brief Called when an eventgroup subscription is pending.
   *
   * \param service_id The service identifier of the service.
   * \param instance_id The instance identifier of the service.
   * \param eventgroup_id The eventgroup identifier.
   */
  virtual void OnEventgroupSubscriptionPending(const std::uint16_t service_id, const std::uint16_t instance_id,
                                               const std::uint16_t eventgroup_id) = 0;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_CONTEXT_H_
