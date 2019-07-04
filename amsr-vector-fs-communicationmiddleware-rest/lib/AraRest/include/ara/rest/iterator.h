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
/**        \file  iterator.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ITERATOR_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ITERATOR_H_

#include <iterator>
#include <type_traits>
#include <utility>

/// @cond DISABLED
#include <vector>  // for documentation only
/// @endcond

namespace ara {
namespace rest {

/// @cond DISABLED
namespace iterators {

// struct Nop {
//   template<typename T>
//   T operator()(T&& t) const { return t - 1; }
// };
}
/// @endcond

/// @cond DISABLE_THIS
struct unspecified_iterator_type : std::vector<int>::iterator {};
struct unspecified_functor_type {
  template <typename... Ts>
  void operator()(Ts&&...) const;
};
/// @endcond

/// @cond DISABLE_THIS
template <typename IterT, typename TransT>
class TransformIterator : private TransT {
  template <typename T>
  struct proxy_type {
    operator T*() const { return &t; }

   private:
    T t;
  };

 public:
  using iterator_category = std::forward_iterator_tag;
  using self_type = TransformIterator;
  using base_iterator_type = IterT;
  using transformer_type = TransT;
  using value_type = typename base_iterator_type::value_type;
  using reference = typename base_iterator_type::reference;
  using pointer = typename base_iterator_type::pointer;
  using difference_type = typename base_iterator_type::difference_type;

  explicit TransformIterator(base_iterator_type i, const transformer_type& x = {}) : TransT{x}, mIter{i} {}

  self_type operator++(int) {
    auto cp = *this;
    mIter++;
    return cp;
  }
  self_type& operator++() {
    mIter++;
    return *this;
  }

  decltype(std::declval<TransT>()(std::declval<value_type>()))& operator*() const { return this->operator()(*mIter); }

  base_iterator_type base() const { return (*mIter).base(); }

  base_iterator_type GetBaseIterator() const { return mIter; }

  friend bool operator==(const self_type& a, const self_type& b) { return a.mIter == b.mIter; }
  friend bool operator!=(const self_type& a, const self_type& b) { return !(a == b); }

 private:
  base_iterator_type mIter;
};

template <typename IterT, typename TransT>
TransformIterator<IterT, TransT> MakeTransformIterator(IterT i, const TransT& t) {
  return TransformIterator<IterT, TransT>(i, t);
}

/// @endcond

/** An iterator range
 *
 * IteratorRange is a simple wrapper for a pair of standard iterators. Usually an entire
 * sequence is accessed from first to its last element. Iterators are tedious since the user
 * is forced to pass around two objects. Wrapping in std::pair<> is even more tedious since
 * range-based for loops will expect functions begin()/end() to exist. Conceptually this type
 * is just a pair with accessors begin()/end(). This type also allows to conveniently access
 * different views of the same sequence. Otherwise a sequence would only have a single pair
 * of begin()/end().
 *
 *
 */
template <typename IterT>
struct IteratorRange {
  using Iterator = IterT;  ///< Type of the underlying pair of iterators

  /**
   * \brief Constructs an IteratorRange from a pair of iterators
   *
   *  For convenient construction, see MakeIteratorRange().
   *
   *  \param first an iterator denoting the start of the sequence
   *  \param last an iterator denoting the end of the sequence
   */
  IteratorRange(Iterator first, Iterator last) : mBegin(first), mEnd(last) {}

  /** \brief Returns the start of the sequence
   *
   *  \return an iterator
   */
  Iterator begin() const { return mBegin; }

  /** \brief Returns the end of the sequence
   *
   *  \return an iterator
   */
  Iterator end() const { return mEnd; }

  /** \brief Non-member equivalent of IteratorRange::begin()
   *  \param r an InteratorRange
   *  \return the start of the sequence
   */
  friend Iterator begin(const IteratorRange& r) { return r.mBegin; }

  /** \brief Non-member equivalent of IteratorRange::end()
   *  \param r an InteratorRange
   *  \return the end of the sequence
   */
  friend Iterator end(const IteratorRange& r) { return r.mEnd; }

 private:
  Iterator mBegin;
  Iterator mEnd;
};

/** \brief Helper for type deduction to construct an IteratorRange
 *
 *  \param a iterator that denotes the start of the sequence
 *  \param b iterator that denotes the end of the sequence
 *  \return an IteratorRange
 */
template <typename IterT>
IteratorRange<IterT> MakeIteratorRange(IterT a, IterT b) {
  return IteratorRange<IterT>{a, b};
}

// template<typename IterT>
// using MoveIterator = std::move_iterator<IterT>;

// // could be extended to work directly on Range too
// template<typename IterT>
// using MoveIteratorRange = IteratorRange<MoveIterator<IterT> >;

// template<typename IterT>
// auto MakeMoveIteratorRange(IterT a, IterT b) {
//   return MoveIteratorRange<IterT>{a, b};
// }

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ITERATOR_H_
