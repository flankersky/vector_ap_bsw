#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_UUID_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_UUID_H_

#include <cstdint>
#include <utility>

#include "ara/rest/allocator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/string.h"
#include "ara/rest/uuid.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Uuid nodes in the OGM
 */
class Uuid : public Value, public Base<Uuid, Value> {
  friend Base<Uuid, Value>;

 public:
  /**
   * needs documentation
   */
  using ValueType = ara::rest::Uuid;

  /**
   * \brief get
   *
   * \return uuid object
   */
  const ValueType& GetValue() const noexcept { return uuid_; }

  /**
   * \brief set
   *
   * \param value uuid object
   */
  void SetValue(const ValueType& value) { uuid_ = value; }

  /**
   * \brief function description
   * Detailed description
   *
   * \param value
   * \return return type
   */
  void SetValue(ValueType&& value) { uuid_ = std::move(value); }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Uuid; }

 protected:
  /**
   * \brief function description
   * Detailed description
   *
   * \param u TODO
   * \return return type
   */
  explicit Uuid(const ara::rest::Uuid& u) : Value(NodeType::Uuid), uuid_(u) {}

  /**
   * \brief function description
   * Detailed description
   *
   * \param u TODO
   * \return return type
   */
  explicit Uuid(ara::rest::Uuid&& u) : Value(NodeType::Uuid), uuid_(std::move(u)) {}

 private:
  /**
   * \brief uuid object
   */
  ValueType uuid_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_UUID_H_
