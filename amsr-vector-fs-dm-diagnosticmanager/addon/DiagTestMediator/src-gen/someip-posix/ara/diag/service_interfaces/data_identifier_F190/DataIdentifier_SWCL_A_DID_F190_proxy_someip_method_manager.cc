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
/**        \file  DataIdentifier_SWCL_A_DID_F190_proxy_someip_method_manager.cc
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service
 *'DataIdentifier_SWCL_A_DID_F190'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/internal/unexpected_application_error_exception.h"
#include "ara/com/internal/unexpected_protocol_state_exception.h"
#include "ara/com/service_not_available_exception.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy_someip_binding.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/* ---- Method 'Read' ------------------------------------------- */

DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead::DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead(
    DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Read::Output>
DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead::HandleMethodRequest(MetaInfoType& MetaInfo) {
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
  header.service_id_ = DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding::kServiceId;
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
class DeserializeDataIdentifier_SWCL_A_DID_F190ReaddataRecord_vin {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_F190ReaddataRecord_vin(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'Element'
   * (/ara/diag/types/fixed_size_array_with_17_uint8_items/Element) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> DeserializedataRecord_vin_Element(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 dataRecord_vin_Element{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__dataRecord_vin_Element{parent};
    deserialization_ok = deserializer__dataRecord_vin_Element.PopFront(dataRecord_vin_Element) > 0U;
    return std::make_pair(dataRecord_vin_Element, deserialization_ok);
  }

  /* ImplementationDataTypeArray deserialization for element 'dataRecord_vin'
   * (/ara/diag/types/fixed_size_array_with_17_uint8_items) */
  template <typename ParentDeserializer>
  std::pair<fixed_size_array_with_17_uint8_items, bool> DeserializedataRecord_vin(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::Deserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__dataRecord_vin{parent};
    fixed_size_array_with_17_uint8_items dataRecord_vin{};
    for (auto& element_n : dataRecord_vin) {
      const auto ret = DeserializedataRecord_vin_Element(&deserializer__dataRecord_vin);
      deserialization_ok &= ret.second;
      if (!deserialization_ok) {
        break;
      }
      element_n = ret.first;
    }
    return std::make_pair(dataRecord_vin, deserialization_ok);
  }

  fixed_size_array_with_17_uint8_items StartDeserialization() {
    auto ret = DeserializedataRecord_vin<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_F190ReaderrorContext {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_F190ReaderrorContext(RootDeserializer* root) : deserializer_{root} {}

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

void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead::HandleMethodResponse(
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
      case proxy::application_errors::UDSServiceFailedOnRead::kErrorCode:
        HandleMethodResponseUDSServiceFailedOnRead(std::move(deserializer), std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("DataIdentifier_SWCL_A_DID_F190")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead::HandleMethodResponseOk(RootDeserializer&& deserializer,
                                                                                        Promise&& p) {
  proxy::methods::Read::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument dataRecord_vin
  DeserializeDataIdentifier_SWCL_A_DID_F190ReaddataRecord_vin<typename std::decay<RootDeserializer>::type>
      arg_dataRecord_vin_deserializer{&deserializer};
  fixed_size_array_with_17_uint8_items dataRecord_vin = arg_dataRecord_vin_deserializer.StartDeserialization();

  ret.dataRecord_vin = dataRecord_vin;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

template <typename RootDeserializer>
void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerRead::HandleMethodResponseUDSServiceFailedOnRead(
    RootDeserializer&& deserializer, Promise&& p) {
  // Deserialize application error context arguments
  // Create one deserializer object for argument errorContext
  DeserializeDataIdentifier_SWCL_A_DID_F190ReaderrorContext<typename std::decay<RootDeserializer>::type>
      arg_errorContext_deserializer{&deserializer};
  UDSResponseCodeType errorContext = arg_errorContext_deserializer.StartDeserialization();

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::UDSServiceFailedOnRead{errorContext}));
}

/* ---- Method 'Write' ------------------------------------------- */

DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite::
    DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite(
        DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Write::Output>
DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite::HandleMethodRequest(
    MetaInfoType& MetaInfo, fixed_size_array_with_17_uint8_items& dataRecord_vin) {
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
  header.service_id_ = DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding::kServiceId;
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
  /* ImplementationDataTypeArray serialization for element 'dataRecord_vin'
   * (/ara/diag/types/fixed_size_array_with_17_uint8_items) */
  {
    marshaller::Serializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_dataRecord_vin{&serializer};
    for (const auto& dataRecord_vin_element : dataRecord_vin) {
      /* ImplementationDataTypeValue serialization for element 'Element'
       * (/ara/diag/types/fixed_size_array_with_17_uint8_items/Element) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_dataRecord_vin)>
            serializer_dataRecord_vin_Element{&serializer_dataRecord_vin};
        serializer_dataRecord_vin_Element.PushBack(dataRecord_vin_element);
        serializer_dataRecord_vin_Element.Close();
      }
    }
    serializer_dataRecord_vin.Close();
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
class DeserializeDataIdentifier_SWCL_A_DID_F190WriteerrorContext {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_F190WriteerrorContext(RootDeserializer* root) : deserializer_{root} {}

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

void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite::HandleMethodResponse(
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
      case proxy::application_errors::UDSServiceFailedOnWrite::kErrorCode:
        HandleMethodResponseUDSServiceFailedOnWrite(std::move(deserializer), std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("DataIdentifier_SWCL_A_DID_F190")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite::HandleMethodResponseOk(Promise&& p) {
  proxy::methods::Write::Output ret{};
  // Deserialize method return value(s)
  /* No arguments to deserialize */

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

template <typename RootDeserializer>
void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerWrite::HandleMethodResponseUDSServiceFailedOnWrite(
    RootDeserializer&& deserializer, Promise&& p) {
  // Deserialize application error context arguments
  // Create one deserializer object for argument errorContext
  DeserializeDataIdentifier_SWCL_A_DID_F190WriteerrorContext<typename std::decay<RootDeserializer>::type>
      arg_errorContext_deserializer{&deserializer};
  UDSResponseCodeType errorContext = arg_errorContext_deserializer.StartDeserialization();

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::UDSServiceFailedOnWrite{errorContext}));
}

/* ---- Method 'Cancel' ------------------------------------------- */

DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel::
    DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel(
        DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::Cancel::Output>
DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel::HandleMethodRequest(MetaInfoType& MetaInfo) {
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
  header.service_id_ = DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding::kServiceId;
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

void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel::HandleMethodResponse(
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
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("DataIdentifier_SWCL_A_DID_F190")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

void DataIdentifier_SWCL_A_DID_F190ProxySomeIpMethodManagerCancel::HandleMethodResponseOk(Promise&& p) {
  proxy::methods::Cancel::Output ret{};

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
