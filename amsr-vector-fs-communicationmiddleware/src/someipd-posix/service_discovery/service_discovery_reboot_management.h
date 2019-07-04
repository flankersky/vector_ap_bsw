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
/**        \file  service_discovery_reboot_management.h
 *        \brief  reboot detection management
 *
 *      \details  Maintains the session id and reboot flag for unicast and multicast messages for reboot detection
 *
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vac/testing/test_adapter.h>
#include <map>
#include <string>
#include <utility>
#include "someip-posix-common/someip/message.h"

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief structure to store session_id and reboot flag for a unicast or multicast message.
 */
struct RebootParam {
  /**
   * \brief Session id identifier.
   */
  someip_posix_common::someip::SessionId session_id_ = someip_posix_common::someip::kSomeIpSdMinimumSessionId;
  /**
   * \brief Reboot flag must be set to 1 after starting the controller and when the
   * session ID changes from 0xFFFF to 0x0001, the reboot flag must be set to 0.
   */
  bool reboot_flag_ = true;
};

/**
 * \brief pair to store ip address and port for message.
 */
using AddressPair = std::pair<someip_posix_common::someip::IpAddress, someip_posix_common::someip::Port>;

/**
 * \brief Maps the information of AddressPair to the session_id and reboot flag
 */
using RebootManagementMap = std::map<AddressPair, RebootParam>;

/**
 * \brief Maintains the session id and reboot flag for unicast and multicast messages for reboot detection.
 */
class ServiceDiscoveryRebootManagement {
 public:
  /**
   * \brief Constructor
   */
  ServiceDiscoveryRebootManagement() = default;
  /**
   * \brief Destructor
   */
  virtual ~ServiceDiscoveryRebootManagement() = default;
  /**
   * \brief Adds a new entry or updates an existing entry in the sd_unicast_sender_reboot_management_map and returns the
   * session id and reboot flag.
   * \param address_pair structure to store ip address and port for message.
   * \return RebootParam next session id and reboot flag.
   */
  RebootParam GetNextUnicastSenderSessionIdAndRebootFlag(const AddressPair& address_pair);

  /**
   * \brief Adds a new entry or updates an existing entry in the sd_multicast_receiver_reboot_management_map and returns
   * the session id and reboot flag.
   * \param address_pair structure to store ip address and port for message.
   * \param session_id identifier for the specific message receiver.
   * \param reboot_flag flag for reboot detection.
   * \return RebootParam next session id and reboot flag.
   */
  RebootParam GetLastMulticastReceiverSessionIdAndRebootFlag(const AddressPair& address_pair,
                                                             someip_posix_common::someip::SessionId session_id,
                                                             bool reboot_flag);

  /**
   * \brief Adds a new entry or updates an existing entry in the sd_unicast_receiver_reboot_management_map and returns
   * the session id and reboot flag.
   * \param address_pair structure to store ip address and port for message.
   * \param session_id identifier for the specific message receiver.
   * \param reboot_flag flag for reboot detection.
   * \return RebootParam last session id and reboot flag.
   */
  RebootParam GetLastUnicastReceiverSessionIdAndRebootFlag(const AddressPair& address_pair,
                                                           someip_posix_common::someip::SessionId session_id,
                                                           bool reboot_flag);

  /**
   * \brief Returns the next session id and reboot flag for the multicast message and increments the session id for a
   * new multicast message being sent.
   * \return RebootParam last session id and reboot flag.
   */
  RebootParam GetNextMulticastSenderSessionIdAndRebootFlag();

  /**
  * \brief Resets the session id and reboot flag for an existing entry in the sd_unicast_receiver_reboot_management_map.
  * When reboot is detected for a provider daemon on reception of multicast message, we reset the session id and reboot
  * flag for unicast from that provider to avoid duplicate notification of reboot.
  * \param address_pair structure to store ip address and port for message.
  */
  void ResetUnicastReceiverRebootParam(const AddressPair& address_pair);

  /**
   * \brief Resets the session id and reboot flag for an existing entry in the
   * sd_multicast_receiver_reboot_management_map.
   * When reboot is detected for a provider daemon on reception of unicast message, we reset the session id and reboot
   * flag for multicast from that provider to avoid duplicate notification of reboot.
   * \param address_pair structure to store ip address and port for message.
   */
  void ResetMulticastReceiverRebootParam(const AddressPair& address_pair);

 private:
  /**
   * \brief Maps the information of unicast message sender to the session_id and reboot flag.
   */
  RebootManagementMap sd_unicast_sender_reboot_management_map_;

  /**
   * \brief Maps the information of unicast message receiver to the session_id and reboot flag.
   */
  RebootManagementMap sd_unicast_receiver_reboot_management_map_;

  /**
   * \brief Maps the information of multicast message receiver to the session_id and reboot flag.
   */
  RebootManagementMap sd_multicast_receiver_reboot_management_map_;

  /**
   * \brief Maintains the information of multicast message sender session_id and reboot flag.
   */
  RebootParam sd_multicast_sender_reboot_param_;

  FRIEND_TEST(RebootManagement, WrapUnicastSessionId);   /**< Friend declaration for testing. */
  FRIEND_TEST(RebootManagement, WrapMulticastSessionId); /**< Friend declaration for testing. */
};
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_REBOOT_MANAGEMENT_H_
