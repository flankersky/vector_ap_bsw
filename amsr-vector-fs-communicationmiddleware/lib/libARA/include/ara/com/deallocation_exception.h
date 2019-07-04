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
/**        \file  deallocation_exception.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_DEALLOCATION_EXCEPTION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_DEALLOCATION_EXCEPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <exception>

namespace ara {
namespace com {
/**
 * \brief DeallocationException
 */
class DeallocationException : public std::exception {
 public:
  /**
   * \brief Constructor
   */
  DeallocationException() = default;

  /**
   * \brief Destructor
   */
  ~DeallocationException() = default;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_DEALLOCATION_EXCEPTION_H_
