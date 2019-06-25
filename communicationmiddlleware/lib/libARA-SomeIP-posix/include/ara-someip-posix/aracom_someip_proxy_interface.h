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
/**        \file  aracom_someip_proxy_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/types.h"
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
 * \brief Interface class for ara::com SOME/IP proxy implementations.
 */
class AraComSomeIpProxyInterface {
 public:
  virtual ~AraComSomeIpProxyInterface() = default;

  /**
   * \brief Receive handler for method responses.
   * \param header SOME/IP header (deserialized)
   * \param deserializer Root deserializer holding the received bytestream.
   */
  virtual void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                                    RootDeserializerAlias&& deserializer) = 0;

  /**
   * \brief Receive handler for event notifications.
   * \param header SOME/IP header (deserialized)
   * \param deserializer Root deserializer holding the received bytestream.
   */
  virtual void HandleEventNotification(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                                       RootDeserializerAlias&& deserializer) = 0;

  /**
   * \brief Handle an update of the subscription state of the event
   * \param event_id The event identifier
   * \param state the updated event subscription state
   */
  virtual void HandleEventSubscriptionStateUpdate(const someip_posix_common::someip::EventId event_id,
                                                  ara::com::SubscriptionState state) = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INTERFACE_H_
