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
/**
 *      \file  string_helper.cc
 *      \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iomanip>
#include <sstream>

#include "string_helper.h"

namespace amsr {
namespace diag {
namespace utility {

std::string ToString(const std::vector<std::uint8_t>& data, std::size_t max_bytes) {
  vac::container::const_array_view<std::uint8_t> view{data};
  return ToString(view, max_bytes);
}

/**
 * \brief Helper Function to create a string representation of raw bytes
 * \param data a chunk of bytes
 * \returns The string representation of the provided data
 */
std::string ToString(vac::container::const_array_view<std::uint8_t> data, std::size_t max_bytes) {
  std::stringstream temp_stream;

  static constexpr std::uint8_t print_size_uint8 = 2;

  vac::container::const_array_view<std::uint8_t>::const_iterator iter = data.cbegin();
  for (std::size_t printed_bytes = 0; ((iter != data.end()) && (printed_bytes < max_bytes)); ++printed_bytes) {
    temp_stream << std::setw(print_size_uint8) << static_cast<std::uint16_t>(*iter);
    ++iter;
  }

  return temp_stream.str();
}

}  // namespace utility
}  // namespace diag
}  // namespace amsr
