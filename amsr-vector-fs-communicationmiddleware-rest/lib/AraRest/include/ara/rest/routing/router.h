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
/**        \file  router.h
 *        \brief  ServerRequest handler for gine-grained message de-multiplexing
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTER_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <functional>
#include <initializer_list>
#include <vector>

#include "ara/rest/allocator.h"
#include "ara/rest/iterator.h"
#include "ara/rest/routing/route.h"
#include "ara/rest/server.h"
#include "ara/rest/server_types.h"
#include "ara/rest/string.h"
#include "ara/rest/types.h"

namespace ara {
namespace rest {
namespace test {
class RouterTest_Match_Test; /**< forward declaration for testing */
}  // namespace test

/** \brief ServerRequest handler for gine-grained message de-multiplexing
 *
 * This data structure maps requests to user-defined handler functions.
 * Per-default a Server only supports a single user-defined request handler
 * function. Router serves as a de-multiplexer to route requests to
 * a set of specific user-defined handler functions. The mapping is performed
 * by pattern matching against RequestMethod and Uri of a ServerRequest.
 *
 * Router maintains a (multi)set of routes. A request is matched against this set
 * until the first element matches. Each route is associated with a user-defined
 * request handler that is invoked with the Route object that matched the request
 * and the original ServerRequest/Reply objects.
 *
 * When matched, a route contains contextual information of this match (such as wildcard matches).
 * The user-defined "route handler" causes three different actions when returned from, which can
 * be controlled through the Route object itself:
 * - no action: If a route handler returns without further action, the router considers processing
 *              complete and returns control to the server
 * - default: A route may request that once the current handler is left, a router default handler is matched next - if
 * set.
 * - yield: A route may request that once the current handler is left, the next matching route is selected - if one
 * exists.
 *
 */
class Router {
  /**< friend declaration for testing */
  friend class test::RouterTest_Match_Test;

 public:
  /**
   * User-define route handler function type
   */
  using RouteHandlerType = Route::RouteHandlerType;

  /**
   * Iterator range of routes
   */
  using RouteRange = IteratorRange<unspecified_iterator_type>;

  /**
   * Const iterator range of routes
   */
  using ConstRouteRange = IteratorRange<unspecified_iterator_type>;

  /** \brief Constructs an empty Router
   *
   *  \param alloc an allocator for all internal dynamic memory requirements
   */
  explicit Router(Allocator* alloc = GetDefaultAllocator());

  /** \brief Constructs a router from a given list of routes
   *
   *  \param routes a list of routes
   *  \param alloc an allocator for all internal dynamic memory requirements
   */
  Router(std::initializer_list<Route> routes, Allocator* alloc = GetDefaultAllocator());

  /** \brief Request handler function
   *
   *  This function serves as the user-defined request handler function passed to Server
   *
   *  \param req a request
   *  \param rep a reply
   */
  void operator()(const ServerRequest& req, ServerReply& rep) const;

  // We also need iterator variants

  /** \brief Inserts a route into the set of potential matches
   *
   *  If a route already exists nothing is inserted.
   *
   *  \param route a route
   *  \return a reference to this
   */
  Router& InsertRoute(const Route& route);

  /** \brief Constructs a route in-place
   *  Similar to Insert except that the route is constructed in-place. The given
   *  arguments are forwarded to the internal Route. If such a route already exists
   *  nothing is inserted.
   *  \param met a set of request methods
   *  \param pat a URI Pattern
   *  \param hdl a user-defined routing handler
   *  \return a reference to this
   */
  Router& EmplaceRoute(RequestMethod met, Pattern pat, const Function<RouteHandlerType>& hdl);

  /** \brief Enables a user to set a default request handler
   * The given handler is called if none of the routes matched of it at least once of the routes
   * called Route::Default().
   * \param hdl a user-defined request handler
   * \return a reference to this.
   */
  Router& SetDefaultHandler(const Function<RequestHandlerType>& hdl);

  // optionals
  /** \brief Returns the number of routes
   *
   *  Returns the number of specified routes, exclusive of the default route.
   *
   *  \return the number of user-defined routes
   */
  std::size_t RouteCount();
  /** \brief Provides direc access to the set of routes
   *
   *  \return an iterator range of routes
   */
  RouteRange Routes();
  /** \brief Provides direc access to the set of routes
   *
   *  \return an iterator range of routes
   */
  ConstRouteRange Routes() const;
  /** \brief Removes a route from the set
   *  \param iter iterator referencing the route to remove
   */
  void RemoveRoute(RouteRange::Iterator iter);
  /** \brief Searches for a given route
   *
   *  \param route route to search for
   *  \return an iterator to the route if it exists in the set or Routes.end() if no such route was found.
   */
  RouteRange::Iterator FindRoute(const Route& route);
  /**
   * Remove a route
   * \param route the Route instance to be removed
   * \return true if success otherwise false.
   */
  bool RemoveRoute(Route& route);

  /** \brief Removes all routes
   */
  void Clear();

  /**
   * \brief The preallocated memory for routes_ to prevent reallocations.
   */
  static constexpr int kReservedRouteMemory = 10;

 private:
  /**
   * \brief Does a Route match with the ServerRequest
   * \param route
   * \param req
   * \returns True if match otherwise false
   */
  bool Match(const Route& route, const ServerRequest& req) const;

  /**
   * \brief The routes of the Router
   */
  std::vector<Route> routes_;

  /**
   * \brief The default request handler function.
   */
  Function<RequestHandlerType> handler_default_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTER_H_
