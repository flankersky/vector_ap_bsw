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
/**        \file  static_list.h
 *        \brief
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_STATIC_LIST_H_
#define LIB_INCLUDE_VAC_CONTAINER_STATIC_LIST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include "vac/container/intrusive_list.h"
#include "vac/language/cpp14_backport.h"
#include "vac/memory/object_pool.h"

namespace vac {
namespace container {

/**
 * \brief Class to implement a StaticList.
 *
 * List Nodes must inherit from StaticListNode<T>.
 */
template <typename T, typename alloc = std::allocator<T>>
class StaticList final {
 public:
  /**
   * \brief Type for nodes of the static list.
   *
   * Derive from this type to create an object that can be a list member.
   * T should be the type of your object.
   */
  class Node final : public vac::container::IntrusiveListNode<Node> {
   public:
    /**
     * \brief Constructor for a node with arguments.
     * \param args elements which to put into the list
     */
    template <typename... Args>
    explicit Node(Args&&... args) : vac::container::IntrusiveListNode<Node>(), elem_(std::forward<Args>(args)...) {}

    /// Constructor for a node without argument.
    Node() : vac::container::IntrusiveListNode<Node>() {}

    /// Deleted copy constructor
    Node(const Node&) = delete;
    /// Deleted move constructor
    Node(Node&&) = delete;
    /**
     * \brief Deleted copy assignment
     * \returns a reference to the assigned-to object
     */
    Node& operator=(const Node&) = delete;
    /**
     * \brief Deleted move assignment
     * \returns a reference to the assigned-to object
     */
    Node& operator=(Node&&) = delete;

    ~Node() = default;

    /**
     * \brief Getter for stored element.
     * \returns a reference to stored element
     */
    T& GetElem() { return elem_; }

    /**
     * \brief Getter for stored element.
     * \returns a reference to stored element
     */
    const T& GetElem() const { return elem_; }

   private:
    /**
     * \brief Object with the business logic type.
     *
     */
    T elem_;
  };

  /// The memory management type used to allocate Node objects
  using StorageType = vac::memory::ObjectPool<Node, alloc>;

  /// Typedef for the size type
  using size_type = typename StorageType::size_type;

  /// Typedef for the element reference type
  using reference = T&;

  /// Typedef for the element reference type
  using const_reference = const T&;

  /// Typedef for the element pointer type
  using pointer = T*;

  /// Typedef for the element pointer type
  using const_pointer = const T*;

  /// The type of the list used to store elements
  using ListType = vac::container::IntrusiveList<Node>;

  /// Typedef for the iterator type of this list
  class iterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    /// Typedef for the iterator of the container
    using base_iterator = typename ListType::iterator;

    /**
     * \brief Construct an iterator from a base_iterator
     * \param it base iterator from which to create this iterator
     */
    explicit iterator(base_iterator& it) : it_(it) {}

    /**
     * \brief Construct an iterator from a base_iterator
     * \param it base iterator from which to create this iterator
     */
    explicit iterator(base_iterator&& it) : it_(it) {}

    /// Copy constructor
    iterator(const iterator&) = default;
    /**
     * \brief Copy assignment
     * \returns a reference to the assigned-to iterator
     */
    iterator& operator=(const iterator&) = default;
    /// Move constructor
    iterator(iterator&&) = default;
    /**
     * \brief Move assignment
     * \returns a reference to the assigned-to iterator
     */
    iterator& operator=(iterator&&) = default;

    ~iterator() = default;

    /**
     * \brief Advance the iterator by one element
     * \returns a copy of the moved iterator
     */
    iterator& operator++() {
      ++it_;
      return *this;
    }

    /**
     * \brief Move the iterator back by one element
     * \returns a copy of the moved iterator
     */
    iterator& operator--() {
      --it_;
      return *this;
    }

    /**
     * \brief Get the list node.
     *
     * \returns A reference to the list node pointed to by this iterator.
     */
    reference operator*() { return it_->GetElem(); }

    /**
     * \brief Get the list node.
     *
     * \returns A reference to the list node pointed to by this iterator.
     */
    pointer operator->() { return &it_->GetElem(); }

    /**
     * \brief Compare two iterators for equality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to the same list node.
     */
    bool operator==(const iterator& other) const { return it_ == other.it_; }

    /**
     * \brief Compare two iterators for inequality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to different list nodes.
     */
    bool operator!=(const iterator& other) const { return it_ != other.it_; }

    /**
     * \brief Access to the underlying containers' iterator
     * \returns a copy of the underlying iterator
     */
    base_iterator GetBaseIterator() { return it_; }

   private:
    /// the underlying containers' iterator
    base_iterator it_;
  };

  /// Typedef for the const iterator type of this list
  class const_iterator final
      : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&> {
   public:
    /// Define types for constant iterator
    using const_iter_types = std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>;

    /// Typedef for the iterator of the container
    using base_iterator = typename ListType::const_iterator;

    /**
     * \brief Construct an iterator from a base_iterator
     * \param it the base_iterator from which to construct the iterator
     */
    explicit const_iterator(base_iterator& it) : it_(it) {}

    /**
     * \brief Construct an iterator from a base_iterator
     * \param it the base_iterator from which to construct the iterator
     */
    explicit const_iterator(base_iterator&& it) : it_(it) {}

    /// Copy constructor
    const_iterator(const const_iterator&) = default;
    /**
     * \brief Copy assignment
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const const_iterator&) = default;
    /// Move constructor
    const_iterator(const_iterator&&) = default;
    /**
     * \brief Move assignment
     * \returns a reference to the assigned-to object
     */
    const_iterator& operator=(const_iterator&&) = default;
    ~const_iterator() = default;

    /**
     * \brief Advance the iterator by one element
     * \returns a reference to the iterator
     */
    const_iterator& operator++() {
      ++it_;
      return *this;
    }

    /**
     * \brief Move the iterator back by one element
     * \returns a reference to the iterator
     */
    const_iterator& operator--() {
      --it_;
      return *this;
    }

    /**
     * \brief Get the list node.
     *
     * \returns A reference to the list node pointed to by this iterator.
     */
    typename const_iter_types::reference operator*() const { return it_->GetElem(); }

    /**
     * \brief Get the list node.
     *
     * \returns A reference to the list node pointed to by this iterator.
     */
    typename const_iter_types::pointer operator->() const { return &it_->GetElem(); }

    /**
     * \brief Compare two iterators for equality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to the same list node.
     */
    bool operator==(const const_iterator& other) const { return it_ == other.it_; }

    /**
     * \brief Compare two iterators for inequality.
     *
     * \param other Iterator to compare to.
     *
     * \returns true if both iterators point to different list nodes.
     */
    bool operator!=(const const_iterator& other) const { return it_ != other.it_; }

    /**
     * \brief Access to the underlying containers' iterator
     * \returns the underlying iterator
     */
    base_iterator GetBaseIterator() { return it_; }

   private:
    base_iterator it_;
  };

  /**
   * \brief Constructor to create an empty StaticList.
   */
  StaticList() = default;

  /// Deleted copy constructor
  StaticList(const StaticList&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  StaticList& operator=(const StaticList&) = delete;
  /// Deleted move constructor
  StaticList(StaticList&&) = delete;
  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  StaticList& operator=(StaticList&&) = delete;

  /**
   * \brief Update the memory allocation
   *
   * The current implementation only allows a single allocation. All further reservations will only limit the visible
   * memory. All subsequent calls to reserve() where new_capacity is greater than the initial new_capacity will be
   * rejected with a bad_alloc.
   *
   * \param new_capacity The number of T's to reserve space for
   *
   * \throw std::runtime_error Exception is thrown if the list is being reallocated.
   */
  void reserve(size_type new_capacity) { storage_.reserve(new_capacity); }

  /**
   * \brief Insert an element at the front of the list.
   *
   * \param args Arguments to instantiate new Object.
   *
   * \throw std::runtime_error  The list is full and no Object can be pushed.
   */
  template <typename... Args>
  void push_front(Args&&... args) {
    typename StorageType::pointer element_ptr = storage_.create(std::forward<Args>(args)...);
    queue_.push_front(*element_ptr);
  }

  /**
   * \brief Remove an element from the front of the list.
   */
  void pop_front() {
    if (!empty()) {
      Node& node_front = *queue_.front().GetSelf();
      queue_.pop_front();
      storage_.destroy(&node_front);
    }
  }

  /**
   * \brief Returns a reference to the first element of the static list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to a Node.
   */
  T& front() {
    // undefined behavior on empty list.
    return queue_.front().GetSelf()->GetElem();
  }

  /**
   * \brief Returns a reference to the first element of the static list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to a Node.
   */
  const T& front() const {
    // undefined behavior on empty list.
    return queue_.front().GetSelf()->GetElem();
  }

  /**
   * \brief Insert an element at the back of the list.
   *
   * \param value Object to copy into the list.
   *
   * \throw std::runtime_error The list is full and no Object can be pushed.
   */
  void push_back(const T& value) {
    typename StorageType::pointer element_ptr = storage_.create(value);
    queue_.push_back(*element_ptr);
  }

  /**
   * \brief Insert an element at the back of the list.
   *
   * \param value Object to move into the list.
   *
   * \throw std::runtime_error The list is full and no Object can be pushed.
   */
  void push_back(T&& value) {
    typename StorageType::pointer element_ptr = storage_.create(std::forward<T>(value));
    queue_.push_back(*element_ptr);
  }

  /**
   * \brief Appends a new element to the end of the container.
   *
   * \param args Arguments to instantiate new Object.
   *
   * \throw std::runtime_error The list is full and no Object can be pushed.
   */
  template <typename... Args>
  void emplace_back(Args&&... args) {
    typename StorageType::pointer element_ptr = storage_.create(std::forward<Args>(args)...);
    queue_.push_back(*element_ptr);
  }
  /**
   * \brief Remove an element from the back of the list.
   */
  void pop_back() {
    if (!empty()) {
      Node& node_back = *queue_.back().GetSelf();
      queue_.pop_back();
      storage_.destroy(&node_back);
    }
  }
  /**
   * \brief Returns a reference to the last element of the static list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to a Node.
   */
  T& back() {
    // undefined behavior on empty list.
    return queue_.back().GetSelf()->GetElem();
  }

  /**
   * \brief Returns a reference to the last element of the static list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to a Node.
   */
  const T& back() const {
    // undefined behavior on empty list.
    return queue_.back().GetSelf()->GetElem();
  }

  /**
   * \brief Determine whether the list is currently empty.
   *
   * \return true if the static list is empty. False if the list has at least one element.
   */
  bool empty() const { return queue_.empty(); }

  /**
   * \brief Determine whether the list is currently full.
   *
   * \return true if the static list is full. False if the list has at least one free place.
   */
  bool full() { return storage_.full(); }

  /**
   * \brief Iterator to the start of the list
   * \returns the iterator at the start of the list
   */
  iterator begin() { return iterator(queue_.begin()); }

  /**
   * \brief Past-The-End iterator of the list
   * \returns the iterator past-the-end
   */
  iterator end() { return iterator(queue_.end()); }

  /**
   * \brief Const Iterator to the start of the list
   * \returns the constant iterator at the start of the list
   */
  const_iterator begin() const { return const_iterator(queue_.begin()); }

  /**
   * \brief Const Past-The-End iterator of the list
   * \returns the constant iterator past-the-end
   */
  const_iterator end() const { return const_iterator(queue_.end()); }

  /**
   * \brief Remove an element pointed to by the iterator.
   *
   * \param elem Iterator pointing to the element to be removed from the list.
   *
   * \return an iterator to the next element in the list.
   */
  iterator erase(iterator elem) {
    Node* elem_to_destroy = elem.GetBaseIterator().GetListNode()->GetSelf();
    iterator next_iterator = iterator(queue_.erase(elem.GetBaseIterator()));
    storage_.destroy(elem_to_destroy);
    return next_iterator;
  }

  /**
   * \brief Removes all elements from the container.
   */
  void clear(void) {
    for (auto it = this->begin(); it != this->end();) {
      it = this->erase(it);
    }
  }

  /**
   * \brief Returns the number of elements in the container.
   *
   * \return The number of elements in the container.
   */
  size_type size(void) const {
    return static_cast<typename std::make_unsigned<typename std::iterator_traits<iterator>::difference_type>::type>(
        std::distance(this->begin(), this->end()));
  }

  /**
   * \brief Insert a new element into the list past the element pointed to by the Iterator.
   *
   * \param where The new object is inserted past the iterator where.
   * \param args Arguments to instantiate new Object.
   *
   * \throw std::runtime_error  The list is full and no Object can be inserted.
   */
  template <typename... Args>
  void insert(iterator where, Args&&... args) {
    typename StorageType::pointer element_ptr = storage_.create(std::forward<Args>(args)...);
    queue_.insert(where.GetBaseIterator(), *element_ptr);
  }

 private:
  /**
   * \brief Array of optional Nodes used to store the elements.
   */
  StorageType storage_;

  /**
   * \brief List of the elements
   */
  ListType queue_;
};

/**
 * \brief Type for nodes of the static list.
 *
 * Derive from this type to create an object that can be a list member.
 * T should be the type of your object.
 */
template <typename T>
using StaticListNode = typename StaticList<T>::Node;
}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_STATIC_LIST_H_
