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
/**        \file  configuration_exceptions.h
 *        \brief  Contains all exceptions for the configuration parts
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_EXCEPTION_CONFIGURATION_EXCEPTIONS_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_EXCEPTION_CONFIGURATION_EXCEPTIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <system_error>

namespace ara {
namespace per {
namespace internal {
namespace json {
namespace exception {

/**
 * \brief Exception for file handling.
 */
class FileNotFound : public std::system_error {
 public:
  /**
   * \brief Constructor
   */
  explicit FileNotFound(const std::string& path)
      : std::system_error(std::error_code()), path_(path), message_("File: " + path_ + " not found!") {}
  /**
   * \brief Returns informations about the occurred error.
   * \return What happened
   */
  const char* what() const throw() { return message_.c_str(); }

 private:
  /**
   * \brief Path to corresponding file that causes this exception.
   */
  std::string path_;
  /**
   * \brief Corresponding message.
   */
  std::string message_;
};

/**
 * \brief Exception for JSON parser errors.
 */
class ParserError : public std::system_error {
 public:
  /**
     * \brief Constructor
     */
  ParserError() : std::system_error(std::error_code()), message_("JSON Parser error occurred!") {}
  /**
   * \brief Returns informations about the occurred error.
   * \return What happened
   */
  const char* what() const noexcept { return message_.c_str(); }

 private:
  /**
   * \brief Corresponding message.
   */
  const std::string message_;
};

/**
 * \brief Exception for JSON validation errors.
 */
class ValidationError : public std::system_error {
 public:
  /**
     * \brief Constructor
     */
  ValidationError() : std::system_error(std::error_code()), message_("JSON validation error occurred!") {}
  /**
   * \brief Returns informations about the occurred error.
   * \return What happened
   */
  const char* what() const noexcept { return message_.c_str(); }

 private:
  /**
   * \brief Corresponding message.
   */
  const std::string message_;
};

}  // namespace exception
}  // namespace json
}  // namespace internal
}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_EXCEPTION_CONFIGURATION_EXCEPTIONS_H_
