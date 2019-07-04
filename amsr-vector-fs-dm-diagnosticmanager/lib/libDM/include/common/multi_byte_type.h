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
/**        \file  multi_byte_type.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_MULTI_BYTE_TYPE_H_
#define LIB_LIBDM_INCLUDE_COMMON_MULTI_BYTE_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <stdexcept>

namespace amsr {
namespace diag {
namespace common {

/**
 * \brief Return a single byte of the stored value_ .
 *
 * \param value The value to get the needed byte from
 * \param byte_offset The offset of the byte to return. byte_offset == 0 denotes the lowest byte.
 * \returns The byte from value_ denoted by byte_offset.
 */
template <typename T>
uint8_t GetByte(const T& value, size_t byte_offset) {
  if (byte_offset >= sizeof(T)) {
    throw std::out_of_range("MultiByteType byte_offset out of range");
  }
  return static_cast<uint8_t>((value >> byte_offset * 8) & 0xFF);
}

/**
 * \brief Set the byte at the given offset to the given value.
 *
 * \param value Reference to the value to modify.
 * \param byte_value The new value to store.
 * \param byte_offset The offset to store the new value at. byte_offset == 0 denotes the lowest byte.
 */
template <typename T>
void SetByte(T& value, const uint8_t byte_value, const size_t byte_offset) {
  if (byte_offset >= sizeof(T)) {
    throw std::out_of_range("MultiByteType byte_offset out of range");
  }

  T clear_mask(0);
  clear_mask = static_cast<T>(0xFF << (byte_offset * 8));
  clear_mask = static_cast<T>(~clear_mask);
  value &= clear_mask;

  T temp(byte_value);
  temp = static_cast<T>(temp << byte_offset * 8);
  value |= temp;
}

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_MULTI_BYTE_TYPE_H_
