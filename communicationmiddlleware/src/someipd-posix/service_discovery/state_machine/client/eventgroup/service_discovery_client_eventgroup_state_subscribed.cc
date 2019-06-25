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
/**        \file  service_discovery_client_eventgroup_state_subscribed.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_subscribed.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupStateSubscribed::ServiceDiscoveryClientEventgroupStateSubscribed()
    : ServiceDiscoveryClientEventgroupState(ServiceDiscoveryClientEventgroupStateHandle::kSubscribed),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupStateSubscribed", "")} {}

bool ServiceDiscoveryClientEventgroupStateSubscribed::IsValidChange(
    const ServiceDiscoveryClientEventgroupStateHandle handle) const {
  return (handle == ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed) ||
         (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending) ||
         (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnEnter(ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsServiceAvailable() && context.IsEventgroupRequested());
  context.StartTtlTimer(context.GetSubscriptionTtl());
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnLeave(ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnRequested(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnReleased(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTtlTimer();
  /* PRS_SOMEIPSD_00431, PRS_SOMEIPSD_00452 */
  context.SendStopSubscribeEventgroup();
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) {
  (void)is_multicast;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  /* PRS_SOMEIPSD_00446, PRS_SOMEIPSD_00449, PRS_SOMEIPSD_00582 */
  /* DO NOT STOP TTL TIMER HERE */
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnStopOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTtlTimer();
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnSubscribeEventgroupAck(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnSubscribeEventgroupNack(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnTimeout(ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnTtlTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsServiceAvailable() && context.IsEventgroupRequested());
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending);
}

void ServiceDiscoveryClientEventgroupStateSubscribed::OnShutdown(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTtlTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
