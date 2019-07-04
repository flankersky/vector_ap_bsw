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
/**        \file  service_discovery_server_state.h
 *        \brief  ServiceDiscoveryServerState
 *
 *      \details  Super class for all states within SD Server State Machine
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_context_interface.h"
#include "vac/statemachine/state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief The handles representing the States defined for the StateMachine.
 */
enum class ServiceDiscoveryServerStateHandle { kDownPhase = 1, kWaitPhase = 2, kRepetitionPhase = 3, kMainPhase = 4 };

/**
 * \brief ServiceDiscoveryServerState
 */
class ServiceDiscoveryServerState
    : public vac::statemachine::State<ServiceDiscoveryServerStateHandle, ServiceDiscoveryServerContextInterface> {
 public:
  /**
   * \brief Constructor.
   *
   * \param handle The handle for this state.
   */
  explicit ServiceDiscoveryServerState(const ServiceDiscoveryServerStateHandle handle);

  /**
   * \brief Destructor.
   */
  virtual ~ServiceDiscoveryServerState() = default;

  /**
   * \brief Called when the provided service instance is brought up.
   *
   * \param context The server state machine context.
   */
  virtual void OnServiceUp(ServiceDiscoveryServerContextInterface& context);

  /**
   * \brief Called when the provided service instance is brought down.
   *
   * \param context The server state machine context.
   */
  virtual void OnServiceDown(ServiceDiscoveryServerContextInterface& context);

  /**
   * \brief Called when network is brought up.
   *
   * \param context The server state machine context.
   */
  virtual void OnNetworkUp(ServiceDiscoveryServerContextInterface& context);

  /**
   * \brief Called when network is brought down.
   *
   * \param context The server state machine context.
   */
  virtual void OnNetworkDown(ServiceDiscoveryServerContextInterface& context);

  /**
   * \brief Called once the installed timer times out.
   *
   * \param context The server state machine context.
   */
  virtual void OnTimeOut(ServiceDiscoveryServerContextInterface& context);

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via Unicast.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                         const std::string& from_address, std::uint16_t from_port,
                                         const ServiceDiscoveryEntry& entry,
                                         const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via Multicast.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context,
                                           const std::string& from_address, std::uint16_t from_port,
                                           const ServiceDiscoveryEntry& entry,
                                           const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called when a SubscribeEventgroup request is received.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                          const std::string& from_address, std::uint16_t from_port,
                                          const ServiceDiscoveryEntry& entry,
                                          const ServiceDiscoveryOptionContainer& options);

  /**
   * \brief Called when a StopSubscribeEventgroup request is received.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  virtual void OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context,
                                              const std::string& from_address, std::uint16_t from_port,
                                              const ServiceDiscoveryEntry& entry,
                                              const ServiceDiscoveryOptionContainer& options);

 protected:
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_H_
