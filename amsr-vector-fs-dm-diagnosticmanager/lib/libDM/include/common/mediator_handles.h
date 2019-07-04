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
/**
 *      \file  mediator_handles.h
 *      \brief  Definition of handles shared between DM and Mediator
 *
 *      \details  This file is used as long as the handles are not provided by the generator or a configuration file.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_MEDIATOR_HANDLES_H_
#define LIB_LIBDM_INCLUDE_COMMON_MEDIATOR_HANDLES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace amsr {
namespace diag {
namespace common {
namespace handles {

// TODO(PAASR-2728) This constants might be removed or changed when a generator is used to generate parts of the
// to generate parts of the Mediator (see e. g. PAASR-2728 and related JIRA tickets).

/**
 * \brief handle type
 * The handle identifies an operation (red/write DID, ...) to be executed by the MEdiator.
 */
using Handle = std::uint32_t;

/**
 * The VIN DID 0xF190 (one parameter: with 17 bytes).
 */
static constexpr Handle kReadDid0xF190 = 0x22F190ul;

/**
 * The VIN DID 0xF190 (one parameter: with 17 bytes).
 */
static constexpr Handle kWriteDid0xF190 = 0x2EF190ul;

/**
 * The supported example DID 0x4711 (two parameters: first with one byte, second with two bytes).
 */
static constexpr Handle kReadDid0x4711 = 0x224711ul;

/**
 * The supported example DID 0x4711 (two parameters: first with one byte, second with two bytes).
 */
static constexpr Handle kWriteDid0x4711 = 0x2E4711ul;

/**
 * The supported example of RID 0x3009 for start routine
 * (two parameters: reset - true/false, maxCounterVal - uint32_motorola).
 */
static constexpr Handle kStartRoutineRid0x3009 = 0x31013009ul;

/**
 * The supported example of RID 0x3009 for stop routine (no parameters).
 */
static constexpr Handle kStopRoutineRid0x3009 = 0x31023009ul;

/**
 * The supported example of RID 0x3009 for request routine results (no parameters).
 */
static constexpr Handle kRequestRoutineResultsRid0x3009 = 0x31033009ul;

} /* namespace handles */
} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_MEDIATOR_HANDLES_H_
