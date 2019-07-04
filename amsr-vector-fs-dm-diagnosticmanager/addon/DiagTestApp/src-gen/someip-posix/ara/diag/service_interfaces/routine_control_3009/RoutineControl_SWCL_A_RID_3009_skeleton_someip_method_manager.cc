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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton_someip_method_manager.cc
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_someip_method_manager.h"
#include <utility>
#include "ara-someip-posix/application_error.h"
#include "ara/com/application_error_exception.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton_someip_binding.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/message_verification.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/* ---- Method 'Start' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009StartMetaInfo {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StartMetaInfo(RootDeserializer* root) : deserializer_{root} {}

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
class DeserializeRoutineControl_SWCL_A_RID_3009StartReq_Reset {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StartReq_Reset(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'Req_Reset'
   * (/AUTOSAR_Platform/ImplementationDataTypes/boolean) */
  template <typename ParentDeserializer>
  std::pair<boolean, bool> DeserializeReq_Reset(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    boolean Req_Reset{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__Req_Reset{
        parent};
    deserialization_ok = deserializer__Req_Reset.PopFront(Req_Reset) > 0U;
    return std::make_pair(Req_Reset, deserialization_ok);
  }

  boolean StartDeserialization() {
    auto ret = DeserializeReq_Reset<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009StartReq_CounterLimit {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StartReq_CounterLimit(RootDeserializer* root)
      : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'Req_CounterLimit'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> DeserializeReq_CounterLimit(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 Req_CounterLimit{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer>
        deserializer__Req_CounterLimit{parent};
    deserialization_ok = deserializer__Req_CounterLimit.PopFront(Req_CounterLimit) > 0U;
    return std::make_pair(Req_CounterLimit, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = DeserializeReq_CounterLimit<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStart : public ara::com::internal::PendingRequest {
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
   * \param Req_Reset Create a snapshot of this method input argument for this functor.
   * \param Req_CounterLimit Create a snapshot of this method input argument for this functor.
   */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStart(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart* binding,
      RoutineControl_SWCL_A_RID_3009Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo, const boolean& Req_Reset, const uint32& Req_CounterLimit)
      : binding_{binding},
        skeleton_{skeleton},
        header_{header},
        MetaInfo_{MetaInfo},
        Req_Reset_{Req_Reset},
        Req_CounterLimit_{Req_CounterLimit} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;
    const boolean Req_Reset = Req_Reset_;
    const uint32 Req_CounterLimit = Req_CounterLimit_;

    /* Catch the possible errors of this method */
    try {
      (void)skeleton_->Start(MetaInfo, Req_Reset, Req_CounterLimit).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStart response{header_};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailedOnStart& application_error) {
      binding_->SendMethodResponseUDSServiceFailedOnStart(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  RoutineControl_SWCL_A_RID_3009Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;

  /**
   * \brief Storing context of method argument 'Req_Reset' in this functor for processing at any time.
   */
  const boolean Req_Reset_;

  /**
   * \brief Storing context of method argument 'Req_CounterLimit' in this functor for processing at any time.
   */
  const uint32 Req_CounterLimit_;
};

RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart(
        RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::HandleMethodRequest(
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
    DeserializeRoutineControl_SWCL_A_RID_3009StartMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();
    // Create one deserializer object for argument Req_Reset
    DeserializeRoutineControl_SWCL_A_RID_3009StartReq_Reset<typename std::decay<decltype(deserializer)>::type>
        arg_Req_Reset_deserializer{&deserializer};
    boolean Req_Reset = arg_Req_Reset_deserializer.StartDeserialization();
    // Create one deserializer object for argument Req_CounterLimit
    DeserializeRoutineControl_SWCL_A_RID_3009StartReq_CounterLimit<typename std::decay<decltype(deserializer)>::type>
        arg_Req_CounterLimit_deserializer{&deserializer};
    uint32 Req_CounterLimit = arg_Req_CounterLimit_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStart functor{
        this, &skeleton_binding_.GetSkeleton(), header_request, MetaInfo, Req_Reset, Req_CounterLimit};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method RoutineControl_SWCL_A_RID_3009::Start. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::SendMethodResponse(
    const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStart& response) {
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

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart::SendMethodResponseUDSServiceFailedOnStart(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header,
    const skeleton::application_errors::UDSServiceFailedOnStart& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailedOnStart::kErrorCode;
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

/* ---- Method 'Stop' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009StopMetaInfo {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009StopMetaInfo(RootDeserializer* root) : deserializer_{root} {}

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
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStop : public ara::com::internal::PendingRequest {
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
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStop(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop* binding,
      RoutineControl_SWCL_A_RID_3009Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, MetaInfo_{MetaInfo} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;

    /* Catch the possible errors of this method */
    try {
      const skeleton::methods::Stop::Output out_val = skeleton_->Stop(MetaInfo).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStop response{header_, out_val};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailedOnStop& application_error) {
      binding_->SendMethodResponseUDSServiceFailedOnStop(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  RoutineControl_SWCL_A_RID_3009Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;
};

RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop(
        RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::HandleMethodRequest(
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
    DeserializeRoutineControl_SWCL_A_RID_3009StopMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorStop functor{this, &skeleton_binding_.GetSkeleton(),
                                                                    header_request, MetaInfo};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method RoutineControl_SWCL_A_RID_3009::Stop. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::SendMethodResponse(
    const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStop& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Stop::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'Resp_RemainingCounts'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)>
        serializer_Resp_RemainingCounts{&serializer};
    serializer_Resp_RemainingCounts.PushBack(out_val.Resp_RemainingCounts);
    serializer_Resp_RemainingCounts.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop::SendMethodResponseUDSServiceFailedOnStop(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header,
    const skeleton::application_errors::UDSServiceFailedOnStop& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailedOnStop::kErrorCode;
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

/* ---- Method 'RequestResults' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsMetaInfo {
 public:
  explicit DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsMetaInfo(RootDeserializer* root)
      : deserializer_{root} {}

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
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorRequestResults : public ara::com::internal::PendingRequest {
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
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorRequestResults(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults* binding,
      RoutineControl_SWCL_A_RID_3009Skeleton* skeleton, ::someip_posix_common::someip::SomeIpMessageHeader header,
      const MetaInfoType& MetaInfo)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, MetaInfo_{MetaInfo} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const MetaInfoType MetaInfo = MetaInfo_;

    /* Catch the possible errors of this method */
    try {
      const skeleton::methods::RequestResults::Output out_val = skeleton_->RequestResults(MetaInfo).get();
      // Assemble the response path information within the binding. That's necessary
      // if the binding has additional info to transport as in the case for the SOME/IP binding.
      const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerRequestResults response{header_, out_val};
      binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
    } catch (const skeleton::application_errors::UDSServiceFailedOnRequestResults& application_error) {
      binding_->SendMethodResponseUDSServiceFailedOnRequestResults(header_, application_error);
    }
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  RoutineControl_SWCL_A_RID_3009Skeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'MetaInfo' in this functor for processing at any time.
   */
  const MetaInfoType MetaInfo_;
};

RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults(
        RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::HandleMethodRequest(
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
    DeserializeRoutineControl_SWCL_A_RID_3009RequestResultsMetaInfo<typename std::decay<decltype(deserializer)>::type>
        arg_MetaInfo_deserializer{&deserializer};
    MetaInfoType MetaInfo = arg_MetaInfo_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    RoutineControl_SWCL_A_RID_3009SkeletonSomeIpFunctorRequestResults functor{this, &skeleton_binding_.GetSkeleton(),
                                                                              header_request, MetaInfo};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method RoutineControl_SWCL_A_RID_3009::RequestResults. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::SendMethodResponse(
    const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerRequestResults& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::RequestResults::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'Resp_RemainingCounts'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)>
        serializer_Resp_RemainingCounts{&serializer};
    serializer_Resp_RemainingCounts.PushBack(out_val.Resp_RemainingCounts);
    serializer_Resp_RemainingCounts.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

void RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults::
    SendMethodResponseUDSServiceFailedOnRequestResults(
        const ::someip_posix_common::someip::SomeIpMessageHeader& header,
        const skeleton::application_errors::UDSServiceFailedOnRequestResults& application_error) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{header};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kError;
  header_response.return_code_ = ara::com::someip_posix::kApplicationErrorSomeIpReturnCodeOffset +
                                 skeleton::application_errors::UDSServiceFailedOnRequestResults::kErrorCode;
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

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
