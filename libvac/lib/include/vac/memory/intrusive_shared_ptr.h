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
/**        \file  intrusive_shared_ptr.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_INTRUSIVE_SHARED_PTR_H_
#define LIB_INCLUDE_VAC_MEMORY_INTRUSIVE_SHARED_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include "vac/testing/test_adapter.h"

namespace vac {
namespace memory {

template <class T>
class IntrusiveSharedPtr;

/**
 * \brief Base class implementing the intrusive shared state.
 *
 * When an IntrusiveSharedPtr reduces the count of the IntrusiveShared to 0, it calls
 * CallDeleter(). The Default implementation does nothing.
 *
 * The reference count is implemented thread-safe.
 */
template <class T>
class IntrusiveShared {
 public:
  /**
   * \brief Typedef for the shared_ptr type supported by this class.
   */
  using shared_ptr = IntrusiveSharedPtr<T>;

  /**
   * \brief Typedef for the reference count.
   */
  using refcount_t = std::int_fast32_t;

  IntrusiveShared() : reference_count_(0) {}
  IntrusiveShared(const IntrusiveShared&) = delete;
  IntrusiveShared(IntrusiveShared&&) = delete;
  IntrusiveShared& operator=(const IntrusiveShared&) = delete;
  IntrusiveShared& operator=(IntrusiveShared&&) = delete;

  virtual ~IntrusiveShared() = default;

  /**
   * \brief Increment the reference count
   *
   * \return The current reference count after the operation completes.
   */
  refcount_t IncrementReferenceCount() noexcept { return ++reference_count_; }

  /**
   * \brief Decrement the reference count
   *
   * Calls the destructor once the reference count reaches 0.
   *
   * \return The current reference count after the operation completes.
   */
  refcount_t DecrementReferenceCount() {
    refcount_t refcount = --reference_count_;
    if (refcount == 0) {
      CallDeleter();
    }
    return refcount;
  }

  /**
   * \brief Get the current reference count
   *
   * \return The current reference count.
   */
  refcount_t GetReferenceCount() const noexcept { return reference_count_; }

  /**
   * \brief Get the pointed-to object
   */
  T* get() noexcept {
    static_assert(std::is_base_of<IntrusiveShared<T>, T>::value, "T must inherit from IntrusiveShared<T>");
    return static_cast<T*>(this);
  }

  /**
   * \brief Creates an IntrusiveSharedPtr<T> from this object.
   */
  shared_ptr IntrusiveSharedFromThis() noexcept { return shared_ptr(*this); }

  /**
   * \brief Interface function to call a deleter object.
   *
   * Call this function to dispose the object once the reference count reaches 0.
   * The default implementation does nothing.
   */
  virtual void CallDeleter() {}

 private:
  std::atomic<refcount_t> reference_count_;
};

/**
 * \brief Intrusive Shared Pointer pointing to an object marked as IntrusiveShared.
 *
 * While IntrusiveShared is implemented threadsafe, access to methods of IntrusiveSharedPtr is not.
 */
template <class T>
class IntrusiveSharedPtr final {
 public:
  IntrusiveSharedPtr() noexcept : ptr_(nullptr) {}

  /**
   * \brief Constructor to create an IntrusiveSharedPtr from a given object
   */
  explicit IntrusiveSharedPtr(IntrusiveShared<T>& IntrusiveShared_object) noexcept : ptr_(&IntrusiveShared_object) {
    IncrementReferenceCount();
  }

  /**
   * \brief Copy construction
   */
  IntrusiveSharedPtr(const IntrusiveSharedPtr& rhs) noexcept : IntrusiveSharedPtr(*rhs.ptr_) {}

  /**
   * \brief Copy Assignment
   */
  IntrusiveSharedPtr& operator=(const IntrusiveSharedPtr& rhs) {
    if (this->ptr_ != rhs.ptr_) {
      DecrementReferenceCount();
      this->ptr_ = rhs.ptr_;
      IncrementReferenceCount();
    }
    return *this;
  }

  /**
   * \brief Assignment from IntrusiveShared
   */
  IntrusiveSharedPtr& operator=(IntrusiveShared<T>& rhs) noexcept {
    IntrusiveSharedPtr tmp(rhs);
    swap(tmp);
    return *this;
  }

  /**
   * \brief Move Constructor
   */
  IntrusiveSharedPtr(IntrusiveSharedPtr&& rhs) noexcept : IntrusiveSharedPtr() { swap(rhs); }

  /**
   * \brief Move-Assignment
   */
  IntrusiveSharedPtr& operator=(IntrusiveSharedPtr&& rhs) noexcept {
    swap(rhs);
    return *this;
  }

  /**
   * \brief Destructor that also destroys the contained object, if it exists.
   */
  ~IntrusiveSharedPtr() noexcept(std::is_nothrow_destructible<T>::value) {
    DecrementReferenceCount();
    ptr_ = nullptr;
  }

  /**
   * \brief Destructor function used to call the destructor of the contained elements.
   */
  void reset() noexcept(std::is_nothrow_destructible<T>::value) {
    IntrusiveSharedPtr tmp;
    swap(tmp);
  }

  /**
   * \brief Exchange the pointed-to objects between this and other.
   */
  void swap(IntrusiveSharedPtr& other) noexcept {
    if (this->ptr_ != other.ptr_) {
      std::swap(this->ptr_, other.ptr_);
    }
  }

  /**
   * \brief Checks whether *this contains a value.
   *
   * \return true if *this contains a value, false if *this does not contain a value.
   */
  explicit operator bool() const noexcept { return ptr_ != nullptr; }

  /**
   * \brief Access the contained value.
   *
   * \returns pointer to the contained object if an object exists, nullptr otherwise.
   */
  T* operator->() noexcept {
    if (ptr_ != nullptr) {
      return ptr_->get();
    } else {
      return nullptr;
    }
  }

  /**
   * \brief Access the contained value.
   *
   * \returns pointer to the contained object if an object exists, nullptr otherwise.
   */
  const T* operator->() const noexcept {
    if (ptr_ != nullptr) {
      return ptr_->get();
    } else {
      return nullptr;
    }
  }

  /**
   * \brief Access the contained value.
   *
   * \returns reference to the contained object. The behavior is undefined if no object is contained.
   */
  T& operator*() noexcept { return *(ptr_->get()); }

 private:
  /**
   * \brief Decrease the reference count by 1
   */
  void DecrementReferenceCount() {
    if (ptr_ != nullptr) {
      ptr_->DecrementReferenceCount();
    }
  }

  /**
   * \brief Increase the reference count by 1
   */
  void IncrementReferenceCount() noexcept {
    if (ptr_ != nullptr) {
      ptr_->IncrementReferenceCount();
    }
  }

  /**
   * \brief pointer to the contained element
   */
  IntrusiveShared<T>* ptr_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_INTRUSIVE_SHARED_PTR_H_
