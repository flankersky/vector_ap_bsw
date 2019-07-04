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
/**        \file  service_discovery_client_eventgroup_state_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_CONTEXT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

enum class ServiceDiscoveryClientEventgroupStateHandle;

/**
 * \brief A Service Discovery Client eventgroup state context.
 */
class ServiceDiscoveryClientEventgroupStateContext {
 public:
  /**
   * \brief A timer delay.
   */
  using TimerDelay = std::chrono::high_resolution_clock::duration;
  /**
   * \brief Destructor of ServiceDiscoveryClientEventgroupStateContext.
   */
  virtual ~ServiceDiscoveryClientEventgroupStateContext() = default;
  /**
   * \brief Indicates whether the service is available or not.
   *
   * \return true if the service is available and false otherwise.
   */
  virtual bool IsServiceAvailable() const = 0;
  /**
   * \brief Indicates whether the SOME/IP eventgroup is requested or not.
   *
   * \return true if the SOME/IP eventgroup is requested and false otherwise.
   */
  virtual bool IsEventgroupRequested() const = 0;
  /**
   * \brief Returns the maximum wait time for a SubscribeEventgroupAck entry.
   *
   * \return A time value.
   */
  virtual TimerDelay GetSubscribeEventgroupAckTimeout() const = 0;
  /**
   * \brief Returns the time value after which the subscription of the SOME/IP eventgroup expires.
   *
   * \return A time value.
   */
  virtual TimerDelay GetSubscriptionTtl() const = 0;
  /**
   * \brief Initiates the transmission of a SubscribeEventgroup entry.
   *
   * \param request_initial_events Indicates whether initial events shall be requested explicitly.
   */
  virtual void SendSubscribeEventgroup(bool request_initial_events) = 0;
  /**
   * \brief Initiates the transmission of a StopSubscribeEventgroup entry.
   */
  virtual void SendStopSubscribeEventgroup() = 0;
  /**
   * \brief Initiates a state change.
   *
   * \param handle The handle of a new state.
   */
  virtual void ChangeState(const ServiceDiscoveryClientEventgroupStateHandle handle) = 0;
  /**
   * \brief Starts the generic timer with a given delay.
   *
   * \param delay A timer delay.
   */
  virtual void StartTimer(TimerDelay delay) = 0;
  /**
   * \brief Stops the generic timer.
   */
  virtual void StopTimer() = 0;
  /**
   * \brief Starts the TTL timer with a given delay.
   *
   * \param delay A timer delay.
   */
  virtual void StartTtlTimer(TimerDelay delay) = 0;
  /**
   * \brief Stops the TTL timer.
   */
  virtual void StopTtlTimer() = 0;
  /**
   * \brief Called by a state when the SOME/IP eventgroup has been subscribed.
   */
  virtual void OnSubscribed() = 0;
  /**
   * \brief Called by a state when the SOME/IP eventgroup has been unsubscribed.
   */
  virtual void OnUnsubscribed() = 0;
  /**
   * \brief Called by a state when the SOME/IP eventgroup subscription is pending.
   */
  virtual void OnSubscriptionPending() = 0;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_CONTEXT_H_
