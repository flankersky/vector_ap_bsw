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
/**        \file  transmission_trigger_policy.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <utility>

#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {
namespace transmission_trigger {

/**
 * \brief Transmission trigger mode.
 *
 * \see ECUC_SoAd_00136
 */
enum class TransmissionTriggerMode {
  kTransmissionTriggerModeAlways,  // Always triggers the transmission
  kTransmissionTriggerModeNever    // Does not trigger the transmission
};

/**
 * \brief Transmission trigger policy.
 */
class TransmissionTriggerPolicy {
 public:
  /**
   * \brief SOME/IP service identifier type.
   */
  using ServiceId = someip_posix_common::someip::ServiceId;
  /**
   * \brief SOME/IP instance identifier type.
   */
  using InstanceId = someip_posix_common::someip::InstanceId;
  /**
   * \brief SOME/IP method identifier type.
   */
  using MethodId = someip_posix_common::someip::MethodId;
  /**
   * \brief A timer delay.
   */
  using TimerDelay = vac::timer::Timer::Clock::duration;
  /**
   * \brief SOME/IP service identifier type.
   */
  using ServiceIdOptional = std::pair<ServiceId, bool>;
  /**
   * \brief SOME/IP instance identifier type.
   */
  using InstanceIdOptional = std::pair<InstanceId, bool>;
  /**
   * \brief SOME/IP method identifier type.
   */
  using MethodIdOptional = std::pair<MethodId, bool>;
  /**
   * \brief A timer delay.
   */
  using TimerDelayOptional = std::pair<TimerDelay, bool>;

  /**
   * \brief Constructor of TransmissionTriggerPolicy.
   *
   * Creates a policy which always triggers the transmission.
   */
  TransmissionTriggerPolicy()
      : service_id_{0U, false},
        instance_id_{0U, false},
        method_id_{0U, false},
        trigger_mode_{TransmissionTriggerMode::kTransmissionTriggerModeAlways},
        trigger_timeout_{TimerDelay::zero(), false} {}

  /**
   * \brief Constructor of TransmissionTriggerPolicy.
   *
   * Creates a policy which triggers the transmission after a given delay.
   *
   * \param delay A timer delay.
   */
  explicit TransmissionTriggerPolicy(TimerDelay delay)
      : service_id_{0U, false},
        instance_id_{0U, false},
        method_id_{0U, false},
        trigger_mode_{TransmissionTriggerMode::kTransmissionTriggerModeNever},
        trigger_timeout_{delay, true} {}
  /**
   * \brief Sets the SOME/IP service identifier to which this policy shall apply.
   *
   * \param service_id A SOME/IP service identifier.
   */
  void SetServiceId(ServiceId service_id) { service_id_ = {service_id, true}; }
  /**
   * \brief Returns the SOME/IP service identifier to which this policy shall apply.
   *
   * \return A SOME/IP service identifier.
   */
  const ServiceIdOptional& GetServiceId() const { return service_id_; }
  /**
   * \brief Sets the SOME/IP instance identifier to which this policy shall apply.
   *
   * \param instance_id A SOME/IP instance identifier.
   */
  void SetInstanceId(InstanceId instance_id) { instance_id_ = {instance_id, true}; }
  /**
   * \brief Returns the SOME/IP instance identifier to which this policy shall apply.
   *
   * \return A SOME/IP instance identifier.
   */
  const InstanceIdOptional& GetInstanceId() const { return instance_id_; }
  /**
   * \brief Sets the SOME/IP method/event identifier to which this policy shall apply.
   *
   * \param method_id A SOME/IP method/event identifier.
   */
  void SetMethodId(MethodId method_id) { method_id_ = {method_id, true}; }
  /**
   * \brief Returns the SOME/IP method identifier to which this policy shall apply.
   *
   * \return A SOME/IP method identifier.
   */
  const MethodIdOptional& GetMethodId() const { return method_id_; }
  /**
   * \brief Returns the transmission trigger mode associated with this policy.
   *
   * \return A transmission trigger mode.
   */
  TransmissionTriggerMode GetTriggerMode() const { return trigger_mode_; }
  /**
   * \brief Returns the transmission trigger timeout associated with this policy.
   *
   * \return A timer delay.
   */
  const TimerDelayOptional& GetTriggerTimeout() const { return trigger_timeout_; }
  /**
   * \brief Checks whether the provided SOME/IP service, instance and method/event identifiers match with this policy.
   *
   * \param service_id A SOME/IP service identifier.
   * \param instance_id A SOME/IP instance identifier.
   * \param method_id A SOME/IP method/event identifier.
   * \return true if the provided SOME/IP service, instance and method identifiers match with this policy and false
   * otherwise.
   */
  bool Matches(ServiceId service_id, InstanceId instance_id, MethodId method_id) const {
    return (!service_id_.second || (service_id_.first == service_id)) &&
           (!instance_id_.second || (instance_id_.first == instance_id)) &&
           (!method_id_.second || (method_id_.first == method_id));
  }
  /**
   * \brief Checks whether this policy matches better than the given one.
   *
   * ServiceID shall be the most important match.
   * The second important match shall be InstanceID.
   * And the least important match is MethodID.
   * Match of both InstanceID and MethodID shall not overrule the match of ServiceID.
   *
   * \param other A transmission trigger policy to compare to.
   * \return true if this policy matches better than the given one and false otherwise.
   */
  bool MatchesBetterThan(const TransmissionTriggerPolicy& other) const {
    const int service_id_weight{7};
    const int instance_id_weight{3};
    const int method_id_weight{2};
    assert(!service_id_.second || !other.service_id_.second || (service_id_.first == other.service_id_.first));
    assert(!instance_id_.second || !other.instance_id_.second || (instance_id_.first == other.instance_id_.first));
    assert(!method_id_.second || !other.method_id_.second || (method_id_.first == other.method_id_.first));
    int weight{0};
    if (service_id_.second != other.service_id_.second) {
      if (service_id_.second) {
        weight += service_id_weight;
      } else {
        weight -= service_id_weight;
      }
    }
    if (instance_id_.second != other.instance_id_.second) {
      if (instance_id_.second) {
        weight += instance_id_weight;
      } else {
        weight -= instance_id_weight;
      }
    }
    if (method_id_.second != other.method_id_.second) {
      if (method_id_.second) {
        weight += method_id_weight;
      } else {
        weight -= method_id_weight;
      }
    }
    return (weight > 0);
  }

 private:
  /**
   * \brief A SOME/IP service identifier.
   */
  ServiceIdOptional service_id_;
  /**
   * \brief A SOME/IP instance identifier.
   */
  InstanceIdOptional instance_id_;
  /**
   * \brief A SOME/IP method/event identifier.
   */
  MethodIdOptional method_id_;
  /**
   * \brief A transmission trigger mode.
   */
  const TransmissionTriggerMode trigger_mode_;
  /**
   * \brief A transmission trigger timeout.
   */
  TimerDelayOptional trigger_timeout_;
};

}  // namespace transmission_trigger
}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_H_
