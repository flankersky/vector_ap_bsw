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
/**        \file  service_discovery_client_offer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/service_discovery_client_offer.h"
#include <cassert>
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/configuration/configuration.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

ServiceDiscoveryClientOffer::ServiceDiscoveryClientOffer()
    : is_valid_{false},
      sd_address_{},
      sd_port_{},
      entry_{},
      options_{},
      is_multicast_{false},
      tcp_option_{-1},
      udp_option_{-1} {}

ServiceDiscoveryClientOffer::ServiceDiscoveryClientOffer(const std::string& sd_address, std::uint16_t sd_port,
                                                         const ServiceDiscoveryEntry& entry,
                                                         const ServiceDiscoveryOptionContainer& options,
                                                         bool is_multicast)
    : is_valid_{true},
      sd_address_{sd_address},
      sd_port_{sd_port},
      entry_{entry},
      options_{options},
      is_multicast_{is_multicast},
      tcp_option_{-1},
      udp_option_{-1} {
  namespace someip = someip_posix_common::someip;
  assert((entry_.ttl_ == 0) || (!options.empty()));
  assert(entry.index_1st_opts_ + entry.number_1st_opts_ <= options.size());
  assert(entry.index_2nd_opts_ + entry.number_2nd_opts_ <= options.size());
  for (int i = entry.index_1st_opts_; i < entry.index_1st_opts_ + entry.number_1st_opts_; ++i) {
    assert(options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kTCP ||
           options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kUDP);
    if (options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kTCP) {
      tcp_option_ = i;
    } else {
      udp_option_ = i;
    }
  }
  for (int i = entry.index_2nd_opts_; i < entry.index_2nd_opts_ + entry.number_2nd_opts_; ++i) {
    assert(options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kTCP ||
           options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kUDP);
    if (options_[i].proto_ == someip::SomeIpSdEndpointOptionProto::kTCP) {
      tcp_option_ = i;
    } else {
      udp_option_ = i;
    }
  }
  /*
   * - An OfferService must contain at least one option
   * - A StopOfferService may have no options
   */
  is_valid_ = (entry.ttl_ == 0) || (tcp_option_ != -1) || (udp_option_ != -1);
}

bool ServiceDiscoveryClientOffer::IsValid() const { return is_valid_; }

bool ServiceDiscoveryClientOffer::IsMulticast() const { return is_multicast_; }

void ServiceDiscoveryClientOffer::Invalidate() { is_valid_ = false; }

ServiceDiscoveryClientOffer::EndpointType ServiceDiscoveryClientOffer::GetSdEndpoint() const {
  return {sd_address_, sd_port_};
}

const ServiceDiscoveryEntry& ServiceDiscoveryClientOffer::GetEntry() const { return entry_; }

const ServiceDiscoveryOptionContainer& ServiceDiscoveryClientOffer::GetOptions() const { return options_; }

bool ServiceDiscoveryClientOffer::IsStopOffer() const { return entry_.ttl_ == 0; }

ServiceDiscoveryClientOffer::EndpointType ServiceDiscoveryClientOffer::GetTcpEndpoint() const {
  using cfg = configuration::Configuration;
  if (tcp_option_ != -1) {
    return {options_[tcp_option_].address_, options_[tcp_option_].port_};
  } else {
    return {cfg::kInvalidAddress, cfg::kInvalidPort};
  }
}

ServiceDiscoveryClientOffer::EndpointType ServiceDiscoveryClientOffer::GetUdpEndpoint() const {
  using cfg = configuration::Configuration;
  if (udp_option_ != -1) {
    return {options_[udp_option_].address_, options_[udp_option_].port_};
  } else {
    return {cfg::kInvalidAddress, cfg::kInvalidPort};
  }
}

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
