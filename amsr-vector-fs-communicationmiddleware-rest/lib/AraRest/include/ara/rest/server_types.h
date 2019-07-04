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
/**        \file  server_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_TYPES_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/endpoint.h"

namespace ara {
namespace rest {
class ServerRequest;  //< fwd
class ServerReply;    //< fwd
class ServerEvent;    //< fwd

/// Type of user-defined request handlers
using RequestHandlerType = void(const ServerRequest&, ServerReply&);

/// Denotes a subscription handler type
using SubscriptionHandlerType = void(ServerEvent);

/// Denotes a callback to call if subscription status changes
using SubscriptionStateHandlerType = void(ServerEvent&, SubscriptionState);

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_TYPES_H_
