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
/**        \file  allocator.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ALLOCATOR_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ALLOCATOR_H_

#include <atomic>
#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#if defined __QNXNTO__
#include <malloc.h>
#endif

// these only exist for reference so we don't forget about 'em
// note that posix_memalign has quite different semantics
/** \brief Allocate size bytes of uninitialized storage whose alignment is specified by alignment
 *  \param alignment specifies the alignment
 *  \param size number of bytes to allocate
 */
extern void* aligned_alloc(std::size_t alignment, std::size_t size) throw();

/** \brief Deallocate memory block
 *  \param ptr
 */
extern void free(void* ptr) throw();

namespace ara {

namespace rest {

/*

  ara::rest::Allocator is interface compatible with std::memory_resource which
  is a runtime polymorphic allocator concept newly added to C++17(!).

  Therefore, as long as our compiler is sufficiently new, just reusing the PMR (polymorphic memory
  region) stuff from C++ would be fine. For completeness, I added the bare minimum here that is
  needed to implement this on our own.

  For now, we only need to support Allocator in the component signatures.
  At some later point all internals will need to perform their allocations
  through this interface.

  Note that there exists std::polymorphic_allocator<> as an adapter which
  enables the use of a PMR allocators within standard containers that rely
  on std::allocator<> arguments. In ara::rest this adapter is called StdAllocator (see below).
  Note also that G/SetDefaultAllocator are also aliases for their std versions.

  Long story short, if std PMR is available we could just use type aliases here.

 */

/**
 * ara::rest::Allocator is interface compatible with std::memory_resource which
 * is a runtime polymorphic allocator concept added to C++17.
 *
 * All types related to ara::rest::Allocator are fully standard compliant
 *
 * If std::pmr is available these types may be simple type aliases.
 *
 */
class Allocator {
 public:
  /** \brief
   *
   *  Detailed description
   *
   */
  Allocator() = default;

  /** \brief Destroys this object
   *
   */
  virtual ~Allocator();

  /** \brief Allocates a memory area
   *
   *  \param bytes desired size of the memory area to be allocated
   *  \param alignment alignment of the memory area
   *  \return a pointer to the allocated memory area
   */
  void* allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));

  /** \brief Releases a memory area
   *
   *  \param p pointer to the allocated memory area
   *  \param bytes size of the allocated memory area
   *  \param alignment alignment of allocated memory area
   */
  void deallocate(void* p, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t));

  /** \brief Tests whether two allocators are equal
   *
   *  Allocators are equal if memory allocated by one can be deallocated by the other.
   *
   *  \param alloc an allocator to compare against
   *  \return true if the two allocators compare equal
   */
  bool is_equal(const Allocator& alloc) const noexcept;

 private:
  virtual void* do_allocate(std::size_t bytes, std::size_t alignment) = 0;
  virtual void do_deallocate(void* p, std::size_t bytes, std::size_t alignment = alignof(std::max_align_t)) = 0;
  virtual bool do_is_equal(const Allocator& other) const noexcept = 0;
};

/** \brief Tests two allocators for equality
 *
 *  \param a an allocator
 *  \param b an allocator
 *  \return true allocators compare equal
 */
bool operator==(const Allocator& a, const Allocator& b);
/** \brief Tests two allocators for inequality
 *
 *  \param a an allocator
 *  \param b an allocator
 *  \return true allocators compare unequal
 */
bool operator!=(const Allocator& a, const Allocator& b);

// using AllocatorArgType = std::allocator_arg_t;

// constexpr auto& AllocatorArg = std::allocator_arg;

namespace allocator {

/**
 * put this in some cpp file; initialize to &NewDeleteAllocator
 */
extern std::atomic<Allocator*> DefaultAllocator;

/**
 * needs documentation
 */
class NewDelete : public Allocator {
 public:
  NewDelete() = default;
  virtual ~NewDelete() {}

  /**
   * \brief Allocate size bytes
   *
   * \param size number of bytes to allocate
   * \param alignment alignment specifies the alignment
   */
  void* do_allocate(std::size_t size, std::size_t alignment) override { return aligned_alloc(size, alignment); }

  /**
   * \brief deallocate memory
   *
   * \param p Pointer to a memory block
   */
  void do_deallocate(void* p, std::size_t /*bytes*/, std::size_t /*alignment*/ = alignof(std::max_align_t)) override {
    free(p);
  }

  /**
   * TODO (need to clarify with visjak)
   *
   * \return TODO (need to clarify with visjak)
   */
  bool do_is_equal(const Allocator&) const noexcept override { return true; }
};
}  // namespace allocator

/** \brief Identical to std::pmr::new_delete_resource
 *
 *  See std::pmr::new_delete_resource for details
 *  \return Allocator
 */
Allocator* NewDeleteAllocator() noexcept;

/** \brief See std::pmr::get_default_allocator for details
 *
 * See std::pmr::get_default_allocator for details
 *
 */
static Allocator* GetDefaultAllocator() noexcept { return allocator::DefaultAllocator; }

/** \brief See std::pmr::set_default_allocator for details
 *
 * See std::pmr::set_default_allocator for details
 * \param a an allocator
 * \return Allocator
 */

// Not being used anywhere, need to check with Visjak if required.
// Allocator* SetDefaultAllocator(Allocator* a) noexcept { return allocator::DefaultAllocator = a; }

/** \brief Adapter that turns an ara::rest::Allocator into a standard-compliant allocator
 *
 *  This adapter type enables application of ara::rest::Allocator in standard container types.
 *
 *  This type is almost identical to std::pmr::polymorphic_allocator. If the latter type is available
 *  a simple type alias is sufficient
 *
 *
 *  See std::pmr::polymorphic_allocator documentation for details
 *
 */
template <typename T>
class StdAllocator {
 public:
  using value_type = T;  ///< Type this allocator is bound to

  /** \brief Default constructs this allocator
   *  See std::pmr::polymorphic_allocator documentation for details
   *
   */
  StdAllocator() noexcept : alloc_(GetDefaultAllocator()) {}

  /** \brief Default constructs this allocator
   *  See std::pmr::polymorphic_allocator documentation for details
   *
   * \param a an allocator
   */
  explicit StdAllocator(Allocator* a) noexcept : alloc_(a) {}

  /** \brief Do not call. See detailed API description.
   * This function exists only for the sake of 'noexcept'. never invoked directly.
   */
  template <typename U>
  StdAllocator(StdAllocator<U> const&) noexcept {}

  /** Allocate
   *  See std::pmr::polymorphic_allocator documentation for details
   *  \param n number of bytes to allocator
   *
   * \return value_type pointer
   */
  value_type* allocate(std::size_t n) { return static_cast<value_type*>(alloc_->allocate(n, alignof(value_type))); }

  /** \brief Deallocate
   *
   *  See std::pmr::polymorphic_allocator documentation for details
   *  \param p pointer to allocated memory region
   *  \param s size of allocated memory region
   */
  void deallocate(value_type* p, std::size_t s) noexcept { alloc_->deallocate(p, s, alignof(value_type)); }

  /** \brief Returns the allocator to use when a standard container using it is copied.
   *
   *  See std::pmr::polymorphic_allocator documentation for details
   *  \return standard-compliant allocator
   */
  StdAllocator select_on_container_copy_construction() const;

  /** \brief Returns the Allocator behind this adapter
   *
   *  See std::pmr::polymorphic_allocator documentation for details
   * \return an allocator
   */
  Allocator* resource() const noexcept;

 private:
  Allocator* alloc_;
};

/** \brief Tests allocators for equality
 * \param a an allocator
 * \param b an allocator
 * \return true if memory allocated in one can be freed via other
 */
template <typename T, typename U>
bool operator==(const StdAllocator<T>& a, const StdAllocator<U>& b) noexcept {
  return a.is_equal(b);
}

/** \brief Tests allocators for inequality
 * \param x an allocator
 * \param y an allocator
 * \return true if memory allocated in x cannot be freed via y
 */
template <typename T, typename U>
bool operator!=(StdAllocator<T> const& x, StdAllocator<U> const& y) noexcept {
  return !(x == y);
}

///@cond DISABLE //NOLINT

// /** \brief This is a "static buffer" allocator /generator/
//  *
//  * The purpose of the generated allocator is primarily to make small buffer optimizations for
//  * types like Function or String explicit. It also is standard-compliant so that it also may
//  * serve as a drop-in replacement for standard-containers.
//  *
//  *  Usage:
//  *    Pass to standard containers like this:
//  *
//  *      using V = std::vector<int, typename A = ara::rest::static_allocator<32>::type<int>
//  *
//  *    This will make vector perform all its allocation within the confined static buffer.
//  *
//  * \tparam N denotes the size of the internal static buffer
//  */
// template <std::size_t N, std::size_t A = alignof(std::max_align_t)>
// struct static_allocator {
//   using store_type = typename std::aligned_storage<N, A>::type;

//   template <typename T>
//   struct type {
//     using value_type = T;

//     type() noexcept {}

//     // exists only for the sake of 'noexcept'. never invoked directly.
//     template <typename U> type(type<U> const&) noexcept {};

//     value_type* allocate(std::size_t n);

//     void deallocate(value_type* p, std::size_t s) noexcept;

//    private:

//     store_type store_;

//   };
// };

// template <std::size_t N, typename  T, std::size_t M, typename  U>
// bool operator==(const typename static_allocator<N>::template type<T>& a,
//                 const typename static_allocator<M>::template type<U>& b) noexcept { return false; }

// template <std::size_t N, typename  T, std::size_t M, typename  U>
// bool operator!=(const typename static_allocator<N>::template type<T>& a,
//                 const typename static_allocator<M>::template type<U>& b) noexcept { return true; }

// TODO(unkown): finish this one...

// specialize for a type and its "domain class"
template <typename T, typename C>
struct SBOTraits {
  static constexpr std::size_t BufferSize = 0;
};

///@endcond //NOLINT

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ALLOCATOR_H_
