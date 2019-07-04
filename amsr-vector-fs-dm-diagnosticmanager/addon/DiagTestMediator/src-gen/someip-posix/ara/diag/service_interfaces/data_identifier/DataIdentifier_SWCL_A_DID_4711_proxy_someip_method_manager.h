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
/**        \file  DataIdentifier_SWCL_A_DID_4711_proxy_someip_method_manager.h
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'DataIdentifier_SWCL_A_DID_4711'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_SOMEIP_METHOD_MANAGER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

// Forward-declaration for back-reference
class DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding;

/**
 * \brief SOME/IP Proxy method manager for method 'Read'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerRead {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Read::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x70U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerRead(
      DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'Read'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::data_identifier::MetaInfoType
   * \return ara::com::Future for return value of service method 'Read'
   **/
  ara::com::Future<proxy::methods::Read::Output> HandleMethodRequest(MetaInfoType& MetaInfo);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'Read'.
   *
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                            RootDeserializerAlias&& deserializer);

 private:
  /**
   * \brief Deserialize response and set promise values in case a valid response with return code OK was received.
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseOk(RootDeserializer&& deserializer, Promise&& p);

  /*
   * \brief Deserialize response and set promise with application error 'UDSServiceFailedOnRead'
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseUDSServiceFailedOnRead(RootDeserializer&& deserializer, Promise&& p);

  /** Collection of pending requests for method 'Read' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'Write'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerWrite {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Write::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x71U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerWrite(
      DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'Write'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::data_identifier::MetaInfoType
   * \param dataRecord_DataElement_one Input argument of type ara::diag::service_interfaces::data_identifier::uint8
   * \param dataRecord_DataElement_two Input argument of type ara::diag::service_interfaces::data_identifier::uint16
   * \return ara::com::Future for return value of service method 'Write'
   **/
  ara::com::Future<proxy::methods::Write::Output> HandleMethodRequest(MetaInfoType& MetaInfo,
                                                                      uint8& dataRecord_DataElement_one,
                                                                      uint16& dataRecord_DataElement_two);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'Write'.
   *
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                            RootDeserializerAlias&& deserializer);

 private:
  /**
   * \brief Deserialize response and set promise values in case a valid response with return code OK was received.
   * \param p Frontend method call promise
   */
  void HandleMethodResponseOk(Promise&& p);

  /*
   * \brief Deserialize response and set promise with application error 'UDSServiceFailedOnWrite'
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseUDSServiceFailedOnWrite(RootDeserializer&& deserializer, Promise&& p);

  /** Collection of pending requests for method 'Write' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'Cancel'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerCancel {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Cancel::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x72U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_4711ProxySomeIpMethodManagerCancel(
      DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'Cancel'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::data_identifier::MetaInfoType
   * \return ara::com::Future for return value of service method 'Cancel'
   **/
  ara::com::Future<proxy::methods::Cancel::Output> HandleMethodRequest(MetaInfoType& MetaInfo);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'Cancel'.
   *
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                            RootDeserializerAlias&& deserializer);

 private:
  /**
   * \brief Deserialize response and set promise values in case a valid response with return code OK was received.
   * \param p Frontend method call promise
   */
  void HandleMethodResponseOk(Promise&& p);

  /** Collection of pending requests for method 'Cancel' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  DataIdentifier_SWCL_A_DID_4711ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_SOMEIP_METHOD_MANAGER_H_
