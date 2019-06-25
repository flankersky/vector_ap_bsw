/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  intrusive_map.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_
#define LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <utility>

#include "vac/testing/test_adapter.h"

namespace vac {
namespace container {

/**
  * \brief Type for nodes of the intrusive map.
  *
  * This serves as a pair of key and value to be inserted in the map as a node.
  * Key type for key in the pair
  * T type of the value in the pair.
  */
template <typename key, typename T>
class IntrusiveMapNode {
 public:
  /**
   * \brief Type of contained key.
   */
  using key_type = key;

  /// Constructor for a node without a map.
  IntrusiveMapNode() : left_(nullptr), right_(nullptr), parent_(nullptr) {}

  /// Deleted copy constructor
  IntrusiveMapNode(const IntrusiveMapNode&) = delete;

  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveMapNode& operator=(const IntrusiveMapNode&) = delete;

  /// Deleted move constructor
  IntrusiveMapNode(IntrusiveMapNode&&) = delete;

  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveMapNode& operator=(IntrusiveMapNode&&) = delete;

  /**
   * \brief Compare the keys of this element to another key.
   * \return int : negative value if new key is less than the other current node key, zero if the both keys are equal,
   *               positive value if new key is greater than the other current node key.
   */
  virtual int KeyCompare(key_type) const = 0;

  /**
   * \brief Compare the keys of this element to the key obtained from another element.
   *
   * This function is necessary as the IntrusiveMap class needs to compare IntrusiveMapNode objects. However, as it
   * does not know how the IntrusiveMapNode stores its key, it cannot extract the key cor comparison using
   * KeyCompare(key_type).
   *
   * \return int : negative value if new key is less than the other current node key, zero if the both keys are equal,
   *               positive value if new key is greater than the other current node key.
   */
  virtual int KeyCompare(const T&) const = 0;

  /// Destructor that removes the node from a map, if it is contained in one.
  virtual ~IntrusiveMapNode() { EraseFromMap(); }

  /**
   * \brief Erase the node from a map.
   */
  void EraseFromMap() {
    // both child present
    if (left_ != nullptr && right_ != nullptr) {
      // Find the node with maximum key value in the left sub tree
      IntrusiveMapNode<key, T>* temp = FindMaxLeft();

      if (temp != nullptr) {
        // Erase temp from the map.
        std::size_t result = temp->EraseNodeWithOneOrNoChild();

        if (result == 1) {
          // adjust the pointers of the  node and parent node.
          temp->SetLeft(left_);
          temp->SetRight(right_);
          temp->SetParent(parent_);

          if (parent_ != nullptr && parent_->Right() == this) {
            parent_->SetRight(temp);
          } else if (parent_ != nullptr && parent_->Left() == this) {
            parent_->SetLeft(temp);
          }
        } else {
          throw std::runtime_error("Error while erasing node from the map");
        }
      }

      // Adjust the left and right parent pointers
      if (left_ != nullptr) {
        left_->parent_ = temp;
      }
      if (right_ != nullptr) {
        right_->parent_ = temp;
      }

      // Adjust the node pointers
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;

      // If node has one or no child
    } else {
      EraseNodeWithOneOrNoChild();
    }
  }

  /**
   * \brief Get the contained element.
   * \returns the contained element
   */
  T* GetSelf() {
    static_assert(std::is_base_of<IntrusiveMapNode<key, T>, T>::value, "T must derive from IntrusiveMapNode");
    return static_cast<T*>(this);
  }

  /**
   * \brief Get the contained element.
   * \returns the contained element
   */
  const T* GetSelf() const {
    static_assert(std::is_base_of<IntrusiveMapNode<key, T>, T>::value, "T must derive from IntrusiveMapNode");
    return static_cast<const T*>(this);
  }

  /**
   * \brief returns the pointer to left node.
   * \return IntrusiveMapNode*
   */
  IntrusiveMapNode* Left() { return left_; }

  /**
   * \brief returns const pointer to left node.
   * \return IntrusiveMapNode*
   */
  const IntrusiveMapNode* Left() const { return left_; }

  /**
   * \brief returns the pointer to right node.
   * \return IntrusiveMapNode*
   */
  IntrusiveMapNode* Right() { return right_; }

  /**
   * \brief returns const pointer to right node.
   * \return IntrusiveMapNode*
   */
  const IntrusiveMapNode* Right() const { return right_; }

  /**
   * \brief returns the pointer to parent node.
   * \return IntrusiveMapNode*
   */
  IntrusiveMapNode* Parent() { return parent_; }

  /**
   * \brief returns const pointer to parent node.
   * \return IntrusiveMapNode*
   */
  const IntrusiveMapNode* Parent() const { return parent_; }

  /**
   * \brief Set the left element.
   * \param new_elem pointer to an IntrusiveMapNode
   */
  void SetLeft(IntrusiveMapNode* new_elem) { left_ = new_elem; }

  /**
   * \brief Set the right element.
   * \param new_elem pointer to an IntrusiveMapNode
   */
  void SetRight(IntrusiveMapNode* new_elem) { right_ = new_elem; }

  /**
   * \brief Set the parent element.
   * \param parent pointer to an IntrusiveMapNode
   */
  void SetParent(IntrusiveMapNode* parent) { parent_ = parent; }

  /**
   * \brief Find min element from the right subtree.
   * \return element from right subtree with minimum key or nullptr if the right subtree is empty.
   */
  const IntrusiveMapNode* FindMinRight() const noexcept {
    const IntrusiveMapNode* min_right = this;
    if (min_right->Right() == nullptr) {
      min_right = nullptr;
    } else {
      min_right = min_right->Right();
      while (min_right != nullptr && min_right->Left() != nullptr) {
        min_right = min_right->Left();
      }
    }
    return min_right;
  }

  /**
   * \brief Find max element from the left subtree.
   * \return element from left subtree with maximum key, or nullptr if the left subtree is emtpy.
   */
  const IntrusiveMapNode* FindMaxLeft() const noexcept {
    const IntrusiveMapNode* max_left = this;
    if (max_left->Left() == nullptr) {
      max_left = nullptr;
    } else {
      max_left = max_left->Left();
      while (max_left != nullptr && max_left->Right() != nullptr) {
        max_left = max_left->Right();
      }
    }
    return max_left;
  }

  /**
   * \brief Find a parent node with immediate large key value.
   * \return Parent with larger key value. or nullptr in case such a parent is not found
   */
  const IntrusiveMapNode* FindLargerParent() const noexcept {
    const IntrusiveMapNode* drag = this;
    const IntrusiveMapNode* larger_parent = this;
    while (larger_parent != nullptr && larger_parent->Left() != drag) {
      // While loop terminates when reaching the end() element or when coming in to the parent from a left child
      drag = larger_parent;
      larger_parent = larger_parent->Parent();
    }  // At this point, larger_parent is either the larger parent or end().
    return larger_parent;
  }

  /**
   * \brief Find a parent node with immediate smaller key value.
   * \return Parent with smaller key value. or nullptr in case such a parent is not found
   */
  const IntrusiveMapNode* FindSmallerParent() const noexcept {
    const IntrusiveMapNode* drag = this;
    const IntrusiveMapNode* smaller_parent = this;
    while (smaller_parent != nullptr && smaller_parent->Right() != drag) {
      // While loop terminates when reaching the end() element or when coming in to the parent from a right child
      drag = smaller_parent;
      smaller_parent = smaller_parent->Parent();
    }  // At this point, larger_parent is either the larger parent or end().
    if (smaller_parent != nullptr && smaller_parent->Parent() == nullptr) {
      smaller_parent = nullptr;
    }
    return smaller_parent;
  }

  /**
   * \brief Find max element from the left subtree.
   * \return element from left subtree with maximum key.
   */
  IntrusiveMapNode* FindMaxLeft() noexcept {
    return const_cast<IntrusiveMapNode*>(static_cast<const IntrusiveMapNode*>(this)->FindMaxLeft());
  }

  /**
   * \brief Find min element from the right subtree.
   * \return element from right subtree with minimum key.
   */
  IntrusiveMapNode* FindMinRight() noexcept {
    return const_cast<IntrusiveMapNode*>(static_cast<const IntrusiveMapNode*>(this)->FindMinRight());
  }

  /**
   * \brief Find a parent node with immediate large key value.
   * \return Parent with larger key value. or nullptr in case such a parent is not found
   */
  IntrusiveMapNode* FindLargerParent() noexcept {
    return const_cast<IntrusiveMapNode*>(static_cast<const IntrusiveMapNode*>(this)->FindLargerParent());
  }

  /**
   * \brief Find a parent node with immediate smaller key value.
   * \return Parent with smaller key value. or nullptr in case such a parent is not found
   */
  IntrusiveMapNode* FindSmallerParent() noexcept {
    return const_cast<IntrusiveMapNode*>(static_cast<const IntrusiveMapNode*>(this)->FindSmallerParent());
  }

 private:
  /**
   * \brief Erase the node from a map.
   * \return Number of elements removed.
   * In case of error number of elements removed will be 0.
   */
  std::size_t EraseNodeWithOneOrNoChild() noexcept {
    std::size_t deleted_count = 0;

    // No or one child present
    if (parent_ != nullptr && (left_ == nullptr || right_ == nullptr)) {
      if (parent_->right_ == this) {
        parent_->right_ = (left_ == nullptr) ? right_ : left_;
      } else if (parent_->left_ == this) {
        parent_->left_ = (left_ == nullptr) ? right_ : left_;
      }

      deleted_count++;

      // Adjust the left and right parent pointers
      if (left_ != nullptr) {
        left_->parent_ = parent_;
      }
      if (right_ != nullptr) {
        right_->parent_ = parent_;
      }

      // Adjust the node pointers
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
    }

    return deleted_count;
  }

  /**
   * \brief Pointer to the left child node.
   */
  IntrusiveMapNode* left_;

  /**
   * \brief Pointer to the right child node.
   */
  IntrusiveMapNode* right_;

  /**
   * \brief Pointer to the parent node.
   */
  IntrusiveMapNode* parent_;
};

/**
 * \brief Type for end node of the intrusive map.
 */
template <typename Key, typename T>
class EndNode : public IntrusiveMapNode<Key, T> {
 public:
  /// constructor
  EndNode() : IntrusiveMapNode<Key, T>() {}
  virtual ~EndNode() = default;

  /**
   * \brief Compares the  key value with the node current node value
   * \return int : always returns 1 because this is the root node with only left subtree so the key is considered to be
   * always greater then any other key in the map.
   */
  int KeyCompare(Key) const override { return 1; }

  /**
   * \brief Compares the  key value with the node current node value
   * \return int : always returns 1 because this is the root node with only left subtree so the key is considered to be
   * always greater then any other key in the map.
   */
  int KeyCompare(const T&) const override { return 1; }
};

/**
 * \brief Iterator for this IntrusiveMap.
 */
template <typename key, typename T>
class IntrusiveMapIterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
 public:
  /// Typedef for the superclass
  using base_type = std::iterator<std::bidirectional_iterator_tag, T>;

  /**
   * \brief Constructor for an iterator
   * \param node The Map from which to construct the iterator
   */
  explicit IntrusiveMapIterator(IntrusiveMapNode<key, T>* node) : node_(node) {}

  /// Copy constructor
  IntrusiveMapIterator(const IntrusiveMapIterator&) = default;
  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveMapIterator& operator=(const IntrusiveMapIterator&) = default;
  /// Move constructor
  IntrusiveMapIterator(IntrusiveMapIterator&&) = default;
  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveMapIterator& operator=(IntrusiveMapIterator&&) = default;

  ~IntrusiveMapIterator() = default;

  /**
   * \brief Return a reference to the contained element
   * \returns a reference to the element type
   */
  T& operator*() { return *(this->GetMapNode()->GetSelf()); }

  /**
   * \brief Return a reference to the contained element
   * \returns a const reference to the element type
   */
  const T& operator*() const { return *(this->GetMapNode()->GetSelf()); }

  /**
   * \brief Get the map node .
   * \returns The contained pointer to the map node
   */
  IntrusiveMapNode<key, T>* GetMapNode() { return node_; }

  /**
   * \brief Get the map node .
   * \returns The contained pointer to the map node
   */
  const IntrusiveMapNode<key, T>* GetMapNode() const { return node_; }

  /**
   * \brief Compare two iterators for equality.
   * \param other Iterator to compare to.
   * \returns true if both iterators point to the same map node.
   */
  bool operator==(const IntrusiveMapIterator& other) const { return node_ == other.node_; }

  /**
   * \brief Compare two iterators for inequality.
   * \param other Iterator to compare to.
   * \returns true if both iterators point to different map nodes.
   */
  bool operator!=(const IntrusiveMapIterator& other) const { return node_ != other.node_; }

  /**
   * \brief increment the iterator by one element.
   * \returns IntrusiveMapIterator to the element with immediate higher key value.
   */
  IntrusiveMapIterator& operator++() {
    IntrusiveMapNode<key, T>* temp_elem = node_;
    if (node_ != nullptr) {
      temp_elem = node_->FindMinRight();

      // if Min right node not found
      if (temp_elem == nullptr) {
        temp_elem = node_->FindLargerParent();
      }
    }
    if (temp_elem != nullptr) {
      // avoid leaving the tree, remain on the end() element
      node_ = temp_elem;
    }
    return *this;
  }

  /**
   * \brief decrement the iterator by one element.
   * \returns IntrusiveMapIterator to the element with an immediate lower key value.
   */
  IntrusiveMapIterator& operator--() {
    IntrusiveMapNode<key, T>* temp_elem = node_;
    if (node_ != nullptr) {
      temp_elem = node_->FindMaxLeft();

      // if Max Left node not found
      if (temp_elem == nullptr) {
        temp_elem = node_->FindSmallerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

 private:
  /// Pointer to the map element.
  IntrusiveMapNode<key, T>* node_;
};

/**
 * \brief Const Iterator for this IntrusiveMap.
 */
template <typename key, typename T>
class ConstIntrusiveMapIterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
 public:
  /// Typedef for the superclass
  using base_type = std::iterator<std::bidirectional_iterator_tag, T>;

  /**
   * \brief Constructor for an iterator
   * \param node the Map from which to construct the iterator
   */
  explicit ConstIntrusiveMapIterator(const IntrusiveMapNode<key, T>* node) : node_(node) {}

  /// Copy constructor
  ConstIntrusiveMapIterator(const ConstIntrusiveMapIterator&) = default;
  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to object
   */
  ConstIntrusiveMapIterator& operator=(const ConstIntrusiveMapIterator&) = default;
  /// Move constructor
  ConstIntrusiveMapIterator(ConstIntrusiveMapIterator&&) = default;
  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to object
   */
  ConstIntrusiveMapIterator& operator=(ConstIntrusiveMapIterator&&) = default;

  ~ConstIntrusiveMapIterator() = default;

  /**
   * \brief Return a reference to the contained element
   * \returns a const reference to the element type
   */
  const T& operator*() const { return *(this->GetMapNode()->GetSelf()); }

  /**
   * \brief Get the map node .
   * \returns The contained pointer to the map node
   */
  const IntrusiveMapNode<key, T>* GetMapNode() const { return node_; }

  /**
   * \brief Compare two iterators for equality.
   * \param other Iterator to compare to.
   * \returns true if both iterators point to the same map node.
   */
  bool operator==(const ConstIntrusiveMapIterator& other) const { return node_ == other.node_; }

  /**
   * \brief Compare two iterators for inequality.
   * \param other Iterator to compare to.
   * \returns true if both iterators point to different map nodes.
   */
  bool operator!=(const ConstIntrusiveMapIterator& other) const { return node_ != other.node_; }

  /**
   * \brief increment the iterator by one element.
   * \returns IntrusiveMapIterator to the element with immediate higher key value.
   */
  ConstIntrusiveMapIterator& operator++() {
    const IntrusiveMapNode<key, T>* temp_elem = node_;
    if (node_ != nullptr) {
      temp_elem = node_->FindMinRight();

      // if Min right node not found
      if (temp_elem == nullptr) {
        temp_elem = node_->FindLargerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

  /**
   * \brief decrement the iterator by one element.
   * \returns IntrusiveMapIterator to the element with an immediate lower key value.
   */
  ConstIntrusiveMapIterator& operator--() {
    const IntrusiveMapNode<key, T>* temp_elem = node_;
    if (node_ != nullptr) {
      temp_elem = node_->FindMaxLeft();

      // if Max Left node not found
      if (temp_elem == nullptr) {
        temp_elem = node_->FindSmallerParent();
      }
    }
    node_ = temp_elem;
    return *this;
  }

 private:
  /// Pointer to the map element.
  const IntrusiveMapNode<key, T>* node_;
};

/**
 * \brief Class to implement an IntrusiveMap.
 */
template <typename key, typename T>
class IntrusiveMap final {
 public:
  /// Typedef for the iterator type of this map
  using iterator = IntrusiveMapIterator<key, T>;

  /// Typedef for the iterator type of this map
  using const_iterator = ConstIntrusiveMapIterator<key, T>;

  /// Constructor
  IntrusiveMap() : map_() { map_.SetParent(nullptr); }

  /// Destructor
  ~IntrusiveMap() {
    // Release all elements from the map starting from the smallest key member
    while (!empty()) {
      erase(begin());
    }
  }

  /**
   * \brief Remove an element from map.
   * \param erase_key key to be erased.
   * \return Number of elements removed.
   */
  std::size_t erase(const key& erase_key) {
    std::size_t deleted_count = 0;
    iterator itr = find(erase_key);

    if (map_.Left() != nullptr && itr != end()) {
      IntrusiveMapNode<key, T>* elem = itr.GetMapNode();
      // Erase the node.
      elem->EraseFromMap();
      deleted_count++;
    }
    return deleted_count;
  }

  /**
     * \brief Remove an element from map.
     * \param elem iterator to the element to be erased.
     * \return Number of elements removed.
     */
  void erase(iterator elem) {
    if (!empty() && elem != end()) {
      if (elem.GetMapNode() != nullptr) {
        elem.GetMapNode()->EraseFromMap();
      }
    }
  }

  /**
   * \brief Determine whether the map is currently empty.
   *
   * \return true if the map is empty. False if the map has at least one element.
   */
  bool empty() const { return (map_.Left() == nullptr); }

  /**
   * \brief Insert a new node (key value pair) into the map at appropriate position.
   * \param node element to be inserted in the map.
   * \return iterator to the new node
   */
  std::pair<iterator, bool> insert(IntrusiveMapNode<key, T>* node) {
    bool is_inserted = false;
    if (map_.Left() == nullptr) {
      map_.SetLeft(node);
      if (node != nullptr) {
        node->SetParent(&map_);
        is_inserted = true;
      }
    } else {
      if (node != nullptr && node->GetSelf() != nullptr) {
        IntrusiveMapNode<key, T>* temp_node = SearchNode(*(node->GetSelf()));
        if (temp_node != nullptr) {
          int result = temp_node->KeyCompare(*(node->GetSelf()));

          if (result > 0) {
            temp_node->SetRight(node);
            node->SetParent(temp_node);
            is_inserted = true;
          } else if (result < 0) {
            temp_node->SetLeft(node);
            node->SetParent(temp_node);
            is_inserted = true;
          } else {
            // Get the node that prevented the insertion.
            node = temp_node;
          }
        }
      }
    }
    return std::make_pair(iterator(node), is_inserted);
  }

  /**
   * \brief find node in map with the input key.
   * \param find_key key to to searched for.
   * \return iterator to a node matching the key or a end_ node
   */
  iterator find(const key& find_key) noexcept {
    IntrusiveMapNode<key, T>* node = SearchNode(find_key);
    if (node != nullptr && node->KeyCompare(find_key) == 0) {
      return iterator(node);
    } else {
      return end();
    }
  }

  /**
   * \brief get iterator to smallest key node.
   * \return iterator to Intrusive map.
   */
  iterator begin() {
    if (empty()) {
      return end();
    } else {
      IntrusiveMapNode<key, T>* node = map_.Left();
      while (node->Left() != nullptr) {
        node = node->Left();
      }
      return iterator(node);
    }
  }

  /**
   * \brief get const iterator to smallest key node.
   * \return const iterator to Intrusive map.
   */
  const_iterator cbegin() const {
    if (empty()) {
      return cend();
    } else {
      const IntrusiveMapNode<key, T>* node = map_.Left();
      while (node->Left() != nullptr) {
        node = node->Left();
      }
      return const_iterator(node);
    }
  }

  /**
   * \brief get iterator to end element.
   * \return iterator to end element.
   */
  iterator end() { return iterator(&map_); }

  /**
     * \brief get iterator to end element.
     * \return iterator to end element.
     */
  const_iterator cend() const { return const_iterator(&map_); }

 private:
  /**
   * \brief search for a node position in map with the input key.
   * \param find_key key to to searched for.
   * \return iterator to a node matching the key or a node just previous to the matching key
   */
  template <class KeyOrNode>
  IntrusiveMapNode<key, T>* SearchNode(const KeyOrNode& find_key) noexcept {
    IntrusiveMapNode<key, T>* temp = map_.Left();
    IntrusiveMapNode<key, T>* prev = nullptr;
    while (temp != nullptr && temp->GetSelf() != nullptr) {
      prev = temp;
      int result = temp->GetSelf()->KeyCompare(find_key);
      if (result > 0) {
        temp = temp->Right();
      } else if (result < 0) {
        temp = temp->Left();
      } else if (result == 0) {
        break;
      }
    }
    return prev;
  }

  /**
   * \brief Pointer to the map.
   * Map_ is the root node and the actual map starts from the map_.left. map_.right will always be nullptr.
   * For an empty map the map_.left will be nullptr.
   */
  EndNode<key, T> map_;

  FRIEND_TEST(IntrusiveMap, IterartorOperations_increment);
  FRIEND_TEST(IntrusiveMap, IterartorOperations_increment2);
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_MAP_H_
