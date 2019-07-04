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
/**        \file  GenericUDSService_skeleton_someip_method_manager.cc
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'GenericUDSService'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/application_error_exception.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton_someip_binding.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/message_verification.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/* ---- Method 'Service' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeGenericUDSServiceServiceSID {
 public:
  explicit DeserializeGenericUDSServiceServiceSID(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'SID' (/AUTOSAR_Platform/ImplementationDataTypes/uint8) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> DeserializeSID(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 SID{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__SID{parent};
    deserialization_ok = deserializer__SID.PopFront(SID) > 0U;
    return std::make_pair(SID, deserialization_ok);
  }

  uint8 StartDeserialization() {
    auto ret = DeserializeSID<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeGenericUDSServiceServiceRequestData {
 public:
  explicit DeserializeGenericUDSServiceServiceRequestData(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> DeserializeRequestData_byte(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 RequestData_byte{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__RequestData_byte{parent};
    deserialization_ok = deserializer__RequestData_byte.PopFront(RequestData_byte) > 0U;
    return std::make_pair(RequestData_byte, deserialization_ok);
  }

  /* ImplementationDataTypeVector deserialization for element 'RequestData' (/ara/diag/types/DataArrayType) */
  template <typename ParentDeserializer>
  std::pair<DataArrayType, bool> DeserializeRequestData(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__RequestData{parent};
    DataArrayType RequestData{};
    while (deserializer__RequestData.HasDataRemaining()) {
      const auto ret = DeserializeRequestData_byte(&deserializer__RequestData);
      deserialization_ok &= ret.second;
      if (!deserialization_ok) {
        break;
      }

      RequestData.push_back(ret.first);
    }
    return std::make_pair(RequestData, deserialization_ok);
  }

  DataArrayType StartDeserialization() {
    auto ret = DeserializeRequestData<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeGenericUDSServiceServiceMetaInfo {
 public:
  explicit DeserializeGenericUDSServiceServiceMetaInfo(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeTypeReference deserialization for element 'MetaInfoKey'
   * (/ara/diag/types/MetaInfoType/MetaInfoKey) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoKeyType, bool> DeserializeMetaInfo_MetaInfoKey(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 MetaInfo_MetaInfoKey{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__MetaInfo_MetaInfoKey{parent};
    deserialization_ok = deserializer__MetaInfo_MetaInfoKey.PopFront(MetaInfo_MetaInfoKey) > 0U;
    return std::make_pair(static_cast<MetaInfoKeyType>(MetaInfo_MetaInfoKey), deserialization_ok);
  }

  /* ImplementationDataTypeString deserialization for element 'MetaInfoValue'
   * (/ara/diag/types/MetaInfoType/MetaInfoValue) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoValueType, bool> DeserializeMetaInfo_MetaInfoValue(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__MetaInfo_MetaInfoValue{parent};
    MetaInfoValueType str;
    const std::size_t str_length = deserializer__MetaInfo_MetaInfoValue.GetLengthFieldValue();
    str.reserve(str_length);
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(deserializer__MetaInfo_MetaInfoValue)>
        bom_deserializer{&deserializer__MetaInfo_MetaInfoValue};
    std::uint8_t b{};
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xEFU);
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xBBU);
    deserialization_ok = bom_deserializer.PopFront(b) > 0U;
    deserialization_ok &= (b == 0xBFU);
    if (deserialization_ok) {
      while (deserializer__MetaInfo_MetaInfoValue.HasDataRemaining()) {
        std::uint8_t c{};
        marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy,
                                 decltype(deserializer__MetaInfo_MetaInfoValue)>
            str_element_deserializer{&deserializer__MetaInfo_MetaInfoValue};
        deserialization_ok = str_element_deserializer.PopFront(c) > 0U;
        if (!deserialization_ok) {
          break;
        }
        str.push_back(c);
      }
    }
    return std::make_pair(str, deserialization_ok);
  }

  /* ImplementationDataTypeMap deserialization for element 'MetaInfo' (/ara/diag/types/MetaInfoType) */
  template <typename ParentDeserializer>
  std::pair<MetaInfoType, bool> DeserializeMetaInfo(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    marshaller::ComplexDataTypeDeserializer<marshaller::BEPayloadUint32LengthFieldPolicy, ParentDeserializer>
        deserializer__MetaInfo{parent};
    MetaInfoType MetaInfo{};
    while (deserializer__MetaInfo.HasDataRemaining()) {
      const auto ret_key = DeserializeMetaInfo_MetaInfoKey(&deserializer__MetaInfo);
      deserialization_ok &= ret_key.second;
      if (!deserialization_ok) {
        break;
      }
      const auto ret_value = DeserializeMetaInfo_MetaInfoValue(&deserializer__MetaInfo);
      deserialization_ok &= ret_value.second;
      if (!deserialization_ok) {
        break;
      }

      const auto ret_emplace = MetaInfo.emplace(std::make_pair(ret_key.first, ret_value.first));
      deserialization_ok &= ret_emplace.second;
      if (!deserialization_ok) {
        break;
      }
    }
    return std::make_pair(MetaInfo, deserialization_ok);
  }

  MetaInfoType StartDeserialization() {
    auto ret = DeserializeMetaInfo<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class GenericUDSServiceSkeletonSomeIpFunctorService : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param SID Create a snapshot of this method input argument for this functor.
   * \param RequestData Create a snapshot of this method input argument for this functor.
   * \param MetaInfo Create a snapshot of this method input argument for this functor.
   */
  GenericUDSServiceSkeletonSomeIpFunctorService(GenericUDSServiceSkeletonSomeIpMethodManagerService* binding,
                                                GenericUDSServiceSkeleton* skeleton,
                                                ::someip_posix_common::someip::SomeIpMessageHeader header,
                                                const uint8& SID, const DataArrayType& RequestData,
                                                const MetaInfoType& MetaInfo)
      : binding_{binding},
        skeleton_{skeleton},
        header_{header},
        SID_{SID},
        RequestData_{RequestData},
        MetaInfo_{MetaInfo} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint8 SID = SID_;
    const DataArrayType RequestData = RequestData_;
    const MetaInfoType MetaInfo = MetaInfo_;

    /* Catch the possible errors of this method */
    try {
      const skeleton::methods::Service::Output out_val = skeleton_->Service(SID, RequestData, MetaInfo).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const GenericUDSServiceSomeIpResponsePathInfoMethodManagerService response{header_, out_val};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailed& application_error) {
      binding_->SendMethodResponseUDSServiceFailed(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  GenericUDSServiceSkeletonSomeIpMethodManagerService* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  GenericUDSServiceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'SID' in this functor for processing at any time.
   */
  const uint8 SID_;

  /**
   * \brief Storing context of method argument 'RequestData' in this functor for processing at any time.
   */
  const DataArrayType RequestData_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;
};

GenericUDSServiceSkeletonSomeIpMethodManagerService::GenericUDSServiceSkeletonSomeIpMethodManagerService(
    GenericUDSServiceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
GenericUDSServiceSkeletonSomeIpMethodManagerService::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      GenericUDSServiceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      GenericUDSServiceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void GenericUDSServiceSkeletonSomeIpMethodManagerService::HandleMethodRequest(
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
    // Create one deserializer object for argument SID
    DeserializeGenericUDSServiceServiceSID<typename std::decay<decltype(deserializer)>::type> arg_SID_deserializer{
        &deserializer};
    uint8 SID = arg_SID_deserializer.StartDeserialization();
    // Create one deserializer object for argument RequestData
    DeserializeGenericUDSServiceServiceRequestData<typename std::decay<decltype(deserializer)>::type>
        arg_RequestData_deserializer{&deserializer};
    DataArrayType RequestData = arg_RequestData_deserializer.StartDeserialization();
    // Create one deserializer object for argument MetaInfo
    DeserializeGenericUDSServiceServiceMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    GenericUDSServiceSkeletonSomeIpFunctorService functor{
        this, &skeleton_binding_.GetSkeleton(), header_request, SID, RequestData, MetaInfo};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError() << "Method request verification failed for method GenericUDSService::Service. Return code: "
                      << static_cast<std::uint16_t>(return_code);
  }
}

void GenericUDSServiceSkeletonSomeIpMethodManagerService::SendMethodResponse(
    const GenericUDSServiceSomeIpResponsePathInfoMethodManagerService& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Service::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeVector serialization for element 'ResponseData' (/ara/diag/types/DataArrayType) */
  {
    marshaller::ComplexDataTypeSerializer<marshaller::BEPayloadUint32LengthFieldPolicy, decltype(serializer)>
        serializer_ResponseData{&serializer};
    for (const auto& out_val_ResponseData_element : out_val.ResponseData) {
      /* ImplementationDataTypeValue serialization for element 'byte' (/ara/diag/types/DataArrayType/byte) */
      {
        marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer_ResponseData)>
            serializer_ResponseData_byte{&serializer_ResponseData};
        serializer_ResponseData_byte.PushBack(out_val_ResponseData_element);
        serializer_ResponseData_byte.Close();
      }
    }
    serializer_ResponseData.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void GenericUDSServiceSkeletonSomeIpMethodManagerService::SendMethodResponseUDSServiceFailed(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header,
    const skeleton::application_errors::UDSServiceFailed& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailed::kErrorCode;
  const UDSResponseCodeType errorContext = application_error.geterrorContext();

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize application error context arguments
  /* ImplementationDataTypeTypeReference serialization for element 'errorContext' (/ara/diag/types/UDSResponseCodeType)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_errorContext{
        &serializer};
    serializer_errorContext.PushBack(static_cast<std::underlying_type<UDSResponseCodeType>::type>(errorContext));
    serializer_errorContext.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
