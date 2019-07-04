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
/**        \file  calculatorInterface_proxy_someip_event_manager.cc
 *        \brief  SOME/IP proxy event handling for events and field notifications of service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vector/calculatorService/calculatorInterface_proxy_someip_event_manager.h"
#include <string>
#include <tuple>
#include <utility>
#include "vector/calculatorService/calculatorInterface.h"
#include "vector/calculatorService/calculatorInterface_proxy_someip_binding.h"

namespace vector {
namespace calculatorService {

/* ---- Event 'divisionByZero' ------------------------------------------- */

CalculatorInterfaceProxySomeIpEventManagerDivisionByZero::CalculatorInterfaceProxySomeIpEventManagerDivisionByZero(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : proxy_binding_(proxy_binding), service_event_(nullptr) {}

void CalculatorInterfaceProxySomeIpEventManagerDivisionByZero::Subscribe(
    ara::com::internal::ProxyEventBase* service_event, ara::com::EventCacheUpdatePolicy cache_policy,
    std::size_t cache_size) {
  if (service_event == nullptr) {
    ara::log::LogWarn() << "No event frontend for subscription state handling given.";
  }

  service_event_ = service_event;
  policy_ = cache_policy;
  cache_size_ = cache_size;
  proxy_binding_.SubscribeEvent(event_id_, this);
}

void CalculatorInterfaceProxySomeIpEventManagerDivisionByZero::Unsubscribe() {
  proxy_binding_.UnsubscribeEvent(event_id_);
  service_event_ = nullptr;
}

template <typename RootDeserializer>
class DeserializeCalculatorInterfaceDivisionByZerodata {
 public:
  explicit DeserializeCalculatorInterfaceDivisionByZerodata(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'data' (/AUTOSAR_Platform/ImplementationDataTypes/boolean)
   */
  template <typename ParentDeserializer>
  std::pair<boolean, bool> Deserializedata(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    boolean data{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__data{parent};
    deserialization_ok = deserializer__data.PopFront(data) > 0U;
    return std::make_pair(data, deserialization_ok);
  }

  boolean StartDeserialization() {
    auto ret = Deserializedata<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpEventManagerDivisionByZero::HandleEventNotification(
    RootDeserializerAlias&& deserializer) {
  // shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Deserialize event sample
  // Create one deserializer object for argument data
  DeserializeCalculatorInterfaceDivisionByZerodata<typename std::decay<decltype(deserializer)>::type>
      arg_data_deserializer{&deserializer};
  boolean data = arg_data_deserializer.StartDeserialization();

  // Store data in invisible sample cache
  this->push(data);

  if (service_event_) {
    // Notify EventReceiveHandler
    service_event_->Notify();
  } else {
    ara::log::LogWarn() << "Notification on event sample reception failed due to missing frontend reference. Avoiding "
                           "nullptr dereference.";
  }
}

void CalculatorInterfaceProxySomeIpEventManagerDivisionByZero::HandleEventSubscriptionStateUpdate(
    ara::com::SubscriptionState state) {
  if (service_event_) {
    // Notify the subscription handler.
    service_event_->NotifySubscriptionStateUpdate(state);
  } else {
    // service_event_ might have already been unset after Unsubscribe call.
    if (state != ara::com::SubscriptionState::kNotSubscribed) {
      ara::log::LogError()
          << "Routing of event subscription state update failed. No subscription found for event 'DivisionByZero'";
    }
  }
}

/* ---- Field notifier 'divideResult' ------------------------------------------- */

CalculatorInterfaceProxySomeIpFieldNotifierDivideResult::CalculatorInterfaceProxySomeIpFieldNotifierDivideResult(
    CalculatorInterfaceProxySomeIpBinding& proxy_binding)
    : proxy_binding_(proxy_binding), service_event_(nullptr) {}

void CalculatorInterfaceProxySomeIpFieldNotifierDivideResult::Subscribe(
    ara::com::internal::ProxyEventBase* service_event, ara::com::EventCacheUpdatePolicy cache_policy,
    std::size_t cache_size) {
  if (service_event == nullptr) {
    ara::log::LogWarn() << "No event frontend for subscription state handling given.";
  }

  service_event_ = service_event;
  policy_ = cache_policy;
  cache_size_ = cache_size;
  proxy_binding_.SubscribeEvent(event_id_, this);
}

void CalculatorInterfaceProxySomeIpFieldNotifierDivideResult::Unsubscribe() {
  proxy_binding_.UnsubscribeEvent(event_id_);
  service_event_ = nullptr;
}

template <typename RootDeserializer>
class DeserializeCalculatorInterfaceDivideResultdata {
 public:
  explicit DeserializeCalculatorInterfaceDivideResultdata(RootDeserializer* root) : deserializer_{root} {}

  /* ImplementationDataTypeValue deserialization for element 'data' (/AUTOSAR_Platform/ImplementationDataTypes/uint32)
   */
  template <typename ParentDeserializer>
  std::pair<uint32, bool> Deserializedata(ParentDeserializer* parent) {
    namespace marshaller = someip_posix_common::someip::serialization;
    bool deserialization_ok{true};
    uint32 data{};
    marshaller::Deserializer<marshaller::BEPayloadNoLengthFieldPolicy, ParentDeserializer> deserializer__data{parent};
    deserialization_ok = deserializer__data.PopFront(data) > 0U;
    return std::make_pair(data, deserialization_ok);
  }

  uint32 StartDeserialization() {
    auto ret = Deserializedata<RootDeserializer>(deserializer_);
    return ret.first;
  }

 private:
  RootDeserializer* deserializer_;
};

void CalculatorInterfaceProxySomeIpFieldNotifierDivideResult::HandleEventNotification(
    RootDeserializerAlias&& deserializer) {
  // shortening
  namespace marshaller = someip_posix_common::someip::serialization;

  // Deserialize event sample
  // Create one deserializer object for argument data
  DeserializeCalculatorInterfaceDivideResultdata<typename std::decay<decltype(deserializer)>::type>
      arg_data_deserializer{&deserializer};
  uint32 data = arg_data_deserializer.StartDeserialization();

  // Store data in invisible sample cache
  this->push(data);

  if (service_event_) {
    // Notify EventReceiveHandler
    service_event_->Notify();
  } else {
    ara::log::LogWarn() << "Notification on event sample reception failed due to missing frontend reference. Avoiding "
                           "nullptr dereference.";
  }
}

void CalculatorInterfaceProxySomeIpFieldNotifierDivideResult::HandleEventSubscriptionStateUpdate(
    ara::com::SubscriptionState state) {
  if (service_event_) {
    // Notify the subscription handler.
    service_event_->NotifySubscriptionStateUpdate(state);
  } else {
    // service_event_ might have already been unset after Unsubscribe call.
    if (state != ara::com::SubscriptionState::kNotSubscribed) {
      ara::log::LogError()
          << "Routing of event subscription state update failed. No subscription found for event 'DivideResult'";
    }
  }
}

}  // namespace calculatorService
}  // namespace vector
