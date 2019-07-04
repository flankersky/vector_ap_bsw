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
/**        \file  someip_posix_application_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_APPLICATION_INTERFACE_H_
#define LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_APPLICATION_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someip_posix {

/**
 * \brief Interface class for callbacks
 */
class SomeipPosixApplicationInterface {
 public:
  virtual ~SomeipPosixApplicationInterface() = default;

  /**
   * \brief Receive handler for SOME/IP messages.
   */
  virtual void HandleReceive(someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::SomeIpPacket packet) = 0;

  /**
   * \brief Handler for notifying about offered services.
   */
  virtual void HandleFindService(someip_posix_common::someip::ServiceInstanceContainer service_instances) = 0;

  /**
   * \brief Handler for notifying about event subscription state change.
   */
  virtual void HandleEventSubscriptionStateUpdate(
      const someip_posix_common::someip::EventSubscriptionState& event_subscription_state_entry) = 0;
};

}  // namespace someip_posix

#endif  // LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_APPLICATION_INTERFACE_H_
