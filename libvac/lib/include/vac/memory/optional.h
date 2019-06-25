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
/**        \file  optional.h
 *        \brief  (Partial) Backport of std::optional from C++17 as vac::memory::optional
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_OPTIONAL_H_
#define LIB_INCLUDE_VAC_MEMORY_OPTIONAL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include "vac/language/cpp17_backport.h"

#include "vac/testing/test_adapter.h"

namespace vac {
namespace memory {

/**
 * \brief Class to manage a static memory location in which an object of given type can be constructed.
 *
 * (Partial) Backport from C++17
 */
template <class T>
class optional final {
 public:
  /**
   * \brief Value type for accessing inner type.
   */
  using value_type = T;

  constexpr optional() noexcept : ptr_(nullptr) {}

  /**
   * \brief Copy construction
   * \param rhs The optional to copy from
   */
  optional(const optional& rhs) noexcept(std::is_nothrow_copy_constructible<T>::value) : optional() { *this = rhs; }

  /**
   * \brief Construction from preexisting value
   * \param rhs The value to construct the optional from
   */
  explicit optional(const T& rhs) noexcept(std::is_nothrow_copy_constructible<T>::value) : optional() { emplace(rhs); }

  /**
   * \brief Inplace construction
   * \param args the arguments to pass to the values constructor
   */
  template <class... Args>
  optional(vac::language::in_place_t, Args&&... args) : optional() {
    emplace(std::forward<Args>(args)...);
  }

  /**
   * \brief Copy Assignment
   * \param rhs The optional to copy from
   * \returns the copied optional
   */
  optional& operator=(const optional& rhs) noexcept(std::is_nothrow_copy_assignable<T>::value) {
    if (this != &rhs) {
      if (rhs.has_value()) {
        if (has_value()) {
          // Both alive, copy-assign the contained value
          *(this->operator->()) = *(rhs.operator->());
        } else {
          // Only the other is alice, copy-construct the contained value
          emplace(*(rhs.operator->()));
        }
      } else {
        reset();
      }
    }
    return *this;
  }

  /**
   * \brief Move Constructor from preexisting value
   * \param rhs The value to construct the optional from
   */
  explicit optional(T&& rhs) noexcept(std::is_nothrow_move_constructible<T>::value) : optional() {
    emplace(std::move(rhs));
  }

  /**
   * \brief Move Constructor
   * \param rhs The moved optional from which to construct a new optional
   */
  optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible<T>::value) : optional() {
    if (rhs.has_value()) {
      emplace(std::move(*(rhs.operator->())));
    }
  }

  /**
   * \brief Move-Assignment
   * \param rhs The optional to copy from
   * \returns the copied optional
   */
  optional& operator=(optional&& rhs) noexcept(std::is_nothrow_move_assignable<T>::value) {
    assert(this != &rhs);
    if (rhs.has_value()) {
      if (has_value()) {
        // Both are valid, move the contained object
        *(this->operator->()) = std::move(*(rhs.operator->()));

      } else {
        // Only the other is valid, move-construct the contained value
        emplace(std::move(*(rhs.operator->())));
      }
    } else {
      reset();
    }
    return *this;
  }

  /**
   * \brief Destructor that also destroys the contained object, if it exists.
   */
  ~optional() noexcept(std::is_nothrow_destructible<T>::value) { reset(); }

  /**
   * \brief Constructor function used to build the contained element.
   * \param args Arguments to pass to the underlying types constructor
   * \returns a reference to the underlying value
   */
  template <typename... Args>
  T& emplace(Args&&... args) {
    reset();
    ptr_ = reinterpret_cast<T*>(::std::addressof(storage_));
    try {
      new (ptr_) T(std::forward<Args>(args)...);
    } catch (...) {
      ptr_ = nullptr;
      throw;
    }
    return *ptr_;
  }

  /**
   * \brief Destructor function used to call the destructor of the contained elements.
   */
  void reset() noexcept(std::is_nothrow_destructible<T>::value) {
    if (ptr_ != nullptr) {
      ptr_->~T();
      ptr_ = nullptr;
    }
  }

  /**
   * \brief Return a reference to the contained value.
   *
   * \return a reference to the contained value if the value exists. Throws otherwise.
   */
  T& value() {
    if (ptr_ != nullptr) {
      return *ptr_;
    } else {
      throw std::runtime_error("bad_optional_access");
    }
  }

  /**
   * \brief Return a reference to the contained value.
   *
   * \return a reference to the contained value if the value exists. Throws otherwise.
   */
  T const& value() const {
    if (ptr_ != nullptr) {
      return *ptr_;
    } else {
      throw std::runtime_error("bad_optional_access");
    }
  }

  /**
   * \brief Return a reference to the contained value or the default value.
   *
   * \param default_value the value to use in case ptr_ is empty
   * \return a reference to the contained value if the value exists. Else returns the default
   */
  template <class U>
      T value_or(U&& default_value) && noexcept {
    if (ptr_ != nullptr) {
      auto tmp = std::move(*ptr_);
      ptr_ = nullptr;
      return tmp;
    } else {
      return static_cast<T>(std::forward<U>(default_value));
    }
  }

  /**
   * \brief Return a reference to the contained value or the default value.
   *
   * \param default_value the value to use in case ptr_ is empty
   * \return a reference to the contained value if the value exists. Else returns the default
   */
  template <class U>
  T value_or(U&& default_value) const& noexcept {
    if (ptr_ != nullptr) {
      return *ptr_;
    } else {
      return static_cast<T>(std::forward<U>(default_value));
    }
  }

  /**
   * \brief Checks whether *this contains a value.
   *
   * \return true if *this contains a value, false if *this does not contain a value.
   */
  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  /**
   * \brief Checks whether *this contains a value.
   *
   * \return true if *this contains a value, false if *this does not contain a value.
   */
  bool has_value() const noexcept { return ptr_ != nullptr; }

  /**
   * \brief Access the contained value.
   *
   * \returns pointer to the contained object if an object exists, nullptr otherwise.
   */
  T* operator->() noexcept { return ptr_; }

  /**
   * \brief Access the contained value.
   *
   * \returns pointer to the contained object if an object exists, nullptr otherwise.
   */
  const T* operator->() const noexcept { return ptr_; }

  /**
   * \brief Access the contained value.
   *
   * \returns reference to the contained object. The behavior is undefined if no object is contained.
   */
  T& operator*() noexcept { return *ptr_; }

  /**
   * \brief Access the contained value.
   *
   * \returns reference to the contained object. The behavior is undefined if no object is contained.
   */
  T const& operator*() const noexcept { return *ptr_; }

 private:
  /**
   * \brief pointer used to access an existing element
   *
   * Also marks whether an element is contained (!= nullptr) or whether the optional is empty (== nullptr).
   */
  T* ptr_;

  /**
   * \brief Static storage for the contained object.
   */
  typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;

  FRIEND_TEST(OptionalTestFixture, construct);
  FRIEND_TEST(OptionalTestFixture, value);
};

/**
 * \brief Creates an optional object from value.
 *
 * \param value the value to construct optional object with
 * \returns The constructed optional object.
 */
template <class V>
optional<typename std::decay<V>::type> make_optional(V&& value) {
  return optional<typename std::decay<V>::type>{std::forward<V>(value)};
}

/**
 * \brief Creates an optional object constructed in-place from args.
 *
 * \param args arguments to be passed to the constructor of T.
 * \returns The constructed optional object.
 */
template <class V, typename... Args>
optional<V> make_optional(Args&&... args) {
  return optional<V>(vac::language::in_place, std::forward<Args>(args)...);
}

/**
 * \brief Compares two optional objects on equality.
 *
 * \param lhs an optional object to compare
 * \param rhs an optional object to compare
 * \returns True if both optional objects are empty or both are filled and the contained values compare to equal
 */
template <class T, class U>
bool operator==(optional<T> const& lhs, optional<U> const& rhs) {
  bool const l_has = lhs.has_value();
  bool const r_has = rhs.has_value();

  if (l_has && r_has) {
    return lhs.value() == rhs.value();
  } else {
    return l_has == r_has;
  }
}

/**
 * \brief Compares two optional objects on inequality.
 *
 * \param lhs an optional object to compare
 * \param rhs an optional object to compare
 * \returns False if both optional objects are empty or both are filled and the contained values compare to equal
 */
template <class T, class U>
bool operator!=(optional<T> const& lhs, optional<U> const& rhs) {
  return !(lhs == rhs);
}

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_OPTIONAL_H_
