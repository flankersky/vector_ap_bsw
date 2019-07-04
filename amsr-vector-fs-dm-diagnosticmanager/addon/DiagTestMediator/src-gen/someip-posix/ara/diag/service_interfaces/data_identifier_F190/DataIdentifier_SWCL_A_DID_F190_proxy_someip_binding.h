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
/**        \file  DataIdentifier_SWCL_A_DID_F190_proxy_someip_binding.h
 *        \brief  SOME/IP binding of service proxy for service 'DataIdentifier_SWCL_A_DID_F190'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_SOMEIP_BINDING_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_SOMEIP_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <tuple>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_interface.h"
#include "ara-someip-posix/service_proxy_someip_event_interface.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_impl_interface.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_someip_event_manager.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_someip_method_manager.h"
#include "ara/log/logging.hpp"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/**
 * \brief SOME/IP proxy binding of Service 'DataIdentifier_SWCL_A_DID_F190'.
 * \details Handles serialization and deserialization of all method calls, events, etc.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding : public DataIdentifier_SWCL_A_DID_F190ProxyImplInterface,
                                                         public ara::com::someip_posix::AraComSomeIpProxyInterface {
 public:
  /**
   * \brief Generated SOME/IP related service ID.
   */
  static constexpr someip_posix_common::someip::ServiceId kServiceId{0x841U};

  /**
   * \brief SOME/IP major version of this service interface deployment.
   */
  static constexpr ::someip_posix_common::someip::MajorVersion kMajorVersion{1U};

  /**
   * \brief Constructor
   * \param instance_id SOME/IP instance identifier to be used
   * \param someip_binding Reference to the related SOME/IP transport binding
   */
  explicit DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding(
      someip_posix_common::someip::InstanceId instance_id,
      ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding);

  /**
   * \brief Destructor releases the proxy-binding instance in the AraComSomeIpBinding.
   */
  virtual ~DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding();

  /**
   * \brief Get the ProxyId this ProxyImplInterface can handle.
   * \returns The ProxyId this ProxyImplInterface can handle.
   */
  ara::com::internal::ProxyId GetProxyId() const override {
    return DataIdentifier_SWCL_A_DID_F190Proxy::GetProxyIdStatic();
  }

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
   * \brief Serialization implementation of service method 'Read'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         dataRecord_vin OUT parameter of type fixed_size_array_with_17_uint8_items
   */
  ara::com::Future<proxy::methods::Read::Output> MethodRead(MetaInfoType& MetaInfo) override {
    return method_manager_Read_.HandleMethodRequest(MetaInfo);
  }

  /**
   * \brief Serialization implementation of service method 'Write'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param dataRecord_vin IN parameter of type fixed_size_array_with_17_uint8_items
   */
  ara::com::Future<proxy::methods::Write::Output> MethodWrite(
      MetaInfoType& MetaInfo, fixed_size_array_with_17_uint8_items& dataRecord_vin) override {
    return method_manager_Write_.HandleMethodRequest(MetaInfo, dataRecord_vin);
  }

  /**
   * \brief Serialization implementation of service method 'Cancel'.
   * \param MetaInfo IN parameter of type MetaInfoType
   */
  ara::com::Future<proxy::methods::Cancel::Output> MethodCancel(MetaInfoType& MetaInfo) override {
    return method_manager_Cancel_.HandleMethodRequest(MetaInfo);
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

  /** Method request/response manager for proxy method 'Read' */
  DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead method_manager_Read_;

  /** Method request/response manager for proxy method 'Write' */
  DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite method_manager_Write_;

  /** Method request/response manager for proxy method 'Cancel' */
  DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel method_manager_Cancel_;

  /* ---- Event manager -------------------------------------------------------------------------------------------- */

  /* ---- Field method/event manager ------------------------------------------------------------------------------- */
};

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_SOMEIP_BINDING_H_
