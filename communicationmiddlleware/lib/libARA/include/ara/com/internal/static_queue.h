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
/**        \file  static_queue.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_STATIC_QUEUE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_STATIC_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <queue>
#include <utility>

namespace ara {
namespace com {
namespace internal {

/**
 * \brief StaticQueue is used as a wrapper to simply exchange the underlying
 * queue type. StaticQueue has an upper limit to avoid "re-allocations" and its memory is allocated once during
 * initialization.
 *
 * \tparam T Type of value to store within this queue.
 */
template <typename T>
class StaticQueue {
 public:
  /**
   * \brief Type of value to store.
   */
  using value_type = T;

  /**
   * \brief Reference type
   */
  using reference = value_type&;

  /**
   * \brief Constant reference type
   */
  using const_reference = const value_type&;

  /**
   * \brief Type of the static queue.
   */
  using QueueType = std::queue<value_type>;

  /**
   * \brief Size type
   */
  using size_type = typename QueueType::size_type;

  /**
   * \brief Current size of this queue.
   *
   * \return the current size (number of elements) queued.
   */
  size_type size() const noexcept { return queue_.size(); }

  /**
   * \brief Inserts one element at the end of the queue.
   *
   * \param val Value as a reference to push into this queue.
   * \return true If queueing was possible, false if the maximum capacity is reached.
   * The current policy is to avoid pushing additional elements to the queue.
   */
  bool push(const value_type& val) {
    bool pushed{true};
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (queue_.size() >= capacity()) {
      pushed = false;
    } else {
      queue_.push(val);
    }
    return pushed;
  }

  /**
   * \brief Inserts one element at the end of the queue.
   *
   * \param val Value as an rvalue to push into this queue.
   * \return true If queueing was possible, false if the maximum capacity is reached.
   * The current policy is to avoid pushing additional elements to the queue.
   */
  bool push(value_type&& val) {
    bool pushed{true};
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (queue_.size() >= capacity()) {
      pushed = false;
    } else {
      queue_.push(std::move(val));
    }
    return pushed;
  }

  /**
   * \brief Removes the next element in the queue and reduces its size by one.
   */
  void pop() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    queue_.pop();
  }

  /**
   * \brief Test if this queue is empty.
   *
   * \return true if this queue is empty, false if there are elements queued.
   */
  bool empty() const noexcept { return queue_.empty(); }

  /**
   * \brief Return the reference to the next element in the queue.
   */
  reference front() { return queue_.front(); }

  /**
   * \brief Return the const reference to the next element in the queue.
   */
  const_reference front() const { return queue_.front(); }

  /**
   * \brief Get the maximum number of elements this StaticVector can hold
   * \returns the number of elements this vector can store
   */
  size_type capacity() const noexcept { return max_capacity_; }

  /**
   * \brief Define an upper limit of elements this queue can hold.
   *
   * \param capacity Maximum limit to store.
   */
  void reserve(size_type capacity) { max_capacity_ = capacity; }

 private:
  /**
   * \brief Elements within this queue.
   */
  QueueType queue_;

  /**
   * \brief Maximum capacity this queue can hold.
   */
  size_type max_capacity_{};

  /**
   * \brief Queue mutex for pushing and popping.
   */
  std::mutex queue_mutex_;
};
}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_STATIC_QUEUE_H_
