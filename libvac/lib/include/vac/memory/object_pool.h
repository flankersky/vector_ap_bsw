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

#ifndef LIB_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_
#define LIB_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <mutex>
#include <new>
#include <utility>

#include "vac/container/static_vector.h"

namespace vac {
namespace memory {

/**
 * \brief Class to manage pools of objects of a specific type.
 *
 * Note: The lifetime of the ObjectPool must exceed the lifetime of the allocated objects. Otherwise, all pointers
 * obtained from the pool will be dangling.
 *
 * Note: Currently, there is only the singleton instance supported, i.e., the object pool does not allow on-stack
 * allocations.
 *
 * Note: ObjectPoolImpl is an internal helper class. Only use ObjectPool or SmartObjectPool in your code.
 *
 * The current implementation of this class heavily relies on exceptions. This is purely for development purposes: The
 * occurence of any of these exceptions indicates either a serious programming error (returning memory to the wrong
 * pool) or a misconfiguration (an insufficient amount of resources was configured). For both cases, it must be ensured
 * prior to production that the error conditions do not occur.
 *
 * TODO(PAASR-1154): Make all methods noexcept.
 */
template <class T, bool moveable, typename alloc = std::allocator<T>>
class ObjectPoolImpl {
  friend class ObjectPoolTestFixture;
  friend class SmartBaseTypeObjectPoolTestFixture;

  static_assert(!(std::is_const<T>::value), "Object pool cannot be used with const Objects.");

 public:
  /// Typedef for raw pointers
  using pointer = T*;

  /// Typedef for raw pointers
  using const_pointer = const T*;

  /// Typedef for the size type used in this implementation
  using size_type = std::size_t;

  /**
   * \brief Constructor to create an empty ObjectPool.
   */
  ObjectPoolImpl() : free_list_(nullptr), allocation_count_(0) {}

  ObjectPoolImpl(const ObjectPoolImpl&) = delete;
  ObjectPoolImpl& operator=(const ObjectPoolImpl&) = delete;

  virtual ~ObjectPoolImpl() = default;

  /**
   * \brief Move constructor
   *
   * This constructor is only available of the "moveable" template parameter is set to true
   *
   * \param other The ObjectPoolImpl to move from
   */
  template <typename U = T, typename = typename std::enable_if<moveable, U>::type>
  ObjectPoolImpl(ObjectPoolImpl&& other) : free_list_() {
    std::lock(other.free_list_mutex_, free_list_mutex_);
    std::lock_guard<std::mutex> other_lock(other.free_list_mutex_, std::adopt_lock);
    std::lock_guard<std::mutex> this_lock(free_list_mutex_, std::adopt_lock);

    storage_ = std::move(other.storage_);
    std::swap(free_list_, other.free_list_);
    allocation_count_ = other.allocation_count_;
    other.allocation_count_ = 0;
  }

  /**
   * \brief Move assignment
   *
   * This constructor is only available of the "moveable" template parameter is set to true
   *
   * \param other The ObjectPoolImpl to move from
   * \return A reference to the moved-to ObjectPoolImpl
   */
  template <typename U = T, typename = typename std::enable_if<moveable, U>::type>
  ObjectPoolImpl& operator=(ObjectPoolImpl&& other) {
    std::lock(other.free_list_mutex_, free_list_mutex_);
    std::lock_guard<std::mutex> other_lock(other.free_list_mutex_, std::adopt_lock);
    std::lock_guard<std::mutex> this_lock(free_list_mutex_, std::adopt_lock);

    storage_.swap(other.storage_);
    std::swap(free_list_, other.free_list_);
    std::swap(allocation_count_, other.allocation_count_);

    return *this;
  }

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param new_capacity The number of T's to reserve space for
   */
  void reserve(size_type new_capacity) {
    std::lock_guard<std::mutex> lock(free_list_mutex_);
    if (new_capacity > storage_.size()) {
      storage_.resize(new_capacity);
      // resize default-constructs all StoredType unions. Their default constructor activates the StoredType* member.

      // Create the free list inside of storage
      free_list_ = nullptr;
      for (StoredType& elem : storage_) {
        elem.free = free_list_;
        free_list_ = &elem;
      }
    }
  }

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
  pointer create(Args&&... args) {
    pointer ptr = this->allocate();
    try {
      new (ptr) T(std::forward<Args>(args)...);
    } catch (...) {
      this->deallocate(ptr);
      throw;
    }
    return ptr;
  }

  /**
   * \brief Destroy an object and return it to the pool.
   *
   * Calls the destructor. For all other properties, see deallocate().
   *
   * \param ptr A pointer to an initialized T.
   */
  void destroy(pointer ptr) {
    if (IsManaged(ptr)) {
      ptr->~T();
      deallocate(ptr);
    } else if (ptr == nullptr) {
      throw std::logic_error("Attempting to deallocate a nullptr");
    } else {
      throw std::bad_alloc();
    }
  }

  /**
   * \brief Destroy an object and return it to the pool.
   *
   * Calls the destructor. For all other properties, see deallocate().
   *
   * \param ptr A pointer to an initialized T.
   */
  void destroy(const_pointer ptr) { destroy(const_cast<pointer>(ptr)); }

  /**
   * \brief Determine whether the allocated memory is full
   *
   * \return false if there is space left in this object pool, i.e., the next call to create() or allocate() will
   * succeed. True otherwise, i.e., the next call to create() or allocate() will throw.
   */
  bool full() const noexcept { return free_list_ == nullptr; }

  /**
   * \brief Determine whether there are not objects currently allocated.
   *
   * \return true if no element is allocated inside this object pool. false otherwise.
   */
  bool empty() const noexcept { return allocation_count_ == 0; }

  /**
   * \brief The total number of objects that can be allocated in this pool
   */
  size_type capacity() const noexcept { return storage_.size(); }

  /**
   * \brief The number of objects currently allocated in this pool
   */
  size_type size() const noexcept { return allocation_count_; }

 private:
  /**
   * \brief Union type for the storage.
   *
   * Allows the storage to be created with the size of max(sizeof(T), sizeof(FreeListNodeType));
   *
   * Note: Unions are not styleguide conformant, this is a deliberate styleguide violation. Justification:
   * The Idea is that the same block of memory can either be interpreted as a node of the free list (state "not
   * allocated") or as an actual block of data (state "allocated"). Changing the meaning of the
   * block upon allocation/deallocation is a fairly common practice.
   *
   * Using the union here is dangerous in the way that the underlying memory may be accessed in the way in which it is
   * not initialized. This can happen for two reasons:
   *   (1) Programming Error inside ObjectPool. ObjectPool is the class that makes the switch between both types. This
   *       must be handled using code inspection and testing.
   *   (2) Accessing through a dangling pointer to the data member. This is just as bad as any dangling pointer access
   *       and is independent of the use of a union.
   *
   * There are multiple other solutions to this problem, but none of them are any better:
   *
   *   (1) Drop the union and use a reinterpret_cast to convert the pointer. This means you have to cast *a lot* in
   *       this class. It also means that we need to juggle min/max(sizeof's), which we get for free using the union.
   *   (2) Make the Free List intrusive, e.g., Create a "Poolable" Interface that classes must implement. However, it
   *       is not clear how you can destroy an object and then still use its free list interpretation.
   *   (3) Make a dedicated, non-intrusive free list (class StoredType { T data; StoredType* free; } rather than union
   *       StoredType). However, this requires management of an additional "non-free-list" to store allocated blocks,
   *       requires a search in this this list to find the correct block upon deallocation, and also uses more memory.
   *
   * Therefore, using the union in this particular case is no worse than any of the other solutions while requiring the
   * least amount of code.
   */
  union StoredType {
    /**
     * \brief Member for the free list
     */
    StoredType* free;
    /**
     * \brief Member for the payload data
     */
    T data;

    StoredType() : free(nullptr) {}
    ~StoredType() { free = nullptr; }
  };

  /**
   * \brief Determine whether a pointed-to location is managed by this ObjectPool.
   *
   * \param ptr the pointer to check
   *
   * \returns true if the location is managed, false otherwise. Also returns false on nullptr.
   */
  bool IsManaged(const T* const ptr) const noexcept {
    if (ptr == nullptr || storage_.data() == nullptr) {
      return false;
    } else {
      return (reinterpret_cast<const void*>(storage_.data()) <= reinterpret_cast<const void*>(ptr) &&
              reinterpret_cast<const void*>(ptr) <= reinterpret_cast<const void*>(&storage_.back()));
    }
  }

  /**
   * \brief Return a pointer to a chunk of memory large enough for storing a T.
   *
   * When the pool is exhausted, an std::bad_alloc is raised.
   *
   * \returns Returns a pointer to a chunk of memory large enough for storing a T. The memory is not initialized, no T
   * is constructed in this place.
   */
  T* allocate() {
    std::unique_lock<std::mutex> lock(free_list_mutex_);
    if (free_list_ == nullptr) {
      // We are out of memory
      throw std::bad_alloc();
    } else {
      StoredType* element = free_list_;
      free_list_ = element->free;
      ++allocation_count_;
      lock.unlock();
      // Deactivate free list member
      element->free = nullptr;
      return &element->data;
    }
  }

  /**
   * \brief Return a chunk of memory to the pool.
   *
   * When a nullptr is passed, an std::logic_error is thrown.
   * When a pointer outside of the memory managed by the pool is passed, a bad_alloc is thrown.
   *
   * The passed object is not destructed.
   *
   * \param ptr Pointer to the chunk of memory to be returned to the pool.
   */
  void deallocate(T* ptr) {
    if (this->IsManaged(ptr)) {
      std::lock_guard<std::mutex> lock(free_list_mutex_);
      // Convert from data member back to union
      StoredType* element = reinterpret_cast<StoredType*>(ptr);
      // Activate free list member
      element->free = free_list_;
      free_list_ = element;
      --allocation_count_;
    } else if (ptr == nullptr) {
      throw std::logic_error("Attempting to deallocate a nullptr");
    } else {
      throw std::bad_alloc();
    }
  }

  /**
   * \brief The actual memory location managed by the pool.
   */
  vac::container::StaticVector<StoredType, alloc> storage_;

  /**
   * \brief Head pointer to the free list.
   */
  StoredType* free_list_;

  /**
   * \brief Mutex to synchronize access to the free_list_
   */
  std::mutex free_list_mutex_;

  /**
   * \brief Number of elements the ObjectPool has handed out and that have not been returned.
   */
  size_type allocation_count_;
};

/**
 * \brief Non-moveable ObjectPool for public usage
 */
template <class T, typename alloc = std::allocator<T>>
using ObjectPool = ObjectPoolImpl<T, false, alloc>;

/**
 * \brief Moveable ObjectPool for public usage
 */
template <class T, typename alloc = std::allocator<T>>
using MoveableObjectPool = ObjectPoolImpl<T, true, alloc>;

/**
  * \brief Deleter for unique_ptrs to ObjectPool-managed objects
  */
template <class T>
class SmartObjectPoolDeleter final {
 public:
  SmartObjectPoolDeleter() noexcept : pool_(nullptr) {}

  /**
   * \brief Constructor
   *
   * \param pool Object pool to return an object to on destruction
   */
  explicit SmartObjectPoolDeleter(ObjectPool<T>* pool) noexcept : pool_(pool) {}

  /**
   * \brief Copy Constructor
   */
  SmartObjectPoolDeleter(const SmartObjectPoolDeleter&) = default;

  /**
   * \brief Copy Assignment
   */
  SmartObjectPoolDeleter& operator=(const SmartObjectPoolDeleter&) = default;

  /**
   * \brief Move Constructor
   */
  SmartObjectPoolDeleter(SmartObjectPoolDeleter&&) = default;

  /**
   * \brief Move Assignment
   */
  SmartObjectPoolDeleter& operator=(SmartObjectPoolDeleter&&) = default;

  ~SmartObjectPoolDeleter() = default;

  /**
   * \brief The actual deleter function
   */
  void operator()(typename ObjectPool<T>::pointer ptr) {
    if (pool_ != nullptr) {
      pool_->destroy(ptr);
    } else {
      if (ptr != nullptr) {
        ptr->~T();
      }
    }
  }

  /**
    * \brief The actual deleter function for const T
    */
  void operator()(typename ObjectPool<T>::const_pointer ptr) {
    if (pool_ != nullptr) {
      pool_->destroy(ptr);
    } else {
      if (ptr != nullptr) {
        ptr->~T();
      }
    }
  }

 private:
  /**
   * \brief The ObjectPool to return an object to on destruction
   */
  ObjectPool<T>* pool_;
};

/**
 * \brief Smart Pointer type used by the ObjectPool
 */
template <class T>
using SmartObjectPoolUniquePtr = std::unique_ptr<T, SmartObjectPoolDeleter<T>>;

/**
 * \brief Smart Pointer type used by the ObjectPool for const Objects
 */
template <class T>
using SmartObjectPoolUniquePtrToConst = std::unique_ptr<const T, SmartObjectPoolDeleter<T>>;

/**
 * \brief ObjectPool that deals in unique_ptrs instead of raw pointers
 */
template <class T, typename alloc = std::allocator<T>>
class SmartObjectPool final {
  friend class ObjectPoolTestFixture;

 public:
  /// Typedef for the raw-pointer ObjectPool used for storage
  using StoragePoolType = ObjectPool<T, alloc>;

  /// Typedef for the size type used in this ObjectPool
  using size_type = typename StoragePoolType::size_type;

  /**
    * \brief Deleter for unique_ptrs to ObjectPool-managed objects
    */
  using Deleter = SmartObjectPoolDeleter<T>;

  /**
   * \brief Smart Pointer type used by the ObjectPool
   */
  using UniquePtr = SmartObjectPoolUniquePtr<T>;

  /**
   * \brief Smart Pointer type used by the ObjectPool
   */
  using UniquePtrToConst = SmartObjectPoolUniquePtrToConst<T>;

  SmartObjectPool() = default;

  /**
   * \brief Copy Constructor
   */
  SmartObjectPool(const SmartObjectPool&) = delete;

  /**
   * \brief Copy Assignment
   */
  SmartObjectPool& operator=(const SmartObjectPool&) = delete;

  /**
   * \brief Move Constructor
   */
  SmartObjectPool(SmartObjectPool&&) = delete;

  /**
   * \brief Move Assignment
   */
  SmartObjectPool& operator=(SmartObjectPool&&) = delete;

  ~SmartObjectPool() = default;

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
   * \returns A unique pointer to an initialized T.
   */
  template <typename... Args>
  UniquePtr create(Args&&... args) {
    typename StoragePoolType::pointer ptr = pool_.create(std::forward<Args>(args)...);
    UniquePtr smart_ptr(ptr, Deleter(&pool_));
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
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_OBJECT_POOL_H_
