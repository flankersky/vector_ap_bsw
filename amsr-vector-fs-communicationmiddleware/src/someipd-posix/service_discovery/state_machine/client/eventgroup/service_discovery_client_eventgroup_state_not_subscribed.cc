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
/**        \file  service_discovery_client_eventgroup_state_not_subscribed.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_not_subscribed.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

ServiceDiscoveryClientEventgroupStateNotSubscribed::ServiceDiscoveryClientEventgroupStateNotSubscribed()
    : ServiceDiscoveryClientEventgroupState(ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientEventgroupStateNotSubscribed", "")} {}

bool ServiceDiscoveryClientEventgroupStateNotSubscribed::IsValidChange(
    const ServiceDiscoveryClientEventgroupStateHandle handle) const {
  return (handle == ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending);
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnEnter(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StopTtlTimer();
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnLeave(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnRequested(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (context.IsServiceAvailable()) {
    context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending);
  }
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnReleased(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context, bool is_multicast) {
  (void)is_multicast;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (context.IsEventgroupRequested()) {
    context.ChangeState(ServiceDiscoveryClientEventgroupStateHandle::kSubscriptionPending);
  }
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnStopOfferService(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnSubscribeEventgroupAck(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnSubscribeEventgroupNack(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnTtlTimeout(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void ServiceDiscoveryClientEventgroupStateNotSubscribed::OnShutdown(
    ServiceDiscoveryClientEventgroupStateContext& context) {
  (void)context;
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
