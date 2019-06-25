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
/**        \file  internal/types.h
 *        \brief  Types used by ara::com API
 *
 *      \details  Specific types needed by the ara::com API for proxy, skeleton and the runtime interface.
 *      \see      SWS_CM_01001
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_TYPES_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <memory>
#include <vector>
#include "ara/com/sample_ptr.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Proxy type identifier
 *
 * This is an service-specific unique identifier used in ara::com to identify a service. Concrete binding
 * implementations will map this identifier to transport specific IDs. E.g. SOME/IP will map it to the SOME/IP
 * ServiceId.
 */
class ProxyIdObject {
 public:
  ProxyIdObject() = default;
  ProxyIdObject(const ProxyIdObject& other) = delete;
  ProxyIdObject& operator=(const ProxyIdObject&) = delete;
};

/// The ProxyId itself is always the class specific static memory address of the ProxyIdObjects
using ProxyId = ProxyIdObject*;

/**
 * \brief Skeleton type identifier
 *
 * This is an service-specific unique identifier used in ara::com to identify a service. Concrete binding
 * implementations will map this identifier to transport specific IDs. E.g. SOME/IP will map it to the SOME/IP
 * ServiceId.
 */
class SkeletonIdObject {
 public:
  SkeletonIdObject() = default;
  SkeletonIdObject(const SkeletonIdObject& other) = delete;
  SkeletonIdObject& operator=(const SkeletonIdObject&) = delete;
};

/// The SkeletonId itself is always the class specific static memory address of the SkeletonIdObjects
using SkeletonId = SkeletonIdObject*;

/**
 * \brief ApplicationError error code
 * \see TPS_MANI_01056
 */
using ApplicationErrorCode = std::uint8_t;

}  // namespace internal

/**
 * \brief Container for a list of service handles
 * \see SWS_CM_00304
 */
template <typename T>
using ServiceHandleContainer = std::vector<T>;

/**
 * \brief  Function wrapper for the handler function that gets called
 * \see SWS_CM_00305
 */
template <typename T>
using FindServiceHandler = std::function<void(ServiceHandleContainer<T>)>;

/**
 * \brief Definition of internal instance ID type that should be binding agnostic.
 */
using InstanceId = std::uint16_t;

/**
 * \brief Numeric representation of InstanceIdentifier::Any
 */
static constexpr InstanceId kInstanceIdAny = 0xFFFF;

/**
 * \brief Definition of service version type
 */
using ServiceVersion = std::uint32_t;

/**
 * \brief Identifier for a triggered FindService request
 * \see SWS_CM_00303
 */
struct FindServiceHandle {
  /**
   * \brief Internal proxy ID
   */
  internal::ProxyId proxy_id;
  /**
   * \brief Internal instance ID
   */
  InstanceId instance_id;
  /**
   * \brief Internal uid
   */
  std::uint32_t uid;

  /**
   * \brief SWS_CM_00303
   */
  bool operator==(const FindServiceHandle& rhs) const {
    return (proxy_id == rhs.proxy_id) && (instance_id == rhs.instance_id) && (uid == rhs.uid);
  }

  /**
   * \brief SWS_CM_00303
   */
  bool operator<(const FindServiceHandle& rhs) const { return (uid < rhs.uid); }
};

/**
 * \brief  Defines the policy of the event cache update
 * \see SWS_CM_00300
 */
enum class EventCacheUpdatePolicy : uint8_t { kLastN, kNewestN };

/**
 * \brief Function wrapper for the handler function that gets called when new event data arrives
 * \see SWS_CM_00309
 */
using EventReceiveHandler = std::function<void()>;

/**
 * \brief  Holds a list of pointers to data samples and is received via event communication.
 * \see SWS_CM_00307
 */
template <typename T>
using SampleContainer = std::vector<T>;

/**
 * \brief Pointer to a data sample allocated by the runtime
 * \see SWS_CM_00308
 */
template <typename T>
using SampleAllocateePtr = std::unique_ptr<T>;

/**
 * \brief Defines the subscription state of an event
 * \see SWS_CM_00310
 */
enum class SubscriptionState : uint8_t { kSubscribed = 0, kNotSubscribed = 1, kSubscriptionPending = 2 };

/**
 * \param Interface for handling event subscription state.
 *
 * \see SWS_CM_00311
 */
class SubscriptionStateChangeHandler {
 public:
  /**
   * \brief Destructor
   */
  virtual ~SubscriptionStateChangeHandler() = default;

  /**
   * \brief The handler function that gets called by the runtime in case the subscription state of an event has changed.
   * The concrete handler must override this function.
   *
   * \param state The current event subscription state.
   */
  virtual void OnChange(const ara::com::SubscriptionState state) = 0;
};

/**
 * \brief Defines the processing modes for the service implementation side
 * \see SWS_CM_00301
 */
enum class MethodCallProcessingMode : uint8_t { kPoll, kEvent, kEventSingleThread };

/**
 * \brief Function representing a filter function provided by the programmer.
 * \see SWS_CM_00171
 * \tparam SampleType Type of single samples
 *
 * Returning true means to accept/keep the sample in the Update() call.
 */
template <typename SampleType>
class FilterFunction {
 public:
  virtual ~FilterFunction() = default;

  /**
   * \brief Filter a single event sample
   * \param data the Event sample to be filtered
   * \return true if the sample shall be stored in the event cache visible to the user
   */
  virtual bool filter(const SampleType& data) const = 0;
};

namespace internal {

/**
 * \brief Default implementation of an ara::com::FilterFunction accepting all samples
 * \tparam SampleType Type of single samples
 */
template <typename SampleType>
class AcceptAllSamplesFilterFunction : public ara::com::FilterFunction<SampleType> {
 public:
  /**
   * \brief Filter a single event sample
   * \param data the Event sample to be filtered
   * \return Always true. Always accept the event sample.
   */
  bool filter(const SampleType& data) const override {
    (void)data;
    return true;
  }
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_TYPES_H_
