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
/**        \file  three_phase_allocator.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_THREE_PHASE_ALLOCATOR_H_
#define LIB_INCLUDE_VAC_MEMORY_THREE_PHASE_ALLOCATOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <cassert>
#include <memory>
#include <mutex>
#include <utility>

namespace vac {

namespace testing {
class AllocationPhaseManagerTestAdapter;
}  // namespace testing

namespace memory {

/**
 * \brief Singleton class to manage the allocation phase during program execution.
 */
class AllocationPhaseManager {
 public:
  /**
   * \brief Possible allocation phases.
   */
  enum class AllocationPhase { allocation = 0, steady = 1, deallocation = 2 };

  /**
   * \brief Initialize a new AllocationPhaseManager or get access to the existing instance.
   */
  static AllocationPhaseManager& GetInstance();

  /**
   * \brief Switch to a new AllocationPhase
   *
   * The switch is only performed if the new AllocationPhase is larger than the old AllocationPhase.
   *
   * \param requested_allocation_phase The new allocation phase to switch to.
   */
  void SetPhase(AllocationPhase requested_allocation_phase) {
    // Obtain the original value
    AllocationPhase current_phase = allocation_phase_;

    // Loop to deal with parallel writes to the allocation phase
    // The loop will terminate when current_phase is greater or equal the requested allocation_phase
    // Iterations of the loop are limited: In every iteration, either the update succeeds and the loop terminates.
    // Or the current_phase will increase by at least 1. As the number of enum values in AllocationPhase is limited, the
    // loop will eventually terminate.
    while (requested_allocation_phase > current_phase) {
      allocation_phase_.compare_exchange_strong(current_phase, requested_allocation_phase);
      current_phase = allocation_phase_;
    }
  }

  /**
   * \brief Test whether allocations are allowed in the current phase
   *
   * \return true in case allocations are allowed. False otherwise.
   */
  bool IsAllocationAllowed() const { return allocation_phase_ == AllocationPhase::allocation; }

  /**
   * \brief Test whether deallocations are allowed in the current phase
   *
   * \return true in case deallocations are allowed. False otherwise.
   */
  bool IsDeallocationAllowed() const { return allocation_phase_ == AllocationPhase::deallocation; }

  /**
   * \brief Return the current allocation phase
   */
  AllocationPhase GetPhase() const { return allocation_phase_; }

 private:
  AllocationPhaseManager() : allocation_phase_(AllocationPhase::allocation) {}

  std::atomic<AllocationPhase> allocation_phase_;

  static std::once_flag flag_;
  static std::unique_ptr<AllocationPhaseManager> instance_;

  friend class AllocationPhaseManagerFixture;
  friend class vac::testing::AllocationPhaseManagerTestAdapter;
};

/**
 * \brief Class that only allocates and logs messages on deallocation but does not actually deallocate
 *
 * This allocator class is intended for use with STL containers. It should satisfy their allocation needs and on the
 * other hand notify us when resources are deallocated.
 */
template <typename T, typename DelegateAllocator = std::allocator<T>>
class ThreePhaseAllocator {
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

  ThreePhaseAllocator() : allocation_phase_manager_(&AllocationPhaseManager::GetInstance()) {}

  /**
   * \brief Copy constructor
   */
  ThreePhaseAllocator(const ThreePhaseAllocator&) = default;

  /**
   * \brief Copy constructor for rebinding
   *
   * \param other The other allocate to initialize from.
   */
  template <typename U>
  explicit ThreePhaseAllocator(const U& other)
      : delegate_allocator_(other.get_delegate()), allocation_phase_manager_(&AllocationPhaseManager::GetInstance()) {}

  virtual ~ThreePhaseAllocator() = default;

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
   * \brief Equality operator overload for cases where other is also a ThreePhaseAllocator.
   *
   * Unwraps the delegate_allocator_ from other and delegates to both delegate_allocator_s.
   *
   * \param other The other allocator to compare to
   * \return The result of DelegateAllocator::operator==(other).GetDelegate()
   */
  bool operator==(const ThreePhaseAllocator<T, DelegateAllocatorType>& other) const {
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
    return !(*this == other);
  }

  /**
   * \brief rebind struct to adapt this allocator to a different type.
   */
  template <typename U>
  struct rebind {
    /**
     * \brief rebind member to adapt this allocator to a different type.
     */
    using other = ThreePhaseAllocator<U, typename DelegateAllocatorType::template rebind<U>::other>;
  };

  /**
   * \brief Allocates a block of memory. Simply forwards to the delegate allocator.
   *
   * \param n the number of elements to allocate
   * \return A pointer to the allocated (uninitialized) memory
   *
   * \throws std::runtime_error in case allocations are currently forbidden
   */
  pointer allocate(std::size_t n) {
    if (GetAllocationPhaseManager().IsAllocationAllowed()) {
      return delegate_allocator_.allocate(n);
    } else {
      throw std::runtime_error("Allocation in wrong phase");
    }
  }

  /**
   * \brief Deallocates a block of memory. Simply forwards to the delegate allocator.
   *
   * \param ptr The memory location to deallocate from
   * \param n the number of elements to deallocate
   *
   * \throws std::runtime_error in case deallocations are currently forbidden
   */
  void deallocate(pointer ptr, std::size_t n) {
    if (GetAllocationPhaseManager().IsDeallocationAllowed()) {
      delegate_allocator_.deallocate(ptr, n);
    } else {
      throw std::runtime_error("Deallocation in wrong phase");
    }
  }

  /**
   * \brief Construct an object in the given memory location.
   *
   * \param p The memory location to construct at
   * \param val The value to construct with
   *
   * \throws std::runtime_error in case allocations are currently forbidden
   */
  void construct(pointer p, const_reference val) {
    if (GetAllocationPhaseManager().IsAllocationAllowed()) {
      delegate_allocator_.construct(p, val);
    } else {
      throw std::runtime_error("Allocation in wrong phase");
    }
  }

  /**
   * \brief Construct an object in the given memory location.
   *
   * \param p The memory location to construct at
   * \param args Arguments to be forwarded to the constructor
   *
   * \throws std::runtime_error in case allocations are currently forbidden
   */
  template <typename U, typename... Args>
  void construct(U* p, Args&&... args) {
    if (GetAllocationPhaseManager().IsAllocationAllowed()) {
      delegate_allocator_.construct(p, std::forward<Args>(args)...);
    } else {
      throw std::runtime_error("Allocation in wrong phase");
    }
  }

  /**
   * \brief Destroy the object at the given memory location.
   *
   * \param p The memory location to destroy at
   *
   * \throws std::runtime_error in case deallocations are currently forbidden
   */
  void destroy(pointer p) {
    if (GetAllocationPhaseManager().IsDeallocationAllowed()) {
      delegate_allocator_.destroy(p);
    } else {
      throw std::runtime_error("Deallocation in wrong phase");
    }
  }

  /**
   * \brief return the delegate allocator implementation
   */
  const DelegateAllocator& GetDelegate() const { return delegate_allocator_; }

 private:
  DelegateAllocator delegate_allocator_;

  /**
   * \brief Gets a reference to the stored AllocationPhaseManager.
   */
  const AllocationPhaseManager& GetAllocationPhaseManager() const {
    assert(allocation_phase_manager_ != nullptr);
    return *allocation_phase_manager_;
  }

  /**
   * \note allocation_phase_manager_ member can be expected to be non-null within this class since
   * - it is initialized with a static instance during construction
   * - cannot be changed through any non-private interface thereafter
   */
  AllocationPhaseManager* allocation_phase_manager_;
};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_THREE_PHASE_ALLOCATOR_H_
