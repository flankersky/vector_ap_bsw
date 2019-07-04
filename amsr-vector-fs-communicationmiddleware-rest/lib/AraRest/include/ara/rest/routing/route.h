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
/**        \file  route.h
 *        \brief  A route is a conditional mapping from a pair of RequestMethod and URI to a user-defined handler
 *function.
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <functional>
#include <initializer_list>

#include "ara/rest/allocator.h"
#include "ara/rest/iterator.h"
#include "ara/rest/routing/match.h"
#include "ara/rest/routing/matches.h"
#include "ara/rest/routing/pattern.h"
#include "ara/rest/server.h"
#include "ara/rest/types.h"

namespace ara {
namespace rest {

// forward declaration
class Matches;
/** \brief Represents a URI route
 *
 * A route is a conditional mapping from a pair of RequestMethod and URI to a user-defined handler
 * function.  A given request is matched against a pattern, if it matches a user-defined handler
 * function is called. In addition to the usual pair of Request and Replay objects the user-defined
 * handler is passed the matching Route itself to enable processing of pattern matches.
 *
 * Routes also define a partial order for route machting. Both RequestMethod and Pattern order
 * must be taken into consideration. Similar to Pattern lexicographic order appplies.
 * A route A compares less-than to a route B (A < B) if and only if RequestMethod compares less-than.
 * Otherwise A < B if and only if Pattern of A compares less-than Pattern of B.
 *
 * RequestMethod S compares less-then RequestMethod T (S < T) according to the following rules:
 * Conceptually, the order of request methods is lexicographic with each enumerator representing a character of a
 * string concatenated by operator '|'. Therefore, for single "digits" it holds that kGET < kPOST < .. etc according to
 * their underlying numeric values. For multiple "digits" it holds that - for example -
 * kGET < kGET|kPOST < kGET|kPUT < ... < kAny. But note that kGET|kPUT == kPUT|kGET.
 * In words, the most precise specifiers (singleton request methods) have precedence over the less precise
 * specificiation over sets of enumerators.
 *
 * Note that also request methods and Uri Pattern also form a lexicographical order with Uri being
 * the "most significant digit" and request method the "least significant digit" of a two-character string.
 * For example "/foo/bar" GET < "/foo/bar" POST|GET.
 *
 */
class Route {
 public:
  /**! friend declaration for test */
  friend class Route_Test_Test;

  /** \brief Instructions for a Router on how to proceed after a route handler functions returns
   *  A route handler function must return one of these values to instruct the router how to
   *  proceed after executing the current handler. The values have the following semantics:
   *  - Accept: Router shall not search for further matches
   *  - Yield: Router shall select the next matching route
   *  - Default: Router shall execute its default handler function
   */
  enum class Upshot {
    Accept,  ///< Finish matching. No further actions
    Yield,   ///< Search for next matching route and execute its handler
    Default  ///< Finish matching and instantly execute the default request handler.
  };

  /**
   * \brief The type of the user-define handler function to be invoked if this Route matches the Pattern
   */
  using RouteHandlerType = Upshot(const ServerRequest&, ServerReply&, const Matches&);

  /** \brief Constructs a route
   *
   *  For details see detailed description of Route.
   *
   *  \param met a disjunction (logical OR) of RequestMethods to match against
   *  \param pattern a URI Pattern to match against
   *  \param handler a user-defined handler
   */
  Route(RequestMethod met, const Pattern& pattern, const Function<RouteHandlerType>& handler);

  /** \brief ara::rest::Server compliant handler function
   *
   *  This function calls the user defined handler function
   *
   * \param req a request
   * \param rep a reply
   * \param matches a matches from route and uri pattern
   * \return the upshot value
   */
  Upshot InvokeHandler(const ServerRequest& req, ServerReply& rep, const Matches& matches) const noexcept;

  /**
   * \brief Get the Request Method.
   */
  RequestMethod GetRequestMethod() const noexcept { return request_method_; }

  /** \brief Provides access to the underlying Pattern object
   *
   *  \return reference to a Pattern
   */
  const Pattern& GetPattern() const noexcept;

  /**
   * \brief Compare two routes on equalness.
   * \param left
   * \param right
   */
  friend bool operator==(const Route& left, const Route& right) noexcept {
    return ((left.GetPattern() == right.GetPattern()) && (left.GetRequestMethod() == right.GetRequestMethod()));
  }

  /**
   * \brief Compare two routes for diversity.
   * \param left
   * \param right
   */
  friend bool operator!=(const Route& left, const Route& right) noexcept { return !(left == right); }

 private:
  /**
   * The user defined request handler.
   */
  Function<RouteHandlerType> handler_;

  /**
   * The requested method type for this route.
   */
  RequestMethod request_method_;

  /**
   * The URI Pattern to match against
   */
  Pattern pattern_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_ROUTE_H_
