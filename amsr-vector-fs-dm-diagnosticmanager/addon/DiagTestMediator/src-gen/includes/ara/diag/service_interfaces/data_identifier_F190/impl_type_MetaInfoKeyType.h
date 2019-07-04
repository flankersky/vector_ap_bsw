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
/**        \file  impl_type_MetaInfoKeyType.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOKEYTYPE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOKEYTYPE_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/**
 * \brief Type MetaInfoKeyType.
 * \details Represents the predefined/valid keys, which are available within the optional MetaInfo the DM provides in
 * service processor calls.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /ara/diag/types/MetaInfoKeyType
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DataIdentifier_SWCL_A_DID_F190
 */
enum class MetaInfoKeyType : uint8 {
  SA = 0,
  TA = 1,
  REQUEST_HANDLE = 2,
  REQUEST_TYPE = 3,
  SUPP_POS_RESP = 6,
  PROTOCOL_ID = 7,
  DOIP_LOCAL_IP = 8,
  DOIP_LOCAL_PORT = 9,
  DOIP_REMOTE_IP = 10,
  DOIP_REMOTE_PORT = 11
};

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_METAINFOKEYTYPE_H_
