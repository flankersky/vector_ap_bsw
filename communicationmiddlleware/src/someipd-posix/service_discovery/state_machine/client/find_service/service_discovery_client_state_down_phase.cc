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
/**        \file  service_discovery_client_state_down_phase.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_down_phase.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateDownPhase::ServiceDiscoveryClientStateDownPhase()
    : ServiceDiscoveryClientState(ServiceDiscoveryClientStateHandle::kDownPhase),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientStateDownPhase", "")} {}

bool ServiceDiscoveryClientStateDownPhase::IsValidChange(const ServiceDiscoveryClientStateHandle handle) const {
  return (handle == ServiceDiscoveryClientStateHandle::kInitialWaitPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kMainPhase);
}

void ServiceDiscoveryClientStateDownPhase::OnEnter(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(!context.IsNetworkUp() || !context.IsServiceRequested());
  if (!context.IsNetworkUp()) {
    context.SetServiceAvailable(false);
  }
}

void ServiceDiscoveryClientStateDownPhase::OnLeave(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
}

void ServiceDiscoveryClientStateDownPhase::OnNetworkUp(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  if (context.IsServiceRequested()) {
    context.ChangeState(ServiceDiscoveryClientStateHandle::kInitialWaitPhase);
  }
}

void ServiceDiscoveryClientStateDownPhase::OnNetworkDown(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.SetServiceAvailable(false);
}

void ServiceDiscoveryClientStateDownPhase::OnServiceRequested(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(!context.IsServiceAvailable() || context.IsNetworkUp());
  if (context.IsServiceAvailable()) {
    /* DO NOT STOP TTL TIMER */
    context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
  } else if (context.IsNetworkUp()) {
    /* PRS_SOMEIPSD_00397 */
    context.ChangeState(ServiceDiscoveryClientStateHandle::kInitialWaitPhase);
  }
}

void ServiceDiscoveryClientStateDownPhase::OnServiceReleased(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateDownPhase::OnOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.StartTimer(context.GetOfferServiceTtl());
  context.SetServiceAvailable(true);
}

void ServiceDiscoveryClientStateDownPhase::OnStopOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.SetServiceAvailable(false);
}

void ServiceDiscoveryClientStateDownPhase::OnTimeout(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.SetServiceAvailable(false);
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
