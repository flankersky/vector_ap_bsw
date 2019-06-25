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
/**        \file  calculatorInterface_proxy_someip_event_manager.h
 *        \brief  SOME/IP proxy event handling for events and field notifications of service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_EVENT_MANAGER_H_
#define ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include "ara-someip-posix/e2e_marshalling.h"
#include "ara-someip-posix/service_proxy_someip_event_interface.h"
#include "ara/com/internal/proxy_event.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"
#include "vector/calculatorService/calculatorInterface.h"

namespace vector {
namespace calculatorService {

// Forward-declaration for back-reference
class CalculatorInterfaceProxySomeIpBinding;

/**
 * \brief SOME/IP Proxy event manager for event 'divisionByZero'.
 * \details Handles event subscriptions and notification deserialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpEventManagerDivisionByZero
    : public ara::com::someip_posix::ServiceProxySomeIpEventInterface,
      public ara::com::internal::ProxyImplEvent<vector::calculatorService::boolean> {
 public:
  /**
   * \brief Constructor
   * \param proxy_binding Reference to the SOME/IP proxy binding used for event subscription handling.
   */
  explicit CalculatorInterfaceProxySomeIpEventManagerDivisionByZero(
      CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Subscribe to the event 'divisionByZero'
   * \param service_event Related frontend event object. Used for notification handling
   * \param cache_policy Update policy of the event cache
   * \param cache_size Size limit of the event cache
   */
  void Subscribe(ara::com::internal::ProxyEventBase* service_event, ara::com::EventCacheUpdatePolicy cache_policy,
                 std::size_t cache_size);

  /**
   * \brief Unsubscribe from the event 'divisionByZero'
   */
  void Unsubscribe();

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Handle a received event notification
   * \param header Deserialized SOME/IP header
   * \param deserializer Root deserializer containing the payload.
   */
  void HandleEventNotification(RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Handle an update of the subscription state of the event
   * \param state the updated event subscription state
   */
  void HandleEventSubscriptionStateUpdate(ara::com::SubscriptionState state) override;

  /**
   * \brief SOME/IP event identifier for this event.
   */
  static constexpr ::someip_posix_common::someip::EventId event_id_{0xB26DU};

 private:
  /** Proxy binding used for event subscription handling */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** The related frontend service event used to notify about new events. */
  ara::com::internal::ProxyEventBase* service_event_;
};

/**
 * \brief SOME/IP Proxy event manager for event 'divideResult'.
 * \details Handles event subscriptions and notification deserialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpFieldNotifierDivideResult
    : public ara::com::someip_posix::ServiceProxySomeIpEventInterface,
      public ara::com::internal::ProxyImplEvent<vector::calculatorService::uint32> {
 public:
  /**
   * \brief Constructor
   * \param proxy_binding Reference to the SOME/IP proxy binding used for event subscription handling.
   */
  explicit CalculatorInterfaceProxySomeIpFieldNotifierDivideResult(
      CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Subscribe to the event 'divideResult'
   * \param service_event Related frontend event object. Used for notification handling
   * \param cache_policy Update policy of the event cache
   * \param cache_size Size limit of the event cache
   */
  void Subscribe(ara::com::internal::ProxyEventBase* service_event, ara::com::EventCacheUpdatePolicy cache_policy,
                 std::size_t cache_size);

  /**
   * \brief Unsubscribe from the event 'divideResult'
   */
  void Unsubscribe();

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Handle a received event notification
   * \param header Deserialized SOME/IP header
   * \param deserializer Root deserializer containing the payload.
   */
  void HandleEventNotification(RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Handle an update of the subscription state of the event
   * \param state the updated event subscription state
   */
  void HandleEventSubscriptionStateUpdate(ara::com::SubscriptionState state) override;

  /**
   * \brief SOME/IP event identifier for this event.
   */
  static constexpr ::someip_posix_common::someip::EventId event_id_{0x80F2U};

 private:
  /** Proxy binding used for event subscription handling */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** The related frontend service event used to notify about new events. */
  ara::com::internal::ProxyEventBase* service_event_;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_EVENT_MANAGER_H_
