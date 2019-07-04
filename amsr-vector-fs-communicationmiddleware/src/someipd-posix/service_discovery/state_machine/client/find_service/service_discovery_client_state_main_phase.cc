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
/**        \file  service_discovery_client_state_main_phase.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_main_phase.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateMainPhase::ServiceDiscoveryClientStateMainPhase()
    : ServiceDiscoveryClientState(ServiceDiscoveryClientStateHandle::kMainPhase),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientStateMainPhase", "")} {}

bool ServiceDiscoveryClientStateMainPhase::IsValidChange(const ServiceDiscoveryClientStateHandle handle) const {
  return (handle == ServiceDiscoveryClientStateHandle::kDownPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kInitialWaitPhase);
}

void ServiceDiscoveryClientStateMainPhase::OnEnter(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsNetworkUp());
  /* DO NOT START TTL TIMER */
}

void ServiceDiscoveryClientStateMainPhase::OnLeave(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateMainPhase::OnNetworkUp(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateMainPhase::OnNetworkDown(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateMainPhase::OnServiceRequested(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateMainPhase::OnServiceReleased(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  /* DO NOT STOP TTL TIMER */
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateMainPhase::OnOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StartTimer(context.GetOfferServiceTtl());
  if (!context.IsServiceAvailable()) {
    context.SetServiceAvailable(true);
  }
}

void ServiceDiscoveryClientStateMainPhase::OnStopOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.SetServiceAvailable(false);
}

void ServiceDiscoveryClientStateMainPhase::OnTimeout(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.SetServiceAvailable(false);
  context.ChangeState(ServiceDiscoveryClientStateHandle::kInitialWaitPhase);
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
