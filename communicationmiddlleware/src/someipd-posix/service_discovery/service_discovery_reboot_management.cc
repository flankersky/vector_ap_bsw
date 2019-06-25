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
/**        \file  service_discovery_reboot_management.cc
 *        \brief  reboot detection management
 *
 *      \details  Maintains the session id and reboot flag for unicast and multicast messages for reboot detection
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/service_discovery_reboot_management.h"

namespace someipd_posix {
namespace service_discovery {

RebootParam ServiceDiscoveryRebootManagement::GetNextUnicastSenderSessionIdAndRebootFlag(
    const AddressPair& address_pair) {
  RebootParam reboot_param;

  auto iter = sd_unicast_sender_reboot_management_map_.find(address_pair);
  if (iter != sd_unicast_sender_reboot_management_map_.end()) {
    ++iter->second.session_id_;

    if (iter->second.session_id_ == someip_posix_common::someip::kSomeIpSdInvalidSessionId) {
      iter->second.session_id_ = someip_posix_common::someip::kSomeIpSdMinimumSessionId;
      iter->second.reboot_flag_ = false;
    }

    reboot_param.session_id_ = iter->second.session_id_;
    reboot_param.reboot_flag_ = iter->second.reboot_flag_;
  } else {
    sd_unicast_sender_reboot_management_map_.insert({address_pair, reboot_param});
  }

  return reboot_param;
}

RebootParam ServiceDiscoveryRebootManagement::GetLastMulticastReceiverSessionIdAndRebootFlag(
    const AddressPair& address_pair, someip_posix_common::someip::SessionId session_id, bool reboot_flag) {
  RebootParam reboot_param;
  reboot_param.session_id_ = session_id;
  reboot_param.reboot_flag_ = reboot_flag;

  auto iter = sd_multicast_receiver_reboot_management_map_.find(address_pair);
  if (iter != sd_multicast_receiver_reboot_management_map_.end()) {
    reboot_param.session_id_ = iter->second.session_id_;
    reboot_param.reboot_flag_ = iter->second.reboot_flag_;
    iter->second.session_id_ = session_id;
    iter->second.reboot_flag_ = reboot_flag;
  } else {
    sd_multicast_receiver_reboot_management_map_.insert({address_pair, reboot_param});
    reboot_param.session_id_ = someip_posix_common::someip::kSomeIpSdInvalidSessionId;
  }

  return reboot_param;
}

RebootParam ServiceDiscoveryRebootManagement::GetLastUnicastReceiverSessionIdAndRebootFlag(
    const AddressPair& address_pair, someip_posix_common::someip::SessionId session_id, bool reboot_flag) {
  RebootParam reboot_param;
  reboot_param.session_id_ = session_id;
  reboot_param.reboot_flag_ = reboot_flag;

  auto iter = sd_unicast_receiver_reboot_management_map_.find(address_pair);
  if (iter != sd_unicast_receiver_reboot_management_map_.end()) {
    reboot_param.session_id_ = iter->second.session_id_;
    reboot_param.reboot_flag_ = iter->second.reboot_flag_;
    iter->second.session_id_ = session_id;
    iter->second.reboot_flag_ = reboot_flag;
  } else {
    sd_unicast_receiver_reboot_management_map_.insert({address_pair, reboot_param});
    reboot_param.session_id_ = someip_posix_common::someip::kSomeIpSdInvalidSessionId;
  }

  return reboot_param;
}

RebootParam ServiceDiscoveryRebootManagement::GetNextMulticastSenderSessionIdAndRebootFlag() {
  RebootParam reboot_param;
  reboot_param.session_id_ = sd_multicast_sender_reboot_param_.session_id_;
  reboot_param.reboot_flag_ = sd_multicast_sender_reboot_param_.reboot_flag_;

  ++sd_multicast_sender_reboot_param_.session_id_;

  if (sd_multicast_sender_reboot_param_.session_id_ == someip_posix_common::someip::kSomeIpSdInvalidSessionId) {
    sd_multicast_sender_reboot_param_.session_id_ = someip_posix_common::someip::kSomeIpSdMinimumSessionId;
    sd_multicast_sender_reboot_param_.reboot_flag_ = false;
  }
  return reboot_param;
}

void ServiceDiscoveryRebootManagement::ResetUnicastReceiverRebootParam(const AddressPair& address_pair) {
  auto iter = sd_unicast_receiver_reboot_management_map_.find(address_pair);
  if (iter != sd_unicast_receiver_reboot_management_map_.end()) {
    iter->second.session_id_ = someip_posix_common::someip::kSomeIpSdInvalidSessionId;
    iter->second.reboot_flag_ = true;
  }
}

void ServiceDiscoveryRebootManagement::ResetMulticastReceiverRebootParam(const AddressPair& address_pair) {
  auto iter = sd_multicast_receiver_reboot_management_map_.find(address_pair);
  if (iter != sd_multicast_receiver_reboot_management_map_.end()) {
    iter->second.session_id_ = someip_posix_common::someip::kSomeIpSdInvalidSessionId;
    iter->second.reboot_flag_ = true;
  }
}

}  // namespace service_discovery
}  // namespace someipd_posix
