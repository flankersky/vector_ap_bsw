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
/**        \file  DM_IPC_skeleton_someip_method_manager.cc
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/application_error_exception.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton_someip_binding.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/message_verification.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/* ---- Method 'RequestData' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeDM_IPCRequestDatahandle {
 public:
  explicit DeserializeDM_IPCRequestDatahandle(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'handle' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializehandle(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 handle{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__handle{parent};
    deserialization_ok = deserializer__handle.PopFront(handle) > 0U;
    return std::make_pair(handle, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializehandle<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeDM_IPCRequestDatameta_info {
 public:
  explicit DeserializeDM_IPCRequestDatameta_info(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeTypeReference deserialization for element 'MetaInfoKey'
   * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoKeyType, bool> Deserializemeta_info_MetaInfoKey(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 meta_info_MetaInfoKey{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__meta_info_MetaInfoKey{parent};
    deserialization_ok = deserializer__meta_info_MetaInfoKey.PopFront(meta_info_MetaInfoKey) > 0U;
    return std::make_pair(static_cast<MetaInfoKeyType>(meta_info_MetaInfoKey), deserialization_ok);
  }

  /* ImplementationDataTypeString deserialization for element 'MetaInfoValue'
   * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoValueType, bool> Deserializemeta_info_MetaInfoValue(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__meta_info_MetaInfoValue{parent};
    MetaInfoValueType str;
    const std::size_t str_length = deserializer__meta_info_MetaInfoValue.GetLengthFieldValue();
    str.reserve(str_length);
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(deserializer__meta_info_MetaInfoValue)>
        bom_deserializer{&deserializer__meta_info_MetaInfoValue};
    std::uint8_t b{};
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xEFU);
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xBBU);
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xBFU);
    if (deserialization_ok) {
      while (deserializer__meta_info_MetaInfoValue.HasDataRemaining()) {
        std::uint8_t c{};
        marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy,
                                 decltype(deserializer__meta_info_MetaInfoValue)>
            str_element_deserializer{&deserializer__meta_info_MetaInfoValue};
        deserialization_ok = str_element_deserializer.PopFront(c) > 0U;
        if (!deserialization_ok) {
          break;
        }
        str.push_back(c);
      }
    }
    return std::make_pair(str, deserialization_ok);
  }

  /* ImplementationDataTypeMap deserialization for element 'meta_info' (/ara/diag/types/MetaInfoType) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoType, bool> Deserializemeta_info(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__meta_info{parent};
    MetaInfoType meta_info{};
    while (deserializer__meta_info.HasDataRemaining()) {
      const auto ret_key = Deserializemeta_info_MetaInfoKey(&deserializer__meta_info);
      deserialization_ok &= ret_key.second;
      if (!deserialization_ok) {
        break;
      }
      const auto ret_value = Deserializemeta_info_MetaInfoValue(&deserializer__meta_info);
      deserialization_ok &= ret_value.second;
      if (!deserialization_ok) {
        break;
      }

      const auto ret_emplace = meta_info.emplace(std::make_pair(ret_key.first, ret_value.first));
      deserialization_ok &= ret_emplace.second;
      if (!deserialization_ok) {
        break;
      }
    }
    return std::make_pair(meta_info, deserialization_ok);
  }

  MetaInfoType StartDeserialization() {
    auto ret = Deserializemeta_info<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeDM_IPCRequestDatarequest_data {
 public:
  explicit DeserializeDM_IPCRequestDatarequest_data(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> Deserializerequest_data_byte(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 request_data_byte{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__request_data_byte{parent};
    deserialization_ok = deserializer__request_data_byte.PopFront(request_data_byte) > 0U;
    return std::make_pair(request_data_byte, deserialization_ok);
  }

  /* ImplementationDataTypeVector deserialization for element 'request_data' (/ara/diag/types/DataArrayType) */
  template <typename ParentDeserializer>
  std::pair<DataArrayType, bool> Deserializerequest_data(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__request_data{parent};
    DataArrayType request_data{};
    while (deserializer__request_data.HasDataRemaining()) {
      const auto ret = Deserializerequest_data_byte(&deserializer__request_data);
      deserialization_ok &= ret.second;
      if (!deserialization_ok) {
        break;
      }

      request_data.push_back(ret.first);
    }
    return std::make_pair(request_data, deserialization_ok);
  }

  DataArrayType StartDeserialization() {
    auto ret = Deserializerequest_data<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class DM_IPCSkeletonSomeIpFunctorRequestData : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param handle Create a snapshot of this method input argument for this functor.
   * \param meta_info Create a snapshot of this method input argument for this functor.
   * \param request_data Create a snapshot of this method input argument for this functor.
   */
  DM_IPCSkeletonSomeIpFunctorRequestData(DM_IPCSkeletonSomeIpMethodManagerRequestData* binding,
                                         DM_IPCSkeleton* skeleton,
                                         ::someip_posix_common::someip::SomeIpMessageHeader header,
                                         const uint32& handle, const MetaInfoType& meta_info,
                                         const DataArrayType& request_data)
      : binding_{binding},
        skeleton_{skeleton},
        header_{header},
        handle_{handle},
        meta_info_{meta_info},
        request_data_{request_data} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint32 handle = handle_;
    const MetaInfoType meta_info = meta_info_;
    const DataArrayType request_data = request_data_;

    /* Catch the possible errors of this method */
    try {
      const skeleton::methods::RequestData::Output out_val =
          skeleton_->RequestData(handle, meta_info, request_data).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const DM_IPCSomeIpResponsePathInfoMethodManagerRequestData response{header_, out_val};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::Failed&) {
      binding_->SendMethodResponseFailed(header_);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  DM_IPCSkeletonSomeIpMethodManagerRequestData* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  DM_IPCSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'handle' in this functor for processing at any time.
   */
  const uint32 handle_;

  /**
   * \brief Storing context of method argument 'meta_info' in this functor for processing at any time.
   */
  const MetaInfoType meta_info_;

  /**
   * \brief Storing context of method argument 'request_data' in this functor for processing at any time.
   */
  const DataArrayType request_data_;
};

DM_IPCSkeletonSomeIpMethodManagerRequestData::DM_IPCSkeletonSomeIpMethodManagerRequestData(
    DM_IPCSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
DM_IPCSkeletonSomeIpMethodManagerRequestData::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      DM_IPCSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      DM_IPCSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void DM_IPCSkeletonSomeIpMethodManagerRequestData::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  std::lock_guard<std::mutex> guard(pending_lock_);

  // SOME/IP message verification
  const someip_posix_common::someip::SomeIpMessageHeader& expected_header{GetExpectedRequestHeader()};
  const someip_posix_common::someip::SomeIpReturnCode return_code{
      someip_posix_common::someip::RequestMessageVerification::DoAllDeploymentChecks(expected_header, header)};
  if (return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    // Shortening
    namespace marshaller = someip_posix_common::someip::serialization;

    // Deserialize the method parameter(s) from payload
    // Create one deserializer object for argument handle
    DeserializeDM_IPCRequestDatahandle<typename std::decay<decltype(deserializer)>::type> arg_handle_deserializer{
        &deserializer};
    uint32 handle = arg_handle_deserializer.StartDeserialization();
    // Create one deserializer object for argument meta_info
    DeserializeDM_IPCRequestDatameta_info<typename std::decay<decltype(deserializer)>::type> arg_meta_info_deserializer{
        &deserializer};
    MetaInfoType meta_info = arg_meta_info_deserializer.StartDeserialization();
    // Create one deserializer object for argument request_data
    DeserializeDM_IPCRequestDatarequest_data<typename std::decay<decltype(deserializer)>::type>
        arg_request_data_deserializer{&deserializer};
    DataArrayType request_data = arg_request_data_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    DM_IPCSkeletonSomeIpFunctorRequestData functor{
        this, &skeleton_binding_.GetSkeleton(), header_request, handle, meta_info, request_data};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError() << "Method request verification failed for method DM_IPC::RequestData. Return code: "
                      << static_cast<std::uint16_t>(return_code);
  }
}

void DM_IPCSkeletonSomeIpMethodManagerRequestData::SendMethodResponse(
    const DM_IPCSomeIpResponsePathInfoMethodManagerRequestData& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::RequestData::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeVector serialization for element 'response_data' (/ara/diag/types/DataArrayType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_response_data{&serializer};
    for (const auto& out_val_response_data_element : out_val.response_data) {
      /* ImplementationDataTypeValue serialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_response_data)>
            serializer_response_data_byte{&serializer_response_data};
        serializer_response_data_byte.PushBack(out_val_response_data_element);
        serializer_response_data_byte.Close();
      }
    }
    serializer_response_data.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void DM_IPCSkeletonSomeIpMethodManagerRequestData::SendMethodResponseFailed(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::Failed::kErrorCode;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize application error context arguments
  /* No arguments to serialize */

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
