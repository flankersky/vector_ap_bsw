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
/**        \file  unexpected_protocol_state_exception.h
 *        \brief  UnexpectedProtocolStateException
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UNEXPECTED_PROTOCOL_STATE_EXCEPTION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UNEXPECTED_PROTOCOL_STATE_EXCEPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <exception>

namespace ara {
namespace com {
namespace internal {

/**
 * \brief UnexpectedProtocolStateException
 */
class UnexpectedProtocolStateException : public std::exception {
 public:
  /**
   * \brief Constructor
   */
  UnexpectedProtocolStateException() = default;
  /**
   * \brief Destructor
   */
  virtual ~UnexpectedProtocolStateException() = default;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_UNEXPECTED_PROTOCOL_STATE_EXCEPTION_H_
