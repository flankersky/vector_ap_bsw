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
/**        \file  GenericUDSService_proxy_someip_binding.h
 *        \brief  SOME/IP binding of service proxy for service 'GenericUDSService'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_SOMEIP_BINDING_H_
#define SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_SOMEIP_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <tuple>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_interface.h"
#include "ara-someip-posix/service_proxy_someip_event_interface.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy_impl_interface.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy_someip_event_manager.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy_someip_method_manager.h"
#include "ara/log/logging.hpp"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/**
 * \brief SOME/IP proxy binding of Service 'GenericUDSService'.
 * \details Handles serialization and deserialization of all method calls, events, etc.
 * \remark generated
 */
class GenericUDSServiceProxySomeIpBinding : public GenericUDSServiceProxyImplInterface,
                                            public ara::com::someip_posix::AraComSomeIpProxyInterface {
 public:
  /**
   * \brief Generated SOME/IP related service ID.
   */
  static constexpr someip_posix_common::someip::ServiceId kServiceId{0x83FU};

  /**
   * \brief SOME/IP major version of this service interface deployment.
   */
  static constexpr ::someip_posix_common::someip::MajorVersion kMajorVersion{1U};

  /**
   * \brief Constructor
   * \param instance_id SOME/IP instance identifier to be used
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit GenericUDSServiceProxySomeIpBinding(someip_posix_common::someip::InstanceId instance_id,
                                               ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Destructor releases the proxy-binding instance in the AraComSomeIpBinding.
   */
  virtual ~GenericUDSServiceProxySomeIpBinding();

  /**
   * \brief Get the ProxyId this ProxyImplInterface can handle.
   * \returns The ProxyId this ProxyImplInterface can handle.
   */
  ara::com::internal::ProxyId GetProxyId() const override { return GenericUDSServiceProxy::GetProxyIdStatic(); }

  /**
   * \brief Receive handler for method responses.
   * \details At the point of this method call from AraComSomeIpBinding, we only have to dispatch method id and session
   * id
   * and look in the correct pending request container, to serve the promise.
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                            RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Receive handler for event notifications.
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleEventNotification(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                               RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Handle an update of the subscription state of the event
   * \param event_id The event identifier
   * \param state The updated event subscription state
   */
  void HandleEventSubscriptionStateUpdate(const someip_posix_common::someip::EventId event_id,
                                          ara::com::SubscriptionState state) override;

  /**
   * \brief Forward a prepared packet to the AraComSomeIpBinding.
   *
   * \param packet the serialized packet from the method.
   */
  void SendMethodRequest(someip_posix_common::someip::SomeIpPacket&& packet);

  /**
   * \brief Subscribe to an event
   * \param event_id The SOME/IP event ID to subscribe to
   * \param event_manager The related event manager instance subscribing to the event
   */
  void SubscribeEvent(someip_posix_common::someip::EventId event_id,
                      ara::com::someip_posix::ServiceProxySomeIpEventInterface* event_manager);

  /**
   * \brief Unsubscribe from an event
   * \param event_id The SOME/IP event ID to unsubscribe from
   */
  void UnsubscribeEvent(someip_posix_common::someip::EventId event_id);

  /**
   * \brief Getter for accessing the SOME/IP binding.
   */
  ara::com::someip_posix::AraComSomeIpBindingInterface& GetBinding() { return someip_binding_; }

  /**
   * \brief Getter for the client id, that was assigned on construction of this object.
   */
  someip_posix_common::someip::ClientId GetCliendId() const { return client_id_; }

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Serialization implementation of service method 'Service'.
   * \param SID IN parameter of type uint8
   * \param RequestData IN parameter of type DataArrayType
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         ResponseData OUT parameter of type DataArrayType
   */
  ara::com::Future<proxy::methods::Service::Output> MethodService(uint8& SID, DataArrayType& RequestData,
                                                                  MetaInfoType& MetaInfo) override {
    return method_manager_Service_.HandleMethodRequest(SID, RequestData, MetaInfo);
  }

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
 private:
  /** Subscribed events with the related event manager */
  using EventSubscriptionsMap =
      std::map<someip_posix_common::someip::EventId, ara::com::someip_posix::ServiceProxySomeIpEventInterface*>;

  /** SOME/IP instance ID used by this binding. */
  someip_posix_common::someip::InstanceId instance_id_;

  /** SOME/IP related client ID used by this concrete proxy instance. */
  someip_posix_common::someip::ClientId client_id_;

  /** Related SOME/IP binding */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;

  /** Logger for tracing and debugging */
  ara::log::Logger& logger_;

  /** Event subscriptions */
  EventSubscriptionsMap event_subscriptions_;

  /** Mutex for protection of event_subscriptions_ */
  std::mutex event_subscriptions_lock_;

  /* ---- Methods manager ------------------------------------------------------------------------------------------ */

  /** Method request/response manager for proxy method 'Service' */
  GenericUDSServiceProxySomeIpMethodManagerService method_manager_Service_;

  /* ---- Event manager -------------------------------------------------------------------------------------------- */

  /* ---- Field method/event manager ------------------------------------------------------------------------------- */
};

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_SOMEIP_BINDING_H_
