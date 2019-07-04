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
/**        \file  impl_type_uint32.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_UINT32_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_UINT32_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/**
 * \brief Type uint32.
 * \details uint32 represents integers with a minimum value of 0 and a maximum value of 4294967295. The order-relation
 * on uint32 is: x < y if y - x is positive. uint32 has a lexical representation consisting of a finite-length sequence
 * of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /AUTOSAR_Platform/ImplementationDataTypes/uint32
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DM_IPC
 */
using uint32 = std::uint32_t;

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_UINT32_H_
