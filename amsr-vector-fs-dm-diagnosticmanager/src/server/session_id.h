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
/**
 *      \file  session_id.h
 *      \brief  Contains the session type
 *
 *      \details  This file contains the definition of the enum SessionId.
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SESSION_ID_H_
#define SRC_SERVER_SESSION_ID_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace diag {
namespace server {

/**
 * \brief Enumerator for diagnostic session IDs.
 */
enum SessionId : uint8_t { kDefault = 0x01, kProgramming = 0x02, kExtended = 0x03, kSafetySystem = 0x04 };

}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SESSION_ID_H_
