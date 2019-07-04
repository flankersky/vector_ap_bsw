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
/**        \file  object_pool.h
 *        \brief  ObjectPool class that can dynamically allocate a set of objects and then never deallocates them but
 *                recycles the memory for new objects.
 *
 *      \details  Currently, this class supports only one initial allocation of memory. All memory is allocated
 *                immediately, even if no objects are created.
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_
#define LIB_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <memory>
#include <utility>

#include "vac/memory/object_pool.h"

namespace vac {
namespace memory {

// forward declaration
class DeleteableSmartPoolObject;

/**
 * Interface for the actual Deleter for a smart pool object.
 */
class SmartObjectPoolDeleterContext {
 public:
  virtual ~SmartObjectPoolDeleterContext() = default;
  /**
   * Actual Deleter function
   * \param ptr Pointer to smart pool object.
   */
  virtual void destroy(DeleteableSmartPoolObject* ptr) = 0;

  /**
   * Actual Deleter function
   * \param ptr const Pointer to smart pool object.
   */
  virtual void destroy(const DeleteableSmartPoolObject* ptr) = 0;
};

/**
 * Proxy on the object side for deleting SmartPoolObjects
 */
class DeleteableSmartPoolObject {
 public:
  /**
   * Constructor for SmartPoolObjects.
   * \param deleter_context DeleterContext for the current object
   */
  explicit DeleteableSmartPoolObject(SmartObjectPoolDeleterContext* deleter_context)
      : deleter_context_(deleter_context) {}

  /**
   * Destructor.
   */
  virtual ~DeleteableSmartPoolObject() = default;

  /**
   * Returns the DeleterContext for deleting the current object
   * \return DeleterContext
   */
  SmartObjectPoolDeleterContext* GetDeleterContext() const { return deleter_context_; }

 private:
  SmartObjectPoolDeleterContext* deleter_context_;
};

/**
 * Master Deleter class using the DeleterContext from the object to delete.
 */
class SmartBaseTypeObjectPoolDeleter final {
 public:
  /**
   * \brief The actual deleter function for const SmartPoolObject
   * \param ptr pointer to object
   */
  void operator()(DeleteableSmartPoolObject* ptr) {
    if (ptr != nullptr) {
      SmartObjectPoolDeleterContext* context = ptr->GetDeleterContext();
      if (context != nullptr) {
        context->destroy(ptr);
      } else {
        delete ptr;
      }
    }
  }

  /**
   * \brief The actual deleter function for const SmartPoolObject
   * \param ptr pointer to object
   */
  void operator()(const DeleteableSmartPoolObject* ptr) {
    if (ptr != nullptr) {
      SmartObjectPoolDeleterContext* context = ptr->GetDeleterContext();
      if (context != nullptr) {
        context->destroy(ptr);
      } else {
        delete ptr;
      }
    }
  }
};

/**
  * \brief Deleter for unique_ptrs to ObjectPool-managed objects
  */
template <class T, typename alloc>
class SmartObjectPoolDeleterContextImpl : public SmartObjectPoolDeleterContext {
  static_assert(std::is_base_of<DeleteableSmartPoolObject, T>::value, "T must implement of DeleteableSmartPoolObject");

 public:
  SmartObjectPoolDeleterContextImpl() noexcept : pool_(nullptr) {}

  virtual ~SmartObjectPoolDeleterContextImpl() = default;

  /**
   * \brief Constructor
   *
   * \param pool Object pool to return an object to on destruction
   */
  explicit SmartObjectPoolDeleterContextImpl(ObjectPool<T, alloc>* pool) noexcept : pool_(pool) {}

  void destroy(DeleteableSmartPoolObject* ptr) override {
    if (ptr != nullptr) {
      if (pool_ != nullptr) {
        assert(typeid(*ptr) == typeid(T));
        pool_->destroy(static_cast<T*>(ptr));
      } else {
        delete ptr;
      }
    }
  }

  void destroy(const DeleteableSmartPoolObject* ptr) override {
    if (ptr != nullptr) {
      if (pool_ != nullptr) {
        assert(typeid(*ptr) == typeid(const T));
        pool_->destroy(static_cast<const T*>(ptr));
      } else {
        delete ptr;
      }
    }
  }

 private:
  /**
   * \brief The ObjectPool to return an object to on destruction
   */
  ObjectPool<T, alloc>* pool_;
};

/**
 * \brief Smart Pointer type used by the ObjectPool
 */
template <class T>
using SmartBaseTypeObjectPoolUniquePtr = std::unique_ptr<T, SmartBaseTypeObjectPoolDeleter>;

/**
 * \brief Smart Pointer type used by the ObjectPool for const Objects
 */
template <class T>
using SmartBaseTypeObjectPoolUniquePtrToConst = std::unique_ptr<const T, SmartBaseTypeObjectPoolDeleter>;

/**
 * \brief ObjectPool that deals in unique_ptrs instead of raw pointers, allowing for base type pointers
 */
template <class T, typename alloc = std::allocator<T>>
class SmartBaseTypeObjectPool {
  friend class SmartBaseTypeObjectPoolTestFixture;
  static_assert(std::is_base_of<DeleteableSmartPoolObject, T>::value, "T must implement of DeleteableSmartPoolObject");

 public:
  /// Typedef for the raw-pointer ObjectPool used for storage
  using StoragePoolType = ObjectPool<T, alloc>;

  /// Typedef for the size type used in this ObjectPool
  using size_type = typename StoragePoolType::size_type;

  /**
    * \brief Deleter for unique_ptrs to ObjectPool-managed objects
    */
  using Deleter = SmartBaseTypeObjectPoolDeleter;

  /**
   * \brief Smart Pointer type used by the ObjectPool
   */
  using UniquePtr = SmartBaseTypeObjectPoolUniquePtr<T>;

  /**
   * \brief Smart Pointer type used by the ObjectPool
   */
  using UniquePtrToConst = SmartBaseTypeObjectPoolUniquePtrToConst<T>;

  /**
   * Constructor.
   */
  SmartBaseTypeObjectPool() : deleter_context_(&pool_) {}

  virtual ~SmartBaseTypeObjectPool() = default;

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param new_capacity The number of T's to reserve space for
   */
  void reserve(size_type new_capacity) { pool_.reserve(new_capacity); }

  /**
   * \brief Allocate and initialize an object of the pool.
   *
   * Calls the constructor with the given set of arguments. For all other properties, see allocate().
   *
   * \param args Arguments forwarded to the constructor
   *
   * \returns A pointer to an initialized T.
   */
  template <typename... Args>
  UniquePtr create(Args&&... args) {
    typename StoragePoolType::pointer ptr = pool_.create(std::forward<Args>(args)..., &deleter_context_);
    UniquePtr smart_ptr(ptr, SmartBaseTypeObjectPoolDeleter());
    return smart_ptr;
  }

  /**
   * \brief Determine whether the allocated memory is full
   *
   * \return false if there is space left in this object pool, i.e., the next call to create() will
   * succeed. True when the next call to create() will throw.
   */
  bool full() const noexcept { return pool_.full(); }

  /**
   * \brief Determine whether there are not objects currently allocated.
   *
   * \return true if no element is allocated inside this object pool. false otherwise.
   */
  bool empty() const noexcept { return pool_.empty(); }

  /**
   * \brief The total number of objects that can be allocated in this pool
   */
  size_type capacity() const noexcept { return pool_.capacity(); }

  /**
   * \brief The number of objects currently allocated in this pool
   */
  size_type size() const noexcept { return pool_.size(); }

 private:
  StoragePoolType pool_;
  SmartObjectPoolDeleterContextImpl<T, alloc> deleter_context_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_SMART_BASE_TYPE_OBJECT_POOL_H_
