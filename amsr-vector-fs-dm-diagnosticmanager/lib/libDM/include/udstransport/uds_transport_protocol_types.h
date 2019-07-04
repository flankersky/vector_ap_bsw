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
/**        \file  uds_transport_protocol_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_TYPES_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <vector>

#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace udstransport {
/**
 * UdsTransportProtocolHandler are flexible "plugins", which need an identification.
 */
/**
 * \brief Type definition of UdsTransportProtocolHandlerID.
 */
using UdsTransportProtocolHandlerID = uint8_t;

/**
 * \brief Type definition of ChannelID.
 */
using ChannelID = uint32_t;

/**
 * \brief define ByteVector datatype, which behaves interface wise like a std::vector<uint8_t>.
 */
using ByteVector = vac::container::array_view<uint8_t>;

}  // namespace udstransport
}  // namespace diag
}  // namespace ara
#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_TYPES_H_
