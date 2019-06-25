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
/**        \file  proxy_impl_event.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_EVENT_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <deque>
#include <mutex>
#include <utility>

#include "ara/com/internal/event_sample.h"
#include "ara/com/internal/proxy_event_base.h"
#include "ara/com/types.h"
#include "ara/e2e/e2exf/transformer.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Template for proxy events of the backend. Handles event cache updates.
 * \tparam SampleType Type of a single event sample
 */
template <typename SampleType>
class ProxyImplEvent {
 public:
  /// FilterFunction type
  using FilterFunction = ara::com::FilterFunction<SampleType>;

  /// SampleContainer type
  using SampleContainer = ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>>;

  /**
   * \brief Destructor
   */
  virtual ~ProxyImplEvent() = default;

  /**
   * \brief Subscribes the event.
   *
   * \param event A pointer to the corresponding proxy event base.
   * \param policy An event cache policy.
   * \param cache_size The maximum number of cached events.
   */
  virtual void Subscribe(ara::com::internal::ProxyEventBase* event, ara::com::EventCacheUpdatePolicy policy,
                         std::size_t cache_size) = 0;

  /**
   * \brief Unsubscribes from an event.
   */
  virtual void Unsubscribe() = 0;

  /**
   * \brief Transfers the received events to the passed event container taking into account the current policy and the
   * passed filter function.
   *
   * \param filter A filter function indicating whether an event should be copied.
   * \param visible_cache An event container which will receive events.
   * \return true if the number of copied events is greater than 0 and false otherwise.
   */
  bool Update(const FilterFunction& filter, SampleContainer& visible_cache) {
    std::lock_guard<std::mutex> guard(sample_cache_lock_);
    bool result = false;
    if (policy_ == ara::com::EventCacheUpdatePolicy::kNewestN) {
      visible_cache.clear();
      for (auto it = invisible_sample_cache_.begin(); it != invisible_sample_cache_.end(); ++it) {
        if (filter.filter(it->GetData())) {
          visible_cache.emplace_back(std::move(it->GetData()), it->GetE2ECheckStatus());
          result = true;
        }
      }
      invisible_sample_cache_.clear();
    } else {
      assert(this->policy_ == ara::com::EventCacheUpdatePolicy::kLastN);
      for (auto it = invisible_sample_cache_.begin(); it != invisible_sample_cache_.end(); ++it) {
        if (filter.filter(it->GetData())) {
          if (visible_cache.size() >= cache_size_) {
            visible_cache.erase(visible_cache.begin());
          }
          visible_cache.emplace_back(std::move(it->GetData()), it->GetE2ECheckStatus());
          result = true;
        }
      }
      invisible_sample_cache_.clear();
    }
    return result;
  }

  /**
   * \brief Access the E2EState of the recent E2ECheck.
   * \return The E2E status of the latest E2E check.
   */
  ara::e2e::state_machine::State GetE2EState() const {
    std::lock_guard<std::mutex> guard(sample_cache_lock_);
    return e2e_state_;
  }

  /**
   * \brief Gets the current subscribed event cache update policy, if the event is currently subscribed.
   * Otherwise, gets the latest subscription policy used.
   * If the event was never subscribed before, ara::com::EventCacheUpdatePolicy::kLastN is returned.
   *
   * \return The current or latest subscription policy used
   */
  ara::com::EventCacheUpdatePolicy GetEventCacheUpdatePolicy() { return policy_; }

 protected:
  /**
   * \brief Event sample cache
   */
  using SampleCache = std::deque<EventSample<SampleType>>;

  /**
   * \brief Stores a new event sample.
   *
   * \param data An event sample.
   */
  void push(SampleType const& data) {
    std::lock_guard<std::mutex> guard(sample_cache_lock_);
    if (invisible_sample_cache_.size() >= cache_size_) {
      invisible_sample_cache_.pop_front();
    }

    // No E2E handling for this event. Therefore set default state and check result
    invisible_sample_cache_.emplace_back(data, ara::e2e::state_machine::E2ECheckStatus::NotAvailable);
    // Update the event specific E2E state machine state
    e2e_state_ = ara::e2e::state_machine::E2EState::NoData;
  }

  /**
   * \brief Stores a new pair of event sample and E2E result
   *
   * \param data An event sample.
   * \param e2e_result E2E check result
   */
  void push(SampleType const& data, const ara::e2e::e2exf::Result e2e_result) {
    std::lock_guard<std::mutex> guard(sample_cache_lock_);
    if (invisible_sample_cache_.size() >= cache_size_) {
      invisible_sample_cache_.pop_front();
    }
    invisible_sample_cache_.emplace_back(data, e2e_result.GetCheckStatus());
    // Update the event specific E2E state machine state
    e2e_state_ = e2e_result.GetState();
  }

  /**
   * \brief An event update policy.
   */
  ara::com::EventCacheUpdatePolicy policy_;

  /**
   * \brief Maximum number of stored events.
   */
  std::size_t cache_size_;

  /**
   * \brief Event storage.
   */
  SampleCache invisible_sample_cache_;

  /**
   * \brief Mutex to protect sample cache and E2E state
   */
  mutable std::mutex sample_cache_lock_;

  /**
   * \brief E2E state machine state
   */
  ara::e2e::state_machine::E2EState e2e_state_{ara::e2e::state_machine::E2EState::NoData};
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_EVENT_H_
