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
/**        \file  crc.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_CRC_CRC_H_
#define LIB_LIBARA_INCLUDE_ARA_CRC_CRC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include "vac/container/array_view.h"

namespace ara {
namespace crc {

/**
 * \brief Crc library taken from classic AUTOSAR and slightly modified for C++.
 */
class Crc {
 public:
  /**
   * \brief Buffer view to wrap the pointers and the data to inspect.
   */
  using BufferView = vac::container::const_array_view<const std::uint8_t>;

  /**
   * \brief Type-alias for better readability
   */
  using CRC32 = std::uint32_t;

  /**
   * \brief Calculate the 32-Bit CRC for Profile 04 polynomial 0xF4'AC'FB'13
   *
   * \param buffer_view Data block to inspect and calculate the CRC for.
   * \param start_value If this is called iteratively for individual data blocks, we may start from this value.
   * \param is_first_call by default this is true
   * \return the calculated CRC over the data block
   */
  static std::uint32_t CalculateCRC32P04(const BufferView buffer_view, const std::uint32_t start_value,
                                         const bool is_first_call = true) noexcept;

  /**
   * \brief Type-alias for better readability
   */
  using CRC64 = std::uint64_t;

  /**
   * \brief This service makes a CRC64 calculation on Crc_Length data bytes,
   * using the Profile 07 polynomial 0x42F0E1EBA9EA3693.
   *
   * \uptrace SWS_Crc_00061
   * \param buffer_view Data block to inspect and calculate the CRC for.
   * \param start_value If this is called iteratively for individual data blocks, we may start from this value.
   * \param is_first_call by default this is true
   * \return the calculated CRC over the data block
   */
  static CRC64 CalculateCRC64P07(const BufferView buffer_view, const CRC64 start_value,
                                 const bool is_first_call = true) noexcept;

  /**
   * \brief Initial value for the CRC calculation with Profile 04 polynomial 0xF4ACFB13.
   *
   * \uptrace SWS_Crc_00057
   */
  static constexpr std::uint32_t kInitialValueCRC32P04{0xFFFFFFFFU};

  /**
   * \brief XOR-value for the CRC calculation with Profile 04 polynomial 0xF4ACFB13.
   *
   * \uptrace SWS_Crc_00057
   */
  static constexpr std::uint32_t kXORValueCRC32P04{0xFFFFFFFFU};

  /**
   * \brief Initial value for the CRC calculation with polynomial 0x42F0E1EBA9EA3693.
   *
   * \uptrace SWS_Crc_00062
   */
  static constexpr std::uint64_t kInitialValueCRC64{0xFFFFFFFFFFFFFFFFULL};

  /**
   * \brief XOR-value for the CRC calculation with polynomial 0x42F0E1EBA9EA3693.
   *
   * \uptrace SWS_Crc_00062
   */
  static constexpr std::uint64_t kXORValueCRC64{0xFFFFFFFFFFFFFFFFULL};
};

}  // namespace crc
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_CRC_CRC_H_
