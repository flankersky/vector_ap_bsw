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
/**        \file  buffer_provider.h
 *        \brief  BufferProvider class that return a pointer to a free buffer of type T.
 *
 *      \details  Currently, this class supports only one initial allocation of memory. All memory is allocated
 *                immediately.
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_
#define LIB_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "vac/language/cpp14_backport.h"
#include "vac/memory/leaky_array_allocator.h"

namespace vac {
namespace memory {
/**
 * \brief BufferProvider Class to manage buffers of objects of a specific type
 */
template <class T>
class BufferProvider final {
 public:
  /// Typedef for raw pointers
  using pointer = T*;

  /// Typedef for the size type used in this implementation
  using size_type = std::size_t;

  /**
   * \brief Constructor.
   */
  BufferProvider() : buffer_size_(0), buffer_number_(0), buffer_storage_(nullptr) {}

  /// Deleted copy constructor
  BufferProvider(const BufferProvider&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  BufferProvider& operator=(const BufferProvider&) = delete;
  /// Deleted move constructor
  BufferProvider(BufferProvider&&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  BufferProvider operator=(BufferProvider&&) = delete;

  ~BufferProvider() = default;

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param number_buffer number of buffers.
   * \param size_buffer size of each buffers.
   */
  void reserve(size_type number_buffer, size_type size_buffer) {
    std::lock_guard<std::mutex> lock(buffer_mutex_);
    if (number_buffer * size_buffer > buffer_size_ * buffer_number_) {
      // We need to allocate additional memory
      // Current implementation can only allocate once initially
      if (buffer_storage_) {
        throw std::logic_error("Reallocation not implemented");
      } else {
        buffer_storage_ = vac::language::make_unique<T[]>(number_buffer * size_buffer);
        buffer_size_ = size_buffer;
        buffer_number_ = number_buffer;
        // Associate each available buffer with a boolean.
        for (size_type i = 0; i < buffer_number_; ++i) {
          free_buffer_map_[&(buffer_storage_.get()[i * buffer_size_])] = true;
        }
      }
    }
  }

  /**
   * \brief Return a pointer to a free Buffer big enough for number_elements. Otherwise return a null pointer.
   *
   * \param number_elements Number of elements needed inside the buffer.
   * \return raw pointer to a free buffer.
   */
  pointer allocate(size_type number_elements) {
    std::lock_guard<std::mutex> lock(buffer_mutex_);
    if (number_elements <= buffer_size_) {
      auto it = std::find_if(free_buffer_map_.begin(), free_buffer_map_.end(),
                             [](const PairRawPtrBool pair) { return pair.second == true; });
      if (it != free_buffer_map_.end()) {
        it->second = false;
        return it->first;
      } else {
        return nullptr;
      }
    } else {
      return nullptr;
    }
  }

  /**
   * \brief Release a buffer.
   *
   * \param ptr pointer to the buffer to be released.
   */
  void deallocate(pointer ptr) {
    std::lock_guard<std::mutex> lock(buffer_mutex_);
    if (ptr == nullptr) {
      throw std::logic_error("Attempting to deallocate a nullptr");
    } else {
      auto it = free_buffer_map_.find(ptr);
      if (it == free_buffer_map_.end()) {
        throw std::bad_alloc();
      } else {
        it->second = true;
      }
    }
  }

 private:
  /// Typedef for storage unique pointers
  using StorageUniquePtr = std::unique_ptr<T[]>;
  /**
   * \brief Type definition for a pair of raw_pointer and boolean.
   */
  using PairRawPtrBool = std::pair<pointer, bool>;
  /**
   * \brief Type definition for map associating a buffer with a boolean to inform if a buffer is already used.
   *
   * TODO(visbft): use static_map when released.
   */
  using FreeBufferMap =
      std::map<typename PairRawPtrBool::first_type, typename PairRawPtrBool::second_type,
               std::less<typename PairRawPtrBool::first_type>, vac::memory::LeakyArrayAllocator<PairRawPtrBool>>;

  /**
   * \brief The amount of memory allocated for one buffer.
   */
  size_type buffer_size_;

  /**
   * \brief Number of buffer currently allocated..
   */
  size_type buffer_number_;

  /**
   * \brief Unique pointer to the allocated memory.
   */
  StorageUniquePtr buffer_storage_;

  /**
   * \brief FreeBufferMap
   */
  FreeBufferMap free_buffer_map_;

  /**
   * \brief Mutex to synchronize access to the buffers.
   */
  std::mutex buffer_mutex_;
};

/**
 * \brief Implement smart buffer provider
 */
template <class T>
class SmartBufferProvider final {
 public:
  /// Typedef for the size type used in this implementation
  using size_type = std::size_t;

  /// Default constructor
  SmartBufferProvider() = default;
  /// Deleted copy constructor
  SmartBufferProvider(const SmartBufferProvider&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  SmartBufferProvider& operator=(const SmartBufferProvider&) = delete;
  /// Deleted move constructor
  SmartBufferProvider(SmartBufferProvider&&) = delete;
  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  SmartBufferProvider& operator=(SmartBufferProvider&&) = delete;

  ~SmartBufferProvider() = default;

  /**
   * Deleter for unique pointers to buffers.
   */
  class SmartBufferProviderDeleter final {
   public:
    /**
     * \brief Constructor
     */
    SmartBufferProviderDeleter() : buffer_provider_(nullptr) {}

    /**
     * \brief overloaded Constructor.
     * \param buffer_provider the buffer provider whose pointers may be
     */
    explicit SmartBufferProviderDeleter(BufferProvider<T>* buffer_provider) : buffer_provider_(buffer_provider) {}

    /**
     * \brief Copy Constructor
     */
    SmartBufferProviderDeleter(const SmartBufferProviderDeleter&) = default;

    /**
     * \brief Copy Assignment
     * \returns a reference to the assigned-to object
     */
    SmartBufferProviderDeleter& operator=(const SmartBufferProviderDeleter&) = default;

    /**
     * \brief Move Constructor
     */
    SmartBufferProviderDeleter(SmartBufferProviderDeleter&&) = default;

    /**
     * \brief Move Assignment
     * \returns a reference to the assigned-to object
     */
    SmartBufferProviderDeleter& operator=(SmartBufferProviderDeleter&&) = default;

    ~SmartBufferProviderDeleter() = default;

    /**
     * \brief The actual deleter function.
     * \param ptr the pointer whose memory shall be deallocated
     */
    void operator()(typename BufferProvider<T>::pointer ptr) {
      if (buffer_provider_ != nullptr) {
        buffer_provider_->deallocate(ptr);
      }
    }

   private:
    /**
     * Pointer to the buffer provider.
     */
    BufferProvider<T>* buffer_provider_;
  };

  /**
   * \brief Type definition for a unique pointer.
   */
  using UniqueBufferPtr = std::unique_ptr<T, SmartBufferProviderDeleter>;

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param number_buffer number of buffers.
   * \param size_buffer size of each buffers.
   */
  void reserve(size_type number_buffer, size_type size_buffer) { buffer_provider_.reserve(number_buffer, size_buffer); }

  /**
   * \brief Return a pointer to a free Buffer big enough for number_elements. Otherwise return a null pointer.
   *
   * \param number_elements Number of elements needed inside the buffer.
   * \return raw pointer to a free buffer.
   */
  UniqueBufferPtr allocate(size_type number_elements) {
    typename BufferProvider<T>::pointer buffer;
    buffer = buffer_provider_.allocate(number_elements);
    UniqueBufferPtr unique_ptr(buffer, SmartBufferProviderDeleter(&buffer_provider_));
    return unique_ptr;
  }

  /**
   * \brief Return the buffer provider.
   * \return Reference to the buffer provider.
   */
  BufferProvider<T>& GetBufferProvider() { return buffer_provider_; }

 private:
  /**
   * \brief Buffer Provider.
   */
  BufferProvider<T> buffer_provider_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_BUFFER_PROVIDER_H_
