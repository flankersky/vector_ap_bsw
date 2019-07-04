#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_REAL_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_REAL_H_

#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/value.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Real nodes in the OGM
 * This type represents a generic ﬂoating point value. ara::rest::Real::ValueType denotes
 * its corresponding C++ data type which must be at least as precise as type long double.
 */
class Real : public Value, public Base<Real, Value> {
  friend class Base<Real, Value>;

 public:
  /**
   * needs documentation
   */
  using ValueType = long double;

  /**
   * \brief Returns its value as a C++ data type
   *  \return a value of type ValueType
   */
  ValueType GetValue() const noexcept { return value_; }

  /**
   * \brief Sets the current value from a C++ data type
   *
   * \param v value
   */
  void SetValue(ValueType v) noexcept { value_ = v; }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Real; }

 protected:
  /**
   * \brief constructor with ValueType parameter
   *
   * \param value TODO
   */
  explicit Real(ValueType value = ValueType{}) : Value(NodeType::Real), value_(value) {}

 private:
  /**
   * \brief value
   */
  ValueType value_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_REAL_H_
