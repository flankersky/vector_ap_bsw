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
/**        \file  utility.h
 *        \brief  TODO
 *
 *      \details  Collection of useful utilities. Partly compensating for C++11 defects.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UTILITY_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UTILITY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <type_traits>

namespace ara {
namespace com {
namespace internal {

/// @cond

/***************************************************************************************************
 * Provides index_sequence support which is lacking by C++11
 *
 */

/** \brief Support functionality to do fancy stuff with tuples
 *  This is is the equivalent of C++14 std::index_sequence that we are lacking here.
 *  We use it in tuple export/import to visit tuple elements one by one.
 */
template <std::size_t...>
struct index_sequence {};

/** \brief  Drop-in replacement for C++14 std::integer_sequence.
 *  See C++ standard or use cases below for documentation.
 */
template <std::size_t N, std::size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

/** \brief  Drop-in replacement for C++14 std::integer_sequence.
 *  See C++ standard or use cases below for documentation.
 */
template <std::size_t... Is>
struct make_index_sequence<0u, Is...> {
  using type = index_sequence<Is...>;
};

/***************************************************************************************************
 * Additional type traits absent from C++(11)
 *
 */

/** \brief Determines whether a type has a const_iterator member
 *
 *  \tparam C type to check
 */
template <typename C>
struct is_iterable {
  using false_type = std::int32_t;
  using true_type = std::int8_t;

  template <class T>
  static false_type check(...);
  template <class T>
  static true_type check(int, typename T::const_iterator = C().end());

  enum { value = sizeof(check<C>(0)) == sizeof(true_type) };
};

/** \brief Determines whether a type has a push_back member function
 *
 *  \tparam C type to check
 */
template <typename C>
struct has_push_back {
  using false_type = std::int32_t;
  using true_type = std::int8_t;

  template <class T>
  static false_type check(...);
  template <class T>
  static true_type check(decltype(static_cast<void (T::*)(const typename T::value_type&)>(&T::push_back))*);

  enum { value = sizeof(check<C>(nullptr)) == sizeof(true_type) };
};

/** \brief Determines whether a type has an insert member function
 *
 *  \tparam C type to check
 */
template <typename C>
struct has_insert {
  using false_type = std::int32_t;
  using true_type = std::int8_t;

  template <class T>
  static false_type check(...);
  template <class T>
  static true_type check(decltype(static_cast<void (T::*)(const typename T::value_type&)>(&T::insert))*);

  enum { value = sizeof(check<C>(nullptr)) == sizeof(true_type) };
};

/// @endcond

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UTILITY_H_
