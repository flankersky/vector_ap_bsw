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
/**        \file  match.h
 *        \brief  Represents the result of a pattern matching on a URI
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCH_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCH_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "ara/rest/string.h"

namespace ara {
namespace rest {

/** \brief Represents the result of a pattern matching on a URI
 *
 * Matches are handles to the pattern matching results and also
 * provide an API for type conversion of such pattern. Pattern wildcard '*'
 * matches exactly one URI path segments while wildcard '**' matches any number of segments.
 */
class Match {
 public:
  /**
   * \brief Constructor
   */
  explicit Match(const StringView& match) : match_(match) {}

  /** \brief Returns a path segment as a string
   *
   *  \return a string of the matches path segment
   */
  StringView Get() const noexcept(std::is_nothrow_constructible<StringView>::value) { return match_; }

  /** \brief Returns a type-converted path segment
   *
   *  Applies a type conversion on the matched path segment. The conversion result is
   *  assigned to the function argument which is subsequently returned.
   *  If conversion fails the function argument is returned unchanged. So either form is valid:
   *  GetAs<string>(), GetAs(string{my_allocator}), GetAs<string>("conversion failed")
   *
   *  \tparam T type to convert to. Type T must be input-streamble.
   *  \param def if conversion fails,
   *  \return The converted value of conversion succeeded, otherwise it returns the function argument.
   */
  template <typename T>
  T GetAs(T&& def = T{}) {
    typename std::remove_reference<T>::type ret_val;
    std::stringstream str_stream;

    String temp_str;
    STRINGVIEW_TO_STRING(temp_str, match_);

    str_stream.str(temp_str);
    try {
      str_stream >> ret_val;
    } catch (...) {  // nothing to do in here
    }

    if (str_stream.fail()) {
      ret_val = std::forward<T>(def);
    }

    return ret_val;
  }

 private:
  /**
   * \brief The match
   */
  const StringView match_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_MATCH_H_
