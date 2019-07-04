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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_subscription_renewal.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::ServiceDiscoveryClientEventgroupStateSubscriptionRenewal()
    : ServiceDiscoveryClientEventgroupState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionRenewal),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupStateSubscriptionRenewal", "")} {}

bool ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::IsValidChange(
    const ServiceDiscoveryClientEventgroupStateHandle handle) const {
  return (handle == ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed) ||
         (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending) ||
         (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnEnter(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  // TODO(PAASR-2153)
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsServiceAvailable() && context.IsEventgroupRequested());
  context.SendSubscribeEventgroup(false);
  context.StartTimer(context.GetSubscribeEventgroupAckTimeout());
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnLeave(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnRequested(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnReleased(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StopTtlTimer();
  context.SendStopSubscribeEventgroup();
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) {
  (void)context;
  (void)is_multicast;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnStopOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StopTtlTimer();
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnSubscribeEventgroupAck(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnSubscribeEventgroupNack(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.SendSubscribeEventgroup(true);
  context.StartTimer(context.GetSubscribeEventgroupAckTimeout());
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnTtlTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsServiceAvailable() && context.IsEventgroupRequested());
  context.OnUnsubscribed();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending);
}

void ServiceDiscoveryClientEventgroupStateSubscriptionRenewal::OnShutdown(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StopTtlTimer();
  context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed);
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
