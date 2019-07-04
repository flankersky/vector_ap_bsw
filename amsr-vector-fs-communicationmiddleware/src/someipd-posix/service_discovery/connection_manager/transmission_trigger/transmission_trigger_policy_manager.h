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
/**        \file  transmission_trigger_policy_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_MANAGER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <map>
#include <vector>

#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_policy.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {
namespace transmission_trigger {

/**
 * \brief Transmission trigger policy manager.
 */
class TransmissionTriggerPolicyManager {
 public:
  /**
   * \brief SOME/IP message header type.
   */
  using SomeIpMessageHeader = someip_posix_common::someip::SomeIpMessageHeader;
  /**
   * \brief SOME/IP service identifier type.
   */
  using ServiceId = someip_posix_common::someip::ServiceId;
  /**
   * \brief SOME/IP instance identifier type.
   */
  using InstanceId = someip_posix_common::someip::InstanceId;
  /**
   * \brief SOME/IP method/event identifier type.
   */
  using MethodId = someip_posix_common::someip::MethodId;

  /**
   * \brief Constructor of TransmissionTriggerPolicyManager.
   */
  TransmissionTriggerPolicyManager();
  /**
   * \brief Adds a new transmission trigger policy.
   *
   * \param policy A transmission trigger policy to add.
   */
  void InstallPolicy(const TransmissionTriggerPolicy& policy);
  /**
   * \brief Finds a transmission trigger policy which matches best the given SOME/IP message header.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param message_header A SOME/IP message header.
   * \return A transmission trigger policy.
   */
  const TransmissionTriggerPolicy& FindBestMatchingPolicy(InstanceId instance_id,
                                                          const SomeIpMessageHeader& message_header) const;

 private:
  /**
   * \brief A container type for transmission trigger policies.
   */
  using ServicePolicyContainer = std::vector<TransmissionTriggerPolicy>;
  /**
   * \brief A key type for a map of containers of transmission trigger policies.
   */
  using PolicyServiceIdKey = std::uint32_t;
  /**
   * \brief A map type for transmission trigger containers.
   */
  using PolicyContainer = std::map<PolicyServiceIdKey, ServicePolicyContainer>;

  /**
   * \brief The key for the container of all transmission trigger policies which have no SOME/IP service identifier set.
   */
  static constexpr PolicyServiceIdKey kServiceIdWildcard{std::numeric_limits<ServiceId>::max() + 1};

  /**
   * \brief A map of transmission trigger containers.
   *
   * There is a single container of transmission trigger policies for each SOME/IP service identifier.
   */
  PolicyContainer policies_;
};

}  // namespace transmission_trigger
}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_POLICY_MANAGER_H_
