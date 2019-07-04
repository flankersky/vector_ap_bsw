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
/**        \file  common.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_COMMON_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_COMMON_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include "ara/per/basicoperations.h"

namespace ara {
namespace per {
namespace common {

/**
 * \brief Converst an BasicOperations::OpenMode to a std::ios::openmode
 * \param openmode The BasicOperations::OpenMode
 * \return Converted std::ios::openmode
 *
 * \throws std::invalid_argument when openmode could not be matched to a std::ios::openmode
 */
inline std::ios::openmode ConvertOpenMode(BasicOperations::OpenMode const& openmode) {
  std::ios_base::openmode result;
  switch (openmode) {
    case BasicOperations::OpenMode::app:
      result = std::ios_base::app;
      break;
    case BasicOperations::OpenMode::binary:
      result = std::ios_base::binary;
      break;
    case BasicOperations::OpenMode::in:
      result = std::ios_base::in;
      break;
    case BasicOperations::OpenMode::out:
      result = std::ios_base::out;
      break;
    case BasicOperations::OpenMode::trunc:
      result = std::ios_base::trunc;
      break;
    case BasicOperations::OpenMode::ate:
      result = std::ios_base::ate;
      break;
    default:
      throw std::invalid_argument("Could not convert given OpenMode");
      break;
  }
  return result;
}

/**
 * \brief Calculates the mask which is created by the combination of multiple BasicOperations::OpenMode's
 * \param openmode The BasicOperations::OpenMode
 * \return An std::ios::openmode combination
 *
 */
inline std::ios_base::openmode CalculateOpenModeMask(BasicOperations::OpenMode const& openmode) {
  std::ios_base::openmode stdom = std::ios_base::openmode(0);

  for (const BasicOperations::OpenMode mode : kOpenModes_) {
    std::underlying_type<BasicOperations::OpenMode>::type combined_mode =
        static_cast<std::underlying_type<BasicOperations::OpenMode>::type>(mode) &
        static_cast<std::underlying_type<BasicOperations::OpenMode>::type>(openmode);
    if (0 != combined_mode) {
      stdom |= ConvertOpenMode(mode);
    }
  }
  return stdom;
}

/**
 * \brief Converts an BasicOperations::SeekDirection to a std::ios_base::seekdir
 * \param seek_dir The BasicOperations::SeekDirection
 * \return Converted std::ios_base::seekdir
 *
 * \throws std::invalid_argument when seek_dir could not be matched to a std::ios_base::seekdir
 */
inline std::ios_base::seekdir ConvertSeekDirection(BasicOperations::SeekDirection const& seek_dir) {
  std::ios_base::seekdir result;
  switch (seek_dir) {
    case BasicOperations::SeekDirection::beg:
      result = std::ios_base::beg;
      break;
    case BasicOperations::SeekDirection::end:
      result = std::ios_base::end;
      break;
    case BasicOperations::SeekDirection::cur:
      result = std::ios_base::cur;
      break;
    default:
      throw std::invalid_argument("Seek_dir not handled in switch statement");
      break;
  }
  return result;
}
}  // namespace common
}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_COMMON_H_
