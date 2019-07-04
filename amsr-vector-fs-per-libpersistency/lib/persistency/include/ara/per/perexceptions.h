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
/**        \file  perexceptions.h
 *        \brief  Implementation of Exceptions as desired by SWS
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_PEREXCEPTIONS_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_PEREXCEPTIONS_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include <string>

namespace ara {
namespace per {
/**
 * \brief The ExceptionLogicError class shall provide an exception which can be thrown by functions defined within the
 * ara::per package. This exception is raised when logical errors occur during runtime.
 *
 * \trace SWS_PER_00076
 */
class ExceptionLogicError : public std::exception {
 public:
  /**
   * \brief Constructor for ExceptionLogicError with message
   * \param message The message saved in the exception
   *
   * \trace SWS_PER_00076
   */
  explicit ExceptionLogicError(const std::string& message) : message_(message) {}

  /**
   * Destructor
   */
  virtual ~ExceptionLogicError() {}

  /**
   * \brief Function to return the saved exception message
   * \return The saved exception message
   *
   * \trace SWS_PER_00132
   */
  const char* what() const noexcept { return message_.c_str(); }

 private:
  /**
   * \brief The saved exception message
   */
  std::string message_;
};

/**
 * \brief The ExceptionStorageLocationNotFound class shall provide an exception which will be thrown if the requested
 * storage location is not found or not configured in the AUTOSAR model.
 *
 * \trace SWS_PER_00060
 */
class ExceptionStorageLocationNotFound : public std::exception {
 public:
  /**
   * \brief Constructor for ExceptionStorageLocationNotFound with message
   * \param location The location saved in the exception
   */
  explicit ExceptionStorageLocationNotFound(const std::string& location) : message_(location) {}

  /**
   * Destructor
   */
  virtual ~ExceptionStorageLocationNotFound() {}

  /**
   * \brief Function to return the saved exception message
   * \return The saved exception message
   *
   * \trace SWS_PER_00133
   */
  const char* what() const noexcept { return message_.c_str(); }

 private:
  /**
   * \brief The saved exception message
   */
  std::string message_;
};

/**
 * \brief The ExceptionPhysicalStorageError class shall provide an exception which is thrown if a severe error which
 * might happen during the operation, such as out of memory or writing/reading to the storage return an error.
 */
class ExceptionPhysicalStorageError : public std::exception {
 public:
  /**
   * \brief Constructor for ExceptionPhysicalStorageError with message
   * \param message The message saved in the exception
   *
   * \trace SWS_PER_00061
   */
  explicit ExceptionPhysicalStorageError(const std::string& message) : message_(message) {}

  /**
   * Destructor
   */
  virtual ~ExceptionPhysicalStorageError() {}

  /**
   * \brief Function to return the saved exception message
   * \return The saved exception message
   *
   * \trace SWS_PER_00134
   */
  const char* what() const noexcept { return message_.c_str(); }

 private:
  /**
   * \brief The saved exception message
   */
  std::string message_;
};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_PEREXCEPTIONS_H_
