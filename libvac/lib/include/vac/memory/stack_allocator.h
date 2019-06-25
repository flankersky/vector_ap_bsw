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
/**        \file  stack_allocator.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_STACK_ALLOCATOR_H_
#define LIB_INCLUDE_VAC_MEMORY_STACK_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "vac/language/cpp14_backport.h"
#include "vac/memory/leaky_array_allocator.h"

namespace vac {
namespace memory {

/**
 * \brief Allocator that allocates multiple times out of a fixed block but only deallocates everything in one go.
 *
 * This allocator is geared towards iterative uses, where in each iteration some memory is allocated and all memory is
 * deallocated at the end of the iteration.
 */
template <class DelegateAllocator = LeakyArrayAllocator<uint8_t>>
class StackAllocator final {
 public:
  /**
   * \brief Type of the delegate allocator
   */
  using allocator_type = DelegateAllocator;

  /**
   * \brief The type for length computations
   */
  using size_type = std::size_t;

  /**
   * \brief Data type used for passed pointers
   */
  using pointer = void*;

  /**
   * \brief Constructor for an empty StackAllocator
   */
  explicit StackAllocator(allocator_type allocator) noexcept : allocator_(allocator),
                                                               memory_(nullptr),
                                                               length_(0),
                                                               bytes_available_(0),
                                                               next_free_(nullptr) {}

  /**
   * \brief Constructor that immediately calls reserve()
   */
  StackAllocator(allocator_type allocator, size_type bytes) : StackAllocator(allocator) { reserve(bytes); }

  StackAllocator(const StackAllocator&) = delete;
  StackAllocator(StackAllocator&&) = delete;
  StackAllocator& operator=(const StackAllocator&) = delete;
  StackAllocator& operator=(StackAllocator&&) = delete;

  ~StackAllocator() {
    if (memory_ != nullptr) {
      allocator_.deallocate(memory_, length_);
    }
  }

  /**
   * \brief Allocate a number of bytes from this allocator.
   *
   * \param bytes the number of bytes to allocate.
   *
   * \returns A pointer to a memory section containing exactly 'bytes' bytes.
   *
   * \throws std::bad_alloc if insufficient memory is available or no memory was reserve()'d.
   */
  pointer allocate(size_type bytes) {
    if (!memory_ || (bytes > bytes_available_)) {
      throw std::bad_alloc();
    }
    pointer allocated = next_free_;
    next_free_ += bytes;
    bytes_available_ -= bytes;
    return allocated;
  }

  /**
   * \brief Deallocation function that does nothing.
   *
   * Actual deallocations from the StackAllocator can only be performed through reset().
   * This guarantees freedom from fragmentation problems.
   */
  void deallocate(pointer) noexcept {}

  /**
   * \brief Reserve a memory block for this StackAllocator
   *
   * \param bytes the number of bytes to reserve.
   *
   * \throws std::runtime_error if reserve() has been called before.
   */
  void reserve(size_type bytes) {
    if (memory_) {
      throw std::runtime_error("Already initialized");
    } else {
      length_ = bytes;
      memory_ = allocator_.allocate(bytes);
      reset();
    }
  }

  /**
   * \brief Drop all allocations that were done on this object.
   */
  void reset() noexcept {
    next_free_ = memory_;
    bytes_available_ = length_;
  }

 private:
  /**
   * \brief Delegate allocator to obtain the memory block from
   */
  allocator_type allocator_;

  /**
   * \brief Pointer to the memory segment to allocate from
   */
  uint8_t* memory_;

  /**
   * \brief Total length of the memory segment to allocate from
   */
  size_type length_;

  /**
   * \brief Unallocated bytes at the end of the memory segment
   */
  size_type bytes_available_;

  /**
   * \brief Pointer to the first unallocated byte of the memory segment
   */
  uint8_t* next_free_;

  friend class StackAllocatorEmptyTestFixture;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_STACK_ALLOCATOR_H_
