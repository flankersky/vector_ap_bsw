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
/**        \file  static_vector.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_
#define LIB_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>

#include "vac/container/array_view.h"

namespace vac {
namespace container {

/**
 * \brief Class to (owning) manage a contiguous memory segment.
 *
 * The size of the memory segment can be set once. The memory segment is allocated when calling StaticVector::reserve()
 * and is deallocated when the object is destroyed.
 * Allocations/Deallocations are performed from the given allocator.
 */
template <typename T, typename alloc = std::allocator<T>>
class StaticVector final {
 public:
  /**
   * \brief Typedef for the array_view used internally.
   */
  using array_view_type = array_view<T>;

  /**
   * \brief Typedef for the size value
   */
  using size_type = typename array_view_type::size_type;

  /**
   * \brief Typedef for the contained element
   */
  using value_type = typename array_view_type::value_type;

  /**
   * \brief Typedef for a pointer
   */
  using pointer = typename array_view_type::pointer;

  /**
   * \brief Typedef for a const pointer
   */
  using const_pointer = typename array_view_type::const_pointer;

  /**
   * \brief Typedef for a reference
   */
  using reference = typename array_view_type::reference;

  /**
   * \brief Typedef for a const reference
   */
  using const_reference = typename array_view_type::const_reference;

  /**
   * \brief Typedef for the allocator used
   */
  using allocator_type = alloc;

  /**
   * \brief Typedef for the allocator type used after rebinding
   */
  using actual_allocator_type = typename allocator_type::template rebind<T>::other;

  /**
   * \brief Typedef for an iterator
   */
  using iterator = typename array_view_type::iterator;

  /**
   * \brief Typedef for a const iterator
   */
  using const_iterator = typename array_view_type::const_iterator;

  StaticVector() : delegate_(array_view<T>()), allocator_(), max_num_elements_(0) {}

  /**
   * \brief Initialize a StaticVector with an allocator
   * \param allocator the allocator to use for initialization
   */
  explicit StaticVector(allocator_type& allocator) : allocator_(allocator), max_num_elements_(0) {}

  /// Deleted copy constructor
  StaticVector(const StaticVector&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  StaticVector& operator=(const StaticVector&) = delete;

  /**
   * \brief Move constructor
   * \param other the vector from which to construct the new vector
   */
  StaticVector(StaticVector&& other) : StaticVector() { this->swap(other); }

  /**
   * \brief Move Assignment
   * \param other the vector from which to construct the new vector
   * \returns a reference to the assigned-to object
   */
  StaticVector& operator=(StaticVector&& other) {
    this->swap(other);
    return *this;
  }

  ~StaticVector() {
    shorten(0);
    if (delegate_.data() != nullptr) {
      allocator_.deallocate(delegate_.data(), max_num_elements_);
    }
  }

  /**
   * \brief Allocate a memory segment from the allocator.
   *
   * \param num_elements The number of elements of T to allocate
   *
   * \throws std::runtime_error if reserve() was called before.
   */
  void reserve(size_type num_elements) {
    if (delegate_.data() != nullptr) {
      throw std::runtime_error("StaticVector has already been reserved");
    }
    max_num_elements_ = num_elements;
    delegate_ = array_view_type(allocator_.allocate(num_elements), 0);
  }

  /**
   * \brief Set the vector to be of a certain size.
   *
   * \param num_elements The number of elements of T to allocate
   *
   * Note: currently, resize() implicitly calls reserve() if it increases the size.
   */
  void resize(size_type num_elements) {
    if (num_elements > max_num_elements_) {
      reserve(num_elements);
    }
    size_type old_num_elements = delegate_.size();

    // Destroy excess elements
    shorten(num_elements);

    // Resize the delegate
    delegate_ = array_view_type(delegate_.data(), num_elements);

    // Initialize new elements
    for (size_type index = old_num_elements; index < delegate_.size(); ++index) {
      new (&delegate_[index]) T();
    }
  }

  /**
   * \brief Shorten the vector to the given size.
   *
   * \param num_elements The number elements of T the vector shall be shortened to.
   *
   * \note Does nothing if num_elements is not smaller than size()
   */
  void shorten(size_type num_elements) {
    // will this #elements actually shorten?
    if (num_elements < delegate_.size()) {
      for (size_type index = num_elements; index < delegate_.size(); ++index) {
        delegate_[index].~T();
      }
      delegate_ = array_view_type(delegate_.data(), num_elements);
    }
  }

  /**
   * \brief Swap the contents of two StaticVector objects including their allocator
   * \param other the second vector whose contents are swapped
   */
  void swap(StaticVector& other) noexcept {
    delegate_.swap(other.delegate_);
    std::swap(allocator_, other.allocator_);
    std::swap(max_num_elements_, other.max_num_elements_);
  }

  /**
   * \brief Get the length of the contained array
   * \returns the number of contained elements
   */
  size_type size() const noexcept { return delegate_.size(); }

  /**
   * \brief Get the maximum number of elements this StaticVector can hold
   * \returns the number of elements this vector can store
   */
  size_type capacity() const noexcept { return max_num_elements_; }

  /**
   * \brief Check whether the contained array is empty
   * \returns true if the vector is empty
   */
  bool empty() const noexcept { return delegate_.empty(); }

  /**
   * \brief Get a pointer to the contained data
   * \returns a pointer to the data
   */
  pointer data() noexcept { return delegate_.data(); }

  /**
   * \brief Get a const pointer to the contained data
   * \returns a pointer to the data
   */
  const_pointer data() const noexcept { return delegate_.data(); }

  /**
   * \brief Iterator to the start of the array
   * \returns an iterator to the start of the vector
   */
  iterator begin() { return delegate_.begin(); }
  /**
   * \brief Past-The-End iterator of the array
   * \returns an iterator to the sentinel value at the end
   */
  iterator end() { return delegate_.end(); }

  /**
   * \brief Const iterator to the start of the array
   * \returns an const iterator to the start of the vector
   */
  const_iterator begin() const { return delegate_.begin(); }
  /**
   * \brief Const Past-The-End iterator of the array
   */
  const_iterator end() const { return delegate_.end(); }

  /**
   * \brief Const iterator to the start of the array
   */
  const_iterator cbegin() const { return delegate_.cbegin(); }
  /**
   * \brief Const past-The-End iterator of the array
   * \returns an const iterator to the sentinel value at the end
   */
  const_iterator cend() const { return delegate_.cend(); }

  /**
   * \brief Get the element at the specified position
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   * \throws std::out_of_range if pos >= size()
   */
  reference at(size_type pos) { return delegate_.at(pos); }

  /**
   * \brief Get the element at the specified position (const version)
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   * \throws std::out_of_range if pos >= size()
   */
  const_reference at(size_type pos) const { return delegate_.at(pos); }

  /**
   * \brief Unchecked array access
   *
   * The behavior is unspecified if the container is empty or if pos >= size()
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   */
  reference operator[](size_type pos) { return delegate_[pos]; }

  /**
   * \brief Unchecked array access
   *
   * The behavior is unspecified if the container is empty or if pos >= size()
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   */
  const_reference operator[](size_type pos) const { return delegate_[pos]; }

  /**
   * \brief Unchecked access to first element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the first element
   */
  reference front() { return delegate_.front(); }

  /**
   * \brief Unchecked access to first element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the first element
   */
  const_reference front() const { return delegate_.front(); }

  /**
   * \brief Unchecked access to last element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the last element
   */
  reference back() { return delegate_.back(); }

  /**
   * \brief Unchecked access to last element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the last element
   */
  const_reference back() const { return delegate_.back(); }

  /**
   * \brief Add an element to the back of the StaticVector.
   *
   * \param elem The element to add.
   *
   * \throws std::bad_alloc If the StaticVector is already full
   */
  void push_back(const T& elem) {
    if (delegate_.size() >= max_num_elements_) {
      throw std::bad_alloc();
    } else {
      delegate_ = array_view_type(delegate_.data(), delegate_.size() + 1);
      new (&delegate_.back()) T(elem);
    }
  }

  /**
   * \brief Add an element to the back of the StaticVector.
   *
   * \param args Constructor arguments for T
   *
   * \throws std::bad_alloc If the StaticVector is already full
   */
  template <typename... Args>
  void emplace_back(Args&&... args) {
    if (delegate_.size() >= max_num_elements_) {
      throw std::bad_alloc();
    } else {
      delegate_ = array_view_type(delegate_.data(), delegate_.size() + 1);
      new (&delegate_.back()) T(std::forward<Args>(args)...);
    }
  }

  /**
   * \brief Remove an element to the back of the StaticVector.
   *
   * \throws std::out_of_range If the StaticVector is empty
   */
  void pop_back() {
    if (delegate_.size() <= 0) {
      throw std::out_of_range("StaticVector::pop_back: vector is empty");
    } else {
      reference elem = delegate_.back();
      elem.~T();
      delegate_ = array_view_type(delegate_.data(), delegate_.size() - 1);
    }
  }

  /**
   * \brief Remove an element anywhere in the static vector
   *
   * \param elem Iterator to the element to be removed
   *
   * \return an iterator to the element following the removed element
   */
  iterator erase(iterator elem) {
    if (elem != end()) {
      // there is at least one element present

      iterator current = elem;
      iterator next = elem + 1;

      while (next != end()) {
        *current = std::move(*next);
        ++current;
        ++next;
      }

      pop_back();
    }

    return elem;
  }

  /**
   * \brief Remove all elements from the vector.
   */
  void clear() { shorten(0); }

 private:
  /**
   * \brief an array_view to implement access to the data.
   *
   * The array_view is also used to hold the pointer to the owned data. Ownership semantics are implemented by the
   * StaticVector itself.
   *
   * The size of the delegate_ represents the current length of the vector, not the amount of memory allocated.
   */
  array_view_type delegate_;

  /**
   * \brief Allocator used to obtain memory.
   */
  actual_allocator_type allocator_;

  /**
   * \brief The maximum number of elements (i.e., amount of memory allocated) that this StaticVector can hold.
   */
  size_type max_num_elements_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_STATIC_VECTOR_H_
