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
/**        \file  proxy_event.h
 *        \brief  TODO
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <type_traits>
#include <utility>
#include "ara/com/internal/proxy_event_base.h"
#include "ara/com/internal/types.h"
#include "ara/e2e/e2exf/e2e_result.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Generic template class for event handling (Subscribe, Unsubscribe, Update, ...) by a proxy.
 * \tparam ProxyImplInterface Related ProxImplInterface
 * \tparam SampleType Type of event samples
 * \tparam EventManagerReturnType Concrete event manager type returned by GetEventManagerMethod
 * \tparam GetEventManagerMethod Method to get the event manager object for event handling in the ProxyImpl
 */
template <typename ProxyImplInterface, typename SampleType, typename EventManagerReturnType,
          EventManagerReturnType& (ProxyImplInterface::*GetEventManagerMethod)()>
class ProxyEvent : public ProxyEventBase {
 public:
  /**
   * \brief Type-alias which is used by other classes such as the field template-class.
   */
  using ImplInterface = ProxyImplInterface;
  /**
   * \brief Constructor
   * \param proxy_impl Pointer to the related backend Proxy implementation.
   */
  explicit ProxyEvent(ImplInterface* proxy_impl) noexcept : proxy_impl_{proxy_impl} {}

  /**
   * \brief Destructor
   */
  ~ProxyEvent() = default;

  /**
   * \brief Subscribes to the skeleton counterpart of this proxy-side event
   * \param policy update policy of the underlying event cache
   * \param cache_size size limit of this cache
   */
  void Subscribe(ara::com::EventCacheUpdatePolicy policy, std::size_t cache_size) {
    (proxy_impl_->*GetEventManagerMethod)().Subscribe(this, policy, cache_size);
  }

  /**
   * \brief Unsubscribes from the skeleton counterpart of this proxy-side event
   */
  void Unsubscribe() { (proxy_impl_->*GetEventManagerMethod)().Unsubscribe(); }

  /**
   * \brief Updates the event cache container visible to the user via GetCachedSamples()
   * \uptrace SWS_CM_00171
   * \param filter A function that indicates whether an event should be copied to the event cache
   * \return true if at least one new event was transferred to the event cache
   */
  bool Update(const ara::com::FilterFunction<SampleType>& filter =
                  ara::com::internal::AcceptAllSamplesFilterFunction<SampleType>()) {
    // Protect concurrent modification of visible_sample_cache_ by Update(), GetCachedSamples(), Cleanup() APIs.
    std::lock_guard<std::mutex> guard(visible_sample_cache_lock_);
    return (proxy_impl_->*GetEventManagerMethod)().Update(filter, visible_sample_cache_);
  }

  /**
   * \brief Returns the event cache container visible to the user
   * \return A reference to the event cache container
   */
  const ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>>& GetCachedSamples() const {
    // Protect concurrent modification of visible_sample_cache_ by Update(), GetCachedSamples(), Cleanup() APIs.
    std::lock_guard<std::mutex> guard(visible_sample_cache_lock_);
    return visible_sample_cache_;
  }

  /**
   * \brief Access the E2EState of the recent E2ECheck.
   *
   * \uptrace SWS_CM_90431
   * \return The E2E status of the latest E2E check.
   */
  ara::e2e::state_machine::State GetE2EState() const { return (proxy_impl_->*GetEventManagerMethod)().GetE2EState(); }

  /**
   * \brief Clears the cached event samples visible to the user if the selected caching policy is
   * ara::com::EventCacheUpdatePolicy::kNewestN. Otherwise has no effect.
   * \uptrace SWS_CM_00171
   */
  void Cleanup() {
    if ((proxy_impl_->*GetEventManagerMethod)().GetEventCacheUpdatePolicy() ==
        ara::com::EventCacheUpdatePolicy::kNewestN) {
      // Protect concurrent modification of visible_sample_cache_ by Update(), GetCachedSamples(), Cleanup() APIs.
      std::lock_guard<std::mutex> guard(visible_sample_cache_lock_);
      visible_sample_cache_.clear();
    }
  }

 private:
  /// SampleContainer type
  using SampleContainer = ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>>;

  /// Pointer to the related ProxyImplInterface used to access the concrete event manager
  ImplInterface* proxy_impl_;

  /// Event cache visible to the user
  SampleContainer visible_sample_cache_;

  /// Mutex to protect visible_sample_cache_
  mutable std::mutex visible_sample_cache_lock_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_H_
