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
/**        \file  service_discovery_client_state_initial_wait_phase.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_initial_wait_phase.h"
#include <cassert>
#include <random>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateInitialWaitPhase::ServiceDiscoveryClientStateInitialWaitPhase()
    : ServiceDiscoveryClientState(ServiceDiscoveryClientStateHandle::kInitialWaitPhase),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientStateInitialWaitPhase", "")} {}

bool ServiceDiscoveryClientStateInitialWaitPhase::IsValidChange(const ServiceDiscoveryClientStateHandle handle) const {
  return (handle == ServiceDiscoveryClientStateHandle::kDownPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kRepetitionPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kMainPhase);
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnEnter(ServiceDiscoveryClientStateContext& context) {
  assert(context.IsNetworkUp() && !context.IsServiceAvailable());
  /* PRS_SOMEIPSD_00399, PRS_SOMEIPSD_00400, PRS_SOMEIPSD_00401 */
  auto delay = GetRandomInitialDelay(context);
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": delay " << delay.count() << " repetitions "
                     << context.GetFindServiceRepetitionsMax();
  if (delay.count() > 0) {
    context.StartTimer(delay);
  } else {
    /* PRS_SOMEIPSD_00404 */
    context.SendFindService();
    if (context.GetFindServiceRepetitionsMax() > 0) {
      /* PRS_SOMEIPSD_00404 */
      context.ChangeState(ServiceDiscoveryClientStateHandle::kRepetitionPhase);
    } else {
      /* PRS_SOMEIPSD_00409 */
      context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
    }
  }
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnLeave(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnNetworkUp(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnNetworkDown(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnServiceRequested(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnServiceReleased(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  /* PRS_SOMEIPSD_00408 */
  context.StopTimer();
  context.StartTimer(context.GetOfferServiceTtl());
  context.SetServiceAvailable(true);
  context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnStopOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateInitialWaitPhase::OnTimeout(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": repetitions " << context.GetFindServiceRepetitionsMax();
  /* PRS_SOMEIPSD_00404 */
  context.SendFindService();
  if (context.GetFindServiceRepetitionsMax() > 0) {
    /* PRS_SOMEIPSD_00404 */
    context.ChangeState(ServiceDiscoveryClientStateHandle::kRepetitionPhase);
  } else {
    /* PRS_SOMEIPSD_00409 */
    context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
  }
}

ServiceDiscoveryClientStateInitialWaitPhase::TimerDelay
ServiceDiscoveryClientStateInitialWaitPhase::GetRandomInitialDelay(ServiceDiscoveryClientStateContext& context) const {
  /* PRS_SOMEIPSD_00401 */
  std::default_random_engine eng((std::random_device())());
  std::uniform_int_distribution<std::uint64_t> dis(context.GetFindServiceInitialDelayMin().count(),
                                                   context.GetFindServiceInitialDelayMax().count());
  ServiceDiscoveryClientStateContext::TimerDelay delay{dis(eng)};
  assert(delay.count() >= context.GetFindServiceInitialDelayMin().count() &&
         delay.count() <= context.GetFindServiceInitialDelayMax().count());
  return delay;
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
