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
/**        \file  application_error.h
 *         \brief ApplicationError SOME/IP specific definitions
 *
 *         \details
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_APPLICATION_ERROR_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_APPLICATION_ERROR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Offset of logical ApplicationError error code to the SOME/IP return code range for application errors.
 * \details Due to the constraint constr_1108 (error codes range is 1..63), SomeIpReturnCode::kRangeServiceErrorsStart
 * cannot used for serialization / deserialization of the error codes.
 * \see SWS_CM_10358,
 * \see SWS_CM_10312
 * \see PRS_SOMEIP_00191
 * \see constr_1108
 */
static constexpr std::uint8_t kApplicationErrorSomeIpReturnCodeOffset{0x1FU};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_APPLICATION_ERROR_H_
