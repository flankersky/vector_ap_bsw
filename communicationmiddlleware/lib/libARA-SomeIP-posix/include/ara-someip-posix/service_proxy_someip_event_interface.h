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
/**        \file  service_proxy_someip_event_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_SERVICE_PROXY_SOMEIP_EVENT_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_SERVICE_PROXY_SOMEIP_EVENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Type-alias for shortening.
 */
using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
    ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

/**
 * \brief Event handling interface of an ServiceProxy for SOME/IP
 */
class ServiceProxySomeIpEventInterface {
 public:
  /**
   * \brief Destructor
   */
  virtual ~ServiceProxySomeIpEventInterface() = default;

  /**
   * \brief Handle a received event notification
   *
   * \param deserializer Root deserializer
   */
  virtual void HandleEventNotification(RootDeserializerAlias&& deserializer) = 0;

  /**
   * \brief Handle an update of the subscription state of the event
   * \param state the updated event subscription state
   */
  virtual void HandleEventSubscriptionStateUpdate(ara::com::SubscriptionState state) = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_SERVICE_PROXY_SOMEIP_EVENT_INTERFACE_H_
