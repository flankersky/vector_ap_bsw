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
/**        \file  cpp14_backport.h
 *        \brief  Contains useful backports of STL features from C++14 to C++11.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_LANGUAGE_CPP14_BACKPORT_H_
#define LIB_INCLUDE_VAC_LANGUAGE_CPP14_BACKPORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <memory>
#include <type_traits>
#include <utility>

namespace vac {
namespace language {

/**
 * \brief Equivalent to C++14 std::enable_if_t
 */
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

/** \brief Equivalent to C++14 std::make_unique for non-arrays
 *  \param args Arguments to the pointee
 *  \return a pointer to the created type
 */
template <typename T, typename... Args, typename = enable_if_t<!std::is_array<T>::value>>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/** \brief Equivalent to C++14 std::make_unique for arrays
 *  \param size Array Size to allocate
 *  \return a pointer to the created array
 */
template <typename T, typename = enable_if_t<std::is_array<T>::value>>
std::unique_ptr<T> make_unique(std::size_t size) {
  return std::unique_ptr<T>(new typename std::remove_extent<T>::type[size]());
}

/**
 * \brief Equivalent to C++14 std::max
 * \param a the first option
 * \param b the second option
 * \returns b if it is larger than a else returns a
 */
template <typename T>
constexpr T max(T a, T b) {  // NOLINT(build/include_what_you_use)
  return (a < b) ? b : a;
}

}  // namespace language
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_LANGUAGE_CPP14_BACKPORT_H_
