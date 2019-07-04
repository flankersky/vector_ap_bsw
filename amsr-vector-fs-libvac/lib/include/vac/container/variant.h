/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  variant.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_VARIANT_H_
#define LIB_INCLUDE_VAC_CONTAINER_VARIANT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <limits>
#include <tuple>
#include <utility>

#include "vac/language/compile_time.h"

namespace vac {
namespace container {

/// Forward declaration for variant
template <typename... Xs>
class variant;

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class T>
struct variant_alternative;

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class T>
struct variant_alternative<I, T const> : std::add_const<typename variant_alternative<I, T>::type> {};

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class T>
struct variant_alternative<I, T volatile> : std::add_volatile<typename variant_alternative<I, T>::type> {};

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class T>
struct variant_alternative<I, T const volatile> : std::add_cv<typename variant_alternative<I, T>::type> {};

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class... Types>
struct variant_alternative<I, variant<Types...>> {
  /// \brief type = Types[I]
  using type = language::compile_time::get_from_index<I, Types...>;
};

/// \brief Provides compile-time index access to the variant types
template <std::size_t I, class T>
using variant_alternative_t = typename variant_alternative<I, T>::type;

/// Forward declaration for holds alternative
template <class T, class... Types>
bool holds_alternative(const variant<Types...>& v) noexcept;

/// Forward declaration for get
template <std::size_t N, class... Types>
variant_alternative_t<N, variant<Types...>>& get(variant<Types...>& v);
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>>&& get(variant<Types...>&& v);
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>> const& get(variant<Types...> const& v);
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>> const&& get(variant<Types...> const&& v);

/**
 * \brief exception type that will be thrown if any invalid access will be made
 * \details see C++17 specification
 */
class bad_variant_access final : public std::exception {
 public:
  /// \brief Default constructor
  bad_variant_access() noexcept {}
  /**
   * \brief Needed method to display error message
   * \returns a message for debugging purposes
   */
  char const* what() const noexcept override { return message; }

 private:
  constexpr static char const* message = "Bad visit / get access";
};

/// \brief Is a marker for an invalid element in variant
constexpr std::size_t variant_npos = std::numeric_limits<std::size_t>::max();

namespace detail {

/// \brief Implementation of comparisons for variants
template <typename... Types>
class VariantCompare {
  /// \brief type of the variants that are compared
  using variant_t = variant<Types...> const&;

 public:
  /**
   * \brief the constructor
   * \param v1 the first variant to compare
   * \param v2 the second variant to compare
   */
  VariantCompare(variant_t v1, variant_t v2) : v1_{v1}, v2_{v2} {}

  /**
   * \brief function that checks for equality of 2 variants
   * \returns true if both variants are the same
   */
  bool equal() && {
    if (v1_.index() != v2_.index()) {
      return false;
    } else if (v1_.valueless_by_exception()) {
      return true;
    } else {
      return compare_internal<std::equal_to, Types...>();
    }
  }

  /**
   * \brief function that checks if one variant is smaller than the other
   * \returns true if v1 is smaller than v2
   */
  bool is_smaller() && {
    if (v2_.valueless_by_exception()) {
      return false;
    } else if (v1_.valueless_by_exception()) {
      return true;
    } else if (v1_.index() < v2_.index()) {
      return true;
    } else if (v1_.index() > v2_.index()) {
      return false;
    } else {
      return compare_internal<std::less, Types...>();
    }
  }

 private:
  /**
   * \brief generic compare function that takes an operator like std::less
   * \tparam Comp The function that compares both items
   * \returns true if the comparison function returns true
   * \precondition both variants are valid and have values of the same type
   */
  template <template <typename> class Comp, typename Cand>
  bool compare_internal() {
    return Comp<Cand>{}(*v1_.template get_if<Cand>(), *v2_.template get_if<Cand>());
  }

  /**
   * \brief generic compare function that takes an operator like std::less
   * \tparam Comp The function that compares both items
   * \returns true if the comparison function returns true for the correct alternative
   * \precondition both variants are valid and have values of the same type
   */
  template <template <typename> class Comp, typename Cand, typename Cand2, typename... Cands>
  bool compare_internal() {
    if (holds_alternative<Cand>(v1_)) {
      return compare_internal<Comp, Cand>();
    } else {
      return compare_internal<Comp, Cand2, Cands...>();
    }
  }

  /// \brief the first variant to compare
  variant_t v1_;
  /// \brief the second variant to compare
  variant_t v2_;
};

/**
 * \brief internal helper for visiting variant items
 * \details Specialization for invalid instructions
 * \returns nothing
 */
template <typename R, typename F, typename V>
inline R TryVisitVariant(F&&, V&) {
  throw bad_variant_access{};
}

/**
 * \brief internal helper for visiting variant items
 * \details Specialization that will dispatch visiting the correct non-pointer type
 * \param visitor the visitor accepting the stored value
 * \param v the visited variant
 * \returns result of the functor call
 */
template <typename R, typename F, typename V, typename X, typename... Xs>
inline R TryVisitVariant(F&& visitor, V& v) {
  typename std::add_pointer<X>::type maybe_pointer{v.template get_if<X>()};
  if (maybe_pointer != nullptr) {
    return visitor.operator()(*maybe_pointer);
  } else {
    return TryVisitVariant<R, F, V, Xs...>(std::forward<F>(visitor), v);
  }
}

/**
 * \brief helper for visiting variant items
 * \param visitor the visitor accepting the stored value
 * \param v the visited variant
 * \returns result of the functor call
 */
template <typename F, typename... Xs>
inline decltype(std::declval<F>().operator()(get<0>(std::declval<variant<Xs...>>()))) VisitVariant(F&& visitor,
                                                                                                   variant<Xs...>& v) {
  using R = decltype(visitor.operator()(get<0>(v)));
  return TryVisitVariant<R, F, variant<Xs...>, Xs...>(std::forward<F>(visitor), v);
}
}  // namespace detail

/**
 * \brief a type-safe union as specified in C++17
 * \details See 'tagged-union' or 'discriminated union'
 */
template <typename... Xs>
class variant final {
  /// \brief The number of variants that may be contained in the variant
  static constexpr std::size_t num_variants = sizeof...(Xs);
  /// \brief max size of containable types
  constexpr static std::size_t storage_size = language::compile_time::get_max_sizeof<Xs...>();
  /// \brief max alignment of containable types
  constexpr static std::size_t storage_alignment = language::compile_time::get_max_alignof<Xs...>();

  /// Unified storage type
  using Storage = typename std::aligned_storage<storage_size, storage_alignment>::type;

  // Assertions
  static_assert(1 < num_variants, "Variant needs at least two variants");
  static_assert(language::compile_time::not_any<std::is_void<Xs>::value...>::value,
                "Variant cannot store void type (use monotype)");
  static_assert(language::compile_time::not_any<std::is_array<Xs>::value...>::value, "Variant cannot store C Arrays");
  static_assert(language::compile_time::not_any<std::is_reference<Xs>::value...>::value,
                "Variant can only contain values (use std::ref)");
  static_assert(!language::compile_time::has_duplicate<Xs...>::value, "There may not be any duplicate types!");

  /**
   * \brief Collection of traits useful for variant
   */
  template <typename X>
  struct VariantTrait {
    /// \brief value type that can be returned
    using decayed = typename std::decay<X>::type;
    /// \brief the index of the underlying type
    static constexpr std::size_t const index = language::compile_time::get_index<decayed, Xs...>();

    /// \brief checks if variants default constructor may throw
    static constexpr bool is_nothrow_default_constructible = std::is_nothrow_default_constructible<X>::value;
    /// \brief checks if variant may have a default constructor
    static constexpr bool is_default_constructible = std::is_default_constructible<X>::value;
    /// \brief checks if type X is the same as the type of variant<Xs...>
    static constexpr bool is_same_variant = std::is_same<typename std::decay<X>::type, variant>::value;
  };

 public:
  /// \brief Default constructor that is only usable if the first specified type is default constructible
  template <typename Default = variant_alternative_t<0, variant>, typename Trait = VariantTrait<Default>,
            typename = typename std::enable_if<Trait::is_default_constructible>::type>
  variant() noexcept(Trait::is_nothrow_default_constructible) : variant_index_{Trait::index} {
    new (&storage_) Default();
  }

  /**
   * \brief Constructor from universal reference type
   * \param value the value that is stored inside the variant
   */
  template <typename X, typename Trait = VariantTrait<X>,
            typename = typename std::enable_if<not Trait::is_same_variant>::type>
  explicit variant(X&& value) : variant_index_{Trait::index} {
    new (&storage_) typename Trait::decayed(std::forward<X>(value));
  }

  /**
   * \brief Copy constructor
   * \param other the variant from which to construct
   */
  variant(variant const& other) : variant_index_{other.variant_index_} {
    copy_storage<Xs...>(other.variant_index_, other.storage_);
  }

  /**
   * \brief Move constructor
   * \param other the variant from which to construct
   */
  variant(variant&& other) : variant_index_{other.variant_index_} {
    if (!other.valueless_by_exception()) {
      move_storage<Xs...>(other.variant_index_, std::move(other.storage_));
    }
  }

  /**
   * \brief Destructor
   * \details will call destructor of contained value
   */
  ~variant() {
    if (is_valid()) {
      destroy_storage<Xs...>(variant_index_);
      variant_index_ = variant_npos;
    }
  }

  /**
   * \brief copy assignment
   * \param other The value that is to be copied
   * \returns a reference on the copied-to variant
   */
  variant& operator=(variant const& other) {
    if (this != &other) {
      prepare_assignment(other.variant_index_);
      copy_storage<Xs...>(other.variant_index_, other.storage_);
    }
    return *this;
  }

  /**
   * \brief move assignment
   * \param other The value that is to be moved
   * \returns a reference on the moved-to variant
   */
  variant& operator=(variant&& other) {
    if (this != &other) {
      prepare_assignment(other.variant_index_);
      move_storage<Xs...>(other.variant_index_, std::move(other.storage_));
    }
    return *this;
  }

  /**
   * \brief assign value in place
   * \param args The value that is to be moved
   * \returns a reference on the moved-to variant
   */
  template <class T, class... Args, typename = typename std::is_constructible<T, Args...>::type>
  T& emplace(Args&&... args) {
    prepare_assignment(variant_npos);
    new (&storage_) typename VariantTrait<T>::decayed(std::forward<Args>(args)...);
    variant_index_ = VariantTrait<T>::index;
    return *get_unsafe<T>(storage_);
  }

  /**
   * \brief assign value in place
   * \param args The value that is to be moved
   * \returns a reference on the moved-to variant
   */
  template <size_t I, class... Args, typename = typename std::is_constructible<variant_alternative_t<I, variant>>::type>
  variant_alternative_t<I, variant>& emplace(Args&&... args) {
    return emplace<variant_alternative_t<I, variant>>(std::forward<Args>(args)...);
  }

  /**
   * \brief get index of the contained alternative
   * \details If the variant is not valid, returns the invalid index.
   * \return the zero-based index of the alternative that is currently held by the variant.
   */
  std::size_t index() const noexcept { return variant_index_; }

  /**
   * \brief Checks if the variant is invalid
   * \return false if and only if the variant holds a value.
   */
  bool valueless_by_exception() const noexcept { return !is_valid(); }

  /**
   * \brief gets the stored value cast to the specified type without any checks
   * \returns a pointer to the stored value
   */
  template <typename T>
  typename std::add_pointer<T>::type get_if() noexcept {
    if (is_valid() && holds_alternative<T>(*this)) {
      return get_unsafe<T>(storage_);
    } else {
      return nullptr;
    }
  }

  /**
   * \brief gets the stored value cast to the specified type without any checks
   * \returns a pointer to the stored value
   */
  template <typename T>
  typename std::add_pointer<T const>::type get_if() const noexcept {
    if (is_valid() && holds_alternative<T>(*this)) {
      return get_unsafe<T>(storage_);
    } else {
      return nullptr;
    }
  }

 private:
  /**
   * \brief gets the stored value cast to the specified type without any checks
   * \param storage the storage from which to get the value from
   * \returns a pointer to the stored value
   */
  template <typename T>
  static typename std::add_pointer<T>::type get_unsafe(Storage& storage) noexcept {
    static_assert(!std::is_same<T, void>::value, "T should not be void");
    static_assert(language::compile_time::any_in<typename std::decay<T>::type, Xs...>(),
                  "T is not contained in this variant!");
    return reinterpret_cast<typename std::add_pointer<T>::type>(&storage);
  }

  /**
   * \brief gets the stored value cast to the specified type without any checks
   * \param storage the storage from which to get the value from
   * \returns a pointer to the stored value
   */
  template <typename T>
  static typename std::add_pointer<T const>::type get_unsafe(Storage const& storage) noexcept {
    static_assert(!std::is_same<T, void>::value, "T should not be void");
    static_assert(language::compile_time::any_in<typename std::decay<T>::type, Xs...>(),
                  "T is not contained in this variant!");
    return reinterpret_cast<typename std::add_pointer<T const>::type>(&storage);
  }

  /**
   * \brief A variant that is valueless by exception is treated as being in an invalid state
   * \return true if and only if the variant holds a value.
   */
  bool is_valid() const noexcept { return variant_index_ != variant_npos; }

  /**
   * \brief Prepares for copy/move assignment
   * \param new_index the index of the new stored type
   */
  void prepare_assignment(std::size_t new_index) {
    if (is_valid()) {
      destroy_storage<Xs...>(variant_index_);
    }
    variant_index_ = new_index;
  }

  /**
   * \brief move the stored value from that to this
   * \param storage the data storage from which to move
   */
  template <typename Hd>
  void move_storage(std::size_t, Storage&& storage) {
    new (&storage_) Hd(std::move(*get_unsafe<Hd>(storage)));
  }

  /**
   * \brief move the stored value from that to this
   * \param offset the type offset
   * \param storage the data storage from which to move
   */
  template <typename Hd, typename Tl1, typename... Tl>
  void move_storage(std::size_t offset, Storage&& storage) {
    if (offset == 0) {
      move_storage<Hd>(0, std::move(storage));
    } else {
      move_storage<Tl1, Tl...>(--offset, std::move(storage));
    }
  }

  /**
   * \brief copy the stored value from that to this
   * \param storage the data storage from which to copy
   */
  template <typename Hd>
  void copy_storage(std::size_t, Storage const& storage) {
    new (&storage_) Hd(*get_unsafe<Hd>(storage));
  }

  /**
   * \brief copy the stored value from that to this
   * \param offset the type offset
   * \param storage the data storage from which to copy
   */
  template <typename Hd, typename Tl1, typename... Tl>
  void copy_storage(std::size_t offset, Storage const& storage) {
    if (offset == 0) {
      copy_storage<Hd>(0, storage);
    } else {
      copy_storage<Tl1, Tl...>(--offset, storage);
    }
  }

  /**
   * \brief Destroys the stored value
   */
  template <typename Hd>
  void destroy_storage(std::size_t) {
    get_unsafe<Hd>(storage_)->~Hd();
  }

  /**
   * \brief Destroys the stored value
   * \param offset the type offset
   */
  template <typename Hd, typename Tl1, typename... Tl>
  void destroy_storage(std::size_t offset) {
    if (offset == 0) {
      destroy_storage<Hd>(0);
    } else {
      destroy_storage<Tl1, Tl...>(--offset);
    }
  }

  /// \brief the actual storage
  Storage storage_;
  /// \brief type-index in which the item is contained
  std::size_t variant_index_;
};

/**
 * \brief Checks if the contained value is of the passed type
 * \param v the variant that shall be tested
 * \returns if the variant contains a value of the passed type
 */
template <class T, class... Types>
bool holds_alternative(variant<Types...> const& v) noexcept {
  return language::compile_time::get_index<T, Types...>() == v.index();
}

/**
 * \brief Get variant value from index as pointer
 * \param v the variant which value shall be returned
 * \returns the value that is contained or nullptr
 */
template <std::size_t N, class... Types>
variant_alternative_t<N, variant<Types...>>* get_if(variant<Types...>* v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get_if<language::compile_time::get_from_index<N, Types...>, Types...>(v);
}

/**
 * \brief Get variant value from index as pointer
 * \param v the variant which value shall be returned
 * \returns the value that is contained or nullptr
 */
template <std::size_t N, class... Types>
variant_alternative_t<N, variant<Types...>> const* get_if(variant<Types...> const* v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get_if<language::compile_time::get_from_index<N, Types...>, Types...>(v);
}

/**
 * \brief Get variant value from type as pointer
 * \param v the variant which value shall be returned
 * \returns the value that is contained or nullptr
 */
template <typename T, typename... Types>
typename std::add_pointer<T>::type get_if(variant<Types...>* v) {
  if (v != nullptr) {
    return v->template get_if<T>();
  } else {
    return nullptr;
  }
}

/**
 * \brief Get variant value from type as pointer
 * \param v the variant which value shall be returned
 * \returns the value that is contained or nullptr
 */
template <typename T, typename... Types>
typename std::add_pointer<T const>::type get_if(variant<Types...> const* v) {
  if (v != nullptr) {
    return v->template get_if<T>();
  } else {
    return nullptr;
  }
}

/**
 * \brief Get variant value from index
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <std::size_t N, class... Types>
variant_alternative_t<N, variant<Types...>>& get(variant<Types...>& v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get<language::compile_time::get_from_index<N, Types...>, Types...>(v);
}

/**
 * \brief Get variant value from index
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>>&& get(variant<Types...>&& v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get<language::compile_time::get_from_index<N, Types...>, Types...>(std::move(v));
}

/**
 * \brief Get variant value from index
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>> const& get(variant<Types...> const& v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get<language::compile_time::get_from_index<N, Types...>, Types...>(v);
}

/**
 * \brief Get variant value from index
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <std::size_t N, typename... Types>
variant_alternative_t<N, variant<Types...>> const&& get(variant<Types...> const&& v) {
  static_assert(N < sizeof...(Types), "Out of bounds access!");
  return get<language::compile_time::get_from_index<N, Types...>, Types...>(std::move(v));
}

/**
 * \brief Get variant value from type
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <class T, class... Types>
T& get(variant<Types...>& v) {
  auto result = get_if<T>(&v);

  if (result == nullptr) {
    throw bad_variant_access{};
  } else {
    return *result;
  }
}

/**
 * \brief Get variant value from type
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <typename T, typename... Types>
T&& get(variant<Types...>&& v) {
  auto result = get_if<T>(&v);

  if (result == nullptr) {
    throw bad_variant_access{};
  } else {
    return std::move(*result);
  }
}

/**
 * \brief Get variant value from type
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <typename T, typename... Types>
T const& get(variant<Types...> const& v) {
  auto result = get_if<T>(&v);

  if (result == nullptr) {
    throw bad_variant_access{};
  } else {
    return *result;
  }
}

/**
 * \brief Get variant value from type
 * \param v the variant which value shall be returned
 * \returns the value that is contained
 */
template <typename T, typename... Types>
T const&& get(variant<Types...> const&& v) {
  auto result = get_if<T>(&v);

  if (result == nullptr) {
    throw bad_variant_access{};
  } else {
    return std::move(*result);
  }
}

/**
 * \brief the equality operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if both variants are the same
 */
template <typename... Types>
bool operator==(variant<Types...> const& v1, variant<Types...> const& v2) {
  return detail::VariantCompare<Types...>{v1, v2}.equal();
}

/**
 * \brief the inequality operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if both variants are not the same
 */
template <typename... Types>
bool operator!=(variant<Types...> const& v1, variant<Types...> const& v2) {
  return !(v1 == v2);
}

/**
 * \brief the less than operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if the first variant is smaller
 */
template <typename... Types>
bool operator<(variant<Types...> const& v1, variant<Types...> const& v2) {
  return detail::VariantCompare<Types...>{v1, v2}.is_smaller();
}

/**
 * \brief the greater than operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if the first variant is greater
 */
template <typename... Types>
bool operator>(variant<Types...> const& v1, variant<Types...> const& v2) {
  return v2 < v1;
}

/**
 * \brief the less or equal than operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if the first variant is smaller or equal
 */
template <typename... Types>
bool operator<=(variant<Types...> const& v1, variant<Types...> const& v2) {
  return !(v1 > v2);
}

/**
 * \brief the greater or equal than operator
 * \param v1 the first variant for comparison
 * \param v2 the second variant for comparison
 * \returns true if the first variant is greater or equal
 */
template <typename... Types>
bool operator>=(variant<Types...> const& v1, variant<Types...> const& v2) {
  return v2 <= v1;
}

/**
 * \brief visit variant with visitor accepting all variant items
 * \param visitor the visitor, a functor accepting all variants
 * \param v the variant to visit
 * \return the result returned by the functor
 */
template <typename Visitor, typename... Types>
decltype(std::declval<Visitor>().operator()(get<0>(std::declval<variant<Types...>>()))) visit(Visitor&& visitor,
                                                                                              variant<Types...>& v) {
  return detail::VisitVariant(std::forward<Visitor>(visitor), v);
}

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_VARIANT_H_
