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
/**        \file  basic_string.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_BASIC_STRING_H_
#define LIB_INCLUDE_VAC_CONTAINER_BASIC_STRING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>  // for std::allocator
#include <string>
#include "vac/memory/three_phase_allocator.h"

namespace vac {
namespace container {

/**
 * \brief basic_string type from libVAC.
 *
 * May be replaced by a custom implementation in the future.
 */
// template <typename character_type>
template <typename character_type, class Traits = std::char_traits<character_type>,
          class Allocator = std::allocator<character_type>>
using basic_string = std::basic_string<character_type, Traits, Allocator>;

/**
 * \brief Typedef for a string of arbitrary character types but fixed to the three phase allocator
 */
template <typename character_type>
using basic_three_phase_string =
    basic_string<character_type, std::char_traits<character_type>, vac::memory::ThreePhaseAllocator<character_type>>;

/**
 * \brief ThreePaseAllocator-Managed equivalent of std::string
 */
using SafeString = basic_three_phase_string<char>;

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_BASIC_STRING_H_
