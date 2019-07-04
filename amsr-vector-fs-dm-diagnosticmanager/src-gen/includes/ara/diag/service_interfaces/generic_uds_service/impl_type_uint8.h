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
/**        \file  impl_type_uint8.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_UINT8_H_
#define SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_UINT8_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/**
 * \brief Type uint8.
 * \details uint8 represents integers with a minimum value of 0 and a maximum value of 255. The order-relation on uint8
 * is: x < y if y - x is positive. uint8 has a lexical representation consisting of a finite-length sequence of decimal
 * digits (#x30-#x39). For example: 1, 0, 126, +10.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /AUTOSAR_Platform/ImplementationDataTypes/uint8
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/GenericUDSService
 */
using uint8 = std::uint8_t;

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_IMPL_TYPE_UINT8_H_
