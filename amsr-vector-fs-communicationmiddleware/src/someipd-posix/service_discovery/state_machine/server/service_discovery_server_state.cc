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
/**        \file  service_discovery_server_state.cc
 *        \brief  ServiceDiscoveryServerState
 *
 *      \details  Super class for all states within SD Server State Machine
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief Constructor.
 */
ServiceDiscoveryServerState::ServiceDiscoveryServerState(const ServiceDiscoveryServerStateHandle handle)
    : vac::statemachine::State<ServiceDiscoveryServerStateHandle, ServiceDiscoveryServerContextInterface>(handle),
      logger_(ara::log::CreateLogger("SDServerState", "")) {}

void ServiceDiscoveryServerState::OnServiceUp(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnServiceDown(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnNetworkUp(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnNetworkDown(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnTimeOut(ServiceDiscoveryServerContextInterface& context) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                            const std::string& from_address, std::uint16_t from_port,
                                                            const ServiceDiscoveryEntry& entry,
                                                            const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                                              const std::string& from_address, std::uint16_t from_port,
                                                              const ServiceDiscoveryEntry& entry,
                                                              const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                             const std::string& from_address, std::uint16_t from_port,
                                                             const ServiceDiscoveryEntry& entry,
                                                             const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

void ServiceDiscoveryServerState::OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                                                 const std::string& from_address,
                                                                 std::uint16_t from_port,
                                                                 const ServiceDiscoveryEntry& entry,
                                                                 const ServiceDiscoveryOptionContainer& options) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << "(Default Implementation: Do Nothing)";
  (void)context;
  (void)from_address;
  (void)from_port;
  (void)entry;
  (void)options;

  assert(false && "Default Implementation: Must be overridden within any all states where it might be called");
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix
