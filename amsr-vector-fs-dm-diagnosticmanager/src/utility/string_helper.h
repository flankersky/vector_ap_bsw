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
/**        \file  string_helper.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_STRING_HELPER_H_
#define SRC_UTILITY_STRING_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <vector>

#include "vac/container/array_view.h"

namespace amsr {
namespace diag {
namespace utility {

/**
 * \brief Helper Function to create a string representation of raw bytes
 * \param data a chunk of bytes
 * \param max_bytes The maximum size of the to be printed string
 * \returns The string representation of the provided data
 */
std::string ToString(const std::vector<std::uint8_t>& data, std::size_t max_bytes);

/**
 * \brief Helper Function to create a string representation of raw bytes
 * \param data a chunk of bytes
 * \param max_bytes The maximum size of the to be printed string
 * \returns The string representation of the provided data
 */
std::string ToString(vac::container::const_array_view<std::uint8_t> data, std::size_t max_bytes);

}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_STRING_HELPER_H_
