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
/**        \file  calculatorInterface_proxy_someip_method_manager.h
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_METHOD_MANAGER_H_
#define ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"
#include "vector/calculatorService/calculatorInterface.h"

namespace vector {
namespace calculatorService {

// Forward-declaration for back-reference
class CalculatorInterfaceProxySomeIpBinding;

/**
 * \brief SOME/IP Proxy method manager for method 'subtract'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpMethodManagerSubtract {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Subtract::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x682U};

  /**
   * \brief Constructor sets a reference to calculatorInterfaceProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit CalculatorInterfaceProxySomeIpMethodManagerSubtract(CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'subtract'
   * \param arg1 Input argument of type vector::calculatorService::uint32
   * \param arg2 Input argument of type vector::calculatorService::uint32
   * \return ara::com::Future for return value of service method 'subtract'
   **/
  ara::com::Future<proxy::methods::Subtract::Output> HandleMethodRequest(uint32& arg1, uint32& arg2);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'subtract'.
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

  /** Collection of pending requests for method 'subtract' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'add'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpMethodManagerAdd {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Add::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x642U};

  /**
   * \brief Constructor sets a reference to calculatorInterfaceProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit CalculatorInterfaceProxySomeIpMethodManagerAdd(CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'add'
   * \param arg1 Input argument of type vector::calculatorService::uint32
   * \param arg2 Input argument of type vector::calculatorService::uint32
   * \return ara::com::Future for return value of service method 'add'
   **/
  ara::com::Future<proxy::methods::Add::Output> HandleMethodRequest(uint32& arg1, uint32& arg2);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'add'.
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

  /** Collection of pending requests for method 'add' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'divide'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpMethodManagerDivide {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::Divide::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x643U};

  /**
   * \brief Constructor sets a reference to calculatorInterfaceProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit CalculatorInterfaceProxySomeIpMethodManagerDivide(CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'divide'
   * \param divident Input argument of type vector::calculatorService::uint32
   * \param divisor Input argument of type vector::calculatorService::uint32
   * \return ara::com::Future for return value of service method 'divide'
   **/
  ara::com::Future<proxy::methods::Divide::Output> HandleMethodRequest(uint32& divident, uint32& divisor);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'divide'.
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

  /** Collection of pending requests for method 'divide' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'divideResultGet'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::fields::DivideResult::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x6A5U};

  /**
   * \brief Constructor sets a reference to calculatorInterfaceProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet(
      CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'divideResultGet'
   * \return ara::com::Future for return value of service method 'divideResultGet'
   **/
  ara::com::Future<proxy::fields::DivideResult::Output> HandleMethodRequest();

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'divideResultGet'.
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

  /** Collection of pending requests for method 'divideResultGet' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

/**
 * \brief SOME/IP Proxy method manager for method 'divideResultSet'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::fields::DivideResult::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x6A6U};

  /**
   * \brief Constructor sets a reference to calculatorInterfaceProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet(
      CalculatorInterfaceProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'divideResultSet'
   * \param in_val Input argument of type vector::calculatorService::uint32
   * \return ara::com::Future for return value of service method 'divideResultSet'
   **/
  ara::com::Future<proxy::fields::DivideResult::Output> HandleMethodRequest(vector::calculatorService::uint32& in_val);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'divideResultSet'.
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

  /** Collection of pending requests for method 'divideResultSet' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  CalculatorInterfaceProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORCLIENT_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_SOMEIP_METHOD_MANAGER_H_
