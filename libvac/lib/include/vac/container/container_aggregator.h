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
/**        \file  container_aggregator.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_CONTAINER_AGGREGATOR_H_
#define LIB_INCLUDE_VAC_CONTAINER_CONTAINER_AGGREGATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <numeric>
#include <stdexcept>
#include <utility>

#include "vac/container/array_view.h"

namespace vac {
namespace container {

/**
 * \brief Meta container class which allows for iterating over all elements of multiple containers of the same type
 */
template <typename ContainerPointer>
class ContainerAggregator final {
 public:
  /// Pointer type to containers (can be raw pointer, unique_ptr, shared_ptr)
  using ContainerPointerType = ContainerPointer;
  /// Type of containers to be aggregated
  using ContainerType = typename std::pointer_traits<ContainerPointerType>::element_type;
  /// Value type of container elements
  using value_type = typename ContainerType::value_type;
  /// Storage type for containers
  using StorageType = vac::container::array_view<ContainerPointer>;

  /// \brief const iterator type
  class const_iterator final : public std::iterator<std::forward_iterator_tag, const value_type> {
   public:
    /// Iterator for containers
    using container_iterator = typename StorageType::const_iterator;
    /// Iterator for container elements
    using element_iterator = typename ContainerType::const_iterator;

    /**
     * \brief Constructor.
     *
     * \param aggregator container to be iterated.
     * \param container_iter
     * \param element_iter
     */
    const_iterator(const ContainerAggregator& aggregator, const container_iterator& container_iter,
                   const element_iterator& element_iter)
        : aggregator_(aggregator), container_iter_(container_iter), element_iter_(element_iter) {}

    /**
     * \brief Move constructor.
     */
    const_iterator(const_iterator&) = default;

    /**
     * \brief Move constructor.
     */
    const_iterator(const_iterator&&) = default;

    /**
     * \brief Copy assignment.
     *
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const_iterator&) = default;

    /**
     * \brief Move assignment.
     *
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const_iterator&&) = default;

    /**
     * \brief destructor.
     */
    ~const_iterator() = default;

    /**
     * \brief Compare two iterators for equality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to the same map node.
     */
    bool operator==(const const_iterator& other) const {
      if (&aggregator_ == &other.aggregator_) {
        if (container_iter_ == aggregator_.registered_containers_.cend()) {
          return container_iter_ == other.container_iter_;
        } else {
          return (container_iter_ == other.container_iter_) && (element_iter_ == other.element_iter_);
        }
      } else {
        return false;
      }
    }

    /**
     * \brief Compare two iterators for inequality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to different map nodes.
     */
    bool operator!=(const const_iterator& other) const { return !(*this == other); }

    /**
     * \brief increment the iterator by one element.
     *
     * \returns incremented iterator.
     */
    const_iterator& operator++() {
      if (aggregator_.cend() != *this) {
        ++element_iter_;
        if (element_iter_ == (*container_iter_)->cend()) {
          // go to the next container containing at least one element
          do {
            ++container_iter_;
            if (container_iter_ == aggregator_.registered_containers_.cend()) {
              // end reached
              element_iter_ = element_iterator{};
              return *this;
            }
          } while ((*container_iter_)->size() == 0);
          element_iter_ = (*container_iter_)->cbegin();
        }
      }
      return *this;
    }

    /**
     * \brief Post increment the iterator by one element.
     *
     * \returns iterator prior to increment
     */
    const_iterator& operator++(int) {
      const_iterator tmp = *this;
      this->operator++();
      return tmp;
    }

    /**
     *  \brief Dereference operator
     */
    const value_type& operator*() const { return *element_iter_; }

   private:
    const ContainerAggregator& aggregator_;
    container_iterator container_iter_;
    element_iterator element_iter_;
  };

  /**
   * \brief Constructor.
   *
   * \param meta_info_map_pointer_buffer view to buffer where the container pointers are added via push_back
   *
   */
  explicit ContainerAggregator(vac::container::array_view<ContainerPointerType> meta_info_map_pointer_buffer)
      : container_pointer_buffer_{meta_info_map_pointer_buffer},
        registered_containers_{meta_info_map_pointer_buffer.data(), 0} {}

  ~ContainerAggregator() = default;

  /**
   * Returns the maximum number of containers the aggregator can carry.
   *
   * \return maximum number of containers
   */
  std::size_t capacity() const { return container_pointer_buffer_.size(); }

  /**
   * \brief Returns the current number of containers registered in the aggregator.
   *
   * \return number of containers
   */
  std::size_t NumberOfRegisteredContainers() const { return registered_containers_.size(); }

  /**
   * \brief Returns the total number of container elements
   *
   * \return number of container elements-
   */
  std::size_t size() const {
    std::size_t total_size = 0;
    for (const auto& container_ptr : registered_containers_) {
      total_size += container_ptr->size();
    }
    return total_size;
  }

  /**
   * \brief Adds the given container to the aggregator.
   *
   * \param container_ptr container to add
   * Throws std::out_of_range when another container would exceed capacity
   */
  void push_back(ContainerPointer container_ptr) {
    if (NumberOfRegisteredContainers() < capacity()) {
      // append container
      *registered_containers_.end() = std::move(container_ptr);
      // update regiistered containers
      registered_containers_ = StorageType(registered_containers_.data(), NumberOfRegisteredContainers() + 1);
    } else {
      throw std::out_of_range("Another container pointer exceeds capacity!");
    }
  }

  /**
   * \brief Returns the first container element.
   *
   * \return the first element of a non-empty container, if no containers were added to the aggregator or all
   * containers are empty cend() is returned
   */
  const_iterator cbegin() const {
    for (auto iter = registered_containers_.cbegin(); iter != registered_containers_.cend(); ++iter) {
      if ((**iter).size() > 0) {
        return const_iterator(*this, iter, (**iter).cbegin());
      }
    }
    return cend();
  }

  /**
   * \brief Returns an iterator pointing to element after the last element after the last container.
   * \return
   */
  const_iterator cend() const { return const_iterator(*this, registered_containers_.cend(), {}); }

  /**
   * \brief Returns the first container element.
   *
   * \return the first element of a non-empty container, if no containers were added to the aggregator or all
   * containers are empty cend() is returned
   */
  const_iterator begin() const { return cbegin(); }

  /**
   * \brief Returns an iterator pointing to element after the last element after the last container.
   * \return
   */
  const_iterator end() const { return cend(); }

 private:
  /** Buffer to the registered container pointers */
  const StorageType container_pointer_buffer_;
  /** View to the registered container pointers */
  StorageType registered_containers_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_CONTAINER_AGGREGATOR_H_
