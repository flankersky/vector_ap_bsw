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
/**        \file  error_code.h
 *        \brief  Contains possible error codes for std::error_code
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ERROR_CODE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ERROR_CODE_H_

#include <cstdint>

namespace ara {
namespace rest {

/**
 * Error codes for errors that occur in client or server
 * kNetworkError is used for network specific errors
 * kDevelopmentError is used when development logic is wrong
 */
enum class ErrorCode : std::uint8_t { kNetworkError = 1, kDevelopmentError = 2 };

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ERROR_CODE_H_
