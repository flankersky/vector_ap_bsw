
#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_NODE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_NODE_H_

#include "ara/rest/allocator.h"
#include "ara/rest/ogm/base.h"
#include "ara/rest/pointer.h"

namespace ara {

namespace rest {

namespace ogm {

/**
 * A Node serves as a base type for runtime polymorphic operations only. It also restricts
 * C++ semantics. The default constructor of Node must be protected and all other constructors and copy- or
 * move-assignment operators must be deleted. A Node must have public virtual destructor.
 * non-copyable (there is Node::Copy)
 */
class Node : public CommonBase, public Base<Node, void, false> {
 public:
  /**
   * \brief destructor
   */
  virtual ~Node() = default;

  /**
   * \brief delete copy constructor
   */
  Node(const Node&) = delete;  ///< Non-copyable; copy with ogm::Copy()

  /**
   * \brief delete assign constructor
   */
  Node& operator=(const Node&) = delete;

  /** \brief Returns a pointer to the allocator that manages this subtree
   *  \return a pointer to an allocator
   */
  Allocator* GetAllocator() noexcept;
  /** \brief Returns a pointer to the allocator that manages this subtree
   *  \return a pointer to an allocator
   */
  const Allocator* GetAllocator() const noexcept;

  /**
   * \brief move constructor
   */
  Node(Node&&) = default;

  /**
   * \brief move assignment
   */
  Node& operator=(Node&&) = default;

  /**
   * Check if types of nodes are matching
   * \return true
   */
  static bool classof(const Node*) { return true; }

  /**
   * \brief get the node's type
   * @return
   */
  NodeType type() const { return node_type_; }

 protected:
  /**
   * \brief constructor
   * \param node_type type of node
   * non-user-constructible
   */
  explicit Node(NodeType node_type) noexcept : CommonBase(nullptr), node_type_(node_type) {}

  /**
   * \brief constructor
   * \param alloc allocator
   * \param node_type type of node
   * non-user-constructible
   */
  Node(Allocator* alloc, NodeType node_type) noexcept : CommonBase(alloc), node_type_(node_type) {}

 private:
  /**
   * \brief the type of the node
   */
  NodeType node_type_;
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_NODE_H_
