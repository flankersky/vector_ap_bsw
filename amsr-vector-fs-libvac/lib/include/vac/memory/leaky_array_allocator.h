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
/**        \file  leaky_array_allocator.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_LEAKY_ARRAY_ALLOCATOR_H_
#define LIB_INCLUDE_VAC_MEMORY_LEAKY_ARRAY_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iostream>
#include <memory>
#include <utility>

namespace vac {
namespace memory {

/**
 * \brief Class that only allocates and logs messages on deallocation but does not actually deallocate
 *
 * This allocator class is intended for use with STL containers. It should satisfy their allocation needs and on the
 * other hand notify us when resources are deallocated.
 */
template <typename T, typename DelegateAllocator = std::allocator<T>>
class LeakyArrayAllocator {
 public:
  /**
   * \brief Type of the delegate allocator used for actual allocations
   */
  using DelegateAllocatorType = DelegateAllocator;

  /**
   * \brief pointer type of the delegate allocator
   */
  using pointer = typename DelegateAllocator::pointer;

  /**
   * \brief const pointer type of the delegate allocator
   */
  using const_pointer = typename DelegateAllocatorType::const_pointer;

  /**
   * \brief reference type of the delegate allocator
   */
  using reference = typename DelegateAllocatorType::reference;

  /**
   * \brief const reference type of the delegate allocator
   */
  using const_reference = typename DelegateAllocatorType::const_reference;

  /**
   * \brief value type of the delegate allocator
   */
  using value_type = typename DelegateAllocator::value_type;

  /**
   * \brief size type of the delegate allocator
   */
  using size_type = typename DelegateAllocator::size_type;

  /**
   * \brief difference type of the delegate allocator
   */
  using difference_type = typename DelegateAllocator::difference_type;

  LeakyArrayAllocator() = default;

  /**
   * \brief Copy Constructor
   */
  LeakyArrayAllocator(const LeakyArrayAllocator&) = default;

  /**
   * \brief Copy Assignment
   * \returns a reference to the assigned-to object
   */
  LeakyArrayAllocator& operator=(const LeakyArrayAllocator&) = default;

  /**
   * \brief Equality operator for allocator.
   *
   * Delegates to the delegate_allocator_
   *
   * \param other The other allocator to compare to
   * \return The result of DelegateAllocator::operator==(other)
   */
  template <typename OtherAllocatorType>
  bool operator==(const OtherAllocatorType& other) const {
    return delegate_allocator_ == other;
  }

  /**
   * \brief Equality operator overload for cases where other is also a LeakyArrayAllocator.
   *
   * Unwraps the delegate_allocator_ from other and delegates to both delegate_allocator_s.
   *
   * \param other The other allocator to compare to
   * \return The result of DelegateAllocator::operator==(other).GetDelegate()
   */
  bool operator==(const LeakyArrayAllocator<T, DelegateAllocatorType>& other) const {
    return delegate_allocator_ == other.GetDelegate();
  }

  /**
   * \brief Inequality operator.
   *
   * Delegates to !(operator==).
   *
   * \param other The other allocator to compare to
   * \return The result of !(this->operator==(other))
   */
  template <typename OtherAllocatorType>
  bool operator!=(const OtherAllocatorType& other) const {
    return !(this == other);
  }

  /**
   * \brief Move Constructor
   */
  LeakyArrayAllocator(LeakyArrayAllocator&&) = default;

  /**
   * \brief Move Assignment
   * \returns a reference to the assigned-to object
   */
  LeakyArrayAllocator& operator=(LeakyArrayAllocator&&) = default;

  /**
   * \brief Copy constructor for rebinding
   *
   * \param other The other allocate to initialize from.
   */
  template <typename U>
  explicit LeakyArrayAllocator(const U& other) : delegate_allocator_(other.GetDelegate()) {}

  virtual ~LeakyArrayAllocator() = default;

  /**
   * \brief rebind struct to adapt this allocator to a different type.
   */
  template <typename U>
  struct rebind {
    /**
     * \brief rebind member to adapt this allocator to a different type.
     */
    typedef LeakyArrayAllocator<U, typename DelegateAllocatorType::template rebind<U>::other> other;
  };

  /**
   * \brief Allocates a block of memory. Simply forwards to the delegate allocator.
   * \param n the number of elements to allocate
   * \returns the allocated block of memory.
   */
  pointer allocate(std::size_t n) { return delegate_allocator_.allocate(n); }

  /**
   * \brief Logs a message that some memory was deallocated.
   * \param ptr the pointer to deallocate
   * \param n the number of elements to deallocate
   */
  void deallocate(pointer ptr, std::size_t n) {
    (void)ptr;
    std::cerr << "LeakyArrayAllocator<" << typeid(T).name() << ">: Leaking " << n << " elements (" << n * sizeof(T)
              << " bytes)." << std::endl;
  }

  /**
   * \brief Construct an object in the given memory location.
   * \param p the location to allocate
   * \param val the value to construct
   */
  void construct(pointer p, const_reference val) { delegate_allocator_.construct(p, val); }

  /**
   * \brief Construct an object in the given memory location.
   * \param p the location to allocate
   * \param args the values to emplace
   */
  template <typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    delegate_allocator_.construct(p, std::forward<Args>(args)...);
  }

  /**
   * \brief Destroy the object at the given memory location.
   * \param p the pointer to the object
   */
  void destroy(pointer p) { delegate_allocator_.destroy(p); }

  /**
   * \brief return the delegate allocator implementation
   * \returns the delegate allocator implementation
   */
  const DelegateAllocator& GetDelegate() const { return delegate_allocator_; }

 private:
  /// the underlying allocator
  DelegateAllocator delegate_allocator_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_LEAKY_ARRAY_ALLOCATOR_H_
