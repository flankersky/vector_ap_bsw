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
/**        \file  service_not_available_exception.h
 *        \brief  ServiceNotAvailableException
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_SERVICE_NOT_AVAILABLE_EXCEPTION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_SERVICE_NOT_AVAILABLE_EXCEPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <exception>

namespace ara {
namespace com {
/**
 * \brief ServiceNotAvailableException
 * \see SWS_CM_10352
 */
class ServiceNotAvailableException : public std::exception {
 public:
  /**
   * \brief Constructor
   * \param service_name Name of service not available.
   */
  explicit ServiceNotAvailableException(const char* service_name);

  /*
   * \brief Destructor
   */
  virtual ~ServiceNotAvailableException() = default;

  /**
   * \brief Get explanation of the ApplicationErrorException.
   * \return Explanatory string
   */
  virtual const char* what() const noexcept;

 private:
  /**
   * \brief Name of service
   */
  const char* service_name_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_SERVICE_NOT_AVAILABLE_EXCEPTION_H_
