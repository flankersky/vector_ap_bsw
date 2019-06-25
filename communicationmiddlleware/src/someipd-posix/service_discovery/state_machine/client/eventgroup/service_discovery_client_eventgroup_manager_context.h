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
/**        \file  service_discovery_client_eventgroup_manager_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_CONTEXT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief A Service Discovery Client eventgroup manager context.
 */
class ServiceDiscoveryClientEventgroupManagerContext {
 public:
  /**
   * \brief Destructor of ServiceDiscoveryClientEventgroupManagerContext.
   */
  virtual ~ServiceDiscoveryClientEventgroupManagerContext() = default;
  /**
   * \brief Initiates the transmission of a SubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   */
  virtual void SendSubscribeEventgroup(std::uint16_t eventgroup_id, bool request_initial_events) = 0;
  /**
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry for the given SOME/IP eventgroup.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void SendStopSubscribeEventgroup(std::uint16_t eventgroup_id) = 0;
  /**
   * \brief Called when a SOME/IP eventgroup has been subscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void OnEventgroupSubscribed(std::uint16_t eventgroup_id) = 0;
  /**
   * \brief Called when a SOME/IP eventgroup has been unsubscribed.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void OnEventgroupUnsubscribed(std::uint16_t eventgroup_id) = 0;
  /**
   * \brief Called when a SOME/IP eventgroup subscription is pending.
   *
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   */
  virtual void OnEventgroupSubscriptionPending(std::uint16_t eventgroup_id) = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_MANAGER_CONTEXT_H_
