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
/**        \file  route.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/rest/routing/route.h"

namespace ara {
namespace rest {

Route::Route(RequestMethod req, const Pattern& pattern, const Function<RouteHandlerType>& handler)
    : handler_(handler), request_method_(req), pattern_(pattern) {}

const Pattern& Route::GetPattern() const noexcept { return pattern_; }

Route::Upshot Route::InvokeHandler(const ServerRequest& req, ServerReply& rep, const Matches& matches) const noexcept {
  // Forward the request to the user defined request handler
  return handler_(req, rep, matches);
}

}  // namespace rest
}  // namespace ara
