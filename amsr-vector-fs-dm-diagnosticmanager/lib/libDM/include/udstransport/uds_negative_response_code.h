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
/**        \file  uds_negative_response_code.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_NEGATIVE_RESPONSE_CODE_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_NEGATIVE_RESPONSE_CODE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace ara {
namespace diag {
namespace udstransport {
/**
 * \brief Enumerator with Uds negative response codes, positive response is needed for manufacturer/supplier indication
 */
enum class UdsNegativeResponseCode : uint8_t {
  kPositiveResponse = 0x00,
  kGeneralReject = 0x10,
  kServiceNotSupported = 0x11,
  kSubFunctionNotSupported = 0x12,
  kIncorrectMessageLengthOrInvalidFormat = 0x13,
  kResponseTooLong = 0x14,
  kBusyRepeatRequest = 0x21,
  kConditionsNotCorrect = 0x22,
  kRequestSequenceError = 0x24,
  kNoResponseFromSubnetComponent = 0x25,
  kFailurePreventsExecutionOfRequestedAction = 0x26,
  kRequestOutOfRange = 0x31,
  kSecurityAccessDenied = 0x33,
  kInvalidKey = 0x35,
  kExceedNumberOfAttempts = 0x36,
  kRequiredTimeDelayNotExpired = 0x37,
  kUploadDownloadNotAccepted = 0x70,
  kTransferDataSuspended = 0x71,
  kGeneralProgrammingFailure = 0x72,
  kWrongBlockSequenceCounter = 0x73,
  kRequestCorrectlyReceived_responsePending = 0x78,
  kSubfunctionNotSupportedInActiveSession = 0x7E,
  kServiceNotSupportedInActiveSession = 0x7F,
  kNoProcessingNoResponse = 0xFF /* not valid for UDS: 0xFF is ISOSAEReserved - see ISO 14229-1:2013 Table A.1 */
};

} /* namespace udstransport */
} /* namespace diag */
} /* namespace ara */
#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_NEGATIVE_RESPONSE_CODE_H_
