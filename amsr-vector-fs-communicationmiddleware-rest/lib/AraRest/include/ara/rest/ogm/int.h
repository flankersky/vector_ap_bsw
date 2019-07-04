#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_INT_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_INT_H_

#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/value.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Int nodes in the OGM
 */
class Int : public Value, public Base<Int, Value> {
  friend class Base<Int, Value>;

 public:
  /**
   * needs documentation
   */
  using ValueType = std::int64_t;

  /** \brief Returns its value as a C++ data type
   *  \return a value of type ValueType
   */
  ValueType GetValue() const noexcept { return value_; }

  /** \brief Sets the current value from a C++ data type
   *  \param v a value
   */
  void SetValue(ValueType v) noexcept { value_ = v; }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Int; }

 protected:
  /**
   * \brief provide allocator constructor
   *
   *  \param value an intial value
   */
  explicit Int(ValueType value = ValueType{}) : Value(NodeType::Int), value_(value) {}

 private:
  /**
   * \brief value
   */
  ValueType value_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_INT_H_
