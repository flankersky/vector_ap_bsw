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
/**        \file  backend_not_available_exception.h
 *        \brief  BackendNotAvailableException
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_BACKEND_NOT_AVAILABLE_EXCEPTION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_BACKEND_NOT_AVAILABLE_EXCEPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <exception>

namespace ara {
namespace com {
/**
 * \brief BackendNotAvailableException
 */
class BackendNotAvailableException : public std::exception {
 public:
  /**
   * \brief Constructor
   */
  BackendNotAvailableException() noexcept {}

  /*
   * \brief Destructor
   */
  virtual ~BackendNotAvailableException() = default;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_BACKEND_NOT_AVAILABLE_EXCEPTION_H_
