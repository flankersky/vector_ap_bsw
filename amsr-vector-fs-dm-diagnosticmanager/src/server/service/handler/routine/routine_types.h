/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  routine_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_TYPES_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <utility>

#include "vac/memory/leaky_array_allocator.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
/**
 * \brief type definition for RID.
 */
using Rid = std::uint16_t;

/**
 * \brief Enumeration for routine operations.
 * The values are based on the values of sub-function parameter for routine control.
 * Changing these values may lead to unexpected behavior.
 */
enum class RoutineOperationType : std::uint8_t { kStart = 0x01, kStop = 0x02, kRequestResult = 0x03 };

}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_TYPES_H_
