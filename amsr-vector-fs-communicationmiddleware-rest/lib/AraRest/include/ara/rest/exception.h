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
/**        \file  exception.h
 *        \brief  Containing exceptions used by the ara::rest implementation.
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_EXCEPTION_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace ara {
namespace rest {

/**
 * \brief Exception thrown when Uri format is invalid.
 */
class IllegalUriFormatException : std::runtime_error {
 public:
  /**
   * \brief Constructor.
   * \param what_arg explanatory string.
   */
  explicit IllegalUriFormatException(const std::string& what_arg) : std::runtime_error(what_arg) {}
  /**
   * \brief Constructor.
   * \param what_arg explanatory string.
   */
  explicit IllegalUriFormatException(const char* what_arg) : std::runtime_error(what_arg) {}
};

/** \brief Indicates a illegally formed UUID
 *
 *  This is typically thrown if a UUID in string format cannot be parsed.
 */
class IllformedUuidException : std::invalid_argument {
 public:
  /**
   * \brief Constructor.
   * \param what_arg explanatory string.
   */
  explicit IllformedUuidException(const std::string& what_arg) : std::invalid_argument(what_arg) {}
  /**
   * \brief Constructor.
   * \param what_arg explanatory string.
   */
  explicit IllformedUuidException(const char* what_arg) : std::invalid_argument(what_arg) {}
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_EXCEPTION_H_
