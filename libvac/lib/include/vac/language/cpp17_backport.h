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
/**        \file  cpp17_backport.h
 *        \brief  Contains useful backports of STL features from C++17 to C++11.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_
#define LIB_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_

#include <utility>

namespace vac {
namespace language {

namespace detail {

using std::swap;

/// \brief helper template in namespace including swap (aka using swap;) to test if a call to swap is well formed
struct swap_test {
  /// overload for swappable resolution
  template <typename T, typename = decltype(swap(std::declval<T&>(), std::declval<T&>()))>
  static std::true_type can_swap(int);

  /// overload for non-swappable resolution
  template <typename>
  static std::false_type can_swap(...);
};

/// \brief helper template to test if a call to swap is well formed
template <typename T>
struct swappable_impl : public swap_test {
  /// \brief type containing the deduction result
  typedef decltype(can_swap<T>(0)) type;
};

/// \brief test implementation for swappable
template <typename T, bool = std::is_const<typename std::remove_reference<T>::type>::value>
struct is_swappable;

/// \brief specialization if the tested type is const
template <typename T>
struct is_swappable<T, true> : public std::false_type {};

/// \brief specialization if the tested type is non-const
template <typename T>
struct is_swappable<T, false> : swappable_impl<T>::type {};

/// \brief implementation of nothrow swappable
template <typename T, bool = is_swappable<T>::value>
struct is_nothrow_swappable;

/// \brief specialization for swappable types
template <typename T>
struct is_nothrow_swappable<T, true>
    : std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))> {};

/// \brief specialization for non-swappable types
template <typename T>
struct is_nothrow_swappable<T, false> : std::false_type {};
}  // namespace detail

/// type of disambiguation tag that can be passed to certain constructors.
struct in_place_t {
  /// Default constructor
  explicit in_place_t() = default;  // NOLINT(runtime/explicit)
};

/// disambiguation tags that can be passed to certain constructors.
constexpr in_place_t in_place{};

/// \brief Is true if swap<T> is possible
template <typename T>
struct is_swappable : detail::is_swappable<T> {};

/// \brief Is true if swap<T> is noexcept
template <typename T>
struct is_nothrow_swappable : detail::is_nothrow_swappable<T> {};

}  // namespace language
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_LANGUAGE_CPP17_BACKPORT_H_
