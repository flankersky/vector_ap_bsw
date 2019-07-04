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
/**        \file  DM_IPC_proxy_someip_method_manager.cc
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/internal/unexpected_application_error_exception.h"
#include "ara/com/internal/unexpected_protocol_state_exception.h"
#include "ara/com/service_not_available_exception.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy_someip_binding.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/* ---- Method 'RequestData' ------------------------------------------- */

DM_IPCProxySomeIpMethodManagerRequestData::DM_IPCProxySomeIpMethodManagerRequestData(
    DM_IPCProxySomeIpBinding& proxy_binding)
    : next_session_id_(0U), proxy_binding_(proxy_binding) {}

ara::com::Future<proxy::methods::RequestData::Output> DM_IPCProxySomeIpMethodManagerRequestData::HandleMethodRequest(
    uint32& handle, MetaInfoType& meta_info, DataArrayType& request_data) {
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
  header.service_id_ = DM_IPCProxySomeIpBinding::kServiceId;
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
  /* ImplementationDataTypeValue serialization for element 'handle' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_handle{
        &serializer};
    serializer_handle.PushBack(handle);
    serializer_handle.Close();
  }
  /* ImplementationDataTypeMap serialization for element 'meta_info' (/ara/diag/types/MetaInfoType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_meta_info{&serializer};
    for (const auto& meta_info_element : meta_info) {
      /* ImplementationDataTypeTypeReference serialization for element 'MetaInfoKey'
       * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_meta_info)>
            serializer_meta_info_MetaInfoKey{&serializer_meta_info};
        serializer_meta_info_MetaInfoKey.PushBack(
            static_cast<std::underlying_type<MetaInfoKeyType>::type>(meta_info_element.first));
        serializer_meta_info_MetaInfoKey.Close();
      }
      /* ImplementationDataTypeString serialization for element 'MetaInfoValue'
       * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
      {
        marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy,
                                              decltype(serializer_meta_info)>
            serializer_meta_info_MetaInfoValue{&serializer_meta_info};
        const std::uint8_t bom0{0xEFU};
        serializer_meta_info_MetaInfoValue.PushBack(bom0);
        const std::uint8_t bom1{0xBBU};
        serializer_meta_info_MetaInfoValue.PushBack(bom1);
        const std::uint8_t bom2{0xBFU};
        serializer_meta_info_MetaInfoValue.PushBack(bom2);
        for (const auto& meta_info_element_second_element : meta_info_element.second) {
          std::uint8_t c = meta_info_element_second_element;
          serializer_meta_info_MetaInfoValue.PushBack(c);
        }
        serializer_meta_info_MetaInfoValue.Close();
      }
    }
    serializer_meta_info.Close();
  }
  /* ImplementationDataTypeVector serialization for element 'request_data' (/ara/diag/types/DataArrayType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_request_data{&serializer};
    for (const auto& request_data_element : request_data) {
      /* ImplementationDataTypeValue serialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_request_data)>
            serializer_request_data_byte{&serializer_request_data};
        serializer_request_data_byte.PushBack(request_data_element);
        serializer_request_data_byte.Close();
      }
    }
    serializer_request_data.Close();
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
class DeserializeDM_IPCRequestDataresponse_data {
 public:
  explicit DeserializeDM_IPCRequestDataresponse_data(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> Deserializeresponse_data_byte(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 response_data_byte{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__response_data_byte{parent};
    deserialization_ok = deserializer__response_data_byte.PopFront(response_data_byte) > 0U;
    return std::make_pair(response_data_byte, deserialization_ok);
  }

  /* ImplementationDataTypeVector deserialization for element 'response_data' (/ara/diag/types/DataArrayType) */
  template <typename ParentDeserializer>
  std::pair<DataArrayType, bool> Deserializeresponse_data(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__response_data{parent};
    DataArrayType response_data{};
    while (deserializer__response_data.HasDataRemaining()) {
      const auto ret = Deserializeresponse_data_byte(&deserializer__response_data);
      deserialization_ok &= ret.second;
      if (!deserialization_ok) {
        break;
      }

      response_data.push_back(ret.first);
    }
    return std::make_pair(response_data, deserialization_ok);
  }

  DataArrayType StartDeserialization() {
    auto ret = Deserializeresponse_data<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void DM_IPCProxySomeIpMethodManagerRequestData::HandleMethodResponse(
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
      case proxy::application_errors::Failed::kErrorCode:
        HandleMethodResponseFailed(std::move(p));
        break;
      default:
        p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedApplicationErrorException()));
        break;
    }
  } else if (return_code == ::someip_posix_common::someip::SomeIpReturnCode::kUnknownService) {
    p.set_exception(std::make_exception_ptr(ara::com::ServiceNotAvailableException("DM_IPC")));
  } else {
    p.set_exception(std::make_exception_ptr(ara::com::internal::UnexpectedProtocolStateException()));
  }
}

template <typename RootDeserializer>
void DM_IPCProxySomeIpMethodManagerRequestData::HandleMethodResponseOk(RootDeserializer&& deserializer, Promise&& p) {
  proxy::methods::RequestData::Output ret{};
  // Deserialize method return value(s)
  // Create one deserializer object for argument response_data
  DeserializeDM_IPCRequestDataresponse_data<typename std::decay<RootDeserializer>::type> arg_response_data_deserializer{
      &deserializer};
  DataArrayType response_data = arg_response_data_deserializer.StartDeserialization();

  ret.response_data = response_data;

  // Set value of promise, notify future of proxy user
  p.set_value(ret);
}

void DM_IPCProxySomeIpMethodManagerRequestData::HandleMethodResponseFailed(Promise&& p) {
  // Deserialize application error context arguments
  /* No arguments to deserialize */

  // Set occurred ApplicationError in promise, notify future of proxy user
  p.set_exception(std::make_exception_ptr(proxy::application_errors::Failed{}));
}

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
