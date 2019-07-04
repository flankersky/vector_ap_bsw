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
/**        \file  impl_type_uint16.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_IMPL_TYPE_UINT16_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_IMPL_TYPE_UINT16_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

/**
 * \brief Type uint16.
 * \details uint16 represents integers with a minimum value of 0 and a maximum value of 65535. The order-relation on
 * uint16 is: x < y if y - x is positive. uint16 has a lexical representation consisting of a finite-length sequence of
 * decimal digits (#x30-#x39). For example: 1, 0, 1267, +10000.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /AUTOSAR_Platform/ImplementationDataTypes/uint16
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DataIdentifier_SWCL_A_DID_4711
 */
using uint16 = std::uint16_t;

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_IMPL_TYPE_UINT16_H_
