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
/**        \file  static_map.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_
#define LIB_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "vac/container/intrusive_map.h"
#include "vac/language/cpp14_backport.h"
#include "vac/memory/object_pool.h"
#include "vac/memory/three_phase_allocator.h"

namespace vac {
namespace container {

/**
 * \brief Class to implement a StaticMap.
 *
 * Map Nodes must inherit from StaticMapNode.
 */
template <typename Key, typename T>
class StaticMap final {
 public:
  /**
   * \brief The type implementing the pair
   */
  using value_type = std::pair<Key, T>;

  /**
   * \brief The key type of this map
   */
  using key_type = Key;

  /**
   * \brief The mapped type of this map
   */
  using mapped_type = T;

  /**
   * \brief Type for nodes of the static Map.
   *
   * Key type for key in the pair
   * T type of the value in the pair.
   */
  class Node final : public vac::container::IntrusiveMapNode<Key, Node> {
   public:
    /**
     * \brief Constructor for a node without argument.
     */
    Node() = delete;

    /**
     * \brief copy constructor.
     */
    Node(const Node&) = delete;

    /**
     * \brief Move constructor.
     * \param other_node node to be moved
     */
    Node(Node&& other_node) = delete;

    /**
     * \brief Deleted copy assignment
     * \returns a reference to the assigned to object
     */
    Node& operator=(const Node&) = delete;

    /**
     * \brief Deleted move assignment
     * \returns a reference to the assigned to object
     */
    Node& operator=(Node&&) = delete;

    /**
     * \brief Forwarding constructor
     * \param args Arguments for the constructor of the Key type
     */
    template <typename... Args>
    Node(Args&&... args)  // NOLINT[runtime/explicit]
        : vac::container::IntrusiveMapNode<Key, Node>(),
          storage_(std::forward<Args>(args)...) {}

    ~Node() = default;

    /**
     * \brief Compares the new key value with the node current node value
     * \param new_key value to be compared
     * \return int : negative value if new key is less than the other current node key, zero if the both keys are equal,
     *              positive value if new key is greater than the other current node key.
     */
    int KeyCompare(Key new_key) const override {
      int retval = 0;
      if (storage_.first < new_key) {
        retval = 1;
      } else if (storage_.first > new_key) {
        retval = -1;
      }
      return retval;
    }

    /**
     * \brief Compares the new key value with the node current node value
     * \param key_node Node containing the key to be compared to
     * \return int : negative value if new key is less than the other current node key, zero if the both keys are equal,
     *              positive value if new key is greater than the other current node key.
     */
    int KeyCompare(const Node& key_node) const override { return KeyCompare(key_node.storage_.first); }

    /**
     * \brief Pair to store the key and value
     */
    value_type storage_;
  };

  /**
   * \brief The memory management type used to allocate Node objects.
   */
  using StorageType = vac::memory::ObjectPool<Node, vac::memory::ThreePhaseAllocator<Node>>;

  /**
   * \brief The size type used in this implementation.
   */
  using size_type = typename StorageType::size_type;

  /**
   * \brief The type of the map used to store elements.
   */
  using MapType = vac::container::IntrusiveMap<Key, Node>;

  /**
   * \brief Typedef for the iterator type of this map.
   */
  class iterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    /**
     * \brief Typedef for the iterator of the container.
     */
    using base_iterator = typename MapType::iterator;

    /**
     * \brief Construct an iterator from a base_iterator.
     * \param it the base iterator from which to construct the object
     */
    explicit iterator(base_iterator& it) : it_(it) {}

    /**
     * \brief Construct an iterator from a base_iterator.
     * \param it the base iterator from which to construct the object
     */
    explicit iterator(base_iterator&& it) : it_(it) {}

    /**
     * \brief Copy constructor.
     */
    iterator(const iterator&) = default;

    /**
     * \brief Copy assignment.
     * \returns a reference to the assigned to object
     */
    iterator& operator=(const iterator&) = default;

    /**
     * \brief Move constructor.
     */
    iterator(iterator&&) = default;

    /**
     * \brief Move assignment.
     * \returns a reference to the assigned to object
     */
    iterator& operator=(iterator&&) = default;

    ~iterator() = default;

    /**
     * \brief Dereference iterator to map element
     */
    value_type& operator*() { return it_.GetMapNode()->GetSelf()->storage_; }

    /**
     * \brief Dereference iterator to map element
     */
    const value_type& operator*() const { return it_.GetMapNode()->GetSelf()->storage_; }

    /**
     * \brief Dereference iterator to map element
     */
    value_type* operator->() { return &(it_.GetMapNode()->GetSelf()->storage_); }

    /**
     * \brief Dereference iterator to map element
     */
    const value_type* operator->() const { return &(it_.GetMapNode()->GetSelf()->storage_); }

    /**
     * \brief increment the iterator by one element.
     * \returns IntrusiveMapIterator to the element with immediate higher key value.
     */
    iterator& operator++() {
      ++it_;
      return *this;
    }

    /**
     * \brief decrement the iterator by one element.
     * \returns IntrusiveMapIterator to the element with an immediate lower key value.
     */
    iterator& operator--() {
      --it_;
      return *this;
    }

    /**
     * \brief Compare two iterators for equality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to the same map node.
     */
    bool operator==(const iterator& other) const { return it_ == other.it_; }

    /**
     * \brief Compare two iterators for inequality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to different map nodes.
     */
    bool operator!=(const iterator& other) const { return it_ != other.it_; }

    /**
     * \brief Access to the underlying containers' iterator
     * \returns a copy of the underlying iterator
     */
    base_iterator GetBaseIterator() { return it_; }

   private:
    base_iterator it_;
  };

  /**
   * \brief Typedef for the const iterator type of this map.
   */
  class const_iterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    /**
     * \brief Typedef for the const iterator of the container.
     */
    using const_iter_types = std::iterator<std::bidirectional_iterator_tag, T>;

    /**
     * \brief Typedef for the const iterator of the container.
     */
    using base_iterator = typename MapType::const_iterator;

    /**
     * \brief Constructor for an iterator from a base_iterator.
     * \param it the base iterator from which to construct the object
     */
    explicit const_iterator(base_iterator& it) : it_(it) {}

    /**
     * \brief Constructor of an iterator from a base_iterator.
     * \param it the base iterator from which to construct the object
     */
    explicit const_iterator(base_iterator&& it) : it_(it) {}

    /**
     * \brief Copy constructor.
     */
    const_iterator(const const_iterator&) = default;

    /**
     * \brief Copy assignment.
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const const_iterator&) = default;

    /**
     * \brief Move constructor.
     */
    const_iterator(const_iterator&&) = default;

    /**
     * \brief Move assignment.
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const_iterator&&) = default;

    /**
     * \brief destructor.
     */
    ~const_iterator() = default;

    /**
     * \brief Dereference iterator to map element
     */
    const value_type& operator*() const { return it_.GetMapNode()->GetSelf()->storage_; }

    /**
     * \brief Dereference iterator to map element
     */
    const value_type* operator->() const { return &(it_.GetMapNode()->GetSelf()->storage_); }

    /**
     * \brief increment the iterator by one element.
     * \returns IntrusiveMapIterator to the element with immediate higher key value.
     */
    const_iterator& operator++() {
      ++it_;
      return *this;
    }

    /**
     * \brief decrement the iterator by one element.
     * \returns IntrusiveMapIterator to the element with an immediate lower key value.
     */
    const_iterator& operator--() {
      --it_;
      return *this;
    }

    /**
     * \brief Compare two iterators for equality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to the same map node.
     */
    bool operator==(const const_iterator& other) const { return it_ == other.it_; }

    /**
     * \brief Compare two iterators for inequality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to different map nodes.
     */
    bool operator!=(const const_iterator& other) const { return it_ != other.it_; }

    /**
     * \brief Access to the underlying containers' iterator
     * \returns a copy to the underlying iterator
     */
    base_iterator GetBaseIterator() { return it_; }

   private:
    base_iterator it_;
  };

  /**
   * \brief Constructor to create an empty Static map.
   */
  StaticMap() = default;

  /// Deleted copy constructor
  StaticMap(const StaticMap&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  StaticMap operator=(const StaticMap&) = delete;
  /// Deleted move constructor
  StaticMap(StaticMap&&) = delete;
  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  StaticMap operator=(StaticMap&&) = delete;

  ~StaticMap() { clear(); }

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param new_capacity The number of nodes to reserve space for
   *
   * \throw std::runtime_error Exception is thrown if the map is being reallocated.
   */
  void reserve(size_type new_capacity) { storage_.reserve(new_capacity); }

  /**
   * \brief Determine whether the map is currently empty.
   *
   * \return true if the static map is empty. False if the map has at least one element.
   */
  bool empty() const { return map_.empty(); }

  /**
   * \brief The number of objects currently allocated in this static map
   */
  size_type size() const noexcept { return storage_.size(); }

  /**
   * \brief Determine whether the map is currently full.
   *
   * \return true if the static map is full. False if the map has at least one free place.
   */
  bool full() const { return storage_.full(); }

  /**
   * \brief Return an iterator to the first element of the map.
   * \returns an iterator to the first element
   */
  iterator begin() { return iterator(map_.begin()); }

  /**
   * \brief get iterator to end element.
   * \return iterator to end element.
   */
  iterator end() { return iterator(map_.end()); }

  /**
   * \brief Return an iterator to the first element of the map.
   * \returns an iterator to the first element
   */
  const_iterator cbegin() const { return const_iterator(map_.cbegin()); }

  /**
   * \brief get iterator to end element.
   * \return iterator to end element.
   */
  const_iterator cend() const { return const_iterator(map_.cend()); }

  /**
   * \brief Remove an element from map.
   * \param erase_key key to be erased.
   * \return Number of elements removed.
   */
  std::size_t erase(const Key& erase_key) {
    std::size_t erased_count = 0;
    iterator itr = find(erase_key);

    if (itr != end()) {
      Node* elem_to_destroy = itr.GetBaseIterator().GetMapNode()->GetSelf();
      erased_count = map_.erase(erase_key);
      storage_.destroy(elem_to_destroy);
    }
    return erased_count;
  }

  /**
   * \brief Remove an element from map.
   * \param elem node to be erased.
   */
  void erase(iterator elem) {
    if (!empty() && elem != end()) {
      Node* node = elem.GetBaseIterator().GetMapNode()->GetSelf();
      if (node != nullptr) {
        node->EraseFromMap();
        storage_.destroy(node);
      }
    }
  }

  /**
   * \brief Remove all elements from the map.
   */
  void clear() {
    while (!empty()) {
      erase(begin());
    }
  }

  /**
   * \brief find node in map with the input key.
   * \param find_key key to to searched for.
   * \return iterator to a node matching the key or a end_ node
   */
  iterator find(const Key& find_key) noexcept { return iterator(map_.find(find_key)); }

  /**
   * \brief Insert a new element into the map.
   * \param  value element to be inserted in the map.
   * \return pair consisting of an iterator to the inserted element (or to the element that prevented the insertion)
   * and
   * a bool denoting whether the insertion took place.
   * \throw std::runtime_error  The map is full and no Object can be inserted.
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    typename StorageType::pointer element_ptr = storage_.create(value);
    auto pair = map_.insert(element_ptr);
    if (pair.second == false) {
      storage_.destroy(element_ptr);
    }
    return std::make_pair(iterator(pair.first), pair.second);
  }

  /**
   * \brief Insert a new element into the map.
   *
   * \param args constructor arguements for the key and value.
   * \return pair consisting of an iterator to the inserted element (or to the element that prevented the insertion)
   * and
   * a bool denoting whether the insertion took place.
   * \throw std::runtime_error  The map is full and no Object can be inserted.
   */
  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args) {
    typename StorageType::pointer element_ptr = storage_.create(std::forward<Args>(args)...);
    auto pair = map_.insert(element_ptr);
    if (pair.second == false) {
      storage_.destroy(element_ptr);
    }
    return std::make_pair(iterator(pair.first), pair.second);
  }

 private:
  /**
   * \brief Instance of objectpool storage.
   */
  StorageType storage_;

  /**
   * \brief map used to store nodes.
   */
  MapType map_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_STATIC_MAP_H_
