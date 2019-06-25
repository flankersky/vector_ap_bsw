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
/**        \file  compile_time.h
 *        \brief  Compile time programming complementing stds type_traits
 *
 *      \details  Provides structs/constexpr functions for compile time programming
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_
#define LIB_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <exception>
#include <limits>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vac/language/cpp14_backport.h"

namespace vac {
namespace language {
namespace compile_time {

/// \brief negates the value in X
template <class X>
using negate_t = std::integral_constant<bool, !X::value>;

template <bool...>
struct bool_sequence {};

/// \brief Is true if all values passed evaluate to true
template <bool... Xs>
using all = std::is_same<bool_sequence<true, Xs...>, bool_sequence<Xs..., true>>;

/// \brief Is true if not any values passed evaluate to true
template <bool... Xs>
using not_any = std::is_same<bool_sequence<false, Xs...>, bool_sequence<Xs..., false>>;

/// \brief Is true if any values passed evaluate to true
template <bool... Xs>
using any = negate_t<not_any<Xs...>>;

/// \brief Is true if not all values passed evaluate to true
template <bool... Xs>
using not_all = negate_t<all<Xs...>>;

/**
 * \brief Is true if the passed value is contained in the passed types
 * \returns true if both types are the same
 */
template <typename T, typename X>
constexpr inline auto any_in() -> bool {
  return std::is_same<T, X>::value;
}
/**
 * \brief Is true if the passed value is contained in the passed types
 * \returns true if the predicate type is in the list of types
 */
template <typename T, typename X, typename... Xs, typename = typename std::enable_if<(sizeof...(Xs) > 0)>::type>
constexpr inline auto any_in() -> bool {
  return any_in<T, X>() || any_in<T, Xs...>();
}

/**
 * \brief static cast for references
 * \param ptr the void pointer that is going to be cast to the passed type
 * \returns the cast value
 */
template <typename X>
inline auto ref_cast(void *ptr) -> X {
  return static_cast<X>(*static_cast<typename std::remove_reference<X *>::type>(ptr));
}

/**
 * \brief Get Maximum sizeof for types
 * \returns the maximum sizeof for the passed types
 */
template <std::size_t N>
constexpr inline auto get_max_sizeof_impl() -> std::size_t {
  return N;
}
/**
 * \brief Get Maximum sizeof for types
 * \returns the maximum sizeof for the passed types
 */
template <std::size_t N, typename X, typename... Xs>
constexpr inline auto get_max_sizeof_impl() -> std::size_t {
  return get_max_sizeof_impl<language::max(N, sizeof(X)), Xs...>();
}
/**
 * \brief Get Maximum sizeof for types
 * \returns the maximum sizeof for the passed types
 */
template <typename... Xs>
constexpr inline auto get_max_sizeof() -> std::size_t {
  return get_max_sizeof_impl<0, Xs...>();
}

/**
 * \brief Get Maximum alignment for types
 * \returns the maximum alignment for the passed types
 */
template <std::size_t N>
constexpr inline auto get_max_alignof_impl() -> std::size_t {
  return N;
}
/**
 * \brief Get Maximum alignment for types
 * \returns the maximum alignment for the passed types
 */
template <std::size_t N, typename X, typename... Xs>
constexpr inline auto get_max_alignof_impl() -> std::size_t {
  return get_max_alignof_impl<language::max(N, alignof(X)), Xs...>();
}
/**
 * \brief Get Maximum alignment for types
 * \returns the maximum alignment for the passed types
 */
template <typename... Xs>
constexpr inline auto get_max_alignof() -> std::size_t {
  return get_max_alignof_impl<0, Xs...>();
}

/// \brief Tailrecursive implementation of index_from_Type
template <std::size_t N, typename T, typename... Xs>
struct index_from_type_tailrec;
/**
 * \brief Tailrecursive implementation of index_from_Type
 * \details Endpoint: If the head of the list is of the wanted type, then return N
 */
template <std::size_t N, typename T, typename... Xs>
struct index_from_type_tailrec<N, T, T, Xs...> : std::integral_constant<std::size_t, N> {};
/**
 * \brief Tailrecursive implementation of index_from_Type
 * \details Recursion: The head of the list is NOT of the wanted type, so increment N and recursive with the lists tail
 */
template <std::size_t N, typename T, typename X, typename... Xs>
struct index_from_type_tailrec<N, T, X, Xs...>
    : std::integral_constant<std::size_t, index_from_type_tailrec<N + 1, T, Xs...>::value> {};
/// \brief Calculate the index for T in the list of Xs
template <typename T, typename... Xs>
struct index_from_type : std::integral_constant<std::size_t, index_from_type_tailrec<0, T, Xs...>::value> {};

/**
 * \brief Gets the index of type X in the list of passed types
 * \details endpoint of recursion if it is not contained (aka '-1' in size_t)
 * \returns the "invalid" value.
 */
template <typename X>
constexpr inline auto get_index(std::size_t = 0) -> std::size_t {
  return std::numeric_limits<std::size_t>::max();
}

/**
 * \brief Gets the index of type X in the list of passed types
 * \details recursive implementation
 * \param acc the accumulator for the index.
 * \returns the index of the type which is equal to the passed predicate type
 */
template <typename X, typename Y, typename... Xs>
constexpr inline auto get_index(std::size_t acc = 0) -> std::size_t {
  return std::is_same<X, Y>::value ? acc : get_index<X, Xs...>(acc + 1);
}

/// \brief Returns the type from the passed index
template <std::size_t N, typename... Xs>
using get_from_index = typename std::tuple_element<N, std::tuple<Xs...>>::type;

/**
 * \brief Checks if any generic parameters are duplicate!
 */
template <typename... Xs>
struct has_duplicate;
/**
 * \brief Checks if any generic parameters are duplicate!
 * \details Endpoint of recursion -> no duplicate found
 */
template <typename X>
struct has_duplicate<X> : std::integral_constant<bool, false> {};
/**
 * \brief Checks if any generic parameters are duplicate!
 * \details Endpoint of recursion -> duplicate found
 */
template <typename X, typename... Xs>
struct has_duplicate<X, X, Xs...> : std::integral_constant<bool, true> {};
/**
 * \brief Checks if any generic parameters are duplicate!
 * \details Recursive implementation
 */
template <typename X, typename Y, typename... Xs>
struct has_duplicate<X, Y, Xs...>
    : std::integral_constant<bool, has_duplicate<X, Xs...>::value || has_duplicate<Y, Xs...>::value> {};

}  // namespace compile_time
}  // namespace language
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_LANGUAGE_COMPILE_TIME_H_
