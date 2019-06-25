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
/**        \file  service_discovery_client_state_repetition_phase.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_repetition_phase.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

ServiceDiscoveryClientStateRepetitionPhase::ServiceDiscoveryClientStateRepetitionPhase()
    : ServiceDiscoveryClientState(ServiceDiscoveryClientStateHandle::kRepetitionPhase),
      logger_{ara::log::CreateLogger("ServiceDiscoveryClientStateRepetitionPhase", "")},
      repetition_counter_{0},
      timer_delay_{0} {}

bool ServiceDiscoveryClientStateRepetitionPhase::IsValidChange(const ServiceDiscoveryClientStateHandle handle) const {
  return (handle == ServiceDiscoveryClientStateHandle::kDownPhase) ||
         (handle == ServiceDiscoveryClientStateHandle::kMainPhase);
}

void ServiceDiscoveryClientStateRepetitionPhase::OnEnter(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(context.IsNetworkUp() && !context.IsServiceAvailable() && context.GetFindServiceRepetitionsMax() > 0);
  repetition_counter_ = 0;
  timer_delay_ = context.GetFindServiceRepetitionsBaseDelay();
  context.StartTimer(timer_delay_);
}

void ServiceDiscoveryClientStateRepetitionPhase::OnLeave(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateRepetitionPhase::OnNetworkUp(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateRepetitionPhase::OnNetworkDown(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateRepetitionPhase::OnServiceRequested(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateRepetitionPhase::OnServiceReleased(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  context.StopTimer();
  context.ChangeState(ServiceDiscoveryClientStateHandle::kDownPhase);
}

void ServiceDiscoveryClientStateRepetitionPhase::OnOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  /* PRS_SOMEIPSD_00408 */
  context.StopTimer();
  context.StartTimer(context.GetOfferServiceTtl());
  context.SetServiceAvailable(true);
  context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
}

void ServiceDiscoveryClientStateRepetitionPhase::OnStopOfferService(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  (void)context;
}

void ServiceDiscoveryClientStateRepetitionPhase::OnTimeout(ServiceDiscoveryClientStateContext& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  assert(repetition_counter_ < context.GetFindServiceRepetitionsMax());
  context.SendFindService();
  /* PRS_SOMEIPSD_00407 */
  repetition_counter_++;
  if (repetition_counter_ < context.GetFindServiceRepetitionsMax()) {
    /* PRS_SOMEIPSD_00406 */
    timer_delay_ *= 2;
    context.StartTimer(timer_delay_);
  } else {
    /* Service is still unavailable */
    /* PRS_SOMEIPSD_00410 */
    context.ChangeState(ServiceDiscoveryClientStateHandle::kMainPhase);
  }
}

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
