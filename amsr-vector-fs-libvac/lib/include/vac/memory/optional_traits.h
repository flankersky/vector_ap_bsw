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
/**        \file  optional_traits.h
 *        \brief  Type-Traits relating to vac::memory::optional
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_
#define LIB_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <type_traits>

namespace vac {
namespace memory {

/**
 * \brief Forward declaration for the optional class.
 */
template <class T>
class optional;

/**
 * \brief Check if type is optional type
 * \details implementation if type is not optional
 */
template <typename C>
struct is_optional : std::false_type {};

/**
 * \brief Check if type is optional type
 * \details implementation if type is optional
 */
template <typename T>
struct is_optional<vac::memory::optional<T>> : std::true_type {};

}  // namespace memory
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_MEMORY_OPTIONAL_TRAITS_H_
