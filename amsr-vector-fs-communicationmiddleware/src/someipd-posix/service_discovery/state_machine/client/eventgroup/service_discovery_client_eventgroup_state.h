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
/**        \file  service_discovery_client_eventgroup_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_context.h"
#include "vac/statemachine/state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief Service Discovery Client eventgroup states.
 */
enum class ServiceDiscoveryClientEventgroupStateHandle {
  kNotSubscribed = 0,        ///< 'NOT_SUBSCRIBED' state
  kSubscriptionPending = 1,  ///< 'SUBSCRIPTION_PENDING' state
  kSubscribed = 2,           ///< 'SUBSCRIBED' state
  kSubscriptionRenewal = 3,  ///< 'SUBSCRIPTION_RENEWAL' state
};

/**
 * \brief A Service Discovery Client eventgroup state.
 */
class ServiceDiscoveryClientEventgroupState
    : public vac::statemachine::State<ServiceDiscoveryClientEventgroupStateHandle,
                                      ServiceDiscoveryClientEventgroupStateContext> {
 public:
  using vac::statemachine::State<ServiceDiscoveryClientEventgroupStateHandle,
                                 ServiceDiscoveryClientEventgroupStateContext>::State;
  /**
   * \brief Destructor of ServiceDiscoveryClientEventgroupState.
   */
  virtual ~ServiceDiscoveryClientEventgroupState() = default;
  /**
   * \brief Called when the SOME/IP eventgroup has been requested.
   *
   * \param context A state context.
   */
  virtual void OnRequested(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when the SOME/IP eventgroup has been released.
   *
   * \param context A state context.
   */
  virtual void OnReleased(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param context A state context.
   * \param is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   */
  virtual void OnOfferService(ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) = 0;
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param context A state context.
   */
  virtual void OnStopOfferService(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param context A state context.
   */
  virtual void OnSubscribeEventgroupAck(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   *
   * \param context A state context.
   */
  virtual void OnSubscribeEventgroupNack(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when a previously scheduled timer delay expires.
   *
   * \param context A state context.
   */
  virtual void OnTimeout(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when a previously scheduled TTL timer delay expires.
   *
   * \param context A state context.
   */
  virtual void OnTtlTimeout(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
  /**
   * \brief Called when the eventgroup shall be shut down.
   *
   * \param context A state context.
   */
  virtual void OnShutdown(ServiceDiscoveryClientEventgroupStateContext& context) = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_H_
