#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_STRING_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_STRING_H_
#include <string>
#include "ara/rest/allocator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/string.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief Represents textual value Nodes in the OGM
 * ara::rest::String::ValueType denotes its corresponding C++ data type.
 * Note that string encoding is ignored by ara::rest. A string is just a dynamic array.
 * The interpretation of encoding speciﬁc characters is not subject to the ara::rest speciﬁcation.
 *
 * Internal static buffer size is implementation defined
 * 'String' does not get an allocator: either it's std::string, then its game over
 * or its a REST string, then that once carries its own allocator.
 */
class String : public Value, public Base<String, Value> {
  friend class Base<String, Value>;

 public:
  /**
   * needs documentation
   */
  using ValueType = StringView;

  /** \brief Returns its value as a C++ data type
   *  \return a value of type ValueType
   */

  ValueType GetValue() const {
    StringView str_view(StringView(STRING_TO_STRINGVIEW(str_value_)));
    return str_view;
  }

  /**
  * \brief Sets the current value from a C++ data type
  * \param v a value
  */
  void SetValue(const ValueType& v) { str_value_.insert(0, v.data(), v.size()); }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::String; }

 protected:
  /**
   * \brief constructor with ValueType parameter
   *
   * must be std::string compatible
   *
   * \param value TODO
   */
  explicit String(ValueType value = ValueType{}) : Value(NodeType::String) {
    str_value_.insert(0, value.data(), value.size());
  }

  /**
   * \brief constructor with allocator and value parameter
   *
   * must be std::string compatible
   *
   * \param alloc TODO
   * \param value TODO
   */
  explicit String(Allocator* alloc, ValueType value = ValueType{}) : Value(NodeType::String) {
    (void)alloc;
    str_value_.insert(0, value.data(), value.size());
  }

 private:
  /**
   * \brief String to persist the memory pointed by the string view.
   */
  std::string str_value_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_STRING_H_
