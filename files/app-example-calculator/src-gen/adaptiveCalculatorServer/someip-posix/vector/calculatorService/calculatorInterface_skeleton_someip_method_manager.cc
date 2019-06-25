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
/**        \file  calculatorInterface_skeleton_someip_method_manager.cc
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_skeleton_someip_method_manager.h"
#include <utility>
#include "ara/com/internal/skeleton_request_handling.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/message_verification.h"
#include "vector/calculatorService/calculatorInterface_skeleton_someip_binding.h"

namespace vector {
namespace calculatorService {

/* ---- Method 'subtract' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializecalculatorInterfaceSubtractarg1 {
 public:
  explicit DeserializecalculatorInterfaceSubtractarg1(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'arg1' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializearg1(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 arg1{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__arg1{parent};
    deserialization_ok = deserializer__arg1.PopFront(arg1) > 0U;
    return std::make_pair(arg1, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializearg1<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializecalculatorInterfaceSubtractarg2 {
 public:
  explicit DeserializecalculatorInterfaceSubtractarg2(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'arg2' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializearg2(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 arg2{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__arg2{parent};
    deserialization_ok = deserializer__arg2.PopFront(arg2) > 0U;
    return std::make_pair(arg2, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializearg2<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class CalculatorInterfaceSkeletonSomeIpFunctorSubtract : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param arg1 Create a snapshot of this method input argument for this functor.
   * \param arg2 Create a snapshot of this method input argument for this functor.
   */
  CalculatorInterfaceSkeletonSomeIpFunctorSubtract(CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract* binding,
                                                   CalculatorInterfaceSkeleton* skeleton,
                                                   ::someip_posix_common::someip::SomeIpMessageHeader header,
                                                   const uint32& arg1, const uint32& arg2)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, arg1_{arg1}, arg2_{arg2} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint32 arg1 = arg1_;
    const uint32 arg2 = arg2_;

    const skeleton::methods::Subtract::Output out_val = skeleton_->subtract(arg1, arg2).get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerSubtract response{header_, out_val};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  CalculatorInterfaceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'arg1' in this functor for processing at any time.
   */
  const uint32 arg1_;

  /**
   * \brief Storing context of method argument 'arg2' in this functor for processing at any time.
   */
  const uint32 arg2_;
};

CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract::CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract(
    CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      CalculatorInterfaceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract::HandleMethodRequest(
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
    // Create one deserializer object for argument arg1
    DeserializecalculatorInterfaceSubtractarg1<typename std::decay<decltype(deserializer)>::type> arg_arg1_deserializer{
        &deserializer};
    uint32 arg1 = arg_arg1_deserializer.StartDeserialization();
    // Create one deserializer object for argument arg2
    DeserializecalculatorInterfaceSubtractarg2<typename std::decay<decltype(deserializer)>::type> arg_arg2_deserializer{
        &deserializer};
    uint32 arg2 = arg_arg2_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    CalculatorInterfaceSkeletonSomeIpFunctorSubtract functor{this, &skeleton_binding_.GetSkeleton(), header_request,
                                                             arg1, arg2};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError() << "Method request verification failed for method CalculatorInterface::Subtract. Return code: "
                      << static_cast<std::uint16_t>(return_code);
  }
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerSubtract::SendMethodResponse(
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerSubtract& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Subtract::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_result{
        &serializer};
    serializer_result.PushBack(out_val.result);
    serializer_result.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

/* ---- Method 'add' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializecalculatorInterfaceAddarg1 {
 public:
  explicit DeserializecalculatorInterfaceAddarg1(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'arg1' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializearg1(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 arg1{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__arg1{parent};
    deserialization_ok = deserializer__arg1.PopFront(arg1) > 0U;
    return std::make_pair(arg1, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializearg1<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializecalculatorInterfaceAddarg2 {
 public:
  explicit DeserializecalculatorInterfaceAddarg2(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'arg2' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializearg2(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 arg2{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__arg2{parent};
    deserialization_ok = deserializer__arg2.PopFront(arg2) > 0U;
    return std::make_pair(arg2, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializearg2<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class CalculatorInterfaceSkeletonSomeIpFunctorAdd : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param arg1 Create a snapshot of this method input argument for this functor.
   * \param arg2 Create a snapshot of this method input argument for this functor.
   */
  CalculatorInterfaceSkeletonSomeIpFunctorAdd(CalculatorInterfaceSkeletonSomeIpMethodManagerAdd* binding,
                                              CalculatorInterfaceSkeleton* skeleton,
                                              ::someip_posix_common::someip::SomeIpMessageHeader header,
                                              const uint32& arg1, const uint32& arg2)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, arg1_{arg1}, arg2_{arg2} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint32 arg1 = arg1_;
    const uint32 arg2 = arg2_;

    const skeleton::methods::Add::Output out_val = skeleton_->add(arg1, arg2).get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerAdd response{header_, out_val};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  CalculatorInterfaceSkeletonSomeIpMethodManagerAdd* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  CalculatorInterfaceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'arg1' in this functor for processing at any time.
   */
  const uint32 arg1_;

  /**
   * \brief Storing context of method argument 'arg2' in this functor for processing at any time.
   */
  const uint32 arg2_;
};

CalculatorInterfaceSkeletonSomeIpMethodManagerAdd::CalculatorInterfaceSkeletonSomeIpMethodManagerAdd(
    CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
CalculatorInterfaceSkeletonSomeIpMethodManagerAdd::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      CalculatorInterfaceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerAdd::HandleMethodRequest(
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
    // Create one deserializer object for argument arg1
    DeserializecalculatorInterfaceAddarg1<typename std::decay<decltype(deserializer)>::type> arg_arg1_deserializer{
        &deserializer};
    uint32 arg1 = arg_arg1_deserializer.StartDeserialization();
    // Create one deserializer object for argument arg2
    DeserializecalculatorInterfaceAddarg2<typename std::decay<decltype(deserializer)>::type> arg_arg2_deserializer{
        &deserializer};
    uint32 arg2 = arg_arg2_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    CalculatorInterfaceSkeletonSomeIpFunctorAdd functor{this, &skeleton_binding_.GetSkeleton(), header_request, arg1,
                                                        arg2};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError() << "Method request verification failed for method CalculatorInterface::Add. Return code: "
                      << static_cast<std::uint16_t>(return_code);
  }
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerAdd::SendMethodResponse(
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerAdd& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Add::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_result{
        &serializer};
    serializer_result.PushBack(out_val.result);
    serializer_result.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

/* ---- Method 'divide' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializecalculatorInterfaceDividedivident {
 public:
  explicit DeserializecalculatorInterfaceDividedivident(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'divident'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializedivident(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 divident{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__divident{
        parent};
    deserialization_ok = deserializer__divident.PopFront(divident) > 0U;
    return std::make_pair(divident, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializedivident<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};
template <typename RootDeserializer>
class DeserializecalculatorInterfaceDividedivisor {
 public:
  explicit DeserializecalculatorInterfaceDividedivisor(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'divisor'
   * (/AUTOSAR_Platform/ImplementationDataTypes/uint32) */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializedivisor(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 divisor{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__divisor{
        parent};
    deserialization_ok = deserializer__divisor.PopFront(divisor) > 0U;
    return std::make_pair(divisor, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializedivisor<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class CalculatorInterfaceSkeletonSomeIpFunctorDivide : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param divident Create a snapshot of this method input argument for this functor.
   * \param divisor Create a snapshot of this method input argument for this functor.
   */
  CalculatorInterfaceSkeletonSomeIpFunctorDivide(CalculatorInterfaceSkeletonSomeIpMethodManagerDivide* binding,
                                                 CalculatorInterfaceSkeleton* skeleton,
                                                 ::someip_posix_common::someip::SomeIpMessageHeader header,
                                                 const uint32& divident, const uint32& divisor)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, divident_{divident}, divisor_{divisor} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint32 divident = divident_;
    const uint32 divisor = divisor_;

    const skeleton::methods::Divide::Output out_val = skeleton_->divide(divident, divisor).get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerDivide response{header_, out_val};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  CalculatorInterfaceSkeletonSomeIpMethodManagerDivide* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  CalculatorInterfaceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'divident' in this functor for processing at any time.
   */
  const uint32 divident_;

  /**
   * \brief Storing context of method argument 'divisor' in this functor for processing at any time.
   */
  const uint32 divisor_;
};

CalculatorInterfaceSkeletonSomeIpMethodManagerDivide::CalculatorInterfaceSkeletonSomeIpMethodManagerDivide(
    CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
CalculatorInterfaceSkeletonSomeIpMethodManagerDivide::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      CalculatorInterfaceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerDivide::HandleMethodRequest(
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
    // Create one deserializer object for argument divident
    DeserializecalculatorInterfaceDividedivident<typename std::decay<decltype(deserializer)>::type>
        arg_divident_deserializer{&deserializer};
    uint32 divident = arg_divident_deserializer.StartDeserialization();
    // Create one deserializer object for argument divisor
    DeserializecalculatorInterfaceDividedivisor<typename std::decay<decltype(deserializer)>::type>
        arg_divisor_deserializer{&deserializer};
    uint32 divisor = arg_divisor_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    CalculatorInterfaceSkeletonSomeIpFunctorDivide functor{this, &skeleton_binding_.GetSkeleton(), header_request,
                                                           divident, divisor};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError() << "Method request verification failed for method CalculatorInterface::Divide. Return code: "
                      << static_cast<std::uint16_t>(return_code);
  }
}

void CalculatorInterfaceSkeletonSomeIpMethodManagerDivide::SendMethodResponse(
    const CalculatorInterfaceSomeIpResponsePathInfoMethodManagerDivide& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::methods::Divide::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'result' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_result{
        &serializer};
    serializer_result.PushBack(out_val.result);
    serializer_result.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

/* ---- Field method 'divideResultGet' ------------------------------------------- */

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class CalculatorInterfaceSkeletonSomeIpFunctorDivideResultGet : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   */
  CalculatorInterfaceSkeletonSomeIpFunctorDivideResultGet(
      CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet* binding, CalculatorInterfaceSkeleton* skeleton,
      ::someip_posix_common::someip::SomeIpMessageHeader header)
      : binding_{binding}, skeleton_{skeleton}, header_{header} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const skeleton::fields::DivideResult::Output out_val = skeleton_->divideResult.Get().get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const CalculatorInterfaceSomeIpResponsePathInfoFieldManagerDivideResultGet response{header_, out_val};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  CalculatorInterfaceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;
};

CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet::
    CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet(
        CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      CalculatorInterfaceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet::HandleMethodRequest(
    const ::someip_posix_common::someip::SomeIpMessageHeader& header, RootDeserializerAlias&& deserializer) {
  // The deserializer object is not used within this scope, as there're no input arguments to deserialize.
  (void)deserializer;
  std::lock_guard<std::mutex> guard(pending_lock_);

  // SOME/IP message verification
  const someip_posix_common::someip::SomeIpMessageHeader& expected_header{GetExpectedRequestHeader()};
  const someip_posix_common::someip::SomeIpReturnCode return_code{
      someip_posix_common::someip::RequestMessageVerification::DoAllDeploymentChecks(expected_header, header)};
  if (return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
    // Shortening
    namespace marshaller = someip_posix_common::someip::serialization;

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    CalculatorInterfaceSkeletonSomeIpFunctorDivideResultGet functor{
        this, &skeleton_binding_.GetSkeleton(), header_request,
    };

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method CalculatorInterface::DivideResultGet. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultGet::SendMethodResponse(
    const CalculatorInterfaceSomeIpResponsePathInfoFieldManagerDivideResultGet& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::fields::DivideResult::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'out_val' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_out_val{
        &serializer};
    serializer_out_val.PushBack(out_val);
    serializer_out_val.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

/* ---- Field method 'divideResultSet' ------------------------------------------- */
template <typename RootDeserializer>
class DeserializecalculatorInterfaceDivideResultSetin_val {
 public:
  explicit DeserializecalculatorInterfaceDivideResultSetin_val(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'in_val' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializein_val(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 in_val{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__in_val{parent};
    deserialization_ok = deserializer__in_val.PopFront(in_val) > 0U;
    return std::make_pair(in_val, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializein_val<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

/**
 * \brief This functor is given to the frontend. Any functor (generic part) can
 * then be stored.
 */
class CalculatorInterfaceSkeletonSomeIpFunctorDivideResultSet : public ara::com::internal::PendingRequest {
 public:
  /**
   * \brief At initialization store all relevant information, to be able to invoke the method call and route the
   * response to the binding-related part again.
   * \param binding A pointer to the binding-related part for forwarding the response after processing the method
   * request.
   * \param skeleton A pointer to the skeleton frontend for calling the concrete method.
   * \param header The SOME/IP header which must be stored for a possbile response (in case this method is not fire &
   * forget).
   * \param in_val Create a snapshot of this method input argument for this functor.
   */
  CalculatorInterfaceSkeletonSomeIpFunctorDivideResultSet(
      CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet* binding, CalculatorInterfaceSkeleton* skeleton,
      ::someip_posix_common::someip::SomeIpMessageHeader header, const vector::calculatorService::uint32& in_val)
      : binding_{binding}, skeleton_{skeleton}, header_{header}, in_val_{in_val} {}

  /**
   * \brief Operator gets called when method invocation is planned in the frontend.
   */
  void operator()() override {
    const uint32 in_val = in_val_;

    const skeleton::fields::DivideResult::Output out_val = skeleton_->divideResult.Set(in_val).get();
    // Assemble the response path information within the binding. That's necessary
    // if the binding has additional info to transport as in the case for the SOME/IP binding.
    const CalculatorInterfaceSomeIpResponsePathInfoFieldManagerDivideResultSet response{header_, out_val};
    binding_->SendMethodResponse(response);  // Forward the result to the response path of the binding.
  }

 private:
  /**
   * \brief Binding-related interface to signalize a response right after the method
   * call.
   */
  CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet* binding_;

  /**
   * \brief Pointer to the skeleton to invoke the method request call.
   */
  CalculatorInterfaceSkeleton* skeleton_;

  /**
   * \brief Buffer the SOME/IP header from the request for the response with the corresponding client ID and session ID.
   */
  ::someip_posix_common::someip::SomeIpMessageHeader header_;

  /**
   * \brief Storing context of method argument 'in_val' in this functor for processing at any time.
   */
  const uint32 in_val_;
};

CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet::
    CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet(
        CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding)
    : skeleton_binding_(skeleton_binding) {}

const ::someip_posix_common::someip::SomeIpMessageHeader&
CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet::GetExpectedRequestHeader() const {
  static constexpr someip_posix_common::someip::SomeIpMessageHeader expected_header{
      CalculatorInterfaceSkeletonSomeIpBinding::kServiceId,
      kMethodId,
      0U,  // length - not part of verification
      0U,  // client id - not part of verification
      0U,  // session id - not part of verification
      someip_posix_common::someip::kProtocolVersion,
      CalculatorInterfaceSkeletonSomeIpBinding::kMajorVersion,
      someip_posix_common::someip::SomeIpMessageType::kRequest,
      someip_posix_common::someip::SomeIpReturnCode::kOk};

  return expected_header;
}

void CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet::HandleMethodRequest(
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
    // Create one deserializer object for argument in_val
    DeserializecalculatorInterfaceDivideResultSetin_val<typename std::decay<decltype(deserializer)>::type>
        arg_in_val_deserializer{&deserializer};
    uint32 in_val = arg_in_val_deserializer.StartDeserialization();

    // Get the header to save it in the functor; this is needed, because on the return path session ID and client
    // ID must match with the given info from the request.
    const ::someip_posix_common::someip::SomeIpMessageHeader& header_request = header;

    // Create a functor to save the context for being able to invoke the skeleton method call on a later time-point.
    CalculatorInterfaceSkeletonSomeIpFunctorDivideResultSet functor{this, &skeleton_binding_.GetSkeleton(),
                                                                    header_request, in_val};

    // Pass payload to skeleton that decides when to process the method call based on the processing-mode.
    // The response path is then called by the skeleton.
    skeleton_binding_.GetSkeleton().HandleMethodRequest(functor);
  } else {
    skeleton_binding_.SendErrorResponseOnMethodRequest(return_code, header);
    ara::log::Logger& logger = ara::log::CreateLogger("Method request verification", "");
    logger.LogError()
        << "Method request verification failed for method CalculatorInterface::DivideResultSet. Return code: "
        << static_cast<std::uint16_t>(return_code);
  }
}

void CalculatorInterfaceSkeletonSomeIpFieldManagerDivideResultSet::SendMethodResponse(
    const CalculatorInterfaceSomeIpResponsePathInfoFieldManagerDivideResultSet& response) {
  // Shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Create a new packet for the response
  ::someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
  ::someip_posix_common::someip::SomeIpMessageHeader header_response{response.header_};
  header_response.message_type_ = someip_posix_common::someip::SomeIpMessageType::kResponse;
  skeleton::fields::DivideResult::Output out_val = response.out_val_;

  // Create root serializer
  marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy> serializer{std::move(packet_response)};

  // Read SOME/IP header with a nested serializer
  marshaller::SomeIpHeaderSerializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> header_serializer{
      &serializer, header_response};
  // Serialize return value(s) of skeleton method call
  /* ImplementationDataTypeValue serialization for element 'out_val' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  {
    marshaller::Serializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(serializer)> serializer_out_val{
        &serializer};
    serializer_out_val.PushBack(out_val);
    serializer_out_val.Close();
  }

  // Will fill in the length field of the payload serialized.
  header_serializer.Close();

  // Transfer back the ownership
  packet_response = serializer.Close();

  // Send packet to the SkeletonBinding, which forwards the packet together with the instance ID to AraComSomeIpBinding.
  skeleton_binding_.SendMethodResponse(std::move(packet_response));
}

}  // namespace calculatorService
}  // namespace vector
