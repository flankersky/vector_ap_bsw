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
/**        \file  array_view.h
 *        \brief  std::array-like container with type-erased length
 *
 *      \details  array_view is a non-owning container for contiguous memory segments of type T.
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_ARRAY_VIEW_H_
#define LIB_INCLUDE_VAC_CONTAINER_ARRAY_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <exception>
#include <iterator>
#include <utility>

namespace vac {
namespace container {

/**
 * \brief Array-like container with type-erased length
 *
 * array_view is a non-owning container for contiguous memory segments of type T.
 */
template <typename T>
class array_view {
 public:
  /// Type of contained element
  using value_type = T;

  /// Type of the size field
  using size_type = std::size_t;

  /// Reference type for contained elements
  using reference = T&;

  /// Const reference type for contained elements
  using const_reference = const typename std::remove_const<T>::type&;

  /// Pointer type for contained elements
  using pointer = T*;

  /// Const pointer type for contained elements
  using const_pointer = const typename std::remove_const<T>::type*;

  /// Iterator for the array_view
  using iterator = pointer;

  /// Const iterator for the array_view
  using const_iterator = const_pointer;

  /// Default constructor for an empty array_view
  constexpr array_view() noexcept : data_(nullptr), size_(0) {}

  /**
   * \brief Constructor to type-erase the length of an std::array
   * \param container the array which to type-erase
   */
  template <typename ContainerType>
  explicit array_view(ContainerType& container) : data_(container.data()), size_(container.size()) {}

  /**
   * \brief Constructor for a raw c-style array
   *
   * When given a nullptr, the size is reset to 0
   *
   * \param data_ptr Pointer to the raw array containing the data
   * \param data_size Length of data in number of elements
   */
  constexpr array_view(pointer data_ptr, size_type data_size) noexcept : data_(data_ptr),
                                                                         size_((data_ptr == nullptr ? 0 : data_size)) {
    // Inline-If in constructor to support constexpr in C++11
  }

  /// Copy constructor
  array_view(const array_view&) = default;

  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to array_view
   */
  array_view& operator=(const array_view&) = default;

  /// Move constructor
  array_view(array_view&&) = default;

  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to array_view
   */
  array_view& operator=(array_view&&) = default;

  virtual ~array_view() = default;

  /**
   * \brief Swap the contents of two array_view objects
   * \param other the array_view to be swapped to the current array
   */
  void swap(array_view& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
  }

  /**
   * \brief Get the length of the contained array
   * \returns the size of the array
   */
  size_type size() const noexcept { return size_; }

  /**
   * \brief Check whether the contained array is empty
   * \returns true if the array is empty
   */
  bool empty() const noexcept { return size_ == 0; }

  /**
   * \brief Get a pointer to the contained data
   * \returns the pointer to the data
   */
  pointer data() noexcept { return data_; }
  /**
   * \brief Get a const pointer to the contained data
   * \returns the const pointer to the data
   */
  const_pointer data() const noexcept { return data_; }

  /**
   * \brief Iterator to the start of the array
   * \returns the iterator at the start of the array
   */
  iterator begin() { return data_; }

  /**
   * \brief Past-The-End iterator of the array
   * \returns the iterator past-the-end
   */
  iterator end() {
    if (data_ == nullptr) {
      return nullptr;
    } else {
      return &data_[size_];
    }
  }

  /**
   * \brief Const Iterator to the start of the array
   * \returns the const iterator at the start of the array
   */
  const_iterator begin() const { return cbegin(); }

  /**
   * \brief Const Past-The-End iterator of the array
   * \returns the const iterator past-the-end
   */
  const_iterator end() const { return cend(); }

  /**
   * \brief Const Iterator to the start of the array
   * \returns the const iterator at the start of the array
   */
  const_iterator cbegin() const { return data_; }
  /**
   * \brief Const Past-The-End iterator of the array
   * \returns the const iterator past-the-end
   */
  const_iterator cend() const {
    if (data_ == nullptr) {
      return nullptr;
    } else {
      return &data_[size_];
    }
  }

  /**
   * \brief Get the element at the specified position
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   * \throws std::out_of_range if pos >= size()
   */
  reference at(size_type pos) {
    if (pos >= size_) {
      throw std::out_of_range("array_view: out of range access");
    } else if (data_ == nullptr) {
      throw std::out_of_range("array_view: out of range access to nullptr");
    }
    return data_[pos];
  }

  /**
   * \brief Get the element at the specified position (const version)
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   * \throws std::out_of_range if pos >= size()
   */
  const_reference at(size_type pos) const {
    if (pos >= size_) {
      throw std::out_of_range("array_view: out of range access");
    } else if (data_ == nullptr) {
      throw std::out_of_range("array_view: out of range access to nullptr");
    }
    return data_[pos];
  }

  /**
   * \brief Unchecked array access
   *
   * The behavior is unspecified if the container is empty or if pos >= size()
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   */
  reference operator[](size_type pos) { return at(pos); }

  /**
   * \brief Unchecked array access
   *
   * The behavior is unspecified if the container is empty or if pos >= size()
   *
   * \param pos Index of the element to get
   * \return A reference to the resulting element
   */
  const_reference operator[](size_type pos) const { return at(pos); }

  /**
   * \brief Unchecked access to first element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the first element
   */
  reference front() { return at(0); }

  /**
   * \brief Unchecked access to first element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the first element
   */
  const_reference front() const { return at(0); }

  /**
   * \brief Unchecked access to last element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the last element
   */
  reference back() {
    if (size_ < 1) {
      throw std::out_of_range("array_view: out of range access");
    }
    return at(size_ - 1);
  }

  /**
   * \brief Unchecked access to last element
   *
   * The behavior is unspecified if the container is empty.
   *
   * \return A reference to the last element
   */
  const_reference back() const {
    if (size_ < 1) {
      throw std::out_of_range("array_view: out of range access");
    }
    return at(size_ - 1);
  }

 protected:
  /// Pointer to the contained data
  pointer data_;
  /// Size of the contained data
  size_type size_;
};

/**
 * \brief Typedef for an array_view to const data.
 *
 * This template ensures that the contained type is a const type.
 *
 * This typedef will not compile if T is a reference (SFINAE)
 */
template <typename T>
using const_array_view = array_view<const typename std::remove_const<T>::type>;

/**
 * \brief Typedef for an array_view to mutable data.
 *
 * This template ensures that the contained type can be modified.
 *
 * This typedef will not compile if T is a reference or T is const-qualified (SFINAE)
 */
template <typename T>
using mutable_array_view = array_view<typename std::enable_if<!std::is_const<T>::value, T>::type>;

/// Swap implementation for compatibility with std::swap
template <typename T>
void swap(array_view<T>& left, array_view<T>& right) noexcept {
  left.swap(right);
}

/**
/// Swap implementation for compatibility with std::swap
 * \param left the array_view to be swapped to the right
 * \param right the array_view to be swapped to the left
 */
}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_ARRAY_VIEW_H_
