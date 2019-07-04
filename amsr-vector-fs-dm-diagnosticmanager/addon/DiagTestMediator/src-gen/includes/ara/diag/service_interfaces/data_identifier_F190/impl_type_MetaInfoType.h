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

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOTYPE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOTYPE_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_MetaInfoValueType.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/**
 * \brief Type MetaInfoType.
 * \details Meta-Inf map, which contains key-value pairs according to MetaInfoKeyType .
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /ara/diag/types/MetaInfoType
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DataIdentifier_SWCL_A_DID_F190
 */
using MetaInfoType = std::map<MetaInfoKeyType, MetaInfoValueType>;

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOTYPE_H_
