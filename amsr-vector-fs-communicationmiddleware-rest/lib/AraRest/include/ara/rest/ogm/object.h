#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_OBJECT_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_OBJECT_H_

#include <utility>
#include <vector>

#include "ara/rest/allocator.h"
#include "ara/rest/iterator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/field.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/pointer.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Object nodes in the OGM
 *  Represents a record. All ara::rest payloads must have an Object as their top-level type.
 *  A record may only be composed of "named" Value instances, by means of Field.
 *  As opposed to Array: unordered but random access.
 */
class Object : public Value, public Base<Object, Value> {
  friend class Base<Object, Value>;

 public:
  /**
   * \brief field type
   */
  using FieldType = Pointer<Field>;

  /**
   * \brief fields type
   */
  using FieldContainerType = std::vector<FieldType>;

  /**
   * \brief TransformOperator
   */
  struct TransformOperator {
    /**
     * \brief operator for transformation
     * \param arg input of transformation, a value_type of Fieldstype
     * \return output of transformation, a dereferenced unique ptr
     */
    Field& operator()(const FieldContainerType::value_type& arg) const noexcept { return *arg; }
  };

  /**
   * \brief Value iterator
   *
   *  Iterator::value_type must of type Value*.
   */
  using Iterator = TransformIterator<FieldContainerType::iterator, TransformOperator>;

  /**
   * \brief Value iterator
   *
   *  Iterator::value_type must of type Value*.
   */
  using ConstIterator = TransformIterator<FieldContainerType::const_iterator, TransformOperator>;

  /**
   * Iterator range
   */
  using FieldRange = IteratorRange<Iterator>;

  /**
   * Iterator range
   */
  using ConstFieldRange = IteratorRange<ConstIterator>;
  /** \brief Returns the number of elements
    *  \return the number of array elements
    */

  std::size_t GetSize() const noexcept { return fields_.size(); }

  /** \brief Returns whether the object holds no elements
   *  \return true if the array holds no elements
   */
  bool IsEmpty() const noexcept { return fields_.size() == 0; }

  /** \brief Returns a range of fields
   *  \return an iterator range of fields
   */
  FieldRange GetFields() noexcept { return FieldRange{Iterator{fields_.begin()}, Iterator{fields_.end()}}; }

  /** \brief Returns a range of fields
   *  \return an iterator range of fields
   */
  ConstFieldRange GetFields() const noexcept {
    return ConstFieldRange{ConstIterator{fields_.begin()}, ConstIterator{fields_.end()}};
  }

  /**
   * \brief has field
   *
   * \param str_view  name of the field to search for
   * \return true if field is available otherwise false
   */
  bool HasField(StringView str_view) const noexcept {
    auto it = Find(str_view);
    return it != GetFields().end();
  }

  /** \brief Searches for a field of the given name
   *
   * If the given field name is not found, the return value will be equal to GetFields().end().
   *
   *  \param str_view  field name to look up
   *  \return an iterator pointing to the position of the element.
   */
  Iterator Find(StringView str_view) noexcept {
    for (auto i(fields_.begin()), ie(fields_.end()); i != ie; ++i) {
      if (str_view.compare((*i).get()->GetName()) == 0) {
        return Iterator{i};
      }
    }
    return Iterator{fields_.end()};
  }

  /** \brief Searches for a field of the given name
   *
   * If the given field name is not found, the return value will be equal to GetFields().end().
   *
   *  \param str_view field name to look up
   *  \return an iterator pointing to the position of the element.
   */
  ConstIterator Find(StringView str_view) const noexcept {
    for (auto i(fields_.begin()), ie(fields_.end()); i != ie; ++i) {
      if (str_view.compare((*i).get()->GetName()) == 0) {
        return ConstIterator{i};
      }
    }
    return ConstIterator{fields_.end()};
  }

  /** \brief Inserts a field into the object
   *
   *  If a field of the same name already exists, no insertion is performed.
   *  In this case the passed pointer to Field is not invalidated.
   *
   *  \param field: field to insert
   *  \return true if insertion was performed.
   */
  void Insert(Pointer<Field>&& field) {
    /* TODO(viseng): normally one checks that the field name isn't already existing while inserting */
    fields_.emplace_back(std::move(field));
  }

  /**
   * \brief Removes value from the set
   *
   *  Removes an element from the set. Removal invalidates all iterators referencing
   *  the respective element.
   *
   *  \param iter an iterator pointing to an element.
   *  \return an iterator pointing to the element following the one just removed.
   */
  Iterator Remove(Iterator iter) {
    if (iter != GetFields().end()) {
      /* remove the element from the fields_ list */
      return Iterator{fields_.erase(iter.GetBaseIterator())};
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
  std::pair<Iterator, Pointer<Field>> Release(Iterator iter) {
    Pointer<Field> tmp = std::move(*(iter.GetBaseIterator()));
    auto it = fields_.erase(iter.GetBaseIterator());
    return {Iterator{it}, std::move(tmp)};
  }

  /** \brief Replaces an element by a new one without the destroying the old one
   *
   *  Replaces a field without destroying it. Instead the replaced element is returned.
   *  Effectively, ownership of the old element is passed back to the user.
   *
   *  \param iter an iterator pointing to the element to be replaced
   *  \param field Field to replace the current value
   *  \return a Pointer to the old element
   */

  Pointer<Field> Replace(Iterator iter, Pointer<Field>&& field) {
    /* store the current element temporary */
    auto tmp{iter.GetBaseIterator()->release()};
    /* update value */
    iter.GetBaseIterator()->reset(field.release());
    /* return the old element */
    return Pointer<Field>{tmp};
  }

  /**
   * \brief clear - remove all fields from the set
   */
  void Clear() { fields_.clear(); }

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
  static bool classof(const Node* o) { return o->type() == NodeType::Object; }

 protected:
  /**
   * \brief constructor with allocator parameter
   *
   * \param alloc TODO
   */
  explicit Object(Allocator* alloc = GetDefaultAllocator()) : Value(NodeType::Object) { (void)alloc; }

  /**
   * \brief templated constructor with pointer parameter
   *
   * cannot use initializer_list
   *
   * \param ts_args TODO
   */
  template <typename... Ts>
  explicit Object(Pointer<Ts>&&... ts_args) : Value(NodeType::Object) {
    // We expand the parameter pack in a brace-enclosed initialized list
    int dummy[] = {0, (Insert(std::move(ts_args)), 0)...};
    ignore(dummy);
  }

  /**
   * \brief templated constructor with allocator and pointer parameter
   *
   * cannot use initializer_list

   * \param alloc TODO
   * \param ts_args TODO
   */
  template <typename... Ts>
  explicit Object(Allocator* alloc, Pointer<Ts>&... ts_args) : Value(NodeType::Object) {
    (void)alloc;
    Object(std::forward<Ts>(ts_args)...);  // NOLINT // due to explicit warning, somehow cpplint got confused here
  }

 private:
  FieldContainerType fields_;

  template <typename T>
  void ignore(T&&) const noexcept {}
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_OBJECT_H_
