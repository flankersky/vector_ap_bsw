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
/**        \file  transmission_trigger_policy_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cassert>
#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_message_queue.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {
namespace transmission_trigger {

constexpr TransmissionTriggerPolicyManager::PolicyServiceIdKey TransmissionTriggerPolicyManager::kServiceIdWildcard;

TransmissionTriggerPolicyManager::TransmissionTriggerPolicyManager() {
  // Install a wildcard policy that catches everything
  auto& wildcard_service_policies{policies_[kServiceIdWildcard]};
  wildcard_service_policies.emplace_back();
}

void TransmissionTriggerPolicyManager::InstallPolicy(const TransmissionTriggerPolicy& policy) {
  auto key{kServiceIdWildcard};
  if (policy.GetServiceId().second) {
    key = policy.GetServiceId().first;
  }
  // Create an empty container for the corresponding specific service id
  if (policies_.find(key) == policies_.end()) {
    policies_.insert({key, {}});
  }
  assert(policies_.find(key) != policies_.end());
  auto& service_policies{policies_[key]};
  service_policies.push_back(policy);
}

const TransmissionTriggerPolicy& TransmissionTriggerPolicyManager::FindBestMatchingPolicy(
    InstanceId instance_id, const SomeIpMessageHeader& message_header) const {
  const TransmissionTriggerPolicy* best_policy{nullptr};
  const auto service_id{message_header.service_id_};
  const auto method_id{message_header.method_id_};
  // First look for a policy with the corresponding service id
  {
    const auto it{policies_.find(message_header.service_id_)};
    if (it != policies_.cend()) {
      auto& service_policies{it->second};
      for (auto& policy : service_policies) {
        if (policy.Matches(service_id, instance_id, method_id)) {
          if (!best_policy || policy.MatchesBetterThan(*best_policy)) {
            best_policy = &policy;
          }
        }
      }
    }
  }
  /*
   * If no policy for the corresponding service id has been found,
   * look for the best match among wildcard service policies.
   */
  if (!best_policy) {
    const auto it{policies_.find(kServiceIdWildcard)};
    if (it != policies_.cend()) {
      auto& service_policies{it->second};
      for (auto& policy : service_policies) {
        if (policy.Matches(service_id, instance_id, method_id)) {
          if (!best_policy || policy.MatchesBetterThan(*best_policy)) {
            best_policy = &policy;
          }
        }
      }
    }
  }
  assert(best_policy);
  return *best_policy;
}

}  // namespace transmission_trigger
}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix
