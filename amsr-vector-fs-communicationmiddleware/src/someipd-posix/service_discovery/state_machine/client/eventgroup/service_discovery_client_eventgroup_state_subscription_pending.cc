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
/**        \file  service_discovery_client_eventgroup_state_subscription_pending.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_subscription_pending.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupStateSubscriptionPending::ServiceDiscoveryClientEventgroupStateSubscriptionPending()
    : ServiceDiscoveryClientEventgroupState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupStateSubscriptionPending", "")} {}

bool ServiceDiscoveryClientEventgroupStateSubscriptionPending::IsValidChange(
    const ServiceDiscoveryClientEventgroupStateHandle handle) const {
  return (handle == ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed) ||
         (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnEnter(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  // TODO(PAASR-2153)
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsServiceAvailable() && context.IsEventgroupRequested());
  context.OnSubscriptionPending();
  /* PRS_SOMEIPSD_00703 */
  context.SendSubscribeEventgroup(true);
  context.StartTimer(context.GetSubscribeEventgroupAckTimeout());
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnLeave(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnRequested(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnReleased(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.SendStopSubscribeEventgroup();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) {
  (void)context;
  (void)is_multicast;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnStopOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnSubscribeEventgroupAck(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.OnSubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnSubscribeEventgroupNack(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.SendSubscribeEventgroup(true);
  context.StartTimer(context.GetSubscribeEventgroupAckTimeout());
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnTtlTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionPending::OnShutdown(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
