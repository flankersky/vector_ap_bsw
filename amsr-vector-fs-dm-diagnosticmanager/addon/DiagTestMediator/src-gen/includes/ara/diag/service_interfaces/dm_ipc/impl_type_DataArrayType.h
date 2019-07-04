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
/**        \file  impl_type_DataArrayType.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_DATAARRAYTYPE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_DATAARRAYTYPE_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "ara/diag/service_interfaces/dm_ipc/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/**
 * \brief Type DataArrayType.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /ara/diag/types/DataArrayType
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DM_IPC
 */
using DataArrayType = std::vector<uint8>;

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_IMPL_TYPE_DATAARRAYTYPE_H_
