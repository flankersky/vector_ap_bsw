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
/**        \file  impl_type_MetaInfoType.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_METAINFOTYPE_H_
#define SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_METAINFOTYPE_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoValueType.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/**
 * \brief Type MetaInfoType.
 * \details Meta-Inf map, which contains key-value pairs according to MetaInfoKeyType .
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /ara/diag/types/MetaInfoType
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/GenericUDSService
 */
using MetaInfoType = std::map<MetaInfoKeyType, MetaInfoValueType>;

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_METAINFOTYPE_H_
