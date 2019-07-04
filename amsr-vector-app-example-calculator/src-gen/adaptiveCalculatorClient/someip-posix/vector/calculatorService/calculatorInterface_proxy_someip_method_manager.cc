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
/**        \file  calculatorInterface_proxy_someip_method_manager.cc
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_proxy_someip_method_manager.h"
#include <utility>
#include "ara/com/internal/unexpected_application_error_exception.h"
#include "ara/com/internal/unexpected_protocol_state_exception.h"
#include "ara/com/service_not_available_exception.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"
#include "vector/calculatorService/calculatorInterface_proxy_someip_binding.h"

namespace vector {
namespace calculatorService {

/* ---- Method 'subtract' ------------------------------------------- */

CalculatorInterfaceProxySomeIpMethodManagerSubtract::CalculatorInterfaceProxySomeIpMethodManagerSubtract(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Subtract::Output>
CalculatorInterfaceProxySomeIpMethodManagerSubtract::HandleMethodRequest(uint32& arg1, uint32& arg2) {
  ara::log::LogDebug() << __func__;

  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Protect concurrent modification of pending_requests_ queue and next_session_id_
  // Required in case proxy methods called by multiple threads
  std::lock_guard<std::mutex> guard(pending_requests_lock_);

  // Get client and session ID
  const ::someip_posix_common::someip::ClientId client_id = proxy_binding_.GetCliendId();
  const ::someip_posix_common::someip::SessionId session_id = next_session_id_;
  ++next_session_id_;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceProxySomeIpBinding::kServiceId;
  header.method_id_ = kMethodId;
  header.client_id_ = client_id;
  header.session_id_ = session_id;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kRequest;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'arg1' (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_arg1{&serializer};
    serializer_arg1.PushBack(arg1);
    serializer_arg1.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'arg2' (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_arg2{&serializer};
    serializer_arg2.PushBack(arg2);
    serializer_arg2.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Create promise for this request
  Promise promise;
  auto future = promise.get_future();
  // Store in pending requests list
  pending_requests_.emplace(session_id, std::move(promise));

  // Send packet to the ProxyBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  proxy_binding_.SendMethodRequest(std::move(buffer));
  return future;
}

template <typename RootDeserializer>
class DeserializecalculatorInterfaceSubtractresult {
 public:
  explicit DeserializecalculatorInterfaceSubtractresult(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializeresult(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 result{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__result{parent};
    deserialization_ok = deserializer__result.PopFront(result) > 0U;
    return std::make_pair(result, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializeresult<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpMethodManagerSubtract::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  const ::someip_posix_common::someip::SessionId session_id = header.session_id_;
  const ::someip_posix_common::someip::ClientId client_id = header.client_id_;
  assert(client_id == proxy_binding_.GetCliendId());  // Must match, is mirrored by the skeleton.
  const ::someip_posix_common::someip::MessageType message_type = header.message_type_;
  const ::someip_posix_common::someip::ReturnCode return_code = header.return_code_;

  Promise p;
  {
    // Protect concurrent modification of pending_requests queue
    // May occur if new packet received by reactor and concurrent method request by application
    std::lock_guard<std::mutex> guard(pending_requests_lock_);

    // Find the right promise to this session
    PendingRequests::iterator it = pending_requests_.find(session_id);
    assert(it != pending_requests_.end() && "No request found for the received response. Routing failed!");
    p = std::move(it->second);

    // Clear the request now from the map.
    // Fulfilling the promise must be the last thing done within this function.
    pending_requests_.erase(it);
  }

  /* Check protocol message type and return code to prevent access to the invalid packet stream */
  if (message_type == someip_posix_common::someip::SomeIpMessageType::kResponse &&
      return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    HandleMethodResponseOk(std::move(deserializer), std::move(p));
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("calculatorInterface")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void CalculatorInterfaceProxySomeIpMethodManagerSubtract::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                                 Promise&& p) {
  proxy::methods::Subtract::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument result
  DeserializecalculatorInterfaceSubtractresult<typename std::decay<RootDeserializer>::type> arg_result_deserializer{
      &deserializer};
  uint32 result = arg_result_deserializer.StartDeserialization();

  ret.result = result;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

/* ---- Method 'add' ------------------------------------------- */

CalculatorInterfaceProxySomeIpMethodManagerAdd::CalculatorInterfaceProxySomeIpMethodManagerAdd(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Add::Output> CalculatorInterfaceProxySomeIpMethodManagerAdd::HandleMethodRequest(
    uint32& arg1, uint32& arg2) {
  ara::log::LogDebug() << __func__;

  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Protect concurrent modification of pending_requests_ queue and next_session_id_
  // Required in case proxy methods called by multiple threads
  std::lock_guard<std::mutex> guard(pending_requests_lock_);

  // Get client and session ID
  const ::someip_posix_common::someip::ClientId client_id = proxy_binding_.GetCliendId();
  const ::someip_posix_common::someip::SessionId session_id = next_session_id_;
  ++next_session_id_;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceProxySomeIpBinding::kServiceId;
  header.method_id_ = kMethodId;
  header.client_id_ = client_id;
  header.session_id_ = session_id;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kRequest;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'arg1' (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_arg1{&serializer};
    serializer_arg1.PushBack(arg1);
    serializer_arg1.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'arg2' (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_arg2{&serializer};
    serializer_arg2.PushBack(arg2);
    serializer_arg2.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Create promise for this request
  Promise promise;
  auto future = promise.get_future();
  // Store in pending requests list
  pending_requests_.emplace(session_id, std::move(promise));

  // Send packet to the ProxyBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  proxy_binding_.SendMethodRequest(std::move(buffer));
  return future;
}

template <typename RootDeserializer>
class DeserializecalculatorInterfaceAddresult {
 public:
  explicit DeserializecalculatorInterfaceAddresult(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializeresult(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 result{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__result{parent};
    deserialization_ok = deserializer__result.PopFront(result) > 0U;
    return std::make_pair(result, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializeresult<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpMethodManagerAdd::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  const ::someip_posix_common::someip::SessionId session_id = header.session_id_;
  const ::someip_posix_common::someip::ClientId client_id = header.client_id_;
  assert(client_id == proxy_binding_.GetCliendId());  // Must match, is mirrored by the skeleton.
  const ::someip_posix_common::someip::MessageType message_type = header.message_type_;
  const ::someip_posix_common::someip::ReturnCode return_code = header.return_code_;

  Promise p;
  {
    // Protect concurrent modification of pending_requests queue
    // May occur if new packet received by reactor and concurrent method request by application
    std::lock_guard<std::mutex> guard(pending_requests_lock_);

    // Find the right promise to this session
    PendingRequests::iterator it = pending_requests_.find(session_id);
    assert(it != pending_requests_.end() && "No request found for the received response. Routing failed!");
    p = std::move(it->second);

    // Clear the request now from the map.
    // Fulfilling the promise must be the last thing done within this function.
    pending_requests_.erase(it);
  }

  /* Check protocol message type and return code to prevent access to the invalid packet stream */
  if (message_type == someip_posix_common::someip::SomeIpMessageType::kResponse &&
      return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    HandleMethodResponseOk(std::move(deserializer), std::move(p));
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("calculatorInterface")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void CalculatorInterfaceProxySomeIpMethodManagerAdd::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                            Promise&& p) {
  proxy::methods::Add::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument result
  DeserializecalculatorInterfaceAddresult<typename std::decay<RootDeserializer>::type> arg_result_deserializer{
      &deserializer};
  uint32 result = arg_result_deserializer.StartDeserialization();

  ret.result = result;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

/* ---- Method 'divide' ------------------------------------------- */

CalculatorInterfaceProxySomeIpMethodManagerDivide::CalculatorInterfaceProxySomeIpMethodManagerDivide(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Divide::Output> CalculatorInterfaceProxySomeIpMethodManagerDivide::HandleMethodRequest(
    uint32& divident, uint32& divisor) {
  ara::log::LogDebug() << __func__;

  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Protect concurrent modification of pending_requests_ queue and next_session_id_
  // Required in case proxy methods called by multiple threads
  std::lock_guard<std::mutex> guard(pending_requests_lock_);

  // Get client and session ID
  const ::someip_posix_common::someip::ClientId client_id = proxy_binding_.GetCliendId();
  const ::someip_posix_common::someip::SessionId session_id = next_session_id_;
  ++next_session_id_;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceProxySomeIpBinding::kServiceId;
  header.method_id_ = kMethodId;
  header.client_id_ = client_id;
  header.session_id_ = session_id;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kRequest;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'divident' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_divident{
        &serializer};
    serializer_divident.PushBack(divident);
    serializer_divident.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'divisor' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_divisor{
        &serializer};
    serializer_divisor.PushBack(divisor);
    serializer_divisor.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Create promise for this request
  Promise promise;
  auto future = promise.get_future();
  // Store in pending requests list
  pending_requests_.emplace(session_id, std::move(promise));

  // Send packet to the ProxyBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  proxy_binding_.SendMethodRequest(std::move(buffer));
  return future;
}

template <typename RootDeserializer>
class DeserializecalculatorInterfaceDivideresult {
 public:
  explicit DeserializecalculatorInterfaceDivideresult(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializeresult(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 result{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__result{parent};
    deserialization_ok = deserializer__result.PopFront(result) > 0U;
    return std::make_pair(result, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializeresult<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpMethodManagerDivide::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  const ::someip_posix_common::someip::SessionId session_id = header.session_id_;
  const ::someip_posix_common::someip::ClientId client_id = header.client_id_;
  assert(client_id == proxy_binding_.GetCliendId());  // Must match, is mirrored by the skeleton.
  const ::someip_posix_common::someip::MessageType message_type = header.message_type_;
  const ::someip_posix_common::someip::ReturnCode return_code = header.return_code_;

  Promise p;
  {
    // Protect concurrent modification of pending_requests queue
    // May occur if new packet received by reactor and concurrent method request by application
    std::lock_guard<std::mutex> guard(pending_requests_lock_);

    // Find the right promise to this session
    PendingRequests::iterator it = pending_requests_.find(session_id);
    assert(it != pending_requests_.end() && "No request found for the received response. Routing failed!");
    p = std::move(it->second);

    // Clear the request now from the map.
    // Fulfilling the promise must be the last thing done within this function.
    pending_requests_.erase(it);
  }

  /* Check protocol message type and return code to prevent access to the invalid packet stream */
  if (message_type == someip_posix_common::someip::SomeIpMessageType::kResponse &&
      return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    HandleMethodResponseOk(std::move(deserializer), std::move(p));
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("calculatorInterface")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void CalculatorInterfaceProxySomeIpMethodManagerDivide::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                               Promise&& p) {
  proxy::methods::Divide::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument result
  DeserializecalculatorInterfaceDivideresult<typename std::decay<RootDeserializer>::type> arg_result_deserializer{
      &deserializer};
  uint32 result = arg_result_deserializer.StartDeserialization();

  ret.result = result;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

/* ---- Field method 'divideResultGet' ------------------------------------------- */

CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet::CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::fields::DivideResult::Output>
CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet::HandleMethodRequest() {
  ara::log::LogDebug() << __func__;

  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Protect concurrent modification of pending_requests_ queue and next_session_id_
  // Required in case proxy methods called by multiple threads
  std::lock_guard<std::mutex> guard(pending_requests_lock_);

  // Get client and session ID
  const ::someip_posix_common::someip::ClientId client_id = proxy_binding_.GetCliendId();
  const ::someip_posix_common::someip::SessionId session_id = next_session_id_;
  ++next_session_id_;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceProxySomeIpBinding::kServiceId;
  header.method_id_ = kMethodId;
  header.client_id_ = client_id;
  header.session_id_ = session_id;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kRequest;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* No arguments to serialize */

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Create promise for this request
  Promise promise;
  auto future = promise.get_future();
  // Store in pending requests list
  pending_requests_.emplace(session_id, std::move(promise));

  // Send packet to the ProxyBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  proxy_binding_.SendMethodRequest(std::move(buffer));
  return future;
}

template <typename RootDeserializer>
class DeserializecalculatorInterfaceDivideResultGetout_val {
 public:
  explicit DeserializecalculatorInterfaceDivideResultGetout_val(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'out_val'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializeout_val(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 out_val{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__out_val{
        parent};
    deserialization_ok = deserializer__out_val.PopFront(out_val) > 0U;
    return std::make_pair(out_val, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializeout_val<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  const ::someip_posix_common::someip::SessionId session_id = header.session_id_;
  const ::someip_posix_common::someip::ClientId client_id = header.client_id_;
  assert(client_id == proxy_binding_.GetCliendId());  // Must match, is mirrored by the skeleton.
  const ::someip_posix_common::someip::MessageType message_type = header.message_type_;
  const ::someip_posix_common::someip::ReturnCode return_code = header.return_code_;

  Promise p;
  {
    // Protect concurrent modification of pending_requests queue
    // May occur if new packet received by reactor and concurrent method request by application
    std::lock_guard<std::mutex> guard(pending_requests_lock_);

    // Find the right promise to this session
    PendingRequests::iterator it = pending_requests_.find(session_id);
    assert(it != pending_requests_.end() && "No request found for the received response. Routing failed!");
    p = std::move(it->second);

    // Clear the request now from the map.
    // Fulfilling the promise must be the last thing done within this function.
    pending_requests_.erase(it);
  }

  /* Check protocol message type and return code to prevent access to the invalid packet stream */
  if (message_type == someip_posix_common::someip::SomeIpMessageType::kResponse &&
      return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    HandleMethodResponseOk(std::move(deserializer), std::move(p));
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("calculatorInterface")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void CalculatorInterfaceProxySomeIpFieldManagerDivideResultGet::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                                       Promise&& p) {
  proxy::fields::DivideResult::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument out_val
  DeserializecalculatorInterfaceDivideResultGetout_val<typename std::decay<RootDeserializer>::type>
      arg_out_val_deserializer{&deserializer};
  uint32 out_val = arg_out_val_deserializer.StartDeserialization();

  ret = out_val;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

/* ---- Field method 'divideResultSet' ------------------------------------------- */

CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet::CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::fields::DivideResult::Output>
CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet::HandleMethodRequest(
    vector::calculatorService::uint32& in_val) {
  ara::log::LogDebug() << __func__;

  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Protect concurrent modification of pending_requests_ queue and next_session_id_
  // Required in case proxy methods called by multiple threads
  std::lock_guard<std::mutex> guard(pending_requests_lock_);

  // Get client and session ID
  const ::someip_posix_common::someip::ClientId client_id = proxy_binding_.GetCliendId();
  const ::someip_posix_common::someip::SessionId session_id = next_session_id_;
  ++next_session_id_;

  // Fill SOME/IP header with data for the request
  someip_posix_common::someip::SomeIpMessageHeader header{};
  header.service_id_ = CalculatorInterfaceProxySomeIpBinding::kServiceId;
  header.method_id_ = kMethodId;
  header.client_id_ = client_id;
  header.session_id_ = session_id;
  header.message_type_ = someip_posix_common::someip::SomeIpMessageType::kRequest;
  header.protocol_version_ = someip_posix_common::someip::kProtocolVersion;
  header.interface_version_ = 1U;
  header.return_code_ = someip_posix_common::someip::SomeIpReturnCode::kOk;
  header.length_ = 0U;  // We cannot set the correct length yet

  // Create packet and serialize the header
  ::someip_posix_common::someip::SomeIpPacket buffer{new ::someip_posix_common::someip::PacketBuffer};

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(buffer)};

  // Serialize SOME/IP header
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header};

  // Serialize payload
  /* ImplementationDataTypeValue serialization for element 'in_val' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_in_val{
        &serializer};
    serializer_in_val.PushBack(in_val);
    serializer_in_val.Close();
  }

  // Close SOME/IP header serializer, update the SOME/IP length field
  header_serializer.Close();

  // Transfer back the ownership
  buffer = serializer.Close();

  // Create promise for this request
  Promise promise;
  auto future = promise.get_future();
  // Store in pending requests list
  pending_requests_.emplace(session_id, std::move(promise));

  // Send packet to the ProxyBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  proxy_binding_.SendMethodRequest(std::move(buffer));
  return future;
}

template <typename RootDeserializer>
class DeserializecalculatorInterfaceDivideResultSetout_val {
 public:
  explicit DeserializecalculatorInterfaceDivideResultSetout_val(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'out_val'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializeout_val(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 out_val{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__out_val{
        parent};
    deserialization_ok = deserializer__out_val.PopFront(out_val) > 0U;
    return std::make_pair(out_val, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializeout_val<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  const ::someip_posix_common::someip::SessionId session_id = header.session_id_;
  const ::someip_posix_common::someip::ClientId client_id = header.client_id_;
  assert(client_id == proxy_binding_.GetCliendId());  // Must match, is mirrored by the skeleton.
  const ::someip_posix_common::someip::MessageType message_type = header.message_type_;
  const ::someip_posix_common::someip::ReturnCode return_code = header.return_code_;

  Promise p;
  {
    // Protect concurrent modification of pending_requests queue
    // May occur if new packet received by reactor and concurrent method request by application
    std::lock_guard<std::mutex> guard(pending_requests_lock_);

    // Find the right promise to this session
    PendingRequests::iterator it = pending_requests_.find(session_id);
    assert(it != pending_requests_.end() && "No request found for the received response. Routing failed!");
    p = std::move(it->second);

    // Clear the request now from the map.
    // Fulfilling the promise must be the last thing done within this function.
    pending_requests_.erase(it);
  }

  /* Check protocol message type and return code to prevent access to the invalid packet stream */
  if (message_type == someip_posix_common::someip::SomeIpMessageType::kResponse &&
      return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    HandleMethodResponseOk(std::move(deserializer), std::move(p));
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("calculatorInterface")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void CalculatorInterfaceProxySomeIpFieldManagerDivideResultSet::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                                       Promise&& p) {
  proxy::fields::DivideResult::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument out_val
  DeserializecalculatorInterfaceDivideResultSetout_val<typename std::decay<RootDeserializer>::type>
      arg_out_val_deserializer{&deserializer};
  uint32 out_val = arg_out_val_deserializer.StartDeserialization();

  ret = out_val;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

}  // namespace calculatorService
}  // namespace vector
