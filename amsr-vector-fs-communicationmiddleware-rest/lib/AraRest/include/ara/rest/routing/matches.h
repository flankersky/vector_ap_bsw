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
/**        \file  matches.h
 *        \brief  Represents the result of a pattern matching on a URI
 *
 *      \details  Matches is an ordered set of Match and represents the currently matched route context. The order
 *corresponds to
 *                the order of wildcard occurrences.
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCHES_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCHES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <stdexcept>
#include <type_traits>
#include <vector>

#include "ara/rest/routing/match.h"
#include "ara/rest/routing/pattern.h"
#include "ara/rest/routing/route.h"
#include "ara/rest/string.h"

namespace ara {
namespace rest {

class Route;
/**
 *\brief Represents the result of a pattern matching on a URI
 */
class Matches {
 public:
  /**
     * \brief type
     */
  using ValueType = Match;

  /**
   * \brief Container type
   */
  using ContainerType = std::vector<Match>;

  /**
   * \brief  iterator
   */
  using Iterator = ContainerType::iterator;

  /**
   * \brief constant iterator
   */
  using ConstIterator = ContainerType::const_iterator;

  /**
   * \brief An IteratorRange of all pattern matches for this Route
   */
  using MatchRange = IteratorRange<Iterator>;

  /**
   * \brief A constant IteratorRange of all pattern matches for this Route
   */
  using ConstMatchRange = IteratorRange<ConstIterator>;

  /**
    * \brief Constructors
    *
    * \param route
    * \param uri_pattern .
    */
  Matches(Route& route, Pattern& uri_pattern);

  /** \brief Provides the number of URI wildcard matches after applying a pattern to a route
   *
   *  After the router matched a given Request against this Route, then this
   *  function returns the number of (wildcard) matches.
   *  \return the number of URI matches
   */
  std::size_t Count() const noexcept;

  /** \brief Provides access to a specific URI match
   *
   *  \param i index to the i'th URI wildcard match
   *  \return return type
   */
  const Match& Get(std::size_t i) const noexcept;

  /** \brief Provides access to the sequence of URI (wildcard) matches
   *
   *  After this route has been matched against a given request, all wildcard URI matches
   *  are accessible with this range.
   *
   *  \return a range of URI matches
   */
  MatchRange Get() noexcept;

  /** \brief Provides access to the sequence of URI (wildcard) matches
   *
   *  After this route has been matched against a given request, all wildcard URI matches
   *  are accessible with this range.
   *
   *  \return a range of URI matches
   */
  ConstMatchRange Get() const noexcept;

  /** \brief prepares the wildcard matches list
     */
  void CreateWildCardList();

  /** \brief Returns the route currently matched
   *  \return a reference to the currently matched route
   */
  const Route& GetRoute() const noexcept;

 private:
  /** \brief instance of route class
   */
  Route* route_;

  /** \brief instance of route class
   */
  ContainerType values_;

  /** \brief container to store route pattern
   */
  std::vector<String> route_pattern_;

  /** \brief container to store uri pattern
   */
  std::vector<String> uri_pattern_;

  /** \brief container to store wild card matches from route pattern
   */
  std::vector<ara::rest::Match> wildcard_matches_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCHES_H_
