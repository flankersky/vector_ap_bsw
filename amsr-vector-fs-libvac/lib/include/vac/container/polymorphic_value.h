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
/**        \file  polymorphic_value.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Matthias Heil                 vismhe        Vector Informatik GmbH
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_
#define LIB_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <type_traits>
#include <utility>
#include "vac/container/variant.h"
#include "vac/language/cpp17_backport.h"

namespace vac {
namespace container {

/**
 * \brief a polymorphic object with value semantics
 * \details a polymorhic object that supports copying without slicing
 */
template <class TBase, class... TDerivedArgs>
class PolymorphicValue final {
  // Assertions
  static_assert(language::compile_time::all<std::is_base_of<TBase, TDerivedArgs>::value...>::value,
                "PolymorphicValues must derive from a common base");

  /** \brief base type */
  using base_type = TBase;
  /** \brief base type pointer*/
  using base_pointer_type = typename std::add_pointer<TBase>::type;
  /** \brief base type pointer*/
  using const_base_pointer_type = typename std::add_pointer<const TBase>::type;
  /** \brief base type reference*/
  using base_reference_type = typename std::add_lvalue_reference<TBase>::type;
  /** \brief base type reference*/
  using const_base_reference_type = typename std::add_lvalue_reference<const TBase>::type;
  /** \brief type used to store the object. base_pointer_type is stored as variant to store nullptr */
  using storage_type = variant<base_pointer_type, TDerivedArgs...>;

 public:
  /**
   * \brief default construction
   * \details the default constructor exploits that default construction of a variant is supported (only) for its first
   * specialization, in our case this is always the base _pointer_ type - which is initialized to nullptr
   */
  PolymorphicValue() noexcept(std::is_nothrow_constructible<storage_type, base_pointer_type>::value)
      : base_ptr_(nullptr), storage_(nullptr) {}

  /**
   * \brief construction from value
   * \param value the value from which to construct
   */
  template <class T, typename = typename std::enable_if<
                         !std::is_same<typename std::decay<T>::type, PolymorphicValue>::value>::type>
  explicit PolymorphicValue(T&& value) noexcept(std::is_nothrow_assignable<storage_type, T>::value)
      : base_ptr_(nullptr), storage_(std::forward<T>(value)) {
    base_ptr_ = storage_.template get_if<T>();
  }

  /**
   * \brief copy constructor
   * \param other the variant from which to construct
   */
  PolymorphicValue(PolymorphicValue const& other) noexcept(std::is_nothrow_copy_assignable<storage_type>::value)
      : base_ptr_(nullptr), storage_(other.storage_) {
    fix_base_ptr();
  }

  /**
   * \brief move constructor
   * \param other the variant from which to construct
   */
  PolymorphicValue(PolymorphicValue&& other) noexcept(std::is_nothrow_move_constructible<storage_type>::value)
      : base_ptr_(nullptr), storage_(std::move(other.storage_)) {
    fix_base_ptr();
  }

  /**
   * \brief destructor
   * \details will call destructor of contained value
   */
  ~PolymorphicValue() { base_ptr_ = nullptr; }

  /**
   * \brief copy assignment
   * \param other The value that is to be copied
   * \returns a reference on the copied-to variant
   */
  PolymorphicValue& operator=(PolymorphicValue const& other) noexcept(
      std::is_nothrow_copy_assignable<storage_type>::value) {
    base_ptr_ = nullptr;
    this->storage_ = other.storage_;
    fix_base_ptr();
    return *this;
  }

  /**
   * \brief store an object into the PolymorphicValue by copy
   * \param value The value that is to be set
   * \returns a reference on the copied-to PolymorphicValue
   */
  template <class T, typename = typename std::enable_if<
                         !std::is_same<typename std::decay<T>::type, PolymorphicValue>::value>::type>
  PolymorphicValue& operator=(const T& value) {
    base_ptr_ = nullptr;
    storage_.template emplace<T>(value);
    base_ptr_ = storage_.template get_if<T>();
    return *this;
  }

  /**
   * \brief move assignment
   * \param other The value that is to be moved
   * \returns a reference on the moved-to variant
   */
  PolymorphicValue& operator=(PolymorphicValue&& other) noexcept(
      vac::language::is_nothrow_swappable<storage_type>::value) {
    base_ptr_ = nullptr;
    std::swap(storage_, other.storage_);
    fix_base_ptr();
    other.fix_base_ptr();
    return *this;
  }

  /**
   * \brief move an object into the PolymorphicValue
   * \param value The value that is to be set
   * \returns a reference on the copied-to PolymorphicValue
   */
  template <class T, typename = typename std::enable_if<
                         !std::is_same<typename std::decay<T>::type, PolymorphicValue>::value>::type>
  PolymorphicValue& operator=(T&& value) {
    base_ptr_ = nullptr;
    storage_.template emplace<T>(std::forward<T>(value));
    base_ptr_ = storage_.template get_if<T>();
    return *this;
  }

  /**
   * \brief emplacement by forwarding
   * \param args the arguments to pass to a constructor
   * \return reference to the constructed element
   */
  template <class T, class... TArgs>
  T& emplace(TArgs&&... args) {
    base_ptr_ = nullptr;
    storage_.template emplace<T>(std::forward<TArgs>(args)...);
    base_ptr_ = storage_.template get_if<T>();
    return *storage_.template get_if<T>();
  }

  /**
   * \brief operator*
   * \return base class reference to the stored object
   */
  base_reference_type operator*() {
    if (empty()) {
      // throw bad_variant_access to be consistent with get<T> while T is not currently stored
      throw bad_variant_access{};
    }
    return *base_ptr_;
  }

  /**
   * \brief operator* const
   * \return base class reference to the stored object
   */
  const_base_reference_type operator*() const { return const_cast<PolymorphicValue*>(this)->operator*(); }

  /**
   * \brief operator ->
   * \return base class pointer to the stored object
   */
  base_pointer_type operator->() noexcept { return base_ptr_; }

  /**
   * \brief operator -> const
   * \return base class pointer to the stored object
   */
  const_base_pointer_type operator->() const noexcept { return base_ptr_; }

  /**
   * \brief get the base class reference
   * \return base class reference to the stored object
   */
  base_reference_type get() { return this->operator*(); }

  /**
   * \brief get the base class reference, const version
   * \return base class reference to the stored object
   */
  const_base_reference_type get() const { return this->operator*(); }

  /**
   * \brief get a derived class reference
   * \return reference to the stored object. will throw if the stored object is not of the requested type
   */
  template <class T>
  typename std::add_lvalue_reference<T>::type get(
      typename std::enable_if<not std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) {
    static_assert(language::compile_time::any<std::is_same<T, TDerivedArgs>::value...>::value,
                  "Must use a derived type");
    return container::get<T>(storage_);
  }

  /**
   * \brief get a derived class reference, const version
   * \return reference to the stored object. will throw if the stored object is not of the requested type
   */
  template <class T>
  typename std::add_lvalue_reference<const T>::type get(
      typename std::enable_if<not std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) const {
    static_assert(language::compile_time::any<std::is_same<T, TDerivedArgs>::value...>::value,
                  "Must use a derived type");
    return container::get<T>(storage_);
  }

  /**
   * \brief get the base class reference
   * \return base class reference to the stored object
   */
  template <class T>
  typename std::add_lvalue_reference<T>::type get(
      typename std::enable_if<std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) {
    return **this;
  }

  /**
   * \brief get the base class reference, const version
   * \return base class reference to the stored object
   */
  template <class T>
  typename std::add_lvalue_reference<const T>::type get(
      typename std::enable_if<std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) const {
    return **this;
  }

  /**
   * \brief get the base class reference
   * \return base class reference to the stored object, or nullptr if no object is stored
   */
  base_pointer_type get_if() { return base_ptr_; }

  /**
   * \brief get the base class reference, const version
   * \return base class reference to the stored object, or nullptr if no object is stored
   */
  const_base_pointer_type get_if() const { return base_ptr_; }

  /**
   * \brief get a non-owning base class pointer
   * \return base class reference to the stored object, or nullptr if no object is stored
   */
  template <class T>
  base_pointer_type get_if(
      typename std::enable_if<std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) {
    return base_ptr_;
  }

  /**
   * \brief get a non-owning base class pointer, const version
   * \return base class reference to the stored object, or nullptr if no object is stored
   */
  template <class T>
  const_base_pointer_type get_if(
      typename std::enable_if<std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) const {
    return base_ptr_;
  }

  /**
   * \brief get a non-owning pointer to the requested derived class
   * \return pointer to the stored class, or nullptr if no or a different derived class is stored
   */
  template <class T>
  typename std::add_pointer<T>::type get_if(
      typename std::enable_if<not std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) {
    static_assert(language::compile_time::any<std::is_same<T, TDerivedArgs>::value...>::value,
                  "Must use a derived type");
    return storage_.template get_if<T>();
  }

  /**
   * \brief get a non-owning pointer to the requested derived class, const version
   * \return pointer to the stored class, or nullptr if no or a different derived class is stored
   */
  template <class T>
  typename std::add_pointer<const T>::type get_if(
      typename std::enable_if<not std::is_same<typename std::decay<T>::type, TBase>::value>::type* = nullptr) const {
    static_assert(language::compile_time::any<std::is_same<T, TDerivedArgs>::value...>::value,
                  "Must use a derived type");
    return storage_.template get_if<T>();
  }

  /**
   * \brief test whether a conrete derived object is stored
   * \return true the stored object is of the given type
   * \return false the stored object is not of the given type
   */
  template <class T>
  bool holds_alternative() const noexcept {
    return container::holds_alternative<T>(storage_);
  }

  /**
   * \brief test if any object is stored
   * \return true there is a stored object
   * \return false no object is stored
   */
  bool empty() const noexcept { return base_ptr_ == nullptr; }

  /**
   * \brief clear the polyobject
   */
  void reset() {
    base_ptr_ = nullptr;
    storage_ = storage_type(nullptr);
  }

 private:
  /**
   * \brief set up base type pointer
   */
  class get_base_ptr_op final {
   public:
    get_base_ptr_op() = default;
    ~get_base_ptr_op() = default;
    get_base_ptr_op(const get_base_ptr_op&) = delete;
    get_base_ptr_op(get_base_ptr_op&&) = delete;
    get_base_ptr_op& operator=(const get_base_ptr_op&) = delete;
    get_base_ptr_op& operator=(get_base_ptr_op&&) = delete;

    template <class Type>
    base_pointer_type operator()(Type& variant) {
      return &variant;
    }
    base_pointer_type operator()(base_pointer_type variant) { return variant; }
  };

  /**
   * \brief set up base type pointer
   */
  void fix_base_ptr() { base_ptr_ = vac::container::visit(get_base_ptr_op{}, storage_); }

 private:
  base_pointer_type base_ptr_{nullptr};
  storage_type storage_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_POLYMORPHIC_VALUE_H_
