#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_VALUE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_VALUE_H_

#include "ara/rest/ogm/base.h"
#include "ara/rest/ogm/node.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * \brief This class represents Value nodes in the OGM
 *
 * A Value is a meta type only used to identify the subset of graph nodes that directly
 * represent values such as Int or Real.
 */
class Value : public Node, public Base<Value, Node, false> {
  friend class Base<Value, Node, false>;

 public:
  /**
   * Check if types of nodes are matching
   * \param o
   * \return true if type of o matches this class type
   */
  static bool classof(const Node* o) noexcept {
    return NodeType::ValueTypesStart < o->type() && o->type() < NodeType::ValueTypesEnd;
  }

 protected:
  /**
   * \brief constructor
   */
  explicit Value(NodeType type) noexcept : Node(nullptr, type) {}  // note the 'nullptr' here

  /**
   * \brief constructor
   */
  explicit Value(Allocator* alloc, NodeType type) noexcept : Node(alloc, type) {}
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_VALUE_H_
