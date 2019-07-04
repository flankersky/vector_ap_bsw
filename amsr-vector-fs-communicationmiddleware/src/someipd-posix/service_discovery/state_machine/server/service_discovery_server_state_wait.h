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
/**        \file  service_discovery_server_state_wait.h
 *        \brief  Initial Wait Phase State of SD Server State Machine
 *
 *      \details  This class handles all events occurs to SD Server State Machine during the initial wait phase
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_WAIT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_WAIT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_state.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

class ServiceDiscoveryServerStatePool;
/**
 * \brief This class handles the events happening during the Initial Wait phase of the SD server state machine.
 * During this phase, no Offers are sent, and no incoming FindService or Eventgroups Subscriptions are handled.
 * At the end of the initial wait period, repetition phase is entered.
 */
class ServiceDiscoveryServerStateWait : public ServiceDiscoveryServerState {
 public:
  /**
   * \brief ServiceDiscoveryServerStateWait
   */
  ServiceDiscoveryServerStateWait();

  /**
   * \brief Context sensitive method called when state is entered.
   *
   * \param context The server state machine context.
   */
  void OnEnter(ServiceDiscoveryServerContextInterface& context) override;

  /**
   * \brief Context sensitive method called when state is left.
   *
   * \param context The server state machine context.
   */
  void OnLeave(ServiceDiscoveryServerContextInterface& context) override;

  /**
   * \brief Checks if change from current state to state with given handle is valid.
   *
   *  \param handle The handle of the requested state.
   *
   *  \return true if change is valid, otherwise returns false.
   */
  bool IsValidChange(const ServiceDiscoveryServerStateHandle handle) const override;

  /**
   * \brief Called when the provided service instance is brought down.
   *
   * \param context The server state machine context.
   */
  void OnServiceDown(ServiceDiscoveryServerContextInterface& context) override;

  /**
   * \brief Called when network is brought down.
   *
   * \param context The server state machine context.
   */
  void OnNetworkDown(ServiceDiscoveryServerContextInterface& context) override;

  /**
   * \brief Called once the initial wait timer times out.
   *
   * \param context The server state machine context.
   */
  void OnTimeOut(ServiceDiscoveryServerContextInterface& context) override;

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via Unicast.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnUnicastFindServiceEntry(ServiceDiscoveryServerContextInterface& context, const std::string& from_address,
                                 std::uint16_t from_port, const ServiceDiscoveryEntry& entry,
                                 const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Called on reception of a SOME/IP SD FindService entry via Multicast.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD FindService entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnMulticastFindServiceEntry(ServiceDiscoveryServerContextInterface& context, const std::string& from_address,
                                   std::uint16_t from_port, const ServiceDiscoveryEntry& entry,
                                   const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Called when a SubscribeEventgroup request is received.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context, const std::string& from_address,
                                  std::uint16_t from_port, const ServiceDiscoveryEntry& entry,
                                  const ServiceDiscoveryOptionContainer& options) override;

  /**
   * \brief Called when a StopSubscribeEventgroup request is received.
   *
   * \param context The server state machine context.
   * \param from_address An IP address identifying sender's IP address.
   * \param from_port A UDP port identifying sender's port.
   * \param entry A SOME/IP SD SubscribeEventgroup entry.
   * \param options A container of options received in a SOME/IP SD message.
   */
  void OnStopSubscribeEventgroupEntry(ServiceDiscoveryServerContextInterface& context, const std::string& from_address,
                                      std::uint16_t from_port, const ServiceDiscoveryEntry& entry,
                                      const ServiceDiscoveryOptionContainer& options) override;
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_STATE_WAIT_H_
