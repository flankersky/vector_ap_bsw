#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_URI_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_URI_H_

#include <utility>

#include "ara/rest/allocator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/value.h"
#include "ara/rest/uri.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Uri nodes in the OGM
 */
class Uri : public Value, public Base<Uri, Value> {
  friend class Base<Uri, Value>;

 public:
  /**
   * needs documentation
   */
  using ValueType = ara::rest::Uri;

  /**
   * \brief get
   *
   * \return uri object
   */
  const ValueType& GetValue() const noexcept { return uri_; }

  /**
   * \brief set
   *
   * \param value uri object
   */
  void SetValue(ValueType& value) { uri_ = value; }

  /**
   * \brief set
   *
   * \param value uri object
   */
  void SetValue(ValueType&& value) { uri_ = std::move(value); }

  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) { return o->type() == NodeType::Uri; }

 protected:
  /**
   * \brief function description
   * Detailed description
   *
   * \param u TODO
   * \return return type
   */
  explicit Uri(const ara::rest::Uri& u) : Value(NodeType::Uri), uri_(u) {}

  /**
   * \brief function description
   * Detailed description
   *
   * \param u TODO
   * \return return type
   */
  explicit Uri(ara::rest::Uri&& u) : Value(NodeType::Uri), uri_(std::move(u)) {}

 private:
  /**
   * \brief uri object
   */
  ValueType uri_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_URI_H_
