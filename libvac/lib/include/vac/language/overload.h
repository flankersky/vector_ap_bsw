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
/**        \file  overload.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_LANGUAGE_OVERLOAD_H_
#define LIB_INCLUDE_VAC_LANGUAGE_OVERLOAD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

namespace vac {
namespace language {

namespace detail {

/**
 * \brief forward declaration of helper for constructing a single visitor from multiple functors
 */
template <typename... TFunctors>
struct overload;

/**
 * \brief helper for constructing a single visitor from multiple functors
 * \details recursive declaration: add overload for first type,
 *          and recursively add overload(s) for the trailing type list
 */
template <typename TFirst, typename... TTail>
struct overload<TFirst, TTail...> : TFirst, overload<TTail...> {
  /**
   * \brief constructor
   * \param first first functor
   * \param tail list of remaining functors
   */
  overload(TFirst first, TTail... tail) : TFirst(first), overload<TTail...>(tail...) {}

  /**
   * \brief overload operator()
   */
  using TFirst::operator();
  /**
   * \brief overload operator()
   */
  using overload<TTail...>::operator();
};

/**
 * \brief helper for constructing a single visitors from multiple functors
 * \details recursive declaration: case to end recursion
 */
template <typename TLast>
struct overload<TLast> : TLast {
  /**
   * \brief constructor
   * \param last only/last functor
   */
  explicit overload(TLast last) : TLast(last) {}

  /**
   * \brief overload operator()
   */
  using TLast::operator();
};

}  // namespace detail

/**
 * \brief create a visitor from a list of lambda expressions
 * \details example usage:
 *  auto visitor = make_overload([](const int&) { return 1; },
 *                               [](const bool&) { return 2; },
 *                               [](const uint32_t&) { return 3; },
 *                               [](const std::string&) { return 4; });
 * \param functors list of functor objects
 * \return a visitor providing all the functors passed as overloaded operator()
 */
template <typename... TFunctors>
detail::overload<TFunctors...> make_overload(TFunctors... functors) {
  return detail::overload<TFunctors...>(functors...);
}

}  // namespace language
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_LANGUAGE_OVERLOAD_H_
