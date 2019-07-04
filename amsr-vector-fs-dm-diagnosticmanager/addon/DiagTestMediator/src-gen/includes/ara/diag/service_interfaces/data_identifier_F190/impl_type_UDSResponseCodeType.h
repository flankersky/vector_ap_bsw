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
/**        \file  impl_type_UDSResponseCodeType.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_UDSRESPONSECODETYPE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_UDSRESPONSECODETYPE_H_

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
 * \brief Type UDSResponseCodeType.
 * \details Represents UDS Error Codes, which can be returned by the external service processor.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /ara/diag/types/UDSResponseCodeType
 * in the namespace of
 * SERVICE-INTERFACE /ara/diag/DataIdentifier_SWCL_A_DID_F190
 */
enum class UDSResponseCodeType : uint8 {
  kGeneralReject = 16,
  kServiceNotSupported = 17,
  kSubfunctionNotSupported = 18,
  kIncorrectMessageLengthOrInvalidFormat = 19,
  kBusyRepeatRequest = 33,
  kConditionsNotCorrect = 34,
  kRequestSequenceError = 36,
  kNoResponseFromSubnetComponent = 37,
  kFailurePreventsExecutionOfRequestedAction = 38,
  kRequestOutOfRange = 49,
  kSecurityAccessDenied = 51,
  kInvalidKey = 53,
  kExceedNumberOfAttempts = 54,
  kRequiredTimeDelayNotExpired = 55,
  kUploadDownloadNotAccepted = 112,
  kTransferDataSuspended = 113,
  kGeneralProgrammingFailure = 114,
  kWrongBlockSequenceCounter = 115,
  kRequestCorrectlyReceivedResponsePending = 120,
  kSubFunctionNotSupportedInActiveSession = 126,
  kServiceNotSupportedInActiveSession = 127,
  kNoProcessingNoResponse = 255
};

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_IMPL_TYPE_UDSRESPONSECODETYPE_H_
