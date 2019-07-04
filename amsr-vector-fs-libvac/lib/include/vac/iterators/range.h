/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by VeInformatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to VeInformatik GmbH.
 *                VeInformatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with VeInformatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  range.h
 *        \brief  Contains a Range iterator
 *
 *      \details  This class can replace most simple for loops by using the range in combination
 *                with the range-based for loop and represents a half closed interval of integers.
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_ITERATORS_RANGE_H_
#define LIB_INCLUDE_VAC_ITERATORS_RANGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>

namespace vac {
namespace iterators {

/**
 * \brief Calculate end ONCE and pass the calculated value to the iterator
 * \details If the step is only +1, then skip it and take the last element -1.
 *   Else remove the remainder of the last step from the last element
 * \param begin is the start value of the range
 * \param end is the exclusive end value of the range.
 * \param step is the step size for each iteration.
 * \returns the calculated end that will be hit via a iteration
 * \pre step must not be equal to zero
 */
template <typename I>
inline auto calculate_end(I begin, I end, I step) -> I {
  if (end == begin) {
    // Empty range
    return begin;
  }

  if (step == 0) {
    throw std::invalid_argument("Step size must not be 0!");
  }

  if (step == 1) {
    // Optimization: Standard use case: step_ is 1
    if (end < begin) {
      // Empty range
      return begin;
    } else {
      // Nonempty range
      return end;
    }
  } else if (step > 1) {
    if (end < begin) {
      // Empty range
      return begin;
    } else {
      I const distance = I(end - begin);
      // Get the last step width
      I const remainder = I(distance % step);

      if (remainder != 0) {
        // The new end that will be hit by a step
        return I(end - remainder + step);
      } else {
        return end;
      }
    }
  } else {
    // Step is smaller than 0
    if (end > begin) {
      // Empty range
      return begin;
    } else {
      I const distance = I(begin - end);
      // Get the last step width
      I const remainder = I(distance % step);
      if (remainder == 0) {
        return end;
      } else {
        // The new end that will be hit by a step
        return I(end + remainder + step);
      }
    }
  }
}

/**
 * \brief Creates a range iterator with [start, end) with an optional step
 * \details start will be included in the range, end will not
 *
 * A range object like Pythons - can be configured with an optional step
 * Example: for (auto n : range(1,10)) { cout << n; }
 */
template <typename I = std::size_t>
class Range final {
  /**
   * \brief Start value for the iterator
   */
  I const begin_;
  /**
   * \brief Step size for the iterator
   */
  I const step_;
  /**
   * \brief Given end value for the iterator
   */
  I const end_;
  /**
   * \brief Real end value for the iterator
   */
  I const real_end_;

  static_assert(std::numeric_limits<I>::is_integer, "Type I needs to be a integer");

 public:
  /**
   * \brief Constructor from initial value, end value and step size.
   * \details The range
   * \param begin The initial value
   * \param end The end value. Defaults to the maximum of that type
   * \param step The step size. Defaults to 1.
   */
  Range(I begin, I end, I step) : begin_{begin}, step_{step}, end_{end}, real_end_{calculate_end(begin, end, step)} {}

  /**
   * \brief Internal class providing the c++ iterator interface
   *
   * Provides everything for a normal forward iterator, so it can be used in algorithms & range-based for
   */
  class iterator final {
    /**
     * \brief The real Range object
     */
    Range const range_;
    /**
     * \brief the value of the current iteration
     */
    I current_;

    auto step_val() const & -> I { return range_.step_; }

   public:
    /**
     * \brief Self type as defined in iterator interface
     */
    using self_type = iterator;
    /**
     * \brief Value Type as defined in iterator interface
     */
    using value_type = I;
    /**
     * \brief Difference type as defined in iterator interface
     */
    using difference_type = I;
    /**
     * \brief Pointer type as defined in iterator interface
     */
    using pointer = value_type *;
    /**
     * \brief Const pointer type as defined in iterator interface
     */
    using const_pointer = value_type const *;
    /**
     * \brief Reference type as defined in iterator interface
     */
    using reference = value_type &;
    /**
     * \brief Const reference type as defined in iterator interface
     */
    using const_reference = value_type const &;
    /**
     * \brief Iterator category as defined in iterator interface
     * Enables only forward iteration
     */
    using iterator_category = std::forward_iterator_tag;

    /**
     * \brief Default (& empty) constructor as defined in iterator interface
     *
     * Required for: Forward Iterator
     */
    iterator() : range_{0, 0, 1}, current_{0} {}

    /**
     * \brief Copy constructor as defined in iterator interface
     * \param it Original iterator to copy from
     *
     * Required for: Iterator
     */
    iterator(iterator const &it) = default;

    /**
     * \brief Copy assignment as defined in iterator interface
     * \param it Original iterator to copy from
     * \returns the copied iterator
     *
     * Required for: Iterator
     */
    auto operator=(iterator const &it) -> self_type & = default;

    /**
     * \brief Default Destructor as defined in iterator interface
     *
     * Required for: Iterator
     */
    ~iterator() = default;

    /**
     * \brief Constructor from Range object (const ref)
     * \param r Range object which contains all constant information
     * \param b the start value for this iterator
     */
    iterator(Range const &r, I b) : range_{r}, current_{b} {}
    /**
     * \brief Constructor from Range object (move)
     * \param r Range object which contains all constant information
     * \param b the start value for this iterator
     */
    iterator(Range &&r, I b) : range_{r}, current_{b} {}

    /**
     * \brief Dereferences the iterator to its value
     * \returns the value of the current iteration by reference
     *
     * Required for: Input/Output Iterator
     */
    auto operator*() -> reference { return current_; }

    /**
     * \brief Dereferences the iterator to its value
     * \returns the value of the current iteration by value
     *
     * Required for: Forward Iterator
     */
    auto operator*() const -> value_type { return current_; }

    /**
     * \brief Dereferences the iterator to a pointer to its value
     * \returns the value of the current iteration as pointer to const
     *
     * Required for: Forward Iterator
     */
    auto operator-> () const -> const_pointer { return &current_; }

    /**
     * \brief Sets the iterator to its next iteration step
     * \returns a reference to the iterator
     *
     * Required for: Iterator
     */
    auto operator++() -> self_type & {
      current_ = value_type(current_ + step_val());
      return *this;
    }

    /**
     * \brief Sets the iterator to its next iteration step
     * \returns a reference to the iterator
     *
     * Required for: Iterator
     */
    auto operator++(int) -> self_type {
      self_type temp{*this};
      ++(*this);
      return temp;
    }

    /**
     * \brief Compares two range iterators for equality
     * \param other the other iterator which to compare
     * \returns true if both are the same
     *
     * Required for: Input/Output Iterator
     */
    auto operator==(self_type const &other) const -> bool { return this->current_ == other.current_; }

    /**
     * \brief Compares two range iterators for inequality
     * \param other the other iterator which to compare
     * \returns false if both are the same
     *
     * Required for: Input/Output Iterator
     */
    auto operator!=(self_type const &other) const -> bool { return !(*this == other); }
  };

  /**
   * \brief Create iterator from start
   * \returns an range iterator object
   */
  auto begin() -> iterator { return iterator{*this, begin_}; }

  /**
   * \brief Create iterator from end
   * \returns an range iterator object
   */
  auto end() -> iterator { return iterator{*this, real_end_}; }
};

/**
 * \brief Creates a Range of integers which can be used as iterator
 * \param end_ is the exclusive end value of the range.
 * \returns a Range of integers that starts with 0 and stepsize 1
 */
template <typename I>
inline auto range(I end_ = std::numeric_limits<I>::max()) -> Range<I> {
  static_assert(std::numeric_limits<I>::is_integer, "Type I needs to be an integer");

  return Range<I>{0, end_, 1};
}

/**
 * \brief Creates a Range of integers which can be used as iterator
 * \param begin_ is the start value of the range
 * \param end_ is the exclusive end value of the range.
 * \param step_ is the step size for each iteration. Can be set to any integer except 0. Defaults to 1.
 * \returns a Range of integers
 */
template <typename I>
inline auto range(I begin_, I end_, I step_ = 1) -> Range<I> {
  static_assert(std::numeric_limits<I>::is_integer, "Type I needs to be an integer");

  return Range<I>{begin_, end_, step_};
}

}  // namespace iterators
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_ITERATORS_RANGE_H_
