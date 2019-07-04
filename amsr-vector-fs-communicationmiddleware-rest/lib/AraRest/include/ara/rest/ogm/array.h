#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_ARRAY_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_ARRAY_H_

#include <cstring>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>

#include "ara/rest/allocator.h"
#include "ara/rest/iterator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/node.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/pointer.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Array nodes in the OGM
 *
 * This type is similar to an Object except that it represents a set of "unnamed" Value
 * instances. Hence Field is not allowed to be attached to Array objects.
 */
class Array : public Value, public Base<Array, Value> {
  friend class Base<Array, Value>;

 public:
  /**
   * \brief field type
   */
  using ValueType = Pointer<Value>;

  /**
   * \brief fields type
   */
  using ValueContainerType = std::vector<ValueType>;

  /**
   * \brief TransformOperator
   */
  struct TransformOperator {
    /**
     * \brief operator for transformation
     * \param arg input of transformation, a value_type of ValuesType
     * \return output of transformation, a dereferenced unique ptr
     */
    Value& operator()(const ValueContainerType::value_type& arg) const noexcept { return *arg; }
  };

  /**
   * \brief Value iterator
   *
   *  Iterator::value_type must of type Value*.
   */
  using Iterator = TransformIterator<ValueContainerType::iterator, TransformOperator>;

  /**
   * \brief Value iterator
   *
   *  Iterator::value_type must of type Value*.
   */
  using ConstIterator = TransformIterator<ValueContainerType::const_iterator, TransformOperator>;

  /**
   * needs documentation
   */
  using ValueRange = IteratorRange<Iterator>;

  /**
   * needs documentation
   */
  using ConstValueRange = IteratorRange<ConstIterator>;

  /** \brief Returns the number of elements
   *  \return the number of array elements
   */
  std::size_t GetSize() const noexcept { return values_.size(); }

  /** \brief Returns whether the array holds no elements
   *  \return true if the array holds no elements
   */
  bool IsEmpty() const noexcept { return values_.size() == 0; }

  /** \brief Returns a Value at a specific index
   *
   *  If the index is out-of-bounds, the result in undefined.
   *
   *  \param size an integral index into the array
   *  \return a reference to a Value
   */
  Value& GetValue(std::size_t size) { return *(values_.at(size)); }

  /** \brief Returns a Value at a specific index
   *
   *  If the index is out-of-bounds, the result in undefined.
   *
   *  \param size an integral index into the array
   *  \return a reference to a Value
   */

  const Value& GetValue(std::size_t size) const { return *(values_.at(size)); }

  /** \brief Returns a range of values
   *  \return an iterator range of values
   */

  ValueRange GetValues() noexcept { return ValueRange{Iterator{values_.begin()}, Iterator{values_.end()}}; }

  /** \brief Returns a range of values
   *  \return an iterator range of values
   */

  ConstValueRange GetValues() const noexcept {
    return ConstValueRange{ConstIterator{values_.begin()}, ConstIterator{values_.end()}};
  }

  /**
   * \brief append value to array
   *
   * \see http://scottmeyers.blogspot.de/2014/07/should-move-only-types-ever-be-passed.html
   * \param value pointer to a value be appended.
   */
  void Append(Pointer<Value>&& value) { values_.emplace_back(std::move(value)); }

  /** \brief Inserts a Value at a specific position into the Array
   *
   *  Inserts a value before the element pointed to by the iterator argument.
   *  To insert a Value ownership has to be passeed to the Array
   *
   *  \param iter an Array iterator
   *  \param value a value to insert.
   */

  void Insert(Iterator iter, Pointer<Value>&& value) { values_.insert(iter.GetBaseIterator(), std::move(value)); }

  /**
   * \brief Removes value from the set
   *
   * Removes an element from the set. Removal invalidates all iterators referencing
   * the respective element.
   *
   * \param iter an iterator pointing to an element.
   * \return an iterator pointing to the element following the one just removed.
   */
  Iterator Remove(Iterator iter) {
    if (iter != GetValues().end()) {
      /* remove the element from the values_ list */
      return Iterator{values_.erase(iter.GetBaseIterator())};
    }
    return iter;
  }

  /** \brief Similar to Remove but does not destroy the removed element
   *
   *  Instead of destroying the removed element, ownership is passed back to the user
   *
   *  \param iter an iterator pointing to the element to be removed
   *  \return a pair of the iterator pointing to the element following the one just deleted and a pointer to the element
   */
  std::pair<Iterator, Pointer<Value>> Release(Iterator iter) {
    Pointer<Value> tmp = std::move(*(iter.GetBaseIterator()));
    auto it = values_.erase(iter.GetBaseIterator());
    return {Iterator{it}, std::move(tmp)};
  }

  /**
   *
   *  this must be clarified: move-iter here? don't forget object.
   *  template<typename IterT>
   *  void Remove(ValueRange, IterT);
   *
   */

  /** \brief Replaces an element by a new one without the destroying the old one
   *
   *  Replaces an array element without destroying it. Instead the replaced element is returned.
   *  Effectively, ownership is passed back to the user.
   *
   *  \param iter an iterator pointing to the element to be removed
   *  \param value a pointer to the value to replace the one pointed to by iter
   *  \return a pointer to the old array element
   */
  Pointer<Value> Replace(Iterator iter, Pointer<Value>&& value) {
    /* store the current element temporary */
    auto tmp{iter.GetBaseIterator()->release()};
    /* update value */
    iter.GetBaseIterator()->reset(value.release());
    /* return the old element */
    return Pointer<Value>{tmp};
  }

  /**
   * \brief clear - remove all values from the set
   */
  void Clear() { values_.clear(); }

  // TODO(x):
  // To be specified: search operations on an array directly as member functions?
  // Or should there be a general purpose library that supports roughly about
  // what ViWi wants (like filtering of a tree, search by value, etc pp)
  //
  // => yes, make this external

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Array; }

 protected:
  /**
   * \brief constructor with allocator parameter
   */
  explicit Array(Allocator* alloc = GetDefaultAllocator()) : Value(alloc, NodeType::Array) {}

  /**
   * \brief templated constructor with pointer parameter
   *
   * cannot use initializer_list
   *
   * \param ptr_args OGM objects to insert into the array
   */
  template <typename... Ts>
  explicit Array(Pointer<Ts>&&... ptr_args) : Value(NodeType::Array) {
    // We expand the parameter pack in a brace-enclosed initialized list
    int dummy[] = {0, (values_.emplace_back(std::move(ptr_args)), 0)...};
    ignore(dummy);
  }

  /**
   * \brief templated constructor with allocator and pointer parameter
   *
   * cannot use initializer_list
   * \param alloc an allocator
   * \param ptr_args OGM objects to insert into the array
   */
  template <typename... Ts>
  Array(Allocator* alloc, Pointer<Ts>&&... ptr_args) : Value(alloc, NodeType::Array) {
    Object(std::forward<Ts>(ptr_args)...);
  }

 private:
  ValueContainerType values_;

  template <typename T>
  void ignore(T&&) const {}
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_ARRAY_H_
