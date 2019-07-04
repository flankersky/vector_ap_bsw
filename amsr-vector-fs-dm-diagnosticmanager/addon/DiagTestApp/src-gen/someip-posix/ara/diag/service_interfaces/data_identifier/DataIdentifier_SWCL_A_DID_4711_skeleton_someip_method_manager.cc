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
/**        \file  DataIdentifier_SWCL_A_DID_4711_skeleton_someip_method_manager.cc
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'DataIdentifier_SWCL_A_DID_4711'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/application_error_exception.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_someip_binding.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/message_verification.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

/* ---- Method 'Read' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_4711ReadMetaInfo {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_4711ReadMetaInfo(RootDeserializer* root) : deserializer_{root} {}

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
class DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorRead : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param MetaInfo Create a snapshot of this method input argument for this functor.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorRead(
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead* binding,
      DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, MetaInfo_{MetaInfo} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;

    /* Catch the possible errors of this method */
    try {
      const skeleton::methods::Read::Output out_val = skeleton_->Read(MetaInfo).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerRead response{header_, out_val};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailedOnRead& application_error) {
      binding_->SendMethodResponseUDSServiceFailedOnRead(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;
};

DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead::
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead(
        DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead::HandleMethodRequest(
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
    // Create one deserializer object for argument MetaInfo
    DeserializeDataIdentifier_SWCL_A_DID_4711ReadMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorRead functor{this, &skeleton_binding_.GetSkeleton(),
                                                                    header_request, MetaInfo};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method DataIdentifier_SWCL_A_DID_4711::Read. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead::SendMethodResponse(
    const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerRead& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Read::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'dataRecord_DataElement_one'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint8) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)>
        serializer_dataRecord_DataElement_one{&serializer};
    serializer_dataRecord_DataElement_one.PushBack(out_val.dataRecord_DataElement_one);
    serializer_dataRecord_DataElement_one.Close();
  }
  /* ImplementationDataTypeValue serialization for element 'dataRecord_DataElement_two'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint16) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)>
        serializer_dataRecord_DataElement_two{&serializer};
    serializer_dataRecord_DataElement_two.PushBack(out_val.dataRecord_DataElement_two);
    serializer_dataRecord_DataElement_two.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead::SendMethodResponseUDSServiceFailedOnRead(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header,
    const skeleton::application_errors::UDSServiceFailedOnRead& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailedOnRead::kErrorCode;
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

/* ---- Method 'Write' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_4711WriteMetaInfo {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_4711WriteMetaInfo(RootDeserializer* root) : deserializer_{root} {}

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
template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_one {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_one(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'dataRecord_DataElement_one'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint8) */
  template <typename ParentDeserializer>
  std::pair<uint8, bool> DeserializedataRecord_DataElement_one(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint8 dataRecord_DataElement_one{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__dataRecord_DataElement_one{parent};
    deserialization_ok = deserializer__dataRecord_DataElement_one.PopFront(dataRecord_DataElement_one) > 0U;
    return std::make_pair(dataRecord_DataElement_one, deserialization_ok);
  }

  uint8 StartDeserialization() {
    auto ret = DeserializedataRecord_DataElement_one<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_two {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_two(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'dataRecord_DataElement_two'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint16) */
  template <typename ParentDeserializer>
  std::pair<uint16, bool> DeserializedataRecord_DataElement_two(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint16 dataRecord_DataElement_two{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__dataRecord_DataElement_two{parent};
    deserialization_ok = deserializer__dataRecord_DataElement_two.PopFront(dataRecord_DataElement_two) > 0U;
    return std::make_pair(dataRecord_DataElement_two, deserialization_ok);
  }

  uint16 StartDeserialization() {
    auto ret = DeserializedataRecord_DataElement_two<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorWrite : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param MetaInfo Create a snapshot of this method input argument for this functor.
   * \param dataRecord_DataElement_one Create a snapshot of this method input argument for this functor.
   * \param dataRecord_DataElement_two Create a snapshot of this method input argument for this functor.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorWrite(
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite* binding,
      DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo, const uint8& dataRecord_DataElement_one, const uint16& dataRecord_DataElement_two)
      : binding_{binding},
        skeleton_{skeleton},
        header_{header},
        MetaInfo_{MetaInfo},
        dataRecord_DataElement_one_{dataRecord_DataElement_one},
        dataRecord_DataElement_two_{dataRecord_DataElement_two} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;
    const uint8 dataRecord_DataElement_one = dataRecord_DataElement_one_;
    const uint16 dataRecord_DataElement_two = dataRecord_DataElement_two_;

    /* Catch the possible errors of this method */
    try {
      (void)skeleton_->Write(MetaInfo, dataRecord_DataElement_one, dataRecord_DataElement_two).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerWrite response{header_};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailedOnWrite& application_error) {
      binding_->SendMethodResponseUDSServiceFailedOnWrite(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;

  /**
   * \brief Storing context of method argument 'dataRecord_DataElement_one' in this functor for processing at any time.
   */
  const uint8 dataRecord_DataElement_one_;

  /**
   * \brief Storing context of method argument 'dataRecord_DataElement_two' in this functor for processing at any time.
   */
  const uint16 dataRecord_DataElement_two_;
};

DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite::
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite(
        DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite::HandleMethodRequest(
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
    // Create one deserializer object for argument MetaInfo
    DeserializeDataIdentifier_SWCL_A_DID_4711WriteMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();
    // Create one deserializer object for argument dataRecord_DataElement_one
    DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_one<
        typename std::decay<decltype(deserializer)>::type>
        arg_dataRecord_DataElement_one_deserializer{&deserializer};
    uint8 dataRecord_DataElement_one = arg_dataRecord_DataElement_one_deserializer.StartDeserialization();
    // Create one deserializer object for argument dataRecord_DataElement_two
    DeserializeDataIdentifier_SWCL_A_DID_4711WritedataRecord_DataElement_two<
        typename std::decay<decltype(deserializer)>::type>
        arg_dataRecord_DataElement_two_deserializer{&deserializer};
    uint16 dataRecord_DataElement_two = arg_dataRecord_DataElement_two_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorWrite functor{
        this,     &skeleton_binding_.GetSkeleton(), header_request,
        MetaInfo, dataRecord_DataElement_one,       dataRecord_DataElement_two};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method DataIdentifier_SWCL_A_DID_4711::Write. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite::SendMethodResponse(
    const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerWrite& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  /* No arguments to serialize */

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite::SendMethodResponseUDSServiceFailedOnWrite(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header,
    const skeleton::application_errors::UDSServiceFailedOnWrite& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailedOnWrite::kErrorCode;
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

/* ---- Method 'Cancel' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeDataIdentifier_SWCL_A_DID_4711CancelMetaInfo {
 public:
  explicit DeserializeDataIdentifier_SWCL_A_DID_4711CancelMetaInfo(RootDeserializer* root) : deserializer_{root} {}

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
class DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorCancel : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param MetaInfo Create a snapshot of this method input argument for this functor.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorCancel(
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel* binding,
      DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, MetaInfo_{MetaInfo} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;

    (void)skeleton_->Cancel(MetaInfo).get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerCancel response{header_};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  DataIdentifier_SWCL_A_DID_4711Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;
};

DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel::
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel(
        DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel::HandleMethodRequest(
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
    // Create one deserializer object for argument MetaInfo
    DeserializeDataIdentifier_SWCL_A_DID_4711CancelMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpFunctorCancel functor{this, &skeleton_binding_.GetSkeleton(),
                                                                      header_request, MetaInfo};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method DataIdentifier_SWCL_A_DID_4711::Cancel. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel::SendMethodResponse(
    const DataIdentifier_SWCL_A_DID_4711SomeIpResponsePathInfoMethodManagerCancel& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  /* No arguments to serialize */

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
