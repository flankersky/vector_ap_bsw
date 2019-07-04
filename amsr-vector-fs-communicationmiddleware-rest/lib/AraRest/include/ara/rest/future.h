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
/**        \file  future.h
 *        \brief  Specific implementation of a future for ara::rest
 *
 *      \details  ara::rest::Future is a composition of basic features of std::future
 *                and methods borrowed from facebook folly::Future.
 *          \see  SWS_CM_00321
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_FUTURE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_FUTURE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <future>
#include <utility>

#include "ara/rest/types.h"

namespace ara {
namespace rest {

/**
 * \brief Operation status for timed wait functions
 * \see SWS_CM_00320
 */
enum class FutureStatus : uint8_t { ready, timeout };

/**
 * \brief ara::com specific Future
 */
template <typename T>
class Future {
 public:
  /** \brief Default constructor
   *
   */
  Future() {}
  /**
   * \brief Default constructor
   */
  explicit Future(std::future<T>&& delegate_future) noexcept : delegate_future_(std::move(delegate_future)) {}

  /**
   * \brief Move constructor
   */
  Future(Future&&) noexcept = default;
  /**
   * \brief Default copy constructor deleted
   */
  Future(const Future&) = delete;
  /**
   * \brief Specialized unwrapping constructor
   */
  explicit Future(Future<Future<T>>&&) noexcept;
  /**
   * \brief Destructor
   */
  ~Future() = default;
  /**
   * \brief Move assignment operator
   */
  Future& operator=(Future&&) noexcept;
  /**
   * \brief Default copy assignment operator deleted
   */
  Future& operator=(const Future&) = delete;
  /**
   * \brief Returns the result
   */
  T get() { return delegate_future_.get(); }
  /**
   * \brief Check if the Future has any shared state
   */
  bool valid() const noexcept { return delegate_future_.valid(); }
  /**
   * \brief Block until the shared state is ready
   */
  void wait() const { delegate_future_.wait(); }
  /**
   * \brief Wait for a specified relative time
   */
  template <class Rep, class Period>
  FutureStatus wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const {
    FutureStatus retval = FutureStatus::timeout;
    auto f_stat = delegate_future_.wait_for(timeout_duration);
    if (std::future_status::ready == f_stat) {
      retval = FutureStatus::ready;
    }
    return retval;
  }
  /**
   * \brief Wait until a specified absolute time
   */
  template <class Clock, class Duration>
  FutureStatus wait_until(const std::chrono::time_point<Clock, Duration>& abs_time) const {
    FutureStatus retval = FutureStatus::timeout;
    auto f_stat = delegate_future_.wait_until(abs_time);
    if (std::future_status::ready == f_stat) {
      retval = FutureStatus::ready;
    }
    return retval;
  }
  /**
   * \brief Set a continuation for when the shared state is ready
   */
  template <typename F>
  auto then(F&& func) -> Future<decltype(func(std::move(*this)))>;
  /**
   * \brief Return true only when the shared state is ready
   */
  bool is_ready() const;

 private:
  std::future<T> delegate_future_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_FUTURE_H_
