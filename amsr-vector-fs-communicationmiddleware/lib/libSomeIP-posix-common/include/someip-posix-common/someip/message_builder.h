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
/**        \file  message_builder.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_BUILDER_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_BUILDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someip_posix_common {
namespace someip {

/**
 * \brief Assemble a SOME/IP error response message for responding to
 * - malformed messages
 * - unknown service (instance)
 * - method not implemented
 *
 * \param return_code SOME/IP return code to write to the SOME/IP header of
 * the error response.
 * \param request_header The unmodified SOME/IP header from the request.
 * This SOME/IP header will be reused and slightly modified for the error
 * response.
 * \param packet Provide a buffer from outside to write the error response to.
 * \return Serialized error response packet.
 */
::someip_posix_common::someip::SomeIpPacket CreateSomeIpErrorHeader(
    const ::someip_posix_common::someip::ReturnCode return_code,
    const ::someip_posix_common::someip::SomeIpMessageHeader& request_header,
    ::someip_posix_common::someip::SomeIpPacket packet);

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MESSAGE_BUILDER_H_
