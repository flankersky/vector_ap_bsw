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
/**        \file  RoutineControl_SWCL_A_RID_3009_proxy_someip_method_manager.cc
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/internal/unexpected_application_error_exception.h"
#include "ara/com/internal/unexpected_protocol_state_exception.h"
#include "ara/com/service_not_available_exception.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_proxy_someip_binding.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/* ---- Method 'Start' ------------------------------------------- */

RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart::
    RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart(
        RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Start::Output>
RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart::HandleMethodRequest(MetaInfoType& MetaInfo,
                                                                                 boolean& Req_Reset,
                                                                                 uint32& Req_CounterLimit) {
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
  header.service_id_ = RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding::kServiceId;
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
  /* ImplementationDataTypeMap serialization for element 'MetaInfo' (/ara/diag/types/MetaInfoType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_MetaInfo{&serializer};
    for (const auto& MetaInfo_element : MetaInfo) {
      /* ImplementationDataTypeTypeReference serialization for element 'MetaInfoKey'
       * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoKey{&serializer_MetaInfo};
        serializer_MetaInfo_MetaInfoKey.PushBack(
            static_cast<std::underlying_type<MetaInfoKeyType>::type>(MetaInfo_element.first));
        serializer_MetaInfo_MetaInfoKey.Close();
      }
      /* ImplementationDataTypeString serialization for element 'MetaInfoValue'
       * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
      {
        marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy,
                                              decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoValue{&serializer_MetaInfo};
        const std::uint8_t bom0{0xEFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom0);
        const std::uint8_t bom1{0xBBU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom1);
        const std::uint8_t bom2{0xBFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom2);
        for (const auto& MetaInfo_element_second_element : MetaInfo_element.second) {
          std::uint8_t c = MetaInfo_element_second_element;
          serializer_MetaInfo_MetaInfoValue.PushBack(c);
        }
        serializer_MetaInfo_MetaInfoValue.Close();
      }
    }
    serializer_MetaInfo.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'Req_Reset'
   * (/AUTOSAR_Platform/ImplementationDataTypes/boolean) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_Req_Reset{
        &serializer};
    serializer_Req_Reset.PushBack(Req_Reset);
    serializer_Req_Reset.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'Req_CounterLimit'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_Req_CounterLimit{
        &serializer};
    serializer_Req_CounterLimit.PushBack(Req_CounterLimit);
    serializer_Req_CounterLimit.Close();
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
class DeserializeRoutineControl_SWCL_A_RID_3009StarterrorContext {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StarterrorContext(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeTypeReference deserialization for element 'errorContext'
   * (/ara/diag/types/UDSResponseCodeType) */
  template <typename ParentDeserializer>
  std::pair<UDSResponseCodeType, bool> DeserializeerrorContext(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 errorContext{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__errorContext{
        parent};
    deserialization_ok = deserializer__errorContext.PopFront(errorContext) > 0U;
    return std::make_pair(static_cast<UDSResponseCodeType>(errorContext), deserialization_ok);
  }

  UDSResponseCodeType StartDeserialization() {
    auto ret = DeserializeerrorContext<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart::HandleMethodResponse(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;
  // Deserializer is not used in this particular case, because there is no return value to deserialize.
  (void)deserializer;

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
    HandleMethodResponseOk(std::move(p));
  } else if (message_type == someip_posix_common::someip::SomeIpMessageType::kError &&
             return_code >= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsStart &&
             return_code <= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd) {
    /* Get logical ApplicationError error code from SOME/IP return code */
    ara::com::internal::ApplicationErrorCode error_code = static_cast<ara::com::internal::ApplicationErrorCode>(
        return_code - ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset);
    /* Dispatch to ApplicationError specific deserialization method */
    switch (error_code) {
      case proxy::application_errors::UDSServiceFailedOnStart::kErrorCode:
        HandleMethodResponseUDSServiceFailedOnStart(std::move(deserializer), std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("RoutineControl_SWCL_A_RID_3009")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart::HandleMethodResponseOk(Promise&& p) {
  proxy::methods::Start::Output ret{};
  // Deserialize method return value(s)
  /* No arguments to deserialize */

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

template <typename RootDeserializer>
void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStart::HandleMethodResponseUDSServiceFailedOnStart(
    RootDeserializer&& deserializer, Promise&& p) {
  // Deserialize application error context arguments
  // Create one deserializer object for argument errorContext
  DeserializeRoutineControl_SWCL_A_RID_3009StarterrorContext<typename std::decay<RootDeserializer>::type>
      arg_errorContext_deserializer{&deserializer};
  UDSResponseCodeType errorContext = arg_errorContext_deserializer.StartDeserialization();

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::UDSServiceFailedOnStart{errorContext}));
}

/* ---- Method 'Stop' ------------------------------------------- */

RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop::RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop(
    RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Stop::Output>
RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop::HandleMethodRequest(MetaInfoType& MetaInfo) {
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
  header.service_id_ = RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding::kServiceId;
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
  /* ImplementationDataTypeMap serialization for element 'MetaInfo' (/ara/diag/types/MetaInfoType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_MetaInfo{&serializer};
    for (const auto& MetaInfo_element : MetaInfo) {
      /* ImplementationDataTypeTypeReference serialization for element 'MetaInfoKey'
       * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoKey{&serializer_MetaInfo};
        serializer_MetaInfo_MetaInfoKey.PushBack(
            static_cast<std::underlying_type<MetaInfoKeyType>::type>(MetaInfo_element.first));
        serializer_MetaInfo_MetaInfoKey.Close();
      }
      /* ImplementationDataTypeString serialization for element 'MetaInfoValue'
       * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
      {
        marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy,
                                              decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoValue{&serializer_MetaInfo};
        const std::uint8_t bom0{0xEFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom0);
        const std::uint8_t bom1{0xBBU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom1);
        const std::uint8_t bom2{0xBFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom2);
        for (const auto& MetaInfo_element_second_element : MetaInfo_element.second) {
          std::uint8_t c = MetaInfo_element_second_element;
          serializer_MetaInfo_MetaInfoValue.PushBack(c);
        }
        serializer_MetaInfo_MetaInfoValue.Close();
      }
    }
    serializer_MetaInfo.Close();
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
class DeserializeRoutineControl_SWCL_A_RID_3009StopResp_RemainingCounts {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StopResp_RemainingCounts(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'Resp_RemainingCounts'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> DeserializeResp_RemainingCounts(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 Resp_RemainingCounts{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__Resp_RemainingCounts{parent};
    deserialization_ok = deserializer__Resp_RemainingCounts.PopFront(Resp_RemainingCounts) > 0U;
    return std::make_pair(Resp_RemainingCounts, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = DeserializeResp_RemainingCounts<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009StoperrorContext {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StoperrorContext(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeTypeReference deserialization for element 'errorContext'
   * (/ara/diag/types/UDSResponseCodeType) */
  template <typename ParentDeserializer>
  std::pair<UDSResponseCodeType, bool> DeserializeerrorContext(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 errorContext{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__errorContext{
        parent};
    deserialization_ok = deserializer__errorContext.PopFront(errorContext) > 0U;
    return std::make_pair(static_cast<UDSResponseCodeType>(errorContext), deserialization_ok);
  }

  UDSResponseCodeType StartDeserialization() {
    auto ret = DeserializeerrorContext<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop::HandleMethodResponse(
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
  } else if (message_type == someip_posix_common::someip::SomeIpMessageType::kError &&
             return_code >= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsStart &&
             return_code <= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd) {
    /* Get logical ApplicationError error code from SOME/IP return code */
    ara::com::internal::ApplicationErrorCode error_code = static_cast<ara::com::internal::ApplicationErrorCode>(
        return_code - ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset);
    /* Dispatch to ApplicationError specific deserialization method */
    switch (error_code) {
      case proxy::application_errors::UDSServiceFailedOnStop::kErrorCode:
        HandleMethodResponseUDSServiceFailedOnStop(std::move(deserializer), std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("RoutineControl_SWCL_A_RID_3009")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                                        Promise&& p) {
  proxy::methods::Stop::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument Resp_RemainingCounts
  DeserializeRoutineControl_SWCL_A_RID_3009StopResp_RemainingCounts<typename std::decay<RootDeserializer>::type>
      arg_Resp_RemainingCounts_deserializer{&deserializer};
  uint32 Resp_RemainingCounts = arg_Resp_RemainingCounts_deserializer.StartDeserialization();

  ret.Resp_RemainingCounts = Resp_RemainingCounts;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

template <typename RootDeserializer>
void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerStop::HandleMethodResponseUDSServiceFailedOnStop(
    RootDeserializer&& deserializer, Promise&& p) {
  // Deserialize application error context arguments
  // Create one deserializer object for argument errorContext
  DeserializeRoutineControl_SWCL_A_RID_3009StoperrorContext<typename std::decay<RootDeserializer>::type>
      arg_errorContext_deserializer{&deserializer};
  UDSResponseCodeType errorContext = arg_errorContext_deserializer.StartDeserialization();

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::UDSServiceFailedOnStop{errorContext}));
}

/* ---- Method 'RequestResults' ------------------------------------------- */

RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults::
    RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults(
        RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::RequestResults::Output>
RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults::HandleMethodRequest(MetaInfoType& MetaInfo) {
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
  header.service_id_ = RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding::kServiceId;
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
  /* ImplementationDataTypeMap serialization for element 'MetaInfo' (/ara/diag/types/MetaInfoType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_MetaInfo{&serializer};
    for (const auto& MetaInfo_element : MetaInfo) {
      /* ImplementationDataTypeTypeReference serialization for element 'MetaInfoKey'
       * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoKey{&serializer_MetaInfo};
        serializer_MetaInfo_MetaInfoKey.PushBack(
            static_cast<std::underlying_type<MetaInfoKeyType>::type>(MetaInfo_element.first));
        serializer_MetaInfo_MetaInfoKey.Close();
      }
      /* ImplementationDataTypeString serialization for element 'MetaInfoValue'
       * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
      {
        marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy,
                                              decltype(serializer_MetaInfo)>
            serializer_MetaInfo_MetaInfoValue{&serializer_MetaInfo};
        const std::uint8_t bom0{0xEFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom0);
        const std::uint8_t bom1{0xBBU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom1);
        const std::uint8_t bom2{0xBFU};
        serializer_MetaInfo_MetaInfoValue.PushBack(bom2);
        for (const auto& MetaInfo_element_second_element : MetaInfo_element.second) {
          std::uint8_t c = MetaInfo_element_second_element;
          serializer_MetaInfo_MetaInfoValue.PushBack(c);
        }
        serializer_MetaInfo_MetaInfoValue.Close();
      }
    }
    serializer_MetaInfo.Close();
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
class DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsResp_RemainingCounts {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsResp_RemainingCounts(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'Resp_RemainingCounts'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> DeserializeResp_RemainingCounts(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 Resp_RemainingCounts{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__Resp_RemainingCounts{parent};
    deserialization_ok = deserializer__Resp_RemainingCounts.PopFront(Resp_RemainingCounts) > 0U;
    return std::make_pair(Resp_RemainingCounts, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = DeserializeResp_RemainingCounts<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009RequestResultserrorContext {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009RequestResultserrorContext(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeTypeReference deserialization for element 'errorContext'
   * (/ara/diag/types/UDSResponseCodeType) */
  template <typename ParentDeserializer>
  std::pair<UDSResponseCodeType, bool> DeserializeerrorContext(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 errorContext{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__errorContext{
        parent};
    deserialization_ok = deserializer__errorContext.PopFront(errorContext) > 0U;
    return std::make_pair(static_cast<UDSResponseCodeType>(errorContext), deserialization_ok);
  }

  UDSResponseCodeType StartDeserialization() {
    auto ret = DeserializeerrorContext<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults::HandleMethodResponse(
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
  } else if (message_type == someip_posix_common::someip::SomeIpMessageType::kError &&
             return_code >= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsStart &&
             return_code <= someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd) {
    /* Get logical ApplicationError error code from SOME/IP return code */
    ara::com::internal::ApplicationErrorCode error_code = static_cast<ara::com::internal::ApplicationErrorCode>(
        return_code - ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset);
    /* Dispatch to ApplicationError specific deserialization method */
    switch (error_code) {
      case proxy::application_errors::UDSServiceFailedOnRequestResults::kErrorCode:
        HandleMethodResponseUDSServiceFailedOnRequestResults(std::move(deserializer), std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("RoutineControl_SWCL_A_RID_3009")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults::HandleMethodResponseOk(
    RootDeserializer&& deserializer, Promise&& p) {
  proxy::methods::RequestResults::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument Resp_RemainingCounts
  DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsResp_RemainingCounts<
      typename std::decay<RootDeserializer>::type>
      arg_Resp_RemainingCounts_deserializer{&deserializer};
  uint32 Resp_RemainingCounts = arg_Resp_RemainingCounts_deserializer.StartDeserialization();

  ret.Resp_RemainingCounts = Resp_RemainingCounts;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

template <typename RootDeserializer>
void RoutineControl_SWCL_A_RID_3009ProxySomeIpMethodManagerRequestResults::
    HandleMethodResponseUDSServiceFailedOnRequestResults(RootDeserializer&& deserializer, Promise&& p) {
  // Deserialize application error context arguments
  // Create one deserializer object for argument errorContext
  DeserializeRoutineControl_SWCL_A_RID_3009RequestResultserrorContext<typename std::decay<RootDeserializer>::type>
      arg_errorContext_deserializer{&deserializer};
  UDSResponseCodeType errorContext = arg_errorContext_deserializer.StartDeserialization();

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::UDSServiceFailedOnRequestResults{errorContext}));
}

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
