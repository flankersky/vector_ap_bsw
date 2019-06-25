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
/**        \file  state_return_type.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_STATECLIENT_INC_INTERNAL_STATE_RETURN_TYPE_H_
#define LIB_STATECLIENT_INC_INTERNAL_STATE_RETURN_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace ara {
namespace exec {

/**
 * \brief Enum class to define the error codes for Machine State operations.
 *
 * \trace SWS_EM_02005
 */
enum class StateReturnType : uint8_t { kSuccess = 0, kBusy = 1, kGeneralError = 2 };

}  // namespace exec
}  // namespace ara

#endif  // LIB_STATECLIENT_INC_INTERNAL_STATE_RETURN_TYPE_H_
