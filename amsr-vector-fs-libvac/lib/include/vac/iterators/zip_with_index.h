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
 *  -----------------------------------------------------------------------------------------------------------------
 */
/**        \file  zip_with_index.h
 *        \brief  Contains an iterator that zips the passed iterator with an index
 *
 *      \details  This class can replace most simple for loops by using ZipWithIndex on an container in combination
 *                with the range-based for loop.
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_ITERATORS_ZIP_WITH_INDEX_H_
#define LIB_INCLUDE_VAC_ITERATORS_ZIP_WITH_INDEX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <utility>

namespace vac {
namespace iterators {

/**
 * \brief Value type of the Zipped Iterator
 */
template <typename T, typename I = std::int32_t>
struct ZipWithIndexValue final {
  /**
   * \brief contains the index of the current iteration
   */
  I index;
  /**
   * \brief contains the value of the current iteration
   */
  T value;
};

/**
 * \brief Zipped Iterable that is able to construct the index iterator
 */
template <typename T, typename I = std::int32_t>
class ZipWithIndex final {
  /**
   * \brief Value iterator in first iteration
   */
  T iterable_;
  /**
   * \brief Initial index of the iterator
   */
  I offset_;
  /**
   * \brief Step size for each step, can be any integral value
   */
  I step_size_;

  static_assert(std::numeric_limits<I>::is_integer, "Type I needs to be a integer");

 public:
  /**
   * \brief Inner class that will create the iterator
   */
  class iterator {
    /**
     * \brief value iterator type
     */
    using iterator_t = decltype(std::begin(std::declval<T>()));

    /**
     * \brief Self Type as defined in iterator interface
     */
    using self_type = iterator;
    /**
     * \brief Value Type as defined in iterator interface
     */
    using value_type = ZipWithIndexValue<typename iterator_t::value_type, I>;
    /**
     * \brief Difference Type as defined in iterator interface
     */
    using difference_type = typename iterator_t::difference_type;
    /**
     * \brief Pointer Type as defined in iterator interface
     */
    using pointer = value_type *;
    /**
     * \brief Reference Type as defined in iterator interface
     */
    using reference = value_type &;
    /**
     * \brief Iterator Category as defined in iterator interface
     * Enables only forward iteration
     */
    using iterator_category = std::forward_iterator_tag;

    /**
     * \brief Value iterator in current iteration
     */
    iterator_t value_iterator_;
    /**
     * \brief Index of the current iteration
     */
    I index_;
    /**
     * \brief Step size for each step, can be any integral value
     */
    I const step_size_;

   public:
    /**
     * \brief Copy Constructor as defined in iterator interface
     * \param it Original iterator to copy from
     *
     * Required for: Iterator
     */
    iterator(iterator const &it) = default;

    /**
     * \brief Constructor that will initialize from offset
     * \param it The iterator that is to be zipped
     * \param zip_object contains information on the offset and stepsize for the index
     */
    iterator(iterator_t &&it, ZipWithIndex const &zip_object)
        : value_iterator_{std::forward<iterator_t>(it)},
          index_{zip_object.offset_},
          step_size_{zip_object.step_size_} {}

    /**
     * \brief Copy Assignment as defined in iterator interface
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
     * \brief Gets the value zipped with the index
     * \returns the value zipped with the index
     */
    auto operator*() const -> value_type { return {index_, *value_iterator_}; }

    /**
     * \brief Sets the iterator to its next iteration step
     * \returns a reference to the modified iterator
     *
     * Required for: Iterator
     */
    auto operator++() -> iterator & {
      index_ += step_size_;
      ++value_iterator_;
      return *this;
    }

    /**
     * \brief Sets the iterator to its next iteration step
     * \returns a reference to an unmodified iterator
     *
     * Required for: Iterator
     */
    auto operator++(int) -> iterator {
      iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    /**
     * \brief Forwards check for equality towards value iterator (the index is nonending!)
     * \param other the other iterator which to compare
     * \returns true if both are the same
     *
     * Required for: Input/Output Iterator
     */
    auto operator==(iterator const &other) const -> bool { return this->value_iterator_ == other.value_iterator_; }
    /**
     * \brief Forwards check for inequality towards value iterator (the index is nonending!)
     * \param other the other iterator which to compare
     * \returns true if both are different
     *
     * Required for: Input/Output Iterator
     */
    auto operator!=(iterator const &other) const -> bool { return this->value_iterator_ != other.value_iterator_; }
  };

  /**
   * \brief Constructor taking iterable, initial offset and constant step size!
   * \param iterable the iterable that is to be zipped
   * \param offset the offset from which the index shall start
   * \param step the stepsize that is added after each step
   */
  ZipWithIndex(T &&iterable, I offset, I step)
      : iterable_{std::forward<T>(iterable)}, offset_{offset}, step_size_{step} {}

  /**
   * \brief Create iterator from start
   * \details We use the passed iterable to create a iterator from start
   * \returns an ZipWithIndex iterator object
   */
  auto begin() const -> iterator { return {std::begin(iterable_), *this}; }
  /**
   * \brief Create iterator from the sentinel value
   * \details We use the passed iterable to create a iterator with the end sentinel value
   * \returns an ZipWithIndex iterator object
   */
  auto end() const -> iterator { return {std::end(iterable_), *this}; }
};

/**
 * \brief Creates an iterable which returns an item which contains the current index & value of the iterator
 * \details Can be customized with initial offset and a stepsize (which may be 0 or negative)
 * \param iterable the iterable that is to be zipped
 * \param offset the offset from which the index shall start
 * \param step the stepsize that is added after each step
 * \returns the iterable zipped with the specified index
 */
template <typename T, typename I = std::int32_t>
auto zip_with_index(T &&iterable, I offset = 0, I step = 1) -> ZipWithIndex<T, I> {
  return {std::forward<T>(iterable), offset, step};
}

}  // namespace iterators
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_ITERATORS_ZIP_WITH_INDEX_H_
