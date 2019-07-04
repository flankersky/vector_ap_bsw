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
/**        \file  router.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/routing/router.h"

#include <algorithm>
#include <initializer_list>
#include <vector>

namespace ara {
namespace rest {

Router::Router(Allocator* alloc) {
  (void)alloc;
  routes_.reserve(kReservedRouteMemory);
}

Router::Router(std::initializer_list<Route> routes, Allocator* alloc) {
  (void)alloc;
  routes_.reserve(routes.size());
  for (auto& route : routes) {
    routes_.emplace_back(route);
  }
}

Router& Router::InsertRoute(const Route& route) {
  /* Check if route is already present, insert only when route not already present */
  if (std::find(routes_.begin(), routes_.end(), route) == routes_.end()) {
    /* route is not present */
    routes_.emplace_back(route);
  }
  return *this;
}

Router& Router::EmplaceRoute(RequestMethod requestMethod, Pattern pattern, const Function<RouteHandlerType>& handler) {
  /* Check if route is already present, insert only when route not already present */
  if (std::find(routes_.begin(), routes_.end(), Route(requestMethod, pattern, handler)) == routes_.end()) {
    /* route is not present */
    routes_.emplace_back(Route(requestMethod, pattern, handler));
  }
  return *this;
}

void Router::operator()(const ServerRequest& req, ServerReply& rep) const {
  bool found_route{false};
  bool call_default_handler{true};
  for (const auto& route : routes_) {
    if (Match(route, req)) {
      String uri_string = ToString(req.GetHeader().GetUri(), Uri::Part::Path);
      StringView uri_view(STRING_TO_STRINGVIEW(uri_string));
      Pattern pattern_req(uri_view);
      Route current_route = route;
      Matches matches(current_route, pattern_req);

      // Call route handler
      Route::Upshot upshot = route.InvokeHandler(req, rep, matches);
      // Evaluate upshot value of the route handler
      switch (upshot) {
        case Route::Upshot::Accept:
          found_route = true;
          break;
        case Route::Upshot::Default:
          found_route = true;
          break;
        case Route::Upshot::Yield:
          // continue searching matching routes
          break;
        default: {
          // Nothing to do in here
        }
      }
      if (found_route) {
        // stop iterating over the routes
        call_default_handler = false;
        break;
      }
    }
  }
  if (call_default_handler) {
    handler_default_(req, rep);  // Throws exception std::bad_function_call if no default handler is set.
  }
}

Router& Router::SetDefaultHandler(const Function<RequestHandlerType>& handler) {
  handler_default_ = handler;
  return *this;
}

std::size_t Router::RouteCount() { return routes_.size(); }

bool Router::RemoveRoute(Route& route) {
  bool success{false};
  auto found = std::find(routes_.begin(), routes_.end(), route);
  while (found != routes_.end()) {
    routes_.erase(found);
    found = std::find(routes_.begin(), routes_.end(), route);
    success = true;
  }
  return success;
}

void Router::Clear() { routes_.clear(); }

bool Router::Match(const Route& route, const ServerRequest& req) const {
  bool match{false};
  // first check the methods
  if ((route.GetRequestMethod() & req.GetHeader().GetMethod()) != 0) {
    // then check whether the path patterns match

    String uri_string = ToString(req.GetHeader().GetUri(), Uri::Part::Path);
    StringView uri_view(STRING_TO_STRINGVIEW(uri_string));
    Pattern pattern_req(uri_view);
    if (pattern_req == route.GetPattern()) {
      match = true;
    }
  }
  return (match);
}

}  // namespace rest
}  // namespace ara
