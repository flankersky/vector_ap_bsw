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
/**        \file  RoutineControl_SWCL_A_RID_3009_proxy_someip_method_manager.h
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_SOMEIP_METHOD_MANAGER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

// Forward-declaration for back-reference
class RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding;

/**
 * \brief SOME/IP Proxy method manager for method 'Start'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Start::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x76U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart(
      RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'Start'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::routine_control_3009::MetaInfoType
   * \param Req_Reset Input argument of type ara::diag::service_interfaces::routine_control_3009::boolean
   * \param Req_CounterLimit Input argument of type ara::diag::service_interfaces::routine_control_3009::uint32
   * \return ara::com::Future for return value of service method 'Start'
   **/
  ara::com::Future<proxy::methods::Start::Output> HandleMethodRequest(MetaInfoType& MetaInfo, boolean& Req_Reset,
                                                                      uint32& Req_CounterLimit);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'Start'.
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
   * \brief Deserialize response and set promise with application error 'UDSServiceFailedOnStart'
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseUDSServiceFailedOnStart(RootDeserializer&& deserializer, Promise&& p);

  /** Collection of pending requests for method 'Start' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'Stop'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Stop::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x77U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop(
      RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'Stop'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::routine_control_3009::MetaInfoType
   * \return ara::com::Future for return value of service method 'Stop'
   **/
  ara::com::Future<proxy::methods::Stop::Output> HandleMethodRequest(MetaInfoType& MetaInfo);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'Stop'.
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
   * \brief Deserialize response and set promise with application error 'UDSServiceFailedOnStop'
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseUDSServiceFailedOnStop(RootDeserializer&& deserializer, Promise&& p);

  /** Collection of pending requests for method 'Stop' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'RequestResults'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::RequestResults::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x78U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults(
      RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'RequestResults'
   * \param MetaInfo Input argument of type ara::diag::service_interfaces::routine_control_3009::MetaInfoType
   * \return ara::com::Future for return value of service method 'RequestResults'
   **/
  ara::com::Future<proxy::methods::RequestResults::Output> HandleMethodRequest(MetaInfoType& MetaInfo);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'RequestResults'.
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
   * \brief Deserialize response and set promise with application error 'UDSServiceFailedOnRequestResults'
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseUDSServiceFailedOnRequestResults(RootDeserializer&& deserializer, Promise&& p);

  /** Collection of pending requests for method 'RequestResults' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_SOMEIP_METHOD_MANAGER_H_
