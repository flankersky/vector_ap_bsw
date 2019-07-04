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
/**        \file  service_discovery_client_eventgroup_state_subscription_renewal.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_SUBSCRIPTION_RENEWAL_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_SUBSCRIPTION_RENEWAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief Service Discovery Client Eventgroup state 'SUBSCRIPTION_RENEWAL'.
 */
class ServiceDiscoveryClientEventgroupStateSubscriptionRenewal : public ServiceDiscoveryClientEventgroupState {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupStateSubscriptionRenewal.
   */
  ServiceDiscoveryClientEventgroupStateSubscriptionRenewal();
  /**
   * \brief Indicates whether a state change is valid or not.
   *
   * \param handle A state handle.
   * \return true if the state change is valid and false otherwise.
   */
  bool IsValidChange(const ServiceDiscoveryClientEventgroupStateHandle handle) const override;
  /**
   * \brief Called when the state is entered.
   *
   * \param context A state context.
   */
  void OnEnter(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when the state is left.
   *
   * \param context A state context.
   */
  void OnLeave(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when the SOME/IP eventgroup has been requested.
   *
   * \param context A state context.
   */
  void OnRequested(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when the SOME/IP eventgroup has been released.
   *
   * \param context A state context.
   */
  void OnReleased(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when a matching OfferService entry has been received.
   *
   * \param context A state context.
   * \param is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   */
  void OnOfferService(ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) override;
  /**
   * \brief Called when a matching StopOfferService entry has been received.
   *
   * \param context A state context.
   */
  void OnStopOfferService(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when a matching SubscribeEventgroupAck entry has been received.
   *
   * \param context A state context.
   */
  void OnSubscribeEventgroupAck(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when a matching SubscribeEventgroupNack entry has been received.
   *
   * \param context A state context.
   */
  void OnSubscribeEventgroupNack(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when a previously scheduled timer delay expires.
   *
   * \param context A state context.
   */
  void OnTimeout(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when a previously scheduled TTL timer delay expires.
   *
   * \param context A state context.
   */
  void OnTtlTimeout(ServiceDiscoveryClientEventgroupStateContext& context) override;
  /**
   * \brief Called when the eventgroup shall be shut down.
   *
   * \param context A state context.
   */
  void OnShutdown(ServiceDiscoveryClientEventgroupStateContext& context) override;

 private:
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_SUBSCRIPTION_RENEWAL_H_
