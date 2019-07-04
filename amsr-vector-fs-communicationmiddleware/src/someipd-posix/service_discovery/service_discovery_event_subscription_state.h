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
/**        \file  service_discovery_event_subscription_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someipd_posix/routing/message.h"

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief Service Discovery event subscription states.
 */
enum class ServiceDiscoveryEventSubscriptionState : std::uint8_t {
  kSubscribed = 0,           ///< 'SUBSCRIBED' state
  kNotSubscribed = 1,        ///< 'NOT_SUBSCRIBED' state
  kSubscriptionPending = 2,  ///< 'SUBSCRIPTION_PENDING' state
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_EVENT_SUBSCRIPTION_STATE_H_
