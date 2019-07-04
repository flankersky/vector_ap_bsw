#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_FIELD_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_FIELD_H_

#include <climits>
#include <utility>

#include "ara/rest/allocator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/node.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/pointer.h"
#include "ara/rest/string.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Field nodes in the OGM
 *
 * Represents a "named" Value instance and is only used in conjunction with Object.
 * Effectively a Field is a pair of a name and a handle to a Value type.
 * At some later time, we need to figure out how to configure this
 * to have non-string keys.
 *
 * Note that field keys could be cached as well a ViWi-specific keyword cache.
 *
 * Ideally, keys are integral IDs of course. At some point, we need to figure
 * out how to provide those.
 */
class Field : public Node, public Base<Field, Node> {
 public:
  friend class Base<Field, Node>;

  /** \brief Return the name of a Field
 *
 * Fields names are immutable. To set a different name a new Field must be inserted.
 *
 *  \return a name
 */
  const ara::rest::StringView GetName() const noexcept { /* TODO(x) need to be updated in spec : returning by value*/
    StringView str_view(STRING_TO_STRINGVIEW(name_));
    return str_view;
  }

  /**
   * \brief set name
   *
   * \param name to be set to the field
   */
  void SetName(const ara::rest::String& name) { name_ = name; }

  /** \brief Returns the value represented by a Field
   *
   *  \return a reference to the current field value
   */

  const Value& GetValue() const noexcept { return *value_; }

  /** \brief Returns the value represented by a Field
   *
   *  \return a reference to the current field value
   */

  Value& GetValue() noexcept { return *value_; }
  /** \brief Sets a new value
   *
   *  The previous value is destroyed
   *
   *  \param v a new Value
   */
  void SetValue(Pointer<Value>&& v) noexcept { value_.reset(v.release()); }

  /** \brief Sets a new value and returns the old one
   *
   *  \param v a new Value
   *  \return the old value
   */
  Pointer<Value> ReplaceValue(Pointer<Value>&& v) noexcept {
    /* store the current element temporary */
    auto tmp{value_.release()};
    /* update value */
    value_.reset(v.release());
    /* return the old element */
    return Pointer<Value>{tmp};
  }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Field; }

 protected:
  /** \brief Constructs a Field
     *
     *  \param name name of this Field
     *  \param value value object attached to this Field
     */
  Field(const ara::rest::String& name, Pointer<Value>&& value)
      : Node(NodeType::Field), name_(name), value_(std::move(value)) {}

  /** \brief Constructs field, providing an allocator
   *
   *  The allocator argument may be used for internal allocation purposes.
   *
   *  \param alloc an allocator
   *  \param name a field name
   *  \param value a field value
   */

  Field(Allocator* alloc, const ara::rest::String& name, Pointer<Value>&& value)
      : Node(NodeType::Field), name_(name), value_(std::move(value)) {
    (void)alloc;
  }

 private:
  /**
   * \brief name
   */
  ara::rest::String name_;

  /**
   * \brief unique pointer to value
   */
  Pointer<Value> value_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_FIELD_H_
