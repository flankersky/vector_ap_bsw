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
/**        \file  intrusive_list.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_LIST_H_
#define LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_LIST_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <iterator>

namespace vac {
namespace container {

/**
 * \brief Type for nodes of the intrusive list.
 *
 * Derive from this type to create an obtain an object that can be a list member.
 * T should be the type of your object.
 */
template <typename T>
class IntrusiveListNode {
 public:
  /// Constructor for a node without a list.
  constexpr IntrusiveListNode() : prev_(this), next_(this) {
    static_assert(std::is_base_of<IntrusiveListNode<T>, T>::value, "T must be a decendant of IntrusiveListNode<T>");
  }

  /// Deleted copy constructor
  IntrusiveListNode(const IntrusiveListNode&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveListNode& operator=(const IntrusiveListNode&) = delete;
  /// Deleted move constructor
  IntrusiveListNode(IntrusiveListNode&&) = delete;
  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveListNode& operator=(IntrusiveListNode&&) = delete;

  /// Destructor that removes the node from a list, if it is contained in one.
  virtual ~IntrusiveListNode() { EraseFromList(); }

  /**
   * \brief Erase the node from a list.
   *
   * Does nothing if the node is not part of a list.
   *
   * Note that this method is used internally by the list implementation. It may be called by users, but typically leads
   * to surprising semantics. It is recommended to use l.erase(it) instead.
   */
  void EraseFromList() noexcept {
    if (next_ != this && prev_ != this) {
      prev_->next_ = next_;
      next_->prev_ = prev_;
      next_ = this;
      prev_ = this;
    }
  }

  /**
   * \brief Get the contained element
   * \returns the contained element
   */
  T* GetSelf() {
    static_assert(std::is_base_of<IntrusiveListNode<T>, T>::value, "T must derive from IntrusiveListNode");
    return static_cast<T*>(this);
  }

  /**
   * \brief Get the contained element
   * \returns the contained element
   */
  const T* GetSelf() const {
    static_assert(std::is_base_of<IntrusiveListNode<T>, T>::value, "T must derive from IntrusiveListNode");
    return static_cast<const T*>(this);
  }

  /**
   * \brief Get the next element
   * \returns the next element
   */
  IntrusiveListNode* Next() { return next_; }

  /**
   * \brief Get the next element
   * \returns the next element
   */
  const IntrusiveListNode* Next() const { return next_; }

  /**
   * \brief Get the previous element
   * \returns the previous element
   */
  IntrusiveListNode* Prev() { return prev_; }

  /**
   * \brief Get the previous element
   * \returns the previous element
   */
  const IntrusiveListNode* Prev() const { return prev_; }

  /**
   * \brief Insert an element after the given element.
   * \param newElem the element to insert
   */
  void InsertAfter(IntrusiveListNode& newElem) noexcept {
    // Forward pointers
    newElem.next_ = this->next_;
    this->next_ = &newElem;

    // Reverse pointers
    newElem.next_->prev_ = &newElem;
    newElem.prev_ = this;
  }

 private:
  /// Pointer to the previous list element
  IntrusiveListNode* prev_;

  /// Pointer to the following list element
  IntrusiveListNode* next_;
};

/**
 * \brief Iterator for this IntrusiveList.
 */
template <typename T>
class IntrusiveListIterator final : public std::iterator<std::bidirectional_iterator_tag, T> {
 public:
  /// Typedef for the superclass
  using base_type = std::iterator<std::bidirectional_iterator_tag, T>;

  /**
   * \brief Constructor for an iterator
   * \param node The list from which to construct the iterator
   */
  explicit IntrusiveListIterator(IntrusiveListNode<T>* node) : node_(node) {}

  /// Copy constructor
  IntrusiveListIterator(const IntrusiveListIterator&) = default;
  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveListIterator& operator=(const IntrusiveListIterator&) = default;
  /// Move constructor
  IntrusiveListIterator(IntrusiveListIterator&&) = default;
  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveListIterator& operator=(IntrusiveListIterator&&) = default;

  ~IntrusiveListIterator() = default;

  /**
   * \brief Advance the iterator by one element.
   * \returns a reference to the current iterator
   */
  IntrusiveListIterator& operator++() {
    node_ = node_->Next();
    return *this;
  }

  /**
   * \brief Back the iterator up by one element.
   * \returns a reference to the current iterator
   */
  IntrusiveListIterator& operator--() {
    node_ = node_->Prev();
    return *this;
  }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  typename base_type::reference operator*() { return *node_->GetSelf(); }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  const typename base_type::reference operator*() const { return *node_->GetSelf(); }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  typename base_type::pointer operator->() { return node_->GetSelf(); }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  const typename base_type::pointer operator->() const { return node_->GetSelf(); }

  /**
   * \brief Get the list node as list node rather than as list element type.
   *
   * Works even for IntrusiveList::end().
   *
   * \returns The contained pointer to the list node
   */
  IntrusiveListNode<T>* GetListNode() { return node_; }

  /**
   * \brief Get the list node as list node rather than as list element type.
   *
   * Works even for IntrusiveList::end().
   *
   * \returns The contained pointer to the list node
   */
  const IntrusiveListNode<T>* GetListNode() const { return node_; }

  /**
   * \brief Compare two iterators for equality.
   *
   * \param other Iterator to compare to.
   *
   * \returns true if both iterators point to the same list node.
   */
  bool operator==(const IntrusiveListIterator& other) const { return node_ == other.node_; }

  /**
   * \brief Compare two iterators for inequality.
   *
   * \param other Iterator to compare to.
   *
   * \returns true if both iterators point to different list nodes.
   */
  bool operator!=(const IntrusiveListIterator& other) const { return node_ != other.node_; }

 private:
  /// Pointer to the list element.
  IntrusiveListNode<T>* node_;
};

/**
 * \brief Const Iterator for this IntrusiveList.
 */
template <typename T>
class ConstIntrusiveListIterator final
    : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&> {
 public:
  /**
   * Define types for constant iterator
   */
  using const_iter_types = std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>;

  /**
   * \brief Constructor for an iterator
   * \param node the list from which to create the iterator
   */
  explicit ConstIntrusiveListIterator(const IntrusiveListNode<T>* node) : node_(node) {}

  /// Copy constructor
  ConstIntrusiveListIterator(const ConstIntrusiveListIterator&) = default;
  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to object
   */
  ConstIntrusiveListIterator& operator=(const ConstIntrusiveListIterator&) = default;
  /// Move constructor
  ConstIntrusiveListIterator(ConstIntrusiveListIterator&&) = default;
  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to object
   */
  ConstIntrusiveListIterator& operator=(ConstIntrusiveListIterator&&) = default;

  ~ConstIntrusiveListIterator() = default;

  /**
   * \brief Advance the iterator by one element.
   * \returns a reference to the current iterator
   */
  ConstIntrusiveListIterator& operator++() {
    node_ = node_->Next();
    return *this;
  }

  /**
   * \brief Back the iterator up by one element.
   * \returns a reference to the current iterator
   */
  ConstIntrusiveListIterator& operator--() {
    node_ = node_->Prev();
    return *this;
  }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  typename const_iter_types::reference operator*() const { return *node_->GetSelf(); }

  /**
   * \brief Get the list node.
   *
   * \returns A reference to the list node pointed to by this iterator.
   */
  typename const_iter_types::pointer operator->() const { return node_->GetSelf(); }

  /**
   * \brief Get the list node as list node rather than as list element type.
   *
   * Works even for IntrusiveList::end().
   *
   * \returns The contained pointer to the list node
   */
  const IntrusiveListNode<T>* GetListNode() const { return node_; }

  /**
   * \brief Compare two iterators for equality.
   *
   * \param other Iterator to compare to.
   *
   * \returns true if both iterators point to the same list node.
   */
  bool operator==(const ConstIntrusiveListIterator& other) const { return node_ == other.node_; }

  /**
   * \brief Compare two iterators for inequality.
   *
   * \param other Iterator to compare to.
   *
   * \returns true if both iterators point to different list nodes.
   */
  bool operator!=(const ConstIntrusiveListIterator& other) const { return node_ != other.node_; }

 private:
  /// Pointer to the list element.
  const IntrusiveListNode<T>* node_;
};

/**
 * \brief Class to implement an IntrusiveList.
 *
 * List Nodes must inherit from IntrusiveListNode<T>.
 */
template <typename T>
class IntrusiveList final {
 public:
  /// Typedef for the iterator type of this list
  using iterator = IntrusiveListIterator<T>;

  /// Typedef for the iterator type of this list
  using const_iterator = ConstIntrusiveListIterator<T>;

  IntrusiveList() : list_() {}

  /// Deleted copy constructor
  IntrusiveList(const IntrusiveList&) = delete;
  /**
   * \brief Deleted copy assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveList& operator=(const IntrusiveList&) = delete;
  /// Deleted move constructor
  IntrusiveList(IntrusiveList&&) = delete;
  /**
   * \brief Deleted move assignment
   * \returns a reference to the assigned-to object
   */
  IntrusiveList& operator=(IntrusiveList&&) = delete;

  ~IntrusiveList() {
    // Release all elements from the list
    while (!empty()) {
      pop_front();
    }
  }

  /**
   * \brief Insert an element at the front of the list.
   * \param elem the new element to add in front
   */
  void push_front(T& elem) { list_.InsertAfter(elem); }

  /**
   * \brief Remove an element from the front of the list and return it.
   * \returns the first element from the list
   */
  IntrusiveListNode<T>* pop_front() {
    if (empty()) {
      return nullptr;
    } else {
      IntrusiveListNode<T>* removed_node = list_.Next();
      removed_node->EraseFromList();
      return removed_node;
    }
  }

  /**
   * \brief Returns a reference to the first element of the intrusive list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to an IntrusiveListNode.
   */
  IntrusiveListNode<T>& front() { return *list_.Next(); }

  /**
   * \brief Returns a reference to the first element of the intrusive list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to an IntrusiveListNode.
   */
  const IntrusiveListNode<T>& front() const { return *list_.Next(); }

  /**
   * \brief Insert an element at the back of the list.
   * \param elem the element to add in the back
   */
  void push_back(T& elem) { list_.Prev()->InsertAfter(elem); }

  /**
   * \brief Remove an element from the back of the list and return it.
   * \returns the last element from the list
   */
  IntrusiveListNode<T>* pop_back() {
    if (empty()) {
      return nullptr;
    } else {
      IntrusiveListNode<T>* removed_node = list_.Prev();
      removed_node->EraseFromList();
      return removed_node;
    }
  }

  /**
   * \brief Returns a reference to the last element of the intrusive list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to an IntrusiveListNode.
   */
  IntrusiveListNode<T>& back() { return *list_.Prev(); }

  /**
   * \brief Returns a reference to the last element of the intrusive list.
   *
   * Calling this function on an empty container causes undefined behavior.
   *
   * \return reference to an IntrusiveListNode.
   */
  const IntrusiveListNode<T>& back() const { return *list_.Prev(); }

  /**
   * \brief Remove an element pointed to by the iterator.
   *
   * \param elem Iterator pointing to the element to be removed from the list.
   *
   * \return an iterator to the next element in the list.
   */
  iterator erase(iterator elem) {
    iterator next_iterator(elem);
    ++next_iterator;
    elem.GetListNode()->EraseFromList();
    return next_iterator;
  }

  /**
   * \brief Remove an element given by-value.
   *
   * Complexity is O(1). The element is removed from whatever intrusive_list it is part of, even if it is not part of
   * this list.
   *
   * \param elem Value to be removed from the list.
   */
  void erase(T& elem) { elem.EraseFromList(); }

  /**
   * \brief Determine whether the list is currently empty.
   *
   * \return true if the list is empty. False if the list has at least one element.
   */
  bool empty() const { return list_.Next() == &list_; }

  /**
   * \brief Iterator to the start of the list
   * \returns the iterator at the start of the list
   */
  iterator begin() { return iterator(list_.Next()); }

  /**
   * \brief Past-The-End iterator of the list
   * \returns the iterator past-the-end
   */
  iterator end() { return iterator(&list_); }

  /**
   * \brief Const Iterator to the start of the list
   * \returns the constant iterator at the start of the list
   */
  const_iterator begin() const { return const_iterator(list_.Next()); }
  /**
   * \brief Const Past-The-End iterator of the list
   * \returns the constant iterator past-the-end
   */
  const_iterator end() const { return const_iterator(&list_); }

  /**
   * \brief Insert a new element into the list past the element pointed to by the IntrusiveListIterator.
   * \param where where to insert
   * \param what what to insert
   * \returns an iterator at the location where the new element was inserted
   */
  iterator insert(iterator where, IntrusiveListNode<T>& what) {
    where.GetListNode()->InsertAfter(what);
    return iterator(&what);
  }

 private:
  /**
   * \brief Pointers to the list.
   *
   * next_ points to the head (beginning of the list), prev_ points to the tail (end of the list).
   */
  IntrusiveListNode<T> list_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_INTRUSIVE_LIST_H_
